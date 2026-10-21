#include "businesslogic.h"
#include "hash_table.h"
#include "linked_list.h"

struct merch
{
    char *name;
    char *desc;
    int price;
    ioopm_list_t *locs;
};

struct shelf
{
    char *shelf;
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

warehouse_t *warehouse_create(void)
{
    return NULL;
}

void warehouse_destroy(warehouse_t *wh)
{
    (void)wh;
}

bool warehouse_has_merch(warehouse_t *wh, char *key)
{
    (void)wh;
    (void)key;
    return false;
}

bool warehouse_has_shelf(warehouse_t *wh, char *shelf)
{
    (void)wh;
    (void)shelf;
    return false;
}

void warehouse_insert(warehouse_t *wh, char *name, char *desc, int price)
{
    (void)wh;
    (void)name;
    (void)desc;
    (void)price;
}