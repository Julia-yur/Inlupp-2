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
    warehouse_t *wh = ioopm_warehouse_create();

    CU_ASSERT_PTR_NOT_NULL(wh);
    CU_ASSERT_TRUE(ioopm_warehouse_is_empty(wh));

    ioopm_warehouse_destroy(wh);
}

void test_insert_merch_one()
{
    warehouse_t *wh = ioopm_warehouse_create();

    CU_ASSERT_TRUE(ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10));
    CU_ASSERT_FALSE(ioopm_warehouse_is_empty(wh));
    CU_ASSERT_TRUE(ioopm_warehouse_has_merch(wh, "DP"));
    CU_ASSERT_EQUAL(ioopm_warehouse_merch_count(wh), 1);

    ioopm_warehouse_destroy(wh);
}

void test_edit_merch(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    ioopm_warehouse_add_merch(wh, "Lampa", "Bordslampa", 199);

    CU_ASSERT_TRUE(ioopm_warehouse_edit_merch(wh, "Lampa", "lamp", "Golvlampa", 299));

    int price;
    CU_ASSERT_TRUE(ioopm_warehouse_lookup_price(wh, "lamp", &price));
    CU_ASSERT_EQUAL(price, 299);

    char *desc;
    CU_ASSERT_TRUE(ioopm_warehouse_lookup_desc(wh, "lamp", &desc));
    CU_ASSERT_STRING_EQUAL(desc, "Golvlampa");

    CU_ASSERT_FALSE(ioopm_warehouse_edit_merch(wh, "Finns inte", "hello", "x", 1));

    ioopm_warehouse_destroy(wh);
}

void test_insert_merch_duplicate(void)
{
    warehouse_t *wh = ioopm_warehouse_create();

    CU_ASSERT_TRUE(ioopm_warehouse_add_merch(wh, "Lampa", "Bordslampa", 12));
    CU_ASSERT_FALSE(ioopm_warehouse_add_merch(wh, "Lampa", "Badrumslampa", 50));
    CU_ASSERT_EQUAL(ioopm_warehouse_merch_count(wh), 1);

    ioopm_warehouse_destroy(wh);
}

void test_remove_merch(void)
{
    warehouse_t *wh = ioopm_warehouse_create();

    ioopm_warehouse_add_merch(wh, "Lampa", "Bordslampa", 199);
    ioopm_warehouse_add_merch(wh, "Stol", "Köksstol", 399);

    CU_ASSERT_TRUE(ioopm_warehouse_remove_merch(wh, "Lampa"));
    CU_ASSERT_FALSE(ioopm_warehouse_has_merch(wh, "Lampa"));
    CU_ASSERT_TRUE(ioopm_warehouse_has_merch(wh, "Stol"));
    CU_ASSERT_EQUAL(ioopm_warehouse_merch_count(wh), 1);

    CU_ASSERT_FALSE(ioopm_warehouse_remove_merch(wh, "Lampa"));
    CU_ASSERT_FALSE(ioopm_warehouse_remove_merch(wh, NULL));
    CU_ASSERT_FALSE(ioopm_warehouse_remove_merch(NULL, "Stol"));

    ioopm_warehouse_destroy(wh);
}

void test_null_args_basic(void)
{
    warehouse_t *wh = ioopm_warehouse_create();

    CU_ASSERT_EQUAL(ioopm_warehouse_merch_count(NULL), 0);
    CU_ASSERT_TRUE(ioopm_warehouse_is_empty(NULL));
    CU_ASSERT_FALSE(ioopm_warehouse_has_merch(NULL, "Lampa"));
    CU_ASSERT_FALSE(ioopm_warehouse_has_merch(wh, NULL));

    ioopm_warehouse_destroy(wh);
    ioopm_warehouse_destroy(NULL); // ska vara ofarligt
}

void test_add_merch_null_args(void)
{
    warehouse_t *wh = ioopm_warehouse_create();

    CU_ASSERT_FALSE(ioopm_warehouse_add_merch(NULL, "Lampa", "Bordslampa", 10));
    CU_ASSERT_FALSE(ioopm_warehouse_add_merch(wh, NULL, "Bordslampa", 10));
    CU_ASSERT_FALSE(ioopm_warehouse_add_merch(wh, "Lampa", NULL, 10));
    CU_ASSERT_TRUE(ioopm_warehouse_is_empty(wh));

    ioopm_warehouse_destroy(wh);
}

