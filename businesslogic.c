#include "businesslogic.h"
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "linked_list.h"
#include "linked_list_iterator.h"
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
    merch_t *merch;
};

struct warehouse
{
    ioopm_hash_table_t *ht_n_S;
    ioopm_hash_table_t *ht_sl_n;
    ioopm_list_t *carts;        //List of our carts to be able to check items reserved in cart.
    int next_cart_id;       
};

typedef struct cart_item
{
    char *name;
    int quantity;
} cart_item_t;

struct cart
{
    int id;
    ioopm_hash_table_t *items;
};

//HASHING FUNKTIONER

/// HASH-FUNKTION TAGEN FRÅN STACK OVERFLOW https://stackoverflow.com/a/7666577, modifierad
static size_t hash_str(elem_t str)
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

static void shelf_destroy(shelf_t *shelf)
{
    if (shelf == NULL)
    {
        return;
    }

    free(shelf->shelf_id);
    free(shelf);
}

static void merch_destroy(merch_t *merch)
{
    if (merch == NULL)
    {
        return;
    }

    free(merch->name);
    free(merch->desc);
    if (merch->locs != NULL)
    {
        ioopm_list_destroy(merch->locs);
    }
    free(merch);
}

static merch_t *find_merch(warehouse_t *wh, char *name)
{
    if (wh == NULL || name == NULL)
    {
        return NULL;
    }

    elem_t result;
    if (!ioopm_hash_table_lookup(wh->ht_n_S, string_elem(name), &result))
    {
        return NULL;
    }
    return result.p;
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
    wh->carts = ioopm_list_create();
    wh->next_cart_id = 1;

    if (wh->ht_n_S == NULL || wh->ht_sl_n == NULL)
    {
        if (wh->ht_n_S != NULL)  ioopm_hash_table_destroy(wh->ht_n_S);
        if (wh->ht_sl_n != NULL) ioopm_hash_table_destroy(wh->ht_sl_n);
        if (wh->carts != NULL) ioopm_list_destroy(wh->carts);
        free(wh);
        return NULL;
    }

    return wh;
}

