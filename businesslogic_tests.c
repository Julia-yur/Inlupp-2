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

    CU_ASSERT_TRUE(warehouse_edit_merch(wh, "Lampa", "Golvlampa", 299));

    int price;
    CU_ASSERT_TRUE(warehouse_lookup_price(wh, "Lampa", &price));
    CU_ASSERT_EQUAL(price, 299);

    char *desc;
    CU_ASSERT_TRUE(warehouse_lookup_desc(wh, "Lampa", &desc));
    CU_ASSERT_STRING_EQUAL(desc, "Golvlampa");

    CU_ASSERT_FALSE(warehouse_edit_merch(wh, "Finns inte", "x", 1));

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

    CU_ASSERT_FALSE(warehouse_edit_merch(NULL, "Lampa", "x", 1));
    CU_ASSERT_FALSE(warehouse_edit_merch(wh, NULL, "x", 1));
    CU_ASSERT_FALSE(warehouse_edit_merch(wh, "Lampa", NULL, 1));
    CU_ASSERT_FALSE(warehouse_edit_merch(wh, "Lampa", "x", -5));

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