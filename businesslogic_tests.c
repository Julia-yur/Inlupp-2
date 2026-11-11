#include <CUnit/Basic.h>
#include "businesslogic.h"
#include <stdio.h>

int init_suite(void)
{
    // Change this function if you want to do something *before* you
    // run a test suite
    return 0;
}

int clean_suite(void)
{
    // Change this function if you want to do something *after* you
    // run a test suite
    return 0;
}

// TESTS ///

void test_create_destroy_wh(void)
{
    warehouse_t *wh = warehouse_create();
    
    CU_ASSERT_PTR_NOT_NULL(wh);
    CU_ASSERT_TRUE(warehouse_is_empty(wh));
   
    warehouse_destroy(wh);
}


void test_insert_merch_one()
{
    warehouse_t *wh = warehouse_create();

    CU_ASSERT_TRUE(warehouse_add_merch(wh, "DP", "Dr Pepper", 10));
    CU_ASSERT_FALSE(warehouse_is_empty(wh));
    CU_ASSERT_TRUE(warehouse_has_merch(wh, "DP"));
    CU_ASSERT_EQUAL(warehouse_merch_count(wh), 1);

    warehouse_destroy(wh);
}

void test_edit_merch(void)
{
      warehouse_t *wh = warehouse_create();
    warehouse_add_merch(wh, "Lampa", "Bordslampa", 199);

    CU_ASSERT_TRUE(warehouse_edit_merch(wh, "Lampa", "lamp", "Golvlampa", 299));

    int price;
    CU_ASSERT_TRUE(warehouse_lookup_price(wh, "lamp", &price));
    CU_ASSERT_EQUAL(price, 299);

    char *desc;
    CU_ASSERT_TRUE(warehouse_lookup_desc(wh, "lamp", &desc));
    CU_ASSERT_STRING_EQUAL(desc, "Golvlampa");

    CU_ASSERT_FALSE(warehouse_edit_merch(wh, "Finns inte", "hello", "x", 1));

    warehouse_destroy(wh);
}

void test_insert_merch_duplicate(void)
{
    warehouse_t *wh = warehouse_create();

    CU_ASSERT_TRUE(warehouse_add_merch (wh, "Lampa", "Bordslampa", 12));
    CU_ASSERT_FALSE(warehouse_add_merch(wh,"Lampa", "Badrumslampa", 50));
    CU_ASSERT_EQUAL(warehouse_merch_count(wh), 1);

    warehouse_destroy(wh);
}

void test_remove_merch(void)
{
    warehouse_t *wh = warehouse_create();

    warehouse_add_merch(wh, "Lampa", "Bordslampa", 199);
    warehouse_add_merch(wh, "Stol", "Köksstol", 399);

    CU_ASSERT_TRUE(warehouse_remove_merch(wh, "Lampa"));
    CU_ASSERT_FALSE(warehouse_has_merch(wh, "Lampa"));
    CU_ASSERT_TRUE(warehouse_has_merch(wh, "Stol"));
    CU_ASSERT_EQUAL(warehouse_merch_count(wh), 1);

    CU_ASSERT_FALSE(warehouse_remove_merch(wh, "Lampa"));
    CU_ASSERT_FALSE(warehouse_remove_merch(wh, NULL));
    CU_ASSERT_FALSE(warehouse_remove_merch(NULL, "Stol"));

    warehouse_destroy(wh);
}

void test_null_args_basic(void)
{
    warehouse_t *wh = warehouse_create();

    CU_ASSERT_EQUAL(warehouse_merch_count(NULL), 0);
    CU_ASSERT_TRUE(warehouse_is_empty(NULL));
    CU_ASSERT_FALSE(warehouse_has_merch(NULL, "Lampa"));
    CU_ASSERT_FALSE(warehouse_has_merch(wh, NULL));

    warehouse_destroy(wh);
    warehouse_destroy(NULL);   // ska vara ofarligt
}

void test_add_merch_null_args(void)
{
    warehouse_t *wh = warehouse_create();

    CU_ASSERT_FALSE(warehouse_add_merch(NULL, "Lampa", "Bordslampa", 10));
    CU_ASSERT_FALSE(warehouse_add_merch(wh, NULL, "Bordslampa", 10));
    CU_ASSERT_FALSE(warehouse_add_merch(wh, "Lampa", NULL, 10));
    CU_ASSERT_TRUE(warehouse_is_empty(wh));

    warehouse_destroy(wh);
}

