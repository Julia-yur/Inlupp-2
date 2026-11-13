#include "utils.h"
#include "businesslogic.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

struct item
{
    char *name;
    char *desc;
    int price;
    char *shelf;
};

typedef struct item item_t;

bool is_menu_choice(char *str)
{
    return strlen(str) == 1;
}

char *ask_question_menu(void)
{
    char *question = "[A]Add merchendise\n"
                     "[L]List merchendise\n"
                     "[D]Remove merchendise\n"
                     "[E]Edit merchendise\n"
                     "[S]Show stock\n"
                     "[P]Replenish stock\n"
                     "[C]Create Cart\n"
                     "[R]Remove cart\n"
                     "[+]Add to cart\n"
                     "[-]Remove from cart\n"
                     "[=]Calculate cost\n"
                     "[O]Checkout\n"
                     "[Q]Quit\n"
                     "Välj meny: ";

    return ask_question(question, is_menu_choice, (convert_func *)strdup).string_value;
}

void menu_merch_add(warehouse_t *wh)
{
    char *name = ask_question_string("Name: ");

    if (warehouse_has_merch(wh, name))
    {
        printf("%s is already present!", name);
        free(name);
        return;
    }

    char *desc = ask_question_string("Description: ");
    int price = ask_question_int("Price: ");

    warehouse_add_merch(wh, name, desc, price);

    free(name);
    free(desc);
}

void menu_merch_list(warehouse_t *wh)
{
    ioopm_hash_table_iterator_t *iter = warehouse_merch_iterator(wh);

    while (ioopm_hash_table_iterator_at_end(iter) == false)
    {
        char *name = ioopm_hash_table_iterator_current_key(iter).s;
        printf("%s\n", name);
        ioopm_hash_table_iterator_advance(iter);

        //DODGE: Skriv endast var 20e merch och fråga N för nej och ENTER och anykey för nästa sida
    }
    
    ioopm_hash_table_iterator_destroy(iter);
}

void menu_merch_remove(warehouse_t *wh)
{
    char *name = ask_question_string("Name of item to remove:");
    bool success = warehouse_remove_merch(wh, name);
    
    if (success) printf("%s was removed successfully\n", name);
    else printf("Fail, %s is not in database\n", name);

    free(name);
}

void menu_merch_edit(warehouse_t *wh)
{
    char *name = ask_question_string("Name of item to edit:");
    if (!warehouse_has_merch(wh, name))
    {
        printf("%s is not in database, edit aborted and discarded.\n", name);
        free(name);
        return;
    }

    char *new_name = ask_question_string("New name: ");
    if (warehouse_has_merch(wh, new_name) && strcmp(name, new_name) != 0)
    {
        printf("%s is already present in databse, edit aborted and discarded.\n", new_name);
        free(new_name);
        free(name);
        return;
    }

    char *desc = ask_question_string("Description: ");
    int price = ask_question_int("Price: ");
    char *con = ask_question_string("Are you sure? y/n\n");
    char confirm = tolower(con[0]);

    if (confirm == 'y')
    {
        bool success = warehouse_edit_merch(wh, name, new_name, desc, price);
        if (success) printf("%s was edited successfully!\n", name);
        else printf("Price cannot be negative.\n");
    }
    else printf("Edit aborted and discarded.\n");
    
    free(name);
    free(new_name);
    free(desc);
    free(con);
}

void menu_merch_stock(warehouse_t *wh)
{
    char *name = ask_question_string("Name of item to show stock:");
    if (!warehouse_has_merch(wh, name))
    {
        printf("%s is not in database.\n", name);
        free(name);
        return;
    }

    ioopm_list_iterator_t *iter = warehouse_stock_iterator(wh, name);
    if (ioopm_list_iterator_at_end(iter))
    {
        printf("%s has no assigned shelves\n", name);
        ioopm_list_iterator_destroy(iter);
        free(name);
        return;
    }

    while (!ioopm_list_iterator_at_end(iter))
    {
        shelf_t *shelf = ioopm_list_iterator_current(iter).p;
        printf("Shelf: %s, Ammount: %d\n", shelf_get_id(shelf), shelf_get_stock(shelf));
        ioopm_list_iterator_advance(iter);
    }

    ioopm_list_iterator_destroy(iter);
    free(name);
}

void menu_merch_replenish(warehouse_t *wh)
{
    char *shelf = ask_question_shelf("Shelf: ");

    char *name = ask_question_string("Name of item to replenish:");
    if (!warehouse_has_merch(wh, name))
    {
        printf("%s is not in database.\n", name);
        free(name);
        free(shelf);
        return;
    }

    int ammount = 0;
    while (ammount <= 0)
    {
        ammount = ask_question_int("Amount to replenish: ");
        if (ammount <= 0) printf("You cant replenish 0 or a negative number!\n");
    }
    
    bool success = warehouse_add_shelf(wh, name, shelf, ammount);
    if (success)
    {
        printf("Replenished %d of %s on shelf %s\n", ammount, name, shelf);
    }
    else
    {
        printf("%s is already uppupied by another item.\n", name);
    }

    free(name);
    free(shelf);
}

