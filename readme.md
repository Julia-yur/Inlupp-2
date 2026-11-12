## Struktur

    businesslogic.c/.h: affärslogiken (warehouse, merch, shelf, cart)
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
- Totalt: Lines 91.0% (896/985), Functions 89.4% (101/113), Branches 74.4% (253/340)
- businesslogic.c: Lines 88.9% (330/371), Functions 88.9% (32/36), Branches 82.1% (184/224)
- businesslogic_tests.c: Lines 98.8% (412/417), Functions 100.0% (35/35), Branches 52.8% (38/72)
- hash_table.c: Lines 95.5% (85/89), Functions 94.4% (17/18), Branches 92.3% (24/26)
- linked_list.c: Lines 63.9% (69/108), Functions 70.8% (17/24), Branches 38.9% (7/18)

## Kompilera och kör
- make bl: bygger testerna för affärslogiken
- make testbl: kör CUnit-testerna
- make memtestbl: kör testerna med valgrind för att kolla efter minnesläckor
- make gdbtestbl: startar testerna i gdb för felsökning
- make ui: bygger användargränssnittet
- make testui: drar igång programmet som vanligt i terminalen
- make testauto: kör programmet automatiskt med testdatan i webstoretest.txt
- make memtestauto: kör det automatiska testet med valgrind
- make covtestbl: kör testerna och kollar kodtäckningen (branch coverage) med gcov
- make covreport: skapar en HTML-rapport över kodtäckningen i mappen html_report