void test_edit_merch_failures(void)
{
    warehouse_t *wh = warehouse_create();
    warehouse_add_merch(wh, "Lampa", "Bordslampa", 199);

    CU_ASSERT_FALSE(warehouse_edit_merch(NULL, "Lampa", "lamp", "x", 1));
    CU_ASSERT_FALSE(warehouse_edit_merch(wh, NULL, "lamp", "x", 1));
    CU_ASSERT_FALSE(warehouse_edit_merch(wh, "Lampa", "lamp", NULL, 1));
    CU_ASSERT_FALSE(warehouse_edit_merch(wh, "Lampa", "lamp", "x", -5));

    // Misslyckade edits ska inte ha ändrat något
    int price;
    char *desc;
    CU_ASSERT_TRUE(warehouse_lookup_price(wh, "Lampa", &price));
    CU_ASSERT_EQUAL(price, 199);
    CU_ASSERT_TRUE(warehouse_lookup_desc(wh, "Lampa", &desc));
    CU_ASSERT_STRING_EQUAL(desc, "Bordslampa");

    warehouse_destroy(wh);
}

void test_lookup_failures(void)
{
    warehouse_t *wh = warehouse_create();
    warehouse_add_merch(wh, "Lampa", "Bordslampa", 199);

    int price = -1;
    char *desc = NULL;

    // Vara som inte finns
    CU_ASSERT_FALSE(warehouse_lookup_price(wh, "Finns inte", &price));
    CU_ASSERT_FALSE(warehouse_lookup_desc(wh, "Finns inte", &desc));

    // NULL-argument
    CU_ASSERT_FALSE(warehouse_lookup_price(NULL, "Lampa", &price));
    CU_ASSERT_FALSE(warehouse_lookup_price(wh, NULL, &price));
    CU_ASSERT_FALSE(warehouse_lookup_price(wh, "Lampa", NULL));
    CU_ASSERT_FALSE(warehouse_lookup_desc(NULL, "Lampa", &desc));
    CU_ASSERT_FALSE(warehouse_lookup_desc(wh, NULL, &desc));
    CU_ASSERT_FALSE(warehouse_lookup_desc(wh, "Lampa", NULL));

    // Utparametrarna ska vara orörda efter misslyckade anrop
    CU_ASSERT_EQUAL(price, -1);
    CU_ASSERT_PTR_NULL(desc);

    warehouse_destroy(wh);
}

void test_has_shelf_empty(void)
{
    warehouse_t *wh = warehouse_create();

    CU_ASSERT_FALSE(warehouse_has_shelf(wh, "A1"));
    CU_ASSERT_FALSE(warehouse_has_shelf(wh, NULL));
    CU_ASSERT_FALSE(warehouse_has_shelf(NULL, "A1"));

    warehouse_destroy(wh);
}

void test_many_merch(void)
{
    warehouse_t *wh = warehouse_create();
    char name[32];

    // Fler varor än hashtabellens startstorlek, så att kollisioner testas
    for (int i = 0; i < 100; i++)
    {
        snprintf(name, sizeof(name), "vara%d", i);
        CU_ASSERT_TRUE(warehouse_add_merch(wh, name, "beskrivning", i));
    }
    CU_ASSERT_EQUAL(warehouse_merch_count(wh), 100);

    for (int i = 0; i < 100; i += 2)
    {
        snprintf(name, sizeof(name), "vara%d", i);
        CU_ASSERT_TRUE(warehouse_remove_merch(wh, name));
    }
    CU_ASSERT_EQUAL(warehouse_merch_count(wh), 50);

    warehouse_destroy(wh);
}

void test_cart_add_merch(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    warehouse_add_shelf(wh, "DP", "A1", 10);        

    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "DP", 2));

    int cost = 0;
    CU_ASSERT_TRUE(cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 20);

    // Att lägga till i kundvagnen ska inte ändra lagersaldot
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "DP"), 10);

    cart_destroy(cart);
    warehouse_destroy(wh);
}

void test_cart_add_same_merch_twice(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);

    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "DP", 2));
    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "DP", 3));

    int cost = 0;
    CU_ASSERT_TRUE(cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 50);

    cart_destroy(cart);
    warehouse_destroy(wh);
}


void test_cart_add_failures(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);

    CU_ASSERT_FALSE(cart_add_merch(cart, wh, "Coke", 1));
    CU_ASSERT_FALSE(cart_add_merch(cart, wh, "DP", 0));
    CU_ASSERT_FALSE(cart_add_merch(cart, wh, "DP", -1));
    CU_ASSERT_FALSE(cart_add_merch(cart, wh, NULL, 1));
    CU_ASSERT_FALSE(cart_add_merch(NULL, wh, "DP", 1));
    CU_ASSERT_FALSE(cart_add_merch(cart, NULL, "DP", 1));

    int cost = 0;
    CU_ASSERT_TRUE(cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 0);

    cart_destroy(cart);
    warehouse_destroy(wh);
}