void test_edit_merch_failures(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    ioopm_warehouse_add_merch(wh, "Lampa", "Bordslampa", 199);

    CU_ASSERT_FALSE(ioopm_warehouse_edit_merch(NULL, "Lampa", "lamp", "x", 1));
    CU_ASSERT_FALSE(ioopm_warehouse_edit_merch(wh, NULL, "lamp", "x", 1));
    CU_ASSERT_FALSE(ioopm_warehouse_edit_merch(wh, "Lampa", "lamp", NULL, 1));
    CU_ASSERT_FALSE(ioopm_warehouse_edit_merch(wh, "Lampa", "lamp", "x", -5));

    // Misslyckade edits ska inte ha ändrat något
    int price;
    char *desc;
    CU_ASSERT_TRUE(ioopm_warehouse_lookup_price(wh, "Lampa", &price));
    CU_ASSERT_EQUAL(price, 199);
    CU_ASSERT_TRUE(ioopm_warehouse_lookup_desc(wh, "Lampa", &desc));
    CU_ASSERT_STRING_EQUAL(desc, "Bordslampa");

    ioopm_warehouse_destroy(wh);
}

void test_lookup_failures(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    ioopm_warehouse_add_merch(wh, "Lampa", "Bordslampa", 199);

    int price = -1;
    char *desc = NULL;

    // Vara som inte finns
    CU_ASSERT_FALSE(ioopm_warehouse_lookup_price(wh, "Finns inte", &price));
    CU_ASSERT_FALSE(ioopm_warehouse_lookup_desc(wh, "Finns inte", &desc));

    // NULL-argument
    CU_ASSERT_FALSE(ioopm_warehouse_lookup_price(NULL, "Lampa", &price));
    CU_ASSERT_FALSE(ioopm_warehouse_lookup_price(wh, NULL, &price));
    CU_ASSERT_FALSE(ioopm_warehouse_lookup_price(wh, "Lampa", NULL));
    CU_ASSERT_FALSE(ioopm_warehouse_lookup_desc(NULL, "Lampa", &desc));
    CU_ASSERT_FALSE(ioopm_warehouse_lookup_desc(wh, NULL, &desc));
    CU_ASSERT_FALSE(ioopm_warehouse_lookup_desc(wh, "Lampa", NULL));

    // Utparametrarna ska vara orörda efter misslyckade anrop
    CU_ASSERT_EQUAL(price, -1);
    CU_ASSERT_PTR_NULL(desc);

    ioopm_warehouse_destroy(wh);
}

void test_has_shelf_empty(void)
{
    warehouse_t *wh = ioopm_warehouse_create();

    CU_ASSERT_FALSE(ioopm_warehouse_has_shelf(wh, "A1"));
    CU_ASSERT_FALSE(ioopm_warehouse_has_shelf(wh, NULL));
    CU_ASSERT_FALSE(ioopm_warehouse_has_shelf(NULL, "A1"));

    ioopm_warehouse_destroy(wh);
}

void test_many_merch(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    char name[32];

    // Fler varor än hashtabellens startstorlek, så att kollisioner testas
    for (int i = 0; i < 100; i++)
    {
        snprintf(name, sizeof(name), "vara%d", i);
        CU_ASSERT_TRUE(ioopm_warehouse_add_merch(wh, name, "beskrivning", i));
    }
    CU_ASSERT_EQUAL(ioopm_warehouse_merch_count(wh), 100);

    for (int i = 0; i < 100; i += 2)
    {
        snprintf(name, sizeof(name), "vara%d", i);
        CU_ASSERT_TRUE(ioopm_warehouse_remove_merch(wh, name));
    }
    CU_ASSERT_EQUAL(ioopm_warehouse_merch_count(wh), 50);

    ioopm_warehouse_destroy(wh);
}

void test_cart_add_merch(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_shelf(wh, "DP", "A1", 10);

    CU_ASSERT_TRUE(ioopm_cart_add_merch(cart, wh, "DP", 2));

    int cost = 0;
    CU_ASSERT_TRUE(ioopm_cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 20);

    // Att lägga till i kundvagnen ska inte ändra lagersaldot
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "DP"), 10);

    // ioopm_cart_destroy(cart);
    ioopm_warehouse_destroy(wh);
}

