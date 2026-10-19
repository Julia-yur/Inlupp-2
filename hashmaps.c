#pragma once
#include "common.h"

/**
 * @file hash_table.h
 * @author Felix Svensson, Julia Nystrand
 * @date 2026-09-17
 * @brief Simple hash table that maps keys to values.
 *
 * The keys and values stored may be of any of the following types:
 * - int
 * - unsigned int
 * - bool
 * - float
 * - void *
 * - char *
 *
 * The hash table does not take ownership of the keys or values,
 * so any heap allocated memory pointed to by any of the pointer types
 * will not be freed when the hash table is destroyed
 *
 */

/// @brief A hash table
typedef struct hash_table ioopm_hash_table_t;

/// @brief Create a new hash table
/// @param hash_fn a hash function for the underlying type of the keys
/// @param eq_fn an equality function for the underlying type of the keys
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn,
                                            ioopm_eq_function *eq_fn);

/// @brief Create a new hash table with a specific number of available buckets
/// @param buckets the number of buckets
/// @pre buckets is more than 0
/// @param hash_fn a hash function for the underlying type of the keys
/// @param eq_fn an equality function for the underlying type of the keys
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_with_buckets(size_t buckets,
                                                  ioopm_hash_function *hash_fn,
                                                  ioopm_eq_function *eq_fn);

/// @brief Delete a hash table and free its memory.
/// Does NOT free the memory of any heap allocated keys or values
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht.
///        Does not take ownership of any heap allocated keys or values
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
/// @pre underlying type of key must be compatible with the
///      hash and eq functions the hash table was created with.
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @pre underlying type of key must be compatible with the hash and eq
///      functions the hash table was created with
/// @param result a pointer where the value will be written to, if found
/// @return true if the key existed in the hash table, false otherwise
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key,
                             elem_t *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @pre underlying type of key must be compatible with the hash and eq
///      functions the hash table was created with
/// @param removed a pointer where the value mapped to by key will be written,
///                if found
/// @return true if the key existed in the hash table, false otherwise
bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key,
                             elem_t *removed);

/// @brief checks if hash table contains a mapping from a given key
/// @param ht hash table operated upon
/// @param key key to lookup
/// @pre underlying type of key must be compatible with the hash and eq
///      functions the hash table was created with
/// @return true if the key existed in the hash table, false otherwise
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief checks if hash table is empty
/// @param ht hash table operated upon
/// @return true if there are no entries in the hash table, false otherwise
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief gets the size (number of entries) of a hash table
/// @param ht hash table operated upon
/// @return the current number of entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);