void test_cart_remove_merch(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);

    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "DP", 2));
    CU_ASSERT_TRUE(cart_remove_merch(cart, "DP"));

    int cost = 0;
    CU_ASSERT_TRUE(cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 0);

    cart_destroy(cart);
    warehouse_destroy(wh);
}


void test_cart_calculate_cost_multiple(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    warehouse_add_merch(wh, "Coke", "Coca Cola", 15);

    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "DP", 2));
    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "Coke", 3));

    int cost = 0;
    CU_ASSERT_TRUE(cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 65);

    cart_destroy(cart);
    warehouse_destroy(wh);
}


void test_cart_checkout(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    warehouse_add_shelf(wh, "DP", "A1", 10);

    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "DP", 3));
    CU_ASSERT_TRUE(cart_checkout(cart, wh));

    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "DP"), 7);

    int cost = 0;
    CU_ASSERT_TRUE(cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 0);

    cart_destroy(cart);
    warehouse_destroy(wh);
}


void test_cart_checkout_atomic(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    warehouse_add_merch(wh, "Coke", "Coca Cola", 15);

    warehouse_add_shelf(wh, "DP", "A1", 10);
    warehouse_add_shelf(wh, "Coke", "B1", 2);

    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "DP", 2));
    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "Coke", 5));

    // Köpet ska misslyckas eftersom det inte finns tillräckligt med Coke
    CU_ASSERT_FALSE(cart_checkout(cart, wh));

    // Inget lagersaldo ska ha ändrats
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "DP"), 10);
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Coke"), 2);

    // Kundvagnen ska fortfarande innehålla produkterna
    int cost = 0;
    CU_ASSERT_TRUE(cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 95);

    cart_destroy(cart);
    warehouse_destroy(wh);
}

void test_add_shelf_and_stock(void)
{
    warehouse_t *wh = warehouse_create();
    warehouse_add_merch(wh, "Kaka", "God", 10);

    CU_ASSERT_TRUE(warehouse_add_shelf(wh, "Kaka", "A1", 5));
    CU_ASSERT_TRUE(warehouse_add_shelf(wh, "Kaka", "A2", 3));
    CU_ASSERT_TRUE(warehouse_has_shelf(wh, "A1"));
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Kaka"), 8);

    warehouse_destroy(wh);
}

void test_add_shelf_failures(void)
{
    warehouse_t *wh = warehouse_create();
    warehouse_add_merch(wh, "Kaka", "God", 10);
    warehouse_add_merch(wh, "Glass", "Kall", 20);
    warehouse_add_shelf(wh, "Kaka", "A1", 5);

    CU_ASSERT_FALSE(warehouse_add_shelf(wh, "Finns inte", "B1", 5)); // okänd vara
    CU_ASSERT_FALSE(warehouse_add_shelf(wh, "Kaka", "B1", 0));       // quantity <= 0
    CU_ASSERT_FALSE(warehouse_add_shelf(wh, "Kaka", "B1", -3));
    CU_ASSERT_FALSE(warehouse_add_shelf(wh, "Glass", "A1", 5));      // hyllan tillhör annan vara
    CU_ASSERT_FALSE(warehouse_add_shelf(NULL, "Kaka", "B1", 5));
    CU_ASSERT_FALSE(warehouse_add_shelf(wh, NULL, "B1", 5));
    CU_ASSERT_FALSE(warehouse_add_shelf(wh, "Kaka", NULL, 5));

    CU_ASSERT_FALSE(warehouse_has_shelf(wh, "B1"));
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Glass"), 0);
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Kaka"), 5);

    warehouse_destroy(wh);
}

void test_add_shelf_same_merch_refills(void)
{
    warehouse_t *wh = warehouse_create();
    warehouse_add_merch(wh, "Kaka", "God", 10);

    CU_ASSERT_TRUE(warehouse_add_shelf(wh, "Kaka", "A1", 5));
    CU_ASSERT_TRUE(warehouse_add_shelf(wh, "Kaka", "A1", 4));   // samma vara: fyller på
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Kaka"), 9);

    warehouse_destroy(wh);
}

