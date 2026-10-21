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

bool warehouse_lookup