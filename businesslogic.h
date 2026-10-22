#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct merch merch_t;
typedef struct shelf shelf_t;
typedef struct warehouse warehouse_t;
typedef struct cart cart_t;

//Warehouse 

//Init a warehouse
warehouse_t *warehouse_create(void);

//Destroy a warehouse
void warehouse_destroy(warehouse_t **wh);

// If warehouse is empty
bool warehouse_is_empty(warehouse_t *wh);

// Counter of merch
size_t warehouse_merch_count(warehouse_t *wh);

// Merchandise

//Lookup if an item is present in the warehouse with its name.
bool warehouse_has_merch(warehouse_t *wh, char *key);


//Insert merch into warehouse.
void warehouse_insert(warehouse_t *wh, char *name, char *desc, int price);

// Remove merch.
bool warehouse_remove_merch(warehouse_t *wh, char *name);

// Edit description or price of merch.
bool warehouse_edit_merch(warehouse_t *wh, char *name, char *new_desc, int new_price);

// Shelves and stock

//Lookup if a storage location is present.
bool warehouse_has_shelf(warehouse_t *wh, char *shelf);

// Place an item on a shelf.
bool warehouse_add_shelf(warehouse_t *wh, char *name, char *shelf, int quantity);

// Get stock balance of an item.
int warehouse_get_stock(warehouse_t *wh, char *name);

// Replenish the stock.
bool warehouse_replenish(warehouse_t *wh, char *shelf, int quantity);

// Carts

// Init a cart.
cart_t *cart_create(int id);
 
// Destroy a cart
void cart_destroy(cart_t **cart);

// Add a merch to a cart.
bool cart_add_merch(cart_t *cart, char *name, int quantity);

// Remove merch from cart.
bool cart_remove_merch(cart_t *cart, char *name);

// Calculate total sum.
int cart_calculate_cost(cart_t *cart, warehouse_t *wh);

// Make a purchase.
bool cart_checkout(cart_t *cart, warehouse_t *wh);