#include <CUnit/Basic.h>
#include "businesslogic.h"

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
    CU_ASSERT_EQUAL(warehouse_merch_count, 1);
    CU_ASSERT_TRUE(warehouse_has_merch(wh, "DP"));

    warehouse_destroy(wh);
}

void test_insert_merch_two()
{
    warehouse_t *wh = warehouse_create();

    CU_ASSERT_TRUE(warehouse_add_merch(wh, "DP", "Dr Pepper", 10));
    CU_ASSERT_FALSE(warehouse_add_merch(wh, "DP", "Dr Pepper", 10));
    CU_ASSERT_FALSE(warehouse_is_empty(wh));
    CU_ASSERT_EQUAL(warehouse_merch_count, 1);
    CU_ASSERT_TRUE(warehouse_has_merch(wh, "DP"));

    warehouse_destroy(wh);
}

void test_insert_merch_remove()
{
    warehouse_t *wh = warehouse_create();

    CU_ASSERT_TRUE(warehouse_add_merch(wh, "DP", "Dr Pepper", 10));
    CU_ASSERT_TRUE(warehouse_remove_merch(wh, "DP"));

    CU_ASSERT_TRUE(warehouse_is_empty(wh));
    CU_ASSERT_EQUAL(warehouse_merch_count, 0);

    warehouse_destroy(wh);
}

void test_merch_lookup()
{
    warehouse_t *wh = warehouse_create();

    warehouse_add_merch(wh, "DP", "Dr Pepper", 10);
    CU_ASSERT_FALSE(warehouse_is_empty(wh));
    CU_ASSERT_EQUAL(warehouse_merch_count, 1);
    
    char *name;
    char *desc;
    int *price;

    CU_ASSERT_TRUE(warehouse_lookup_name(wh, "DP", &name));
    CU_ASSERT_TRUE(warehouse_lookup_desc(wh, "DP", &desc));
    CU_ASSERT_TRUE(warehouse_lookup_price(wh, "DP", &price));

    CU_ASSERT_STRING_EQUAL(name, "DP");
    CU_ASSERT_STRING_EQUAL(desc, "Dr Pepper");
    CU_ASSERT_EQUAL(price, 10);

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
    CU_pSuite my_test_suite = CU_add_suite("Linked list test suite", init_suite, clean_suite);
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
        (CU_add_test(my_test_suite, "Insert one merch", test_insert_merch_one) == NULL) ||
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