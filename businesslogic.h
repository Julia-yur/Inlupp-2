#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "hash_table_iterator.h"
#include "linked_list_iterator.h"

typedef struct merch merch_t;
typedef struct shelf shelf_t;
typedef struct warehouse warehouse_t;
typedef struct cart cart_t;

//Warehouse 

//Init a warehouse
warehouse_t *ioopm_warehouse_create(void);

//Destroy a warehouse
void ioopm_warehouse_destroy(warehouse_t *wh);

// If warehouse is empty
bool ioopm_warehouse_is_empty(warehouse_t *wh);

// Counter of merch
size_t ioopm_warehouse_merch_count(warehouse_t *wh);

// Merchandise

//Lookup if an item is present in the warehouse with its name.
bool ioopm_warehouse_has_merch(warehouse_t *wh, char *key);

//Add merch into warehouse if there is none.
bool ioopm_warehouse_add_merch(warehouse_t *wh, char *name, char *desc, int price);

// Writes a pointer to the description to *result. The string is owned by
// the warehouse: do not free it, and do not use it after edit/remove.
bool ioopm_warehouse_lookup_desc(warehouse_t *wh, char *name, char **result);

bool ioopm_warehouse_lookup_price(warehouse_t *wh, char *name, int *result);

// Remove merch.
bool ioopm_warehouse_remove_merch(warehouse_t *wh, char *name);

// Edit description or price of merch.
bool ioopm_warehouse_edit_merch(warehouse_t *wh, char *name, char *new_name, char *new_desc, int new_price);

//Merch iterator to keep encapsulation
ioopm_hash_table_iterator_t *ioopm_warehouse_merch_iterator(warehouse_t *wh);

// Shelves and stock

//Lookup if a storage location is present.
bool ioopm_warehouse_has_shelf(warehouse_t *wh, char *shelf);

// Place an item on a shelf.
bool ioopm_warehouse_add_shelf(warehouse_t *wh, char *name, char *shelf, int quantity);

// Get stock balance of an item.
int ioopm_warehouse_get_stock(warehouse_t *wh, char *name);

// Replenish the stock.
bool ioopm_warehouse_replenish(warehouse_t *wh, char *shelf, int quantity);

// Removes `quantity` units of the merchandise `name`.
// Stock is taken from the shelves in the order they were added (oldest shelf first).
// Returns false without changing anything if the merchandise does not exist
// or the total stock is insufficient.
bool ioopm_warehouse_remove_stock(warehouse_t *wh, char *name, int quantity);

//Stock iterator to keep encapsulation
ioopm_list_iterator_t *ioopm_warehouse_stock_iterator(warehouse_t *wh, char *name);

//get shelf id, to be used in ui with the iterator
char *ioopm_shelf_get_id(shelf_t *shelf);

//get shelf stock
int ioopm_shelf_get_stock(shelf_t *shelf);

// Carts

// Init a cart.
cart_t *ioopm_cart_create(warehouse_t *wh);
 
// Destroy a cart
void ioopm_cart_destroy(cart_t *cart);

//remove a cart
bool ioopm_cart_remove(warehouse_t *wh, int cart_id);

int ioopm_cart_get_id(cart_t *cart);

//find a cart in warehouse by it's id
cart_t *ioopm_warehouse_get_cart(warehouse_t *wh, int cart_id);

//remove an ammount of merch from cart
bool ioopm_cart_remove_merch_ammount(cart_t *cart, char *name, int ammount);

// Add a merch to a cart.
bool ioopm_cart_add_merch(cart_t *cart, warehouse_t *wh, char *name, int quantity);

// Remove merch from cart.
bool ioopm_cart_remove_merch(cart_t *cart, char *name);

// Calculate total sum.
bool ioopm_cart_calculate_cost(cart_t *cart, warehouse_t *wh, int *result);

// Make a purchase.
bool ioopm_cart_checkout(cart_t *cart, warehouse_t *wh);