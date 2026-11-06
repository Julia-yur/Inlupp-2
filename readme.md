#Inlupp2

##Funktioner vi förmodligen behöver, typ i av ordning hur vi borde implementera dem

warehouse create
warehouse destroy
(warehouse is empty)
(warehouse merch count)

add merch
list merch
(has merch)
(get merch)
(total number of merch)
remove merch
edit merch

shelf create (append a list?)
shelf destroy

show stock of merch
(has shelf)
replenish stock of merch(edit stock)

create cart
destroy cart
(show cart)
remove from cart
calculate cost
checkout

warehouse_insert became warehouse_add_merch and was changed from void to bool,
so doubles and errors could be reported.

warehouse_lookup_name was removed, and lookup_desc was changed from merch_t *result to char
**result, because a non transparent type cant be filled in from the outside.

warehouse_destroy was changed from warehouse_t *wh for consistency with the rest.

cart_add_merch got warehouse_t *wh and cart_calculate_cost became bool with out parameter, to report errors.

cart_item_t was implemented so the cart is owning its own keys.

shelf_t needs to get merch_t *merch when the design for the shelves got clear,
and warehouse_remove_stock was missing when cart_checkout was implemented.