#include "businesslogic.h"
#include "hash_table.h"
#include "linked_list.h"

struct merch
{
    char *name;
    char *desc;
    int price;
    ioopm_list_t *loc;
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