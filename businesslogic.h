#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct merch merch_t;
typedef struct shelf shelf_t;
typedef struct warehouse warehouse_t;
typedef struct cart cart_t;

//Init a warehouse
warehouse_t *warehouse_create(void);

//Destroy a warehouse
void warehouse_destroy(warehouse_t *wh);

//Lookup if an item is present in the warehouse with its name.
bool warehouse_has_merch(warehouse_t *wh, char *key);

//Lookup if a storage location is present.
bool warehouse_has_shelf(warehouse_t *wh, char *shelf);

//Insert merch into warehouse.
void warehouse_insert(warehouse_t *wh, char *name, char *desc, int price);