void test_cart_add_same_merch_twice(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_shelf(wh, "DP", "A01", 10);

    CU_ASSERT_TRUE(ioopm_cart_add_merch(cart, wh, "DP", 2));
    CU_ASSERT_TRUE(ioopm_cart_add_merch(cart, wh, "DP", 3));

    int cost = 0;
    CU_ASSERT_TRUE(ioopm_cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 50);

    // ioopm_cart_destroy(cart);
    ioopm_warehouse_destroy(wh);
}

void test_cart_add_failures(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);

    CU_ASSERT_FALSE(ioopm_cart_add_merch(cart, wh, "Coke", 1));
    CU_ASSERT_FALSE(ioopm_cart_add_merch(cart, wh, "DP", 0));
    CU_ASSERT_FALSE(ioopm_cart_add_merch(cart, wh, "DP", -1));
    CU_ASSERT_FALSE(ioopm_cart_add_merch(cart, wh, NULL, 1));
    CU_ASSERT_FALSE(ioopm_cart_add_merch(NULL, wh, "DP", 1));
    CU_ASSERT_FALSE(ioopm_cart_add_merch(cart, NULL, "DP", 1));

    int cost = 0;
    CU_ASSERT_TRUE(ioopm_cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 0);

    // ioopm_cart_destroy(cart);
    ioopm_warehouse_destroy(wh);
}

void test_cart_remove_merch(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_shelf(wh, "DP", "A01", 10);

    CU_ASSERT_TRUE(ioopm_cart_add_merch(cart, wh, "DP", 2));
    CU_ASSERT_TRUE(ioopm_cart_remove_merch(cart, "DP"));

    int cost = 0;
    CU_ASSERT_TRUE(ioopm_cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 0);

    // ioopm_cart_destroy(cart);
    ioopm_warehouse_destroy(wh);
}

void test_cart_calculate_cost_multiple(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_merch(wh, "Coke", "Coca Cola", 15);
    ioopm_warehouse_add_shelf(wh, "DP", "A01", 10);
    ioopm_warehouse_add_shelf(wh, "Coke", "B01", 10);

    CU_ASSERT_TRUE(ioopm_cart_add_merch(cart, wh, "DP", 2));
    CU_ASSERT_TRUE(ioopm_cart_add_merch(cart, wh, "Coke", 3));

    int cost = 0;
    CU_ASSERT_TRUE(ioopm_cart_calculate_cost(cart, wh, &cost));
    CU_ASSERT_EQUAL(cost, 65);

    // ioopm_cart_destroy(cart);
    ioopm_warehouse_destroy(wh);
}

void test_cart_checkout(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_shelf(wh, "DP", "A1", 10);

    CU_ASSERT_TRUE(ioopm_cart_add_merch(cart, wh, "DP", 3));
    CU_ASSERT_TRUE(ioopm_cart_checkout(cart, wh));

    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "DP"), 7);

    int cost = 0;
    CU_ASSERT_PTR_EQUAL(ioopm_warehouse_get_cart(wh, 1), NULL);

    // ioopm_cart_destroy(cart);
    ioopm_warehouse_destroy(wh);
}

void test_add_shelf_and_stock(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    ioopm_warehouse_add_merch(wh, "Kaka", "God", 10);

    CU_ASSERT_TRUE(ioopm_warehouse_add_shelf(wh, "Kaka", "A1", 5));
    CU_ASSERT_TRUE(ioopm_warehouse_add_shelf(wh, "Kaka", "A2", 3));
    CU_ASSERT_TRUE(ioopm_warehouse_has_shelf(wh, "A1"));
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Kaka"), 8);

    ioopm_warehouse_destroy(wh);
}

void test_add_shelf_failures(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    ioopm_warehouse_add_merch(wh, "Kaka", "God", 10);
    ioopm_warehouse_add_merch(wh, "Glass", "Kall", 20);
    ioopm_warehouse_add_shelf(wh, "Kaka", "A1", 5);

    CU_ASSERT_FALSE(ioopm_warehouse_add_shelf(wh, "Finns inte", "B1", 5)); // okänd vara
    CU_ASSERT_FALSE(ioopm_warehouse_add_shelf(wh, "Kaka", "B1", 0));       // quantity <= 0
    CU_ASSERT_FALSE(ioopm_warehouse_add_shelf(wh, "Kaka", "B1", -3));
    CU_ASSERT_FALSE(ioopm_warehouse_add_shelf(wh, "Glass", "A1", 5)); // hyllan tillhör annan vara
    CU_ASSERT_FALSE(ioopm_warehouse_add_shelf(NULL, "Kaka", "B1", 5));
    CU_ASSERT_FALSE(ioopm_warehouse_add_shelf(wh, NULL, "B1", 5));
    CU_ASSERT_FALSE(ioopm_warehouse_add_shelf(wh, "Kaka", NULL, 5));

    CU_ASSERT_FALSE(ioopm_warehouse_has_shelf(wh, "B1"));
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Glass"), 0);
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Kaka"), 5);

    ioopm_warehouse_destroy(wh);
}