void warehouse_destroy(warehouse_t *wh) 
{
    if (wh == NULL)
    {
        return;
    }

    ioopm_hash_table_iterator_t *iter =
    ioopm_hash_table_iterator_create(wh->ht_n_S);

    while (!ioopm_hash_table_iterator_at_end(iter))
    {
        elem_t value = ioopm_hash_table_iterator_current_value(iter);
        merch_destroy(value.p);
        ioopm_hash_table_iterator_advance(iter);
    }
    ioopm_hash_table_iterator_destroy(iter);

    // Frigör alla hyllor
    ioopm_hash_table_iterator_t *shelf_iter =
    ioopm_hash_table_iterator_create((wh)->ht_sl_n);

    while (!ioopm_hash_table_iterator_at_end(shelf_iter))
    {
        elem_t value = ioopm_hash_table_iterator_current_value(shelf_iter);
        shelf_destroy(value.p); 
        ioopm_hash_table_iterator_advance(shelf_iter);
    }
    ioopm_hash_table_iterator_destroy(shelf_iter);

    // Förstör hashtabellerna
    ioopm_hash_table_destroy((wh)->ht_n_S);
    ioopm_hash_table_destroy((wh)->ht_sl_n);

    //Frigöt carts
    if (wh->carts != NULL)
    {
        ioopm_list_iterator_t *cart_iter = ioopm_list_iterator_create(wh->carts);
        while (!ioopm_list_iterator_at_end(cart_iter))
        {
            cart_t *c = ioopm_list_iterator_current(cart_iter).p;
            cart_destroy(c);
            ioopm_list_iterator_advance(cart_iter);
        }
        ioopm_list_iterator_destroy(cart_iter);
        ioopm_list_destroy(wh->carts);
    }

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
size_t warehouse_merch_count(warehouse_t *wh)
{
    if (wh == NULL)
    {
        return 0;
    }
    
    return ioopm_hash_table_size(wh->ht_n_S);
}

// Merchandise funktioner

bool warehouse_has_merch(warehouse_t *wh, char *key)
{
    if (wh == NULL || key == NULL)
    {
        return false;
    }

    return ioopm_hash_table_has_key(wh->ht_n_S, string_elem(key));
}

bool warehouse_add_merch(warehouse_t *wh, char *name, char *desc, int price)
{
   if (wh == NULL || name == NULL || desc == NULL || price < 0 || warehouse_has_merch(wh, name))
    {
        return false;
    }

    merch_t *merch = calloc(1, sizeof(merch_t));
    if (merch == NULL)
    {
        return false;
    }

    merch->name = strdup(name);
    merch->desc = strdup(desc);
    merch->price = price;
    merch->locs = ioopm_list_create();

    if (merch->name == NULL || merch->desc == NULL || merch->locs == NULL)
    {
        merch_destroy(merch);   // free(NULL) är ofarligt
        return false;
    }

    ioopm_hash_table_insert(wh->ht_n_S, string_elem(merch->name), ptr_elem(merch));
    return true;
}

bool warehouse_remove_merch(warehouse_t *wh, char *name)
{
    merch_t *merch = find_merch(wh, name);
    if (merch == NULL)
    {
        return false;
    }

    // 1. Ta bort och frigör alla hyllor som tillhör varan
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->locs);
    while (!ioopm_list_iterator_at_end(it))
    {
        shelf_t *shelf = ioopm_list_iterator_current(it).p;

        elem_t removed_shelf;
        // Ta bort ur tabellen INNAN shelf_id frigörs (nyckeln pekar på den strängen)
        ioopm_hash_table_remove(wh->ht_sl_n, string_elem(shelf->shelf_id), &removed_shelf);
        shelf_destroy(shelf);

        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);

    // 2. Ta bort varan ur tabellen INNAN namnet frigörs (samma nyckelskäl)
    elem_t removed;
    ioopm_hash_table_remove(wh->ht_n_S, string_elem(name), &removed);

    // 3. Frigör varan och dess lista
    merch_destroy(removed.p);
    return true;
}

bool warehouse_edit_merch(warehouse_t *wh, char *name, char *new_name, char *new_desc, int new_price)
{
    if (wh == NULL || name == NULL || new_desc == NULL || new_price < 0)
    {
        return false;
    }

    elem_t result;

    if (!ioopm_hash_table_lookup(wh->ht_n_S, string_elem(name), &result))
    {
        return false;
    }

    elem_t removed;

    ioopm_hash_table_remove(wh->ht_n_S, string_elem(name), &removed);

    merch_t *merch = removed.p;

    char *desc_copy = strdup(new_desc);
    if (desc_copy == NULL)
    {
        return false;
    }

    char *name_copy = strdup(new_name);
    if (name_copy == NULL)
    {
        return false;
    }

    free(merch->desc);
    free(merch->name);
    merch->name = name_copy;
    merch->desc = desc_copy;
    merch->price = new_price;

    ioopm_hash_table_insert(wh->ht_n_S, string_elem(merch->name), ptr_elem(merch));

    //DODE: Iterera genom carts och ändra keys för att inte förstöra deras ht. Gör detta med en hjälpfunktion och in liten loop här.

    return true;

}

bool warehouse_lookup_desc(warehouse_t *wh, char *name, char **result)
{
    merch_t *merch = find_merch(wh, name);
    if (merch == NULL || result == NULL)
    {
        return false;
    }
    *result = merch->desc;
    return true;
}

bool warehouse_lookup_price(warehouse_t *wh, char *name, int *result)
{
    merch_t *merch = find_merch(wh, name);
    if (merch == NULL || result == NULL)
    {
        return false;
    }
    *result = merch->price;
    return true;
}

ioopm_hash_table_iterator_t *warehouse_merch_iterator(warehouse_t *wh)
{
    return ioopm_hash_table_iterator_create(wh->ht_n_S);
}