void menu_cart_create(warehouse_t *wh)
{
    cart_t *c = cart_create(wh);
    printf("Cart number %d was created\n", cart_get_id(c));
}

void menu_cart_remove(warehouse_t *wh)
{
    int id = ask_question_int("ID of cart to remove:");
    
    char *str = ask_question_string("Are you shure? y/n\n");
    char val = tolower(str[0]);
    free(str);

    if (val == 'y')
    {
        if (cart_remove(wh, id))
        {
            printf("Cart remove success\n");
        }
        else printf("Cart remove fail, not in database\n");
    }
    else printf("Remove aborted\n");
}

void menu_cart_merch_add(warehouse_t *wh)
{
    int id = ask_question_int("ID of cart:");
    cart_t *cart = warehouse_get_cart(wh, id);
    if (cart == NULL)
    {
        printf("Cart remove fail, not in database\n");
        return;
    }

    char *name = ask_question_string("Name: ");
    if (!warehouse_has_merch(wh, name))
    {
        printf("Cart remove fail, not in database\n");
        free(name);
        return;
    }

    int ammount = ask_question_int("Ammount: ");
    if (cart_add_merch(cart, wh, name, ammount))
    {
        printf("Added %d of %s in cart %d\n", ammount, name, id);
    }
    else
    {
        printf("Fail, invalid ammount or stock is reserved.\n");
    }

    free(name);
}

void menu_cart_merch_remove(warehouse_t *wh)
{
    int id = ask_question_int("ID of cart:");
    cart_t *cart = warehouse_get_cart(wh, id);
    if (cart == NULL)
    {
        printf("Cart remove fail, not in database\n");
        return;
    }

    char *name = ask_question_string("Name: ");
    int ammount = ask_question_int("Ammount: ");
    if (cart_add_merch(cart, wh, name, ammount))
    {
        printf("Removed %d of %s in cart %d\n", ammount, name, id);
    }
    else
    {
        printf("Fail, could not remove.\n");
    }

    free(name);
}

void menu_cart_cost(warehouse_t *wh)
{
    int id = ask_question_int("ID of cart:");
    cart_t *cart = warehouse_get_cart(wh, id);
    if (cart == NULL)
    {
        printf("Cart remove fail, not in database\n");
        return;
    }

    int result = 0;
    if (cart_calculate_cost(cart, wh, &result))
    {
        printf("Cost for cart %d: %d\n", id, result);
    }
    else printf("Price fault?");
}

void menu_cart_checkout(warehouse_t *wh)
{
    int id = ask_question_int("ID of cart:");
    cart_t *cart = warehouse_get_cart(wh, id);
    if (cart == NULL)
    {
        printf("Cart remove fail, not in database\n");
        return;
    }

    if (cart_checkout(cart, wh))
    {
        printf("Cart %d checked out\n", id);
    }
    else printf("Checkout fault?");
}

bool menu_quit(warehouse_t *wh)
{
    char *str = ask_question_string("Are you shure you want to quit? y/n\n");
    char val = tolower(str[0]);
    free(str);

    if (val == 'y')
    {
        warehouse_destroy(wh);
        return true;
    }
    else return false;
}

void event_loop(warehouse_t *wh)
{
    while (true)
    {
        char *str = ask_question_menu();
        char val = toupper(str[0]);
        free(str);
    

        if (val == 'A')
        {
            menu_merch_add(wh);
        }
        if (val == 'L')
        {
            menu_merch_list(wh);
        }
        if (val == 'D')
        {
            menu_merch_remove(wh);
        }
        if (val == 'E')
        {
            menu_merch_edit(wh);
        }
        if (val == 'S')
        {
            menu_merch_stock(wh);
        }
        if (val == 'P')
        {
            menu_merch_replenish(wh);
        }
        if (val == 'C')
        {
            menu_cart_create(wh);
        }
        if (val == 'R')
        {
            menu_cart_remove(wh);
        }
        if (val == '+')
        {
            menu_cart_merch_add(wh);
        }
        if (val == '-')
        {
            menu_cart_merch_remove(wh);
        }
        if (val == '=')
        {
            menu_cart_cost(wh);
        }
        if (val == 'O')
        {
            menu_cart_checkout(wh);
        }
        if (val == 'Q')
        {
            bool quit = menu_quit(wh);

            if (quit) break;
        }
    }
}

int main(void)
{
    warehouse_t *wh = warehouse_create();

    event_loop(wh);
    
    return 0;
}