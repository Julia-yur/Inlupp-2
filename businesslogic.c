#include "businesslogic.h"
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "linked_list.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>
struct merch
{
    char *name;
    char *desc;
    int price;
    ioopm_list_t *locs;
};

struct shelf
{
    char *shelf_id;
    int stock;
};

struct warehouse
{
    ioopm_hash_table_t *ht_n_S;
    ioopm_hash_table_t *ht_sl_n;
};

struct cart
{
    int id;
    ioopm_hash_table_t *items;
};

//HASHING FUNKTIONER

/// HASH-FUNKTION TAGEN FRÅN STACK OVERFLOW https://stackoverflow.com/a/7666577, modifierad
size_t hash_str(elem_t str)
{
    size_t hash = 5381;
    int c;

    while ((c = *str.s++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

//String compare för elem_t från inluppsbeskrivningen
static bool string_key_eq(elem_t k1, elem_t k2)
{
  return strcmp(k1.s, k2.s) == 0; // if elem_t has field charstring
}

// Warehouse funktioner

warehouse_t *warehouse_create(void)
{
    warehouse_t *wh = calloc(1, sizeof(struct warehouse));

    if (wh == NULL)
    {
        return NULL;

    }

    wh->ht_n_S = ioopm_hash_table_create(hash_str, string_key_eq);
    wh->ht_sl_n = ioopm_hash_table_create(hash_str, string_key_eq);

    return wh;
}

void warehouse_destroy(warehouse_t *wh)
{ 

 /*   // Frigör alla merch-objekt som lagras i ht_n_S.
    ioopm_hash_table_iterator_t *iter =
        ioopm_hash_table_iterator_create(warehouse->ht_n_S);

    while (!ioopm_hash_table_iterator_at_end(iter))
    {
        elem_t value =
            *ioopm_hash_table_iterator_current_value(iter);

        merch_t *merch = value.p;

        ioopm_hash_table_iterator_advance(iter);
    }

    ioopm_hash_table_iterator_destroy(iter); */

    // Förstör hash tables interna struktur.
    ioopm_hash_table_destroy(wh->ht_n_S);
    ioopm_hash_table_destroy(wh->ht_sl_n);

    free(wh);
}

// If warehouse is empty
bool warehouse_is_empty(warehouse_t *wh)
{
    if (wh == NULL)
    {
        return true;
    }

    return ioopm_hash_table_is_empty(wh->ht_n_S);

}

// Counter of merch
/* size_t warehouse_merch_count(warehouse_t *wh);
{
    if (wh == NULL)
    {
        return 0;
    }
    
    return ioopm_hash_table_size(&wh->ht_n_S);
} */

// Merchandise funktioner

/* bool warehouse_has_merch(warehouse_t *wh, char *key)
{
    if (wh == NULL || key == NULL)
    {
        return false;
    }

    return ioopm_hash_table_has_key(wh->ht_n_S, string_elem(key));
} */

void warehouse_add_merch(warehouse_t *wh, char *name, char *desc, int price)
{
    /* if (warehouse_has_merch(wh, name))
    {
        return;
    } */

    merch_t *merch = malloc(sizeof(merch_t));

    merch->name = strdup(name);
    merch->desc = strdup(desc);
    merch->price = price;
    merch->locs = ioopm_list_create();

    ioopm_hash_table_insert(wh->ht_n_S, string_elem(merch->name), ptr_elem(merch));
}

/* bool warehouse_remove_merch(warehouse_t *wh, char *name);
{
    if (wh == NULL || name == NULL)
    {
        return false;
    }

    elem_t removed;
    if (!ioopm_hash_table_remove(wh->ht_n_S, string_elem(name) &removed))
    {
        return false;
    }

    merch_t *merch = removed.p;

    merch_destroy(merch);
    return true;
}

bool warehouse_edit_merch(warehouse_t *wh, char *name, char *new_desc, int new_price);

{
    if (wh == NULL || name == NULL || new_desc == NULL || new_price == NULL)
    {
        return false;
    }

    elem_t result;

    if (!ioopm_hash_table_lookup(wh->ht_n_S, string_elem(name), &result))
    {
        return false;
    }

    merch_t 

}


// Shelves and stock

bool warehouse_has_shelf(warehouse_t *wh, char *shelf)
{
    if (wh == NULL || key == NULL)
    {
        return false;
    }
    return ioopm_hash_table_has_key(wh->ht_sl_n, string_elem(key));

}

// Carts */
