#pragma once
#include <stddef.h>
#include <stdbool.h>
#include "hash_table_iterator.h"
#include "linked_list_iterator.h"

typedef struct merch merch_t;
typedef struct shelf shelf_t;
typedef struct warehouse warehouse_t;
typedef struct cart cart_t;

/**
 * @file businesslogic.h
 * @brief Business logic for a warehouse: merchandise, shelves, stock and shopping carts.
 *
 * Ownership rules:
 *  - Strings passed IN are copied. The caller keeps ownership of its own strings.
 *  - Pointers returned OUT (descriptions, names, shelf ids, carts) belong to the
 *    warehouse. Do not free them, and do not use them after the object they
 *    came from has been edited or removed.
 *  - Iterators returned by the warehouse must be destroyed by the caller.
 */

//Warehouse 

/// @brief Creates an empty warehouse.
/// @return the new warehouse, or NULL if allocation fails.
/// Must be destroyed with warehouse_destroy().
warehouse_t *warehouse_create(void);

/// @brief Destroys a warehouse and and everything it owns: all merchandise, shelves
/// and all carts created with cart_create(wh).
/// @param wh the warehouse (NULL is allowed and does nothing)
/// @warning Do not call cart_destroy() on carts created with cart_create(wh);
void warehouse_destroy(warehouse_t *wh);

/// @brief Checks whether the warehouse conatins any merchandise.
/// @param wh the warehouse
/// @return true if there is no merchandise (or wh is NULL) 
bool warehouse_is_empty(warehouse_t *wh);

/// @brief Counts the different kinds of merchandise in the warehouse.
/// @param wh the warehouse
/// @return the number of merchandise entries, or NULL if wh is 0
size_t warehouse_merch_count(warehouse_t *wh);

// Merchandise   

/// @brief Checks whether merchandise with the given name exists.
/// @param wh the warehouse
/// @param the merhcandise name
/// @return true if it exists, false otherwise (also if an argument is NULL)
bool warehouse_has_merch(warehouse_t *wh, char *key);

/// @brief Adds new merchandise with no shelves
/// @param wh the warehouse
/// @param name the unique name
/// @param desc the description
/// @param price the price (must be >= 0)
/// @return true if added; false if an argument is NULL, price if negative, 
///         name is already in use or allocation fails.
bool warehouse_add_merch(warehouse_t *wh, char *name, char *desc, int price);

/// @brief Looks up the description of a merchandise
/// @param wh the warehouse
/// @param name the unique name
/// @param result writes a pointer to the description 
/// @return true if found, otherwise false
/// @note The string is owned by the warehouse: do not free it
///       and do not use it after edit/remove.
bool warehouse_lookup_desc(warehouse_t *wh, char *name, char **result);

/// @brief Looks up the price of a merchandise
/// @param wh the warehouse
/// @param name the merchandise name
/// @param result writes a pointer to the price
/// @return true if found, otherwise false
bool warehouse_lookup_price(warehouse_t *wh, char *name, int *result);

/// @brief Removes merchandise together with all its shelves
/// @param wh the warehouse
/// @param name the mechandise name
/// @return true if removed, false otherwise
bool warehouse_remove_merch(warehouse_t *wh, char *name);

/// @brief Changes the name, description and price of a merch. 
///        Shelves and stock follows along
/// @param wh the warehouse
/// @param name current name
/// @param new_name the new changed name
/// @param new_desc the new description of the merch
/// @param new_price the new price (must be >=0)
/// @return true if edited; false if NULL, the price is negative, 
///         merchandise does not exist, or allocation fails
bool warehouse_edit_merch(warehouse_t *wh, char *name, char *new_name, char *new_desc, int new_price);

/// @brief Creates an iterator over the merchandises name
/// @pre wh is not NULL
/// @param wh the warehouse
/// @return the iterator, or NULL if allocation fails. The caller must call
///         ioopm_has_table_iteration_destroy() on it.
ioopm_hash_table_iterator_t *warehouse_merch_iterator(warehouse_t *wh);

// Shelves and stock

/// @brief Looks up whether a storage location is present in shelf
/// @param wh the warehouse
/// @param shelf the shelf id
/// @return true if shelf exists, false otherwise (or if argument is NULL)
bool warehouse_has_shelf(warehouse_t *wh, char *shelf);

/// @brief  Puts `quantity` units of a merchandise on a shelf. Creates the shelf
///        if it does not exist; otherwise adds to the stock already there.
/// @param wh the warehouse
/// @param name the merchandise name
/// @param shelf the shelf id
/// @param quantity number of units, must be > 0
/// @return true if it succeeds, false if the merchandise does not exist, quantity
///         is not positive, allocation fails or if shelf holds a different merchandise.
bool warehouse_add_shelf(warehouse_t *wh, char *name, char *shelf, int quantity);

/// @brief Gets the total stock of a merchandise over all its shelves.
/// @param wh the warehouse
/// @param name the merchandise name
/// @return the total stock, 0 if merchandise does not exist or has no shelves
int warehouse_get_stock(warehouse_t *wh, char *name);

/// @brief Adds quantity units to an existing shelf
/// @param wh the warehouse
/// @param shelf the shelf id
/// @param quantity the number of units, must be > 0
/// @return true if shelf exists, false if shelf does not exist,
///         number of units is negative or 0
bool warehouse_replenish(warehouse_t *wh, char *shelf, int quantity);