void test_add_shelf_same_merch_refills(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    ioopm_warehouse_add_merch(wh, "Kaka", "God", 10);

    CU_ASSERT_TRUE(ioopm_warehouse_add_shelf(wh, "Kaka", "A1", 5));
    CU_ASSERT_TRUE(ioopm_warehouse_add_shelf(wh, "Kaka", "A1", 4)); // samma vara: fyller på
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Kaka"), 9);

    ioopm_warehouse_destroy(wh);
}

void test_replenish(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    ioopm_warehouse_add_merch(wh, "Kaka", "God", 10);
    ioopm_warehouse_add_shelf(wh, "Kaka", "A1", 5);

    CU_ASSERT_TRUE(ioopm_warehouse_replenish(wh, "A1", 10));
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Kaka"), 15);

    CU_ASSERT_FALSE(ioopm_warehouse_replenish(wh, "Finns inte", 5));
    CU_ASSERT_FALSE(ioopm_warehouse_replenish(wh, "A1", 0));
    CU_ASSERT_FALSE(ioopm_warehouse_replenish(wh, "A1", -1));
    CU_ASSERT_FALSE(ioopm_warehouse_replenish(wh, NULL, 5));
    CU_ASSERT_FALSE(ioopm_warehouse_replenish(NULL, "A1", 5));
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Kaka"), 15);

    ioopm_warehouse_destroy(wh);
}

void test_remove_stock(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    ioopm_warehouse_add_merch(wh, "Kaka", "God", 10);
    ioopm_warehouse_add_shelf(wh, "Kaka", "A1", 5);
    ioopm_warehouse_add_shelf(wh, "Kaka", "A2", 3);

    // Uttag över flera hyllor (äldsta först): tömmer A1 och tar 1 från A2
    CU_ASSERT_TRUE(ioopm_warehouse_remove_stock(wh, "Kaka", 6));
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Kaka"), 2);

    // För stort uttag ska inte ändra något
    CU_ASSERT_FALSE(ioopm_warehouse_remove_stock(wh, "Kaka", 3));
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Kaka"), 2);

    CU_ASSERT_FALSE(ioopm_warehouse_remove_stock(wh, "Kaka", 0));
    CU_ASSERT_FALSE(ioopm_warehouse_remove_stock(wh, "Finns inte", 1));
    CU_ASSERT_FALSE(ioopm_warehouse_remove_stock(NULL, "Kaka", 1));

    // Tom hylla finns kvar och kan fyllas på igen
    CU_ASSERT_TRUE(ioopm_warehouse_remove_stock(wh, "Kaka", 2));
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Kaka"), 0);
    CU_ASSERT_TRUE(ioopm_warehouse_has_shelf(wh, "A1"));
    CU_ASSERT_TRUE(ioopm_warehouse_replenish(wh, "A1", 4));
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Kaka"), 4);

    ioopm_warehouse_destroy(wh);
}

void test_get_stock_failures(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    ioopm_warehouse_add_merch(wh, "Kaka", "God", 10);

    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Kaka"), 0); // inga hyllor
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "Finns inte"), 0);
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, NULL), 0);
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(NULL, "Kaka"), 0);

    ioopm_warehouse_destroy(wh);
}

void test_add_merch_negative_price(void)
{
    warehouse_t *wh = ioopm_warehouse_create();

    CU_ASSERT_FALSE(ioopm_warehouse_add_merch(wh, "Kaka", "God", -1));
    CU_ASSERT_TRUE(ioopm_warehouse_is_empty(wh));
    CU_ASSERT_TRUE(ioopm_warehouse_add_merch(wh, "Gratis", "Provbit", 0));

    ioopm_warehouse_destroy(wh);
}

