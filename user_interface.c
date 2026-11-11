#include "utils.h"
#include "businesslogic.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


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
    char *desc = ask_question_string("Description: ");
    int price = ask_question_int("Price: ");
    char *con = ask_question_string("Are you sure? y/n\n");
    char confirm = con[0];

    if (confirm == 'y')
    {
        bool success = warehouse_edit_merch(wh, name, desc, price);
        if (success) printf("%s was edited successfully!", name);
        else printf("Fail! %s ...", name); // FIXTHIS: Vi måste kunna byta namnet också
    }
    else printf("Edit aborted and discarded.");
    
    free(name);
    free(desc);
    free(con);
}

void menu_merch_stock(warehouse_t *wh)
{
    (void) wh; // Vi behöver kolla över warehouse_get stock eller inkludera list_iterator i bl.h 
}

void menu_merch_replenish(warehouse_t *wh)
{
    char *name = ask_question_string("Name of item to replenish:");
    char *shelf = ask_question_string("Shelf: ");
    int ammount = ask_question_int("Amount to replenish: ");
    
    bool success = warehouse_replenish(char *name, char *shelf, int ammount);
    //vi behöver utöka warehouse_replenish för att ta in namn också, den ska läsa
    //namn och om hyllan inte finns ska den skaps.
}

void menu_cart_create(warehouse_t *wh)
{
    cart_t *c = cart_create(); // we need to make the cart count increase logic here to keep encapsulation.
}

void menu_cart_remove(warehouse_t *wh)
{
    char *name = ask_question_string("Name of item to remove:");
    bool success = cart(wh, name);
    
    if (success) printf("%s was removed successfully\n", name);
    else printf("Fail, %s is not in database\n", name);

    free(name);
}

void menu_cart_merch_add(warehouse_t *wh)
{

}

void menu_cart_merch_remove(warehouse_t *wh)
{

}

void menu_cart_cost(warehouse_t *wh)
{

}

void menu_cart_checkout(warehouse_t *)
{

}

bool menu_quit(warehouse_t *wh)
{
    char *str = ask_question_string("Are you shure you want to quit? y/n\n");
    char val = str[0];
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
        char val = str[0];
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
            printf("Inte ännu implementerad\n");
        }
        if (val == 'P')
        {
            printf("Inte ännu implementerad\n");
        }
        if (val == 'C')
        {
            printf("Inte ännu implementerad\n");
        }
        if (val == 'R')
        {
            printf("Inte ännu implementerad\n");
        }
        if (val == '+')
        {
            printf("Inte ännu implementerad\n");
        }
        if (val == '-')
        {
            printf("Inte ännu implementerad\n");
        }if (val == '=')
        {
            printf("Inte ännu implementerad\n");
        }
        if (val == 'O')
        {
            printf("Inte ännu implementerad\n");
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