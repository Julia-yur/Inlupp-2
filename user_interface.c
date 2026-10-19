#include "utils.h"
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

void event_loop(void)
{
    while (true)
    {
        char *str = ask_question_menu();
        char val = str[0];
        free(str);
    

        if (val == 'A')
        {
            printf("Inte ännu implementerad\n");
        }
        if (val == 'L')
        {
            printf("Inte ännu implementerad\n");
        }
        if (val == 'D')
        {
            printf("Inte ännu implementerad\n");
        }
        if (val == 'E')
        {
            printf("Inte ännu implementerad\n");
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
            break;
        }
    }
}

int main(void)
{


    event_loop();
    
    return 0;
}