void test_cart_checkout_multiple_shelves(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_shelf(wh, "DP", "A1", 2);
    ioopm_warehouse_add_shelf(wh, "DP", "A2", 5);

    CU_ASSERT_TRUE(ioopm_cart_add_merch(cart, wh, "DP", 4));
    CU_ASSERT_TRUE(ioopm_cart_checkout(cart, wh));
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "DP"), 3);

    // ioopm_cart_destroy(cart);
    ioopm_warehouse_destroy(wh);
}

void test_cart_remove_failures(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    CU_ASSERT_FALSE(ioopm_cart_remove_merch(cart, "Finns inte"));
    CU_ASSERT_FALSE(ioopm_cart_remove_merch(cart, NULL));
    CU_ASSERT_FALSE(ioopm_cart_remove_merch(NULL, "DP"));

    ioopm_warehouse_destroy(wh);
}

void test_cart_checkout_empty_and_null(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    CU_ASSERT_TRUE(ioopm_cart_checkout(cart, wh)); // tom vagn: lyckas utan att göra något
    CU_ASSERT_FALSE(ioopm_cart_checkout(NULL, wh));
    CU_ASSERT_FALSE(ioopm_cart_checkout(cart, NULL));

    // ioopm_cart_destroy(cart);
    ioopm_warehouse_destroy(wh);
}

void test_cart_cost_merch_removed(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_shelf(wh, "DP", "A01", 10);
    ioopm_cart_add_merch(cart, wh, "DP", 2);
    ioopm_warehouse_remove_merch(wh, "DP");

    int cost = 123;
    CU_ASSERT_FALSE(ioopm_cart_calculate_cost(cart, wh, &cost)); // varan finns inte längre
    CU_ASSERT_FALSE(ioopm_cart_calculate_cost(NULL, wh, &cost));
    CU_ASSERT_FALSE(ioopm_cart_calculate_cost(cart, wh, NULL));

    // ioopm_cart_destroy(cart);
    ioopm_warehouse_destroy(wh);
}

void test_warehouse_cart_interact(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *c1 = ioopm_cart_create(wh);
    cart_t *c2 = ioopm_cart_create(wh);

    CU_ASSERT_EQUAL(ioopm_cart_get_id(c1), 1);
    CU_ASSERT_EQUAL(ioopm_cart_get_id(c2), 2);

    CU_ASSERT_PTR_EQUAL(ioopm_warehouse_get_cart(wh, 1), c1)
    CU_ASSERT_PTR_EQUAL(ioopm_warehouse_get_cart(wh, 2), c2)

    CU_ASSERT_TRUE(ioopm_cart_remove(wh, 1));
    CU_ASSERT_PTR_EQUAL(ioopm_warehouse_get_cart(wh, 1), NULL);
    CU_ASSERT_FALSE(ioopm_cart_remove(wh, 1));
    CU_ASSERT_FALSE(ioopm_cart_remove(wh, 123));

    ioopm_warehouse_destroy(wh);
}

void test_cart_remove_ammount(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_shelf(wh, "DP", "A01", 10);
    ioopm_cart_add_merch(cart, wh, "DP", 5);

    CU_ASSERT_TRUE(ioopm_cart_remove_merch_ammount(cart, "DP", 2));
    int result = 0;
    ioopm_cart_calculate_cost(cart, wh, &result);
    CU_ASSERT_EQUAL(result, 30);

    CU_ASSERT_FALSE(ioopm_cart_remove_merch_ammount(cart, "DP", -1));
    CU_ASSERT_FALSE(ioopm_cart_remove_merch_ammount(cart, "Finns inte", 2));

    CU_ASSERT_TRUE(ioopm_cart_remove_merch_ammount(cart, "DP", 3));
    ioopm_cart_calculate_cost(cart, wh, &result);
    CU_ASSERT_EQUAL(result, 0);

    ioopm_warehouse_destroy(wh);
}

void test_cart_reserved_stock(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *c1 = ioopm_cart_create(wh);
    cart_t *c2 = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_shelf(wh, "DP", "A1", 5);

    CU_ASSERT_TRUE(ioopm_cart_add_merch(c1, wh, "DP", 3));
    CU_ASSERT_FALSE(ioopm_cart_add_merch(c2, wh, "DP", 3));
    CU_ASSERT_TRUE(ioopm_cart_add_merch(c2, wh, "DP", 2));
    CU_ASSERT_FALSE(ioopm_cart_add_merch(c1, wh, "DP", 1));

    ioopm_warehouse_destroy(wh);
}

