#include "common.h"
#include "hash_table.h"
#include "hash_table_iterator.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_NO_BUCKETS 17

/// @brief A hash table entry
typedef struct entry entry_t;

/// @brief A struct representing a hash table entry
struct entry {
  /// The entry's key
  elem_t key;
  /// The entry's value
  elem_t value;
  /// Points to the next entry in the bucket (possibly NULL)
  entry_t *next;
};

/// @brief A struct representing the hash table itself
struct hash_table {
  /// The number of entries currenly in the hash table
  size_t size;
  /// The number of buckets the hash table contains
  size_t n_buckets;
  /// An equality function to check if two keys are equal
  ioopm_eq_function *eq_fn;
  /// A hash function to calculate the hash of the keys
  ioopm_hash_function *hash_fn;
  /// An array of size n_buckets containing the hash table's buckets
  entry_t **buckets;
};

/// @brief Creates a new entry with a given key and value
/// @param key the entry's key
/// @param key the entry's value
/// @return a pointer to the new entry
static entry_t *entry_create(elem_t key, elem_t value) {
  entry_t *entry = calloc(1, sizeof(entry_t));
  entry->key = key;
  entry->value = value;
  return entry;
}

/// @brief Frees the memory of a hash table entry
/// @param entry a pointer to the entry to free
static void entry_destroy(entry_t *entry) { free(entry); }

/// @brief Find the incoming pointer that points to the entry with a given key,
/// if it exists
///
/// This will point to the 'next' pointer of the previous
/// entry in the bucket if there is one, or the bucket's index in the buckets
/// array if there aren't any entries in that bucket already
///
/// @param ht the hash table to look in
/// @param key the key of the entry
/// @return a pointer to the pointer to the entry with the key if it exists.
/// Otherwise, a pointer to the pointer where the entry should be stored
static entry_t **find_inc_ptr(ioopm_hash_table_t *ht, elem_t key) {
  // find the correct bucket
  size_t bucket = ht->hash_fn(key) % ht->n_buckets;

  // look for an entry with the key we want and return the pointer pointing to
  // it
  entry_t **current = &ht->buckets[bucket];
  while (*current != NULL) {
    if (ht->eq_fn((*current)->key, key)) {
      break;
    }
    current = &(*current)->next;
  }
  return current;
}

ioopm_hash_table_t *ioopm_hash_table_with_buckets(size_t n_buckets,
                                                  ioopm_hash_function *hash_fn,
                                                  ioopm_eq_function *eq_fn) {
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  ht->size = 0;
  ht->n_buckets = n_buckets;
  ht->eq_fn = eq_fn;
  ht->hash_fn = hash_fn;
  ht->buckets = calloc(n_buckets, sizeof(entry_t));
  return ht;
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn,
                                            ioopm_eq_function *eq_fn) {
  return ioopm_hash_table_with_buckets(DEFAULT_NO_BUCKETS, hash_fn, eq_fn);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
  for (size_t i = 0; i < ht->n_buckets; i++) {
    entry_t *to_destroy = ht->buckets[i];
    while (to_destroy) {
      entry_t *next = to_destroy->next;
      entry_destroy(to_destroy);
      to_destroy = next;
    }
  }
  free(ht->buckets);
  free(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) {
  // find pointer to where we should place the entry
  entry_t **entry = find_inc_ptr(ht, key);

  // if there was an entry there update the value, otherwise create a new entry
  // and put it at the pointer location
  if (*entry) {
    (*entry)->value = value;
  } else {
    *entry = entry_create(key, value);
    ht->size++;
  }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key,
                             elem_t *result) {
  entry_t **entry = find_inc_ptr(ht, key);

  // if the key exists write the value, otherwise indicate that the lookup
  // failed
  if (*entry) {
    *result = (*entry)->value;
    return true;
  } else {
    return false;
  }
}

bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key,
                             elem_t *removed) {
  entry_t **inc = find_inc_ptr(ht, key);
  if (!*inc) {
    return false;
  }

  entry_t *to_remove = *inc;
  *inc = to_remove->next;

  *removed = to_remove->value;
  entry_destroy(to_remove);
  ht->size--;
  return true;
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht) { return ht->size; }

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key) {
  // We don't care about any potential value from the lookup
  elem_t _;
  return ioopm_hash_table_lookup(ht, key, &_);
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht) { return ht->size == 0; }

// End of hash table definitions, start of hash table iterator definitions

/// @brief A struct representing the state of a hash table iterator
struct hash_table_iterator {
  /// The hash table to iterate over
  ioopm_hash_table_t *ht;
  /// The current bucket the iterator is iterating over
  size_t current_bucket;
  /// The current entry the iterator is pointing to (possibly NULL)
  entry_t *current_entry;
};

ioopm_hash_table_iterator_t *
ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht) {
  ioopm_hash_table_iterator_t *it =
      calloc(1, sizeof(ioopm_hash_table_iterator_t));
  it->ht = ht;
  it->current_bucket = 0;
  it->current_entry = *ht->buckets;

  // The first bucket might be empty,
  // so we need to advance to the first actual entry
  if (!it->current_entry) {
    ioopm_hash_table_iterator_advance(it);
  }
  return it;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it) {
  free(it);
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it) {
  // Once we've iterated over all the buckets,
  // we know there can't be any more entries
  return it->current_bucket >= it->ht->n_buckets;
}

void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it) {
  if (ioopm_hash_table_iterator_at_end(it)) {
    return;
  } else if (it->current_entry && it->current_entry->next) {
    it->current_entry = it->current_entry->next;
  } else {
    it->current_bucket += 1;
    if (ioopm_hash_table_iterator_at_end(it)) {
      return;
    }
    // if the next bucket existed, advance to the next entry or to the end
    it->current_entry = it->ht->buckets[it->current_bucket];
    if (!it->current_entry) {
      ioopm_hash_table_iterator_advance(it);
    }
  }
}

elem_t ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it) {
  return it->current_entry->key;
}

elem_t
ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it) {
  return it->current_entry->value;
}