// Shelves and stock

bool warehouse_has_shelf(warehouse_t *wh, char *shelf)
{
    if (wh == NULL || shelf == NULL)
    {
        return false;
    }
    return ioopm_hash_table_has_key(wh->ht_sl_n, string_elem(shelf));

}

bool warehouse_add_shelf(warehouse_t *wh, char *name, char *shelf, int quantity)
{
    if (wh == NULL || name == NULL || shelf == NULL || quantity <= 0)
    {
        return false;
    }

    merch_t *merch = find_merch(wh, name);
    if (merch == NULL)
    {
        return false;
    }

    // Hyllan finns redan?
    elem_t found;
    if (ioopm_hash_table_lookup(wh->ht_sl_n, string_elem(shelf), &found))
    {
        shelf_t *existing = found.p;
        if (existing->merch != merch)
        {
            return false;               // används av en annan vara
        }
        existing->stock += quantity;    // samma vara: fyll på
        return true;
    }

    shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
    if (new_shelf == NULL)
    {
        return false;
    }

    new_shelf->shelf_id = strdup(shelf);
    if (new_shelf->shelf_id == NULL)
    {
        free(new_shelf);
        return false;
    }

    new_shelf->stock = quantity;
    new_shelf->merch = merch;

    ioopm_hash_table_insert(
        wh->ht_sl_n,
        string_elem(new_shelf->shelf_id),
        ptr_elem(new_shelf)
    );

    ioopm_list_append(merch->locs, ptr_elem(new_shelf));

    return true;
}

int warehouse_get_stock(warehouse_t *wh, char *name)
{
    merch_t *merch = find_merch(wh, name);
    if (merch == NULL)
    {
        return 0;
    }

    int total = 0;
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->locs);

    while (!ioopm_list_iterator_at_end(it))
    {
        shelf_t *shelf = ioopm_list_iterator_current(it).p;
        total += shelf->stock;
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);

    return total;
}

bool warehouse_replenish(warehouse_t *wh, char *shelf, int quantity)
{
    if (wh == NULL || shelf == NULL || quantity <= 0)
    {
        return false;
    }

    elem_t found;
    if (!ioopm_hash_table_lookup(wh->ht_sl_n, string_elem(shelf), &found))
    {
        return false;
    }

    ((shelf_t *)found.p)->stock += quantity;
    return true;
}

bool warehouse_remove_stock(warehouse_t *wh, char *name, int quantity)
{
    merch_t *merch = find_merch(wh, name);
    if (merch == NULL || quantity <= 0 || warehouse_get_stock(wh, name) < quantity)
    {
        return false;           // totalen räcker inte: ändra ingenting
    }
    
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(merch->locs);

    while (!ioopm_list_iterator_at_end(it) && quantity > 0)
    {
        shelf_t *shelf = ioopm_list_iterator_current(it).p;
        int take = shelf->stock < quantity ? shelf->stock : quantity;
        shelf->stock -= take;
        quantity -= take;
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);

    return true;
}

ioopm_list_iterator_t *warehouse_stock_iterator(warehouse_t *wh, char *name)
{
    merch_t *merch = find_merch(wh, name);

    return ioopm_list_iterator_create(merch->locs);
}

char *shelf_get_id(shelf_t *shelf)
{
    return shelf->shelf_id;
}

int shelf_get_stock(shelf_t *shelf)
{
    if (shelf == NULL) return 0;
    return shelf->stock;
}

// Carts

cart_t *cart_create(warehouse_t *wh)
{
    cart_t *cart = calloc(1, (sizeof(struct cart)));
    if (cart == NULL)
    {
        return NULL;
    }

    cart->items = ioopm_hash_table_create(hash_str, string_key_eq);
    if (cart->items == NULL)
    {
        free(cart);
        return NULL;
    }
    if (wh != NULL && wh->carts != NULL)
    {
        cart->id = wh->next_cart_id++;
        ioopm_list_append(wh->carts, ptr_elem(cart));
    }
    else
    {
        cart->id = 1;
    }
    return cart;
}