void test_iterators(void)
{
    warehouse_t *wh = ioopm_warehouse_create();

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_merch(wh, "Tro", "cadero", 10);
    ioopm_warehouse_add_shelf(wh, "DP", "A01", 10);

    ioopm_hash_table_iterator_t *mit = ioopm_warehouse_merch_iterator(wh);
    ioopm_list_iterator_t *dp_it = ioopm_warehouse_stock_iterator(wh, "DP");
    ioopm_list_iterator_t *tro_it = ioopm_warehouse_stock_iterator(wh, "Tro");

    CU_ASSERT_FALSE(ioopm_hash_table_iterator_at_end(mit));
    CU_ASSERT_FALSE(ioopm_list_iterator_at_end(dp_it));
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(tro_it));

    shelf_t *s = ioopm_list_iterator_current(dp_it).p;

    CU_ASSERT_STRING_EQUAL(ioopm_shelf_get_id(s), "A01");
    CU_ASSERT_EQUAL(ioopm_shelf_get_stock(s), 10);

    ioopm_hash_table_iterator_advance(mit);
    ioopm_list_iterator_advance(dp_it);

    CU_ASSERT_FALSE(ioopm_hash_table_iterator_at_end(mit));
    CU_ASSERT_TRUE(ioopm_list_iterator_at_end(dp_it));
    
    ioopm_hash_table_iterator_advance(mit);

    CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(mit));

    ioopm_hash_table_iterator_destroy(mit);
    ioopm_list_iterator_destroy(dp_it);
    ioopm_list_iterator_destroy(tro_it);
    ioopm_warehouse_destroy(wh);
}

void test_reserved_merch(void)
{
    warehouse_t *wh = ioopm_warehouse_create();
    cart_t *cart = ioopm_cart_create(wh);
    cart_t *empty_cart = ioopm_cart_create(wh);

    ioopm_warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    ioopm_warehouse_add_shelf(wh, "DP", "A01", 10);

    ioopm_cart_add_merch(cart, wh, "DP", 5);

    CU_ASSERT_TRUE(ioopm_warehouse_edit_merch(wh, "DP", "DPZ", "desc", 5));

    int cost = 0;
    ioopm_cart_calculate_cost(cart, wh, &cost);
    CU_ASSERT_EQUAL(cost, 25);

    ioopm_cart_checkout(cart, wh);
    CU_ASSERT_EQUAL(ioopm_warehouse_get_stock(wh, "DPZ"), 5);

    ioopm_warehouse_destroy(wh);
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
        (CU_add_test(my_test_suite, "Add shelf and stock", test_add_shelf_and_stock) == NULL) ||
        (CU_add_test(my_test_suite, "Add shelf failures", test_add_shelf_failures) == NULL) ||
        (CU_add_test(my_test_suite, "Add shelf same merch refills", test_add_shelf_same_merch_refills) == NULL) ||
        (CU_add_test(my_test_suite, "Replenish", test_replenish) == NULL) ||
        (CU_add_test(my_test_suite, "Remove stock", test_remove_stock) == NULL) ||
        (CU_add_test(my_test_suite, "Get stock failures", test_get_stock_failures) == NULL) ||
        (CU_add_test(my_test_suite, "Add merch negative price", test_add_merch_negative_price) == NULL) ||
        (CU_add_test(my_test_suite, "Cart checkout multiple shelves", test_cart_checkout_multiple_shelves) == NULL) ||
        (CU_add_test(my_test_suite, "Cart remove failures", test_cart_remove_failures) == NULL) ||
        (CU_add_test(my_test_suite, "Cart checkout empty and null", test_cart_checkout_empty_and_null) == NULL) ||
        (CU_add_test(my_test_suite, "Cart cost merch removed", test_cart_cost_merch_removed) == NULL) ||
        (CU_add_test(my_test_suite, "Carts connected to warehouse", test_warehouse_cart_interact) == NULL) ||
        (CU_add_test(my_test_suite, "Carts remove a specific ammount of items", test_cart_remove_ammount) == NULL) ||
        (CU_add_test(my_test_suite, "Carts test reserved stock", test_cart_reserved_stock) == NULL) ||
        (CU_add_test(my_test_suite, "Test iterators", test_iterators) == NULL) ||
        (CU_add_test(my_test_suite, "Edit reserved merch", test_reserved_merch) == NULL) ||
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