void test_replenish(void)
{
    warehouse_t *wh = warehouse_create();
    warehouse_add_merch(wh, "Kaka", "God", 10);
    warehouse_add_shelf(wh, "Kaka", "A1", 5);

    CU_ASSERT_TRUE(warehouse_replenish(wh, "A1", 10));
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Kaka"), 15);

    CU_ASSERT_FALSE(warehouse_replenish(wh, "Finns inte", 5));
    CU_ASSERT_FALSE(warehouse_replenish(wh, "A1", 0));
    CU_ASSERT_FALSE(warehouse_replenish(wh, "A1", -1));
    CU_ASSERT_FALSE(warehouse_replenish(wh, NULL, 5));
    CU_ASSERT_FALSE(warehouse_replenish(NULL, "A1", 5));
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Kaka"), 15);

    warehouse_destroy(wh);
}

void test_remove_stock(void)
{
    warehouse_t *wh = warehouse_create();
    warehouse_add_merch(wh, "Kaka", "God", 10);
    warehouse_add_shelf(wh, "Kaka", "A1", 5);
    warehouse_add_shelf(wh, "Kaka", "A2", 3);

    // Uttag över flera hyllor (äldsta först): tömmer A1 och tar 1 från A2
    CU_ASSERT_TRUE(warehouse_remove_stock(wh, "Kaka", 6));
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Kaka"), 2);

    // För stort uttag ska inte ändra något
    CU_ASSERT_FALSE(warehouse_remove_stock(wh, "Kaka", 3));
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Kaka"), 2);

    CU_ASSERT_FALSE(warehouse_remove_stock(wh, "Kaka", 0));
    CU_ASSERT_FALSE(warehouse_remove_stock(wh, "Finns inte", 1));
    CU_ASSERT_FALSE(warehouse_remove_stock(NULL, "Kaka", 1));

    // Tom hylla finns kvar och kan fyllas på igen
    CU_ASSERT_TRUE(warehouse_remove_stock(wh, "Kaka", 2));
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Kaka"), 0);
    CU_ASSERT_TRUE(warehouse_has_shelf(wh, "A1"));
    CU_ASSERT_TRUE(warehouse_replenish(wh, "A1", 4));
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Kaka"), 4);

    warehouse_destroy(wh);
}

void test_get_stock_failures(void)
{
    warehouse_t *wh = warehouse_create();
    warehouse_add_merch(wh, "Kaka", "God", 10);

    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Kaka"), 0);        // inga hyllor
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "Finns inte"), 0);
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, NULL), 0);
    CU_ASSERT_EQUAL(warehouse_get_stock(NULL, "Kaka"), 0);

    warehouse_destroy(wh);
}

void test_add_merch_negative_price(void)
{
    warehouse_t *wh = warehouse_create();

    CU_ASSERT_FALSE(warehouse_add_merch(wh, "Kaka", "God", -1));
    CU_ASSERT_TRUE(warehouse_is_empty(wh));
    CU_ASSERT_TRUE(warehouse_add_merch(wh, "Gratis", "Provbit", 0));

    warehouse_destroy(wh);
}

void test_cart_checkout_multiple_shelves(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    warehouse_add_shelf(wh, "DP", "A1", 2);
    warehouse_add_shelf(wh, "DP", "A2", 5);

    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "DP", 4));
    CU_ASSERT_TRUE(cart_checkout(cart, wh));
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "DP"), 3);

    cart_destroy(cart);
    warehouse_destroy(wh);
}

void test_cart_reusable_after_checkout(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    warehouse_add_shelf(wh, "DP", "A1", 10);

    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "DP", 2));
    CU_ASSERT_TRUE(cart_checkout(cart, wh));

    // Vagnen ska fungera som vanligt efter utcheckning (testar cart_replace_items)
    CU_ASSERT_TRUE(cart_add_merch(cart, wh, "DP", 3));
    CU_ASSERT_TRUE(cart_checkout(cart, wh));
    CU_ASSERT_EQUAL(warehouse_get_stock(wh, "DP"), 5);

    cart_destroy(cart);
    warehouse_destroy(wh);
}

void test_cart_remove_failures(void)
{
    cart_t *cart = cart_create(1);

    CU_ASSERT_FALSE(cart_remove_merch(cart, "Finns inte"));
    CU_ASSERT_FALSE(cart_remove_merch(cart, NULL));
    CU_ASSERT_FALSE(cart_remove_merch(NULL, "DP"));

    cart_destroy(cart);
}

void test_cart_checkout_empty_and_null(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    CU_ASSERT_TRUE(cart_checkout(cart, wh));     // tom vagn: lyckas utan att göra något
    CU_ASSERT_FALSE(cart_checkout(NULL, wh));
    CU_ASSERT_FALSE(cart_checkout(cart, NULL));

    cart_destroy(cart);
    warehouse_destroy(wh);
}

