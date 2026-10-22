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

// Warehouse funktioner

warehouse_t *warehouse_create(void)
{
    warehouse_t *wh = calloc(1, sizeof (struct warehouse));

    if (wh == NULL)
    {
        return NULL;

    }

    wh->ht_n_S = ioopm_hash_table_create(string_hash, string_eq);
    wh->ht_sl_n = ioopm_hash_table_create(string_hash, string_eq);
    
    if (wh->ht_n_S == NULL || wh->ht_sl_n == NULL)
    {
        ioopm_hash_table_destroy(&wh->ht_n_S);
        ioopm_hash_table_destroy(&wh->ht_sl_n);
        free(wh);
        return NULL;
    }

    return wh;

}

void warehouse_destroy(warehouse_t **wh)
{
    if (wh == NULL || *wh == NULL)
    {
        return;
    }

    warehouse_t *warehouse = *wh;

    // Frigör alla merch-objekt som lagras i ht_n_S.
    ioopm_hash_table_iterator_t *iter =
        ioopm_hash_table_iterator_create(&warehouse->ht_n_S);

    while (!ioopm_hash_table_iterator_at_end(&iter))
    {
        elem_t value =
            *ioopm_hash_table_iterator_current_value(&iter);

        merch_t *merch = value.p;
        merch_destroy(merch);

        ioopm_hash_table_iterator_advance(&iter);
    }

    ioopm_hash_table_iterator_destroy(&iter);

    // Förstör hash tables interna struktur.
    ioopm_hash_table_destroy(&warehouse->ht_n_S);
    ioopm_hash_table_destroy(&warehouse->ht_sl_n);

    free(warehouse);
    *wh = NULL;
}

// If warehouse is empty
bool warehouse_is_empty(warehouse_t *wh);
{
    if (wh == NULL)
    {
        return true;
    }

    return ioopm
}



// Counter of merch
size_t warehouse_merch_count(warehouse_t *wh);

// Merchandise funktioner

bool warehouse_has_merch(warehouse_t *wh, char *key)
{
    if (wh == NULL || key == NULL)
    {
        return false;
    }

    return ioopm_hash_table_has_key(wh->ht_n_S, string_elem(key));
}

bool warehouse_has_shelf(warehouse_t *wh, char *shelf)
{
    if (wh == NULL || key == NULL)
    {
        return false;
    }
    return ioopm_hash_table_has_key(wh->ht_sl_n, string_elem(key));

}

void warehouse_insert(warehouse_t *wh, char *name, char *desc, int price)
{
    if (wh == NULL || name == NULL || desc == NULL)
    {
        return;
    }

    if (warehouse_has_merch(wh, name))
    {
        return;
    }

    merch_t *merch = malloc(sizeof(merch_t));
    if (merch == NULL)
    {
        return;
    }

    merch->name = strdup(name);
    merch->desc = strdup(desc);
    merch->price = price;
    merch->locs = ioopm_list_create();

    if (merch->name == NULL || merch->desc == NULL || merch->locs == NULL)
    {
        free(merch->name);
        free(merch->desc);

        if (merch->locs != NULL)
        {
            ioopm_list_destroy(&merch->locs);
        }

        free(merch);
        return;
    }

    ioopm_hash_table_insert(
        &wh->ht_n_S,
        string_elem(merch->name),
        ptr_elem(merch)
    );
}