int cart_get_id(cart_t *cart)
{
    if (cart != NULL)
    {
        return cart->id;
    }
    else return 0;
}

cart_t *warehouse_get_cart(warehouse_t *wh, int cart_id)
{
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(wh->carts);

    cart_t *cart = NULL;
    while (!ioopm_list_iterator_at_end(it))
    {
        cart_t *c = ioopm_list_iterator_current(it).p;
        if (c->id == cart_id)
        {
            cart = c;
            break;
        }
        ioopm_list_iterator_advance(it);
    }
    ioopm_list_iterator_destroy(it);
    return cart;
}

bool cart_remove(warehouse_t *wh, int cart_id)
{
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(wh->carts);

    int i = 0;  //behöver index för list_remove
    while (!ioopm_list_iterator_at_end(it))
    {
        cart_t *c = ioopm_list_iterator_current(it).p;
        if (c->id == cart_id)
        {
            ioopm_list_iterator_destroy(it);
            ioopm_list_remove(wh->carts, i);
            cart_destroy(c);
            return true;
        }
        ioopm_list_iterator_advance(it);
        i++;
    }
    ioopm_list_iterator_destroy(it);
    return false;
}

bool cart_remove_merch_ammount(cart_t *cart, char *name, int ammount)
{
    elem_t result;
    if (ammount < 0 || !ioopm_hash_table_lookup(cart->items, string_elem(name), &result))
    {
        return false;
    }
    if (ammount == 0) return true;

    cart_item_t *item = result.p;
    if (ammount >= item->quantity) return cart_remove_merch(cart, name);
    item->quantity -= ammount;
    return true;
}

void cart_destroy(cart_t *cart)
{
    if (cart == NULL)
    {
        return;
    }

    ioopm_hash_table_iterator_t *iter =
        ioopm_hash_table_iterator_create(cart->items);

    while (!ioopm_hash_table_iterator_at_end(iter))
    {
        cart_item_t *item = ioopm_hash_table_iterator_current_value(iter).p;
        free(item->name);
        free(item);
        ioopm_hash_table_iterator_advance(iter);
    }
    ioopm_hash_table_iterator_destroy(iter);

    ioopm_hash_table_destroy(cart->items);
    free(cart);
}

// Byter ut vagnens items mot en redan skapad tom tabell och frigör de gamla.
static void cart_replace_items(cart_t *cart, ioopm_hash_table_t *fresh)
{
    ioopm_hash_table_iterator_t *iter =
        ioopm_hash_table_iterator_create(cart->items);

    if (iter != NULL)
    {
        while (!ioopm_hash_table_iterator_at_end(iter))
        {
            cart_item_t *item = ioopm_hash_table_iterator_current_value(iter).p;
            free(item->name);
            free(item);
            ioopm_hash_table_iterator_advance(iter);
        }
        ioopm_hash_table_iterator_destroy(iter);
    }

    ioopm_hash_table_destroy(cart->items);
    cart->items = fresh;
}

//helper för att kolla hur många av en vara som redan ligger i carts
static int cart_reserved_stock(warehouse_t *wh, char *name)
{
    int reserved = 0;
    ioopm_list_iterator_t *it = ioopm_list_iterator_create(wh->carts);

    while (!ioopm_list_iterator_at_end(it))
    {
        cart_t *c = ioopm_list_iterator_current(it).p;
        elem_t result;
        if (ioopm_hash_table_lookup(c->items, string_elem(name), &result))
        {
            cart_item_t *item = result.p;
            reserved += item->quantity;
        }
        ioopm_list_iterator_advance(it);
    }

    ioopm_list_iterator_destroy(it);
    return reserved;
}