/// @brief Removes `quantity` units of the merchandise `name`.
///         Stock is taken from the shelves in the order they were added (oldest shelf first).
/// @param wh the warehouse
/// @param name the merchandise name
/// @param quantity number of units, must be > 0
/// @return true if merchandise exits, false if merchandise does not exist,
///         quantity is not positive or total stock is unsufficient
bool warehouse_remove_stock(warehouse_t *wh, char *name, int quantity);

/// @brief Creates an iterator ove the shelves and the merchandise (oldest first)
///        Each shelf is an shelf_t. 
/// @pre the warehouse is NOT null and merch exists
/// @param wh the warehouse
/// @param name the merchandise name 
/// @return the iterator, or NULL on allocation failure. The caller must call
///         ioopm_list_iterator_destroy() on it.
/// @note do not change the warehouse while iterating
ioopm_list_iterator_t *warehouse_stock_iterator(warehouse_t *wh, char *name);

/// @brief Gets the id of a shelf, for use with warehouse_stock_iterator.
/// @return the shelf id, owned by the shelf; do not free it
char *shelf_get_id(shelf_t *shelf);

/// @brief Gets the number of units on a shelf, 
/// @return the stock, o if shelf is NULL
int shelf_get_stock(shelf_t *shelf);

// Carts

/// @brief Creates an empty cart. 
///        With a warehouse, the cart gets a unique id (1, 2, 3, ...), is
///        registered in the warehouse and is owned by it. With wh == NULL 
///        a standalone cart with id 1 created, owned by the caller.
/// @param wh the warehouse, or NULL
/// @return the cart, or NULL if allocation fails
cart_t *cart_create(warehouse_t *wh);
 
/// @brief Destroys a standalone cart.
/// @param cart the cart (NULL is allowed and does nothing)
/// @return  Never use this on a cart registered in a warehouse; use
///          cart_remove() or let warehouse_destroy() free it.
void cart_destroy(cart_t *cart);

/// @brief Removes a cart from the warehouse and destroys it.
///        Units the cart had reserved become available again.
/// @pre wh is not NULL
/// @param wh the warehouse
/// @param cart_id id of the cart
/// @return true if removed, false if no cart has that id
bool cart_remove(warehouse_t *wh, int cart_id);

/// @brief Gets the id of a cart.
/// @return the id, 0 if cart is NULL
int cart_get_id(cart_t *cart);

/// @brief Finds a cart in warehouse by its id
/// @param wh the warehouse
/// @param cart_id the id of a cart
/// @return the cart, or NULL if not found. The cart is owned by its warehouse
cart_t *warehouse_get_cart(warehouse_t *wh, int cart_id);

/// @brief Removes an ammount of merch from cart
/// @param cart the cart
/// @param name the merchandise name
/// @param ammount number of units, must be >= 0
/// @return true of success, false if amount is negative or if merchandise
///         is not in cart
bool cart_remove_merch_ammount(cart_t *cart, char *name, int ammount);

/// @brief Adds merchandise to a cart and reserves the units.
///        If the merchandise is already in the cart, its quantity is increased.
///        The stock in the warehouse is not changed until the cart is checked out.
/// @param cart the cart
/// @param wh the warehouse
/// @param name name of the merchandise
/// @param quantity number of units, must be > 0
/// @return true on success; false if an argument is NULL, quantity is not
///         positive, the merchandise does not exist, or there is not enough
///         unreserved stock (units already in any cart count as reserved)
bool cart_add_merch(cart_t *cart, warehouse_t *wh, char *name, int quantity);


/// @brief Removes a merchandise item from a cart.
///        Removes the merchandise with the given name from the cart
///        and frees the memory allocated for the cart item.
/// @param cart The shopping cart to remove the merchandise from.
/// @param name The name of the merchandise to remove.
/// @return true if the merchandise was successfully removed, false otherwise.
bool cart_remove_merch(cart_t *cart, char *name);


/// @brief Renames a merchandise item in a shopping cart.
///         Updates the name of a merchandise item after it has been renamed
///         in the warehouse. If an item with the new name already exists
///         in the cart, their quantities are merged.
/// @param cart the shopping cart to update.
/// @param old_name the current name of the merchandise.
/// @param new_name the new name of the merchandise.
/// @return true if the cart was updated or did not contain the merchandise false otherwise.
bool cart_rename_merch(cart_t *cart, char *old_name, char *new_name);

/// @brief Calculates the total price of the cart.
/// @param cart the cart
/// @param wh the warehouse
/// @param result set to the total; untouched on failure
/// @return true on success; false if an argument is NULL or a merchandise in
///         the cart no longer exists
bool cart_calculate_cost(cart_t *cart, warehouse_t *wh, int *result);

/// @brief Buys everything in the cart: reduces the stock and empties the cart.
///        All-or-nothing: if anything fails, nothing is changed.
/// @param cart the cart (stays valid and registered afterwards)
/// @param wh the warehouse
/// @return true on success; false if an argument is NULL, a merchandise no
///         longer exists, or the stock is insufficient
bool cart_checkout(cart_t *cart, warehouse_t *wh);
