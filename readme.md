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

Backend i C för ett varuhus. Hanterar varor, hyllor, lagersaldo och varukorgar.

##Struktur

- businesslogic.c/.h: affärslogiken (warehouse, merch, shelf, cart)
- businesslogic_tests.c: CUnit-tester
- hash_table.c/.h, linked_list.c/.h (+ iteratorer): generiska datastrukturer

##Datamodell

- Ett lager (warehouse) har två hashtabeller: varunamn → vara, och hyllnamn → hylla.
- En vara kan ligga på flera hyllor, men en hylla kan bara innehålla en sorts vara.
- En varukorg (cart) är ett separat objekt med varunamn och antal. Den påverkar inte
  lagret förrän cart_checkout anropas.
- Typerna är opaka i headerfilen, så anroparen kommer bara åt dem via funktionerna.

##Designval och avgränsningar

- Uttag ur lagret (warehouse_remove_stock) sker från hyllorna i den ordning de lades till.
- Tomma hyllor (stock 0) behålls och kan fyllas på igen med warehouse_replenish.
- cart_checkout och warehouse_remove_stock är atomiska: allt kontrolleras först,
  och vid misslyckande ändras ingenting.
- Allokeringsfel hanteras i de flesta funktioner, men inte för
  ioopm_hash_table_insert (returnerar void) eller skapande av iteratorer.
- Varje struct äger sina strängar (strdup) och frigör dem själv.