void test_cart_cost_merch_removed(void)
{
    warehouse_t *wh = warehouse_create();
    cart_t *cart = cart_create(1);

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    cart_add_merch(cart, wh, "DP", 2);
    warehouse_remove_merch(wh, "DP");

    int cost = 123;
    CU_ASSERT_FALSE(cart_calculate_cost(cart, wh, &cost));  // varan finns inte längre
    CU_ASSERT_FALSE(cart_calculate_cost(NULL, wh, &cost));
    CU_ASSERT_FALSE(cart_calculate_cost(cart, wh, NULL));

    cart_destroy(cart);
    warehouse_destroy(wh);
}

int main()
{
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
    {
        return CU_get_error();
    }

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite = CU_add_suite("Warehouse test suite", init_suite, clean_suite);
    if (my_test_suite == NULL)
    {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // This is where we add the test functions to our test suite.
    // For each call to CU_add_test we specify the test suite, the
    // name or description of the test, and the function that runs
    // the test in question. If you want to add another test, just
    // copy a line below and change the information
    if (
        (CU_add_test(my_test_suite, "Create destroy wh", test_create_destroy_wh) == NULL) ||
        (CU_add_test(my_test_suite, "Insert merch one", test_insert_merch_one) == NULL) ||
        (CU_add_test(my_test_suite, "Insert merch duplicate", test_insert_merch_duplicate) == NULL) ||
        (CU_add_test(my_test_suite, "Remove merch", test_remove_merch) == NULL) ||
        (CU_add_test(my_test_suite, "Edit merch", test_edit_merch) == NULL) ||
        (CU_add_test(my_test_suite, "NULL args basic", test_null_args_basic) == NULL) ||
        (CU_add_test(my_test_suite, "Add merch NULL args", test_add_merch_null_args) == NULL) ||
        (CU_add_test(my_test_suite, "Edit merch failures", test_edit_merch_failures) == NULL) ||
        (CU_add_test(my_test_suite, "Lookup failures", test_lookup_failures) == NULL) ||
        (CU_add_test(my_test_suite, "Has shelf empty", test_has_shelf_empty) == NULL) ||
        (CU_add_test(my_test_suite, "Many merch", test_many_merch) == NULL) ||
        (CU_add_test(my_test_suite, "Cart add merch", test_cart_add_merch) == NULL) ||
        (CU_add_test(my_test_suite, "Cart add same merch twice", test_cart_add_same_merch_twice) == NULL) ||
        (CU_add_test(my_test_suite, "Cart add failures", test_cart_add_failures) == NULL) ||
        (CU_add_test(my_test_suite, "Cart remove merch", test_cart_remove_merch) == NULL) ||
        (CU_add_test(my_test_suite, "Cart calculate cost multiple", test_cart_calculate_cost_multiple) == NULL) ||
        (CU_add_test(my_test_suite, "Cart checkout", test_cart_checkout) == NULL) ||
        (CU_add_test(my_test_suite, "Cart checkout atomic", test_cart_checkout_atomic) == NULL) ||
        (CU_add_test(my_test_suite, "Add shelf and stock", test_add_shelf_and_stock) == NULL) ||
        (CU_add_test(my_test_suite, "Add shelf failures", test_add_shelf_failures) == NULL) ||
        (CU_add_test(my_test_suite, "Add shelf same merch refills", test_add_shelf_same_merch_refills) == NULL) ||
        (CU_add_test(my_test_suite, "Replenish", test_replenish) == NULL) ||
        (CU_add_test(my_test_suite, "Remove stock", test_remove_stock) == NULL) ||
        (CU_add_test(my_test_suite, "Get stock failures", test_get_stock_failures) == NULL) ||
        (CU_add_test(my_test_suite, "Add merch negative price", test_add_merch_negative_price) == NULL) ||
        (CU_add_test(my_test_suite, "Cart checkout multiple shelves", test_cart_checkout_multiple_shelves) == NULL) ||
        (CU_add_test(my_test_suite, "Cart reusable after checkout", test_cart_reusable_after_checkout) == NULL) ||
        (CU_add_test(my_test_suite, "Cart remove failures", test_cart_remove_failures) == NULL) ||
        (CU_add_test(my_test_suite, "Cart checkout empty and null", test_cart_checkout_empty_and_null) == NULL) ||
        (CU_add_test(my_test_suite, "Cart cost merch removed", test_cart_cost_merch_removed) == NULL) ||
        0)

    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }
    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // This is where the tests are actually run!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();
    return CU_get_error();
}