bool cart_add_merch(cart_t *cart, warehouse_t *wh, char *name, int quantity)
{
    if (cart == NULL || wh == NULL || name == NULL || quantity <= 0)
    {
        return false;
    }
    if (!warehouse_has_merch(wh, name))
    {
        return false;
    }
    if (cart_reserved_stock(wh, name) + quantity > warehouse_get_stock(wh, name)) 
    {
        return false;
    }

// Redan i vagnen: öka antalet via pekaren, ingen ny insert
    elem_t found;
    if (ioopm_hash_table_lookup(cart->items, string_elem(name), &found))
    {
        cart_item_t *item = found.p;
        item->quantity += quantity;
        return true;
    }

    // Ny rad
    cart_item_t *item = calloc(1, sizeof(cart_item_t));
    if (item == NULL)
    {
        return false;
    }

    item->name = strdup(name);
    if (item->name == NULL)
    {
        free(item);
        return false;
    }
    item->quantity = quantity;

    ioopm_hash_table_insert(cart->items, string_elem(item->name), ptr_elem(item));
    return true;
}

bool cart_remove_merch(cart_t *cart, char *name)
{
    if (cart == NULL || name == NULL)
    {
        return false;
    }

    elem_t removed;
    if (!ioopm_hash_table_remove(cart->items, string_elem(name), &removed))
    {
        return false;
    }

    cart_item_t *item = removed.p;
    free(item->name);
    free(item);
    return true;
}

bool cart_calculate_cost(cart_t *cart, warehouse_t *wh, int *result)
{
    if (cart == NULL || wh == NULL || result == NULL)
    {
        return false;
    }

    int total = 0;
    ioopm_hash_table_iterator_t *iter =
        ioopm_hash_table_iterator_create(cart->items);

    while (!ioopm_hash_table_iterator_at_end(iter))
    {
        cart_item_t *item = ioopm_hash_table_iterator_current_value(iter).p;

        int price;
        if (!warehouse_lookup_price(wh, item->name, &price))
        {
            ioopm_hash_table_iterator_destroy(iter);
            return false;
        }

        total += price * item->quantity;
        ioopm_hash_table_iterator_advance(iter);
    }

    ioopm_hash_table_iterator_destroy(iter);
    *result = total;
    return true;
}

bool cart_checkout(cart_t *cart, warehouse_t *wh)
{
    if (cart == NULL || wh == NULL)
    {
        return false;
    }

    // Pass 1: kontrollera att allt finns och räcker, ändra ingenting
    ioopm_hash_table_iterator_t *iter =
        ioopm_hash_table_iterator_create(cart->items);
    if (iter == NULL)                                  
    {
        return false;
    }

    while (!ioopm_hash_table_iterator_at_end(iter))
    {
        cart_item_t *item = ioopm_hash_table_iterator_current_value(iter).p;

        if (!warehouse_has_merch(wh, item->name) ||
            warehouse_get_stock(wh, item->name) < item->quantity)
        {
            ioopm_hash_table_iterator_destroy(iter);
            return false;
        }
        ioopm_hash_table_iterator_advance(iter);
    }
    ioopm_hash_table_iterator_destroy(iter);

    // Skapa den nya tomma tabellen nu, så vi kan avbryta innan något ändras
    ioopm_hash_table_t *fresh = ioopm_hash_table_create(hash_str, string_key_eq);
    if (fresh == NULL)
    {
        return false;
    }


    // Pass 2: allt är OK, minska lagret
    iter = ioopm_hash_table_iterator_create(cart->items);
    if (iter == NULL)                                  
    {
        ioopm_hash_table_destroy(fresh);
        return false;
    }
    while (!ioopm_hash_table_iterator_at_end(iter))
    {
        cart_item_t *item = ioopm_hash_table_iterator_current_value(iter).p;
        warehouse_remove_stock(wh, item->name, item->quantity);
        ioopm_hash_table_iterator_advance(iter);
    }
    ioopm_hash_table_iterator_destroy(iter);

    cart_replace_items(cart, fresh);
    return true;
}
