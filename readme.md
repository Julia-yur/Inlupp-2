## Struktur
businesslogic.c/.h: affärslogiken (warehouse, merch, shelf, cart)
user_interface.c: Bygg upp menyer och hantera input från utils.c/.h och passera vidare till backend.
businesslogic_tests.c: CUnit-tester
hash_table.c/.h, linked_list.c/.h (+ iteratorer): generiska datastrukturer

## Datamodell
Ett lager (warehouse) har två hashtabeller: varunamn → vara, och hyllnamn → hylla.
En vara kan ligga på flera hyllor, men en hylla kan bara innehålla en sorts vara.
En varukorg (cart) är ett separat objekt med varunamn och antal. Den påverkar inte lagret förrän cart_checkout anropas.
Typerna är opaka i headerfilen, så anroparen kommer bara åt dem via funktionerna.

## Designval och avgränsningar

Uttag ur lagret (warehouse_remove_stock) sker från hyllorna i den ordning de lades till.
Tomma hyllor (stock 0) behålls och kan fyllas på igen med warehouse_replenish.
cart_checkout och warehouse_remove_stock är atomiska: allt kontrolleras först, och vid misslyckande ändras ingenting.
Allokeringsfel hanteras i de flesta funktioner, men inte för ioopm_hash_table_insert (returnerar void) eller skapande av iteratorer.
Varje struct äger sina strängar (strdup) och frigör dem själv.

## Testteckning
- businesslogic.c: Lines 93.5% (346/370), Functions 100% (36/36), Branches 86.8% (184/212)
- businesslogic_tests.c: Lines 98.9% (440/445), Functions 100.0% (36/36), Branches 52.7% (39/74)

## Kompilera och kör
- make bl: bygger testerna för affärslogiken
- make testbl: kör CUnit-testerna
- make memtestbl: kör testerna med valgrind för att kolla efter minnesläckor
- make gdbtestbl: startar testerna i gdb för felsökning
- make ui: bygger användargränssnittet
- make testui: drar igång programmet som vanligt i terminalen
- make memtestui: drar igång programmet med valgrind
- make testauto: kör programmet automatiskt med testdatan i webstoretest.txt (byt ut webstorestest.txt med valfri testfil)
- make memtestauto: kör det automatiska testet med valgrind
- make covtestbl: kör testerna och kollar kodtäckningen (branch coverage) med gcov
- make covreport: skapar en HTML-rapport över kodtäckningen i mappen html_report


## Kod från tidigare
- hash_table och hash_table iterator kommer från Julias inupp1.
- linked_list, linked_list_iterator och utils kommer från Nils inlupp1 och labb.

### Problem:

- Finns ingen möjlighet för inmatning av en tom string i utils, (i efterhand så bryter det nog mot SoC att ändra i utils för att göra en specifik uppgift. Lösningen borde vara i user_interface.)
- Bugg i utils, is_shelf tillåter annat än bara en stor bokstav följt av två siffror. (strlen < 2) byttes ut till (strlen != 3).