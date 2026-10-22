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
  return strcmp(k1.s, k2.s) == 0; // if elem_t has field char *string
}

// Warehouse funktioner

warehouse_t *warehouse_create(void)
{
    warehouse_t *wh = calloc(1, sizeof (struct warehouse));

    wh->ht_n_S = ioopm_hash_table_create(hash_str, string_key_eq);
    wh->ht_sl_n = ioopm_hash_table_create(hash_str, string_key_eq);

    return wh;
}

void warehouse_destroy(warehouse_t *wh)
{
/*     // Frigör alla merch-objekt som lagras i ht_n_S.
    ioopm_hash_table_iterator_t *iter =
        ioopm_hash_table_iterator_create(&wh->ht_n_S);

    while (!ioopm_hash_table_iterator_at_end(&iter))
    {
        elem_t value =
            ioopm_hash_table_iterator_current_value(&iter);

        merch_t *merch = value;
        merch_destroy(merch);

        ioopm_hash_table_iterator_advance(&iter);
    }

    ioopm_hash_table_iterator_destroy(&iter);
 */
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

    return (ioopm_hash_table_is_empty(wh->ht_n_S));
}



// Counter of merch
/* size_t warehouse_merch_count(warehouse_t *wh); */

// Merchandise funktioner

/* bool warehouse_has_merch(warehouse_t *wh, char *key)
{
    if (wh == NULL || key == NULL)
    {
        return false;
    }

    return ioopm_hash_table_has_key(wh->ht_n_S, string_elem(key));
} */

/* bool warehouse_has_shelf(warehouse_t *wh, char *shelf)
{
    if (wh == NULL || key == NULL)
    {
        return false;
    }
    return ioopm_hash_table_has_key(wh->ht_sl_n, string_elem(key));

} */

void warehouse_insert(warehouse_t *wh, char *name, char *desc, int price)
{

    /* if (warehouse_has_merch(wh, name))
    {
        return;
    } */

    merch_t *merch = malloc(sizeof(merch_t));

    merch->name = strdup(name);
    merch->desc = strdup(desc);
    merch->price = price;

    ioopm_hash_table_insert(
        wh->ht_n_S,
        string_elem(merch->name),
        ptr_elem(merch)
    );
}