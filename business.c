#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "hash_table.h"
#include "linked_list_iterator.h"
#include "hash_table_iterator.h"
#include <ctype.h>
#include "utils.h"

typedef struct merch { char *name, *desc; int price; } merch_t;

static ioopm_hash_table_t *merch_ht;   /* namn -> merch_t*, skapas i main med string_hash/string_eq */

static size_t string_hash(elem_t key) {
    size_t h = 5381;
    for (const char *s = key.s; *s; s++) h = h * 33 + (unsigned char)*s;
    return h;
}
static bool string_eq(elem_t a, elem_t b) { return strcmp(a.s, b.s) == 0; }


static bool read_confirm(void) {
    char buf[256];
    read_line("Confirm (y/n): ", buf, sizeof buf);
    return buf[0] == 'Y' || buf[0] == 'y';
}

/* Hjälpfunktioner */
static merch_t *merch_find(char *name) {
    elem_t e;
    return ioopm_hash_table_lookup(merch_ht, string_elem(name), &e) ? e.p : NULL;
}
static merch_t *merch_new(char *name, char *desc, int price) {
    merch_t *m = malloc(sizeof *m);
    *m = (merch_t){ strdup(name), strdup(desc), price };
    return m;
}
static void merch_free(merch_t *m) { free(m->name); free(m->desc); free(m); }

static int cmp_merch(const void *a, const void *b) {
    return strcmp((*(merch_t *const *)a)->name, (*(merch_t *const *)b)->name);
}
/* Alla varor sorterade på namn. Anroparen gör free() på arrayen. */
static merch_t **merch_sorted(size_t *n) {
    *n = ioopm_hash_table_size(merch_ht);
    if (*n == 0) return NULL;
    merch_t **arr = malloc(*n * sizeof *arr);
    size_t i = 0;
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(merch_ht);
    for (; !ioopm_hash_table_iterator_at_end(it); ioopm_hash_table_iterator_advance(it))
        arr[i++] = ioopm_hash_table_iterator_current_value(it).p;
    ioopm_hash_table_iterator_destroy(it);
    qsort(arr, *n, sizeof *arr, cmp_merch);
    return arr;
}

/* A */
void add_merch(void) {
    char *name = ask_question_string("Namn?");
    char *desc = ask_question_string("Beksrivning?");
    int price = ask_question_int("Pris i öre?");

    if (merch_find(name)) 
    { 
        puts("Error: varan finns redan.");
        free(name);
        free(desc);
        return NULL;
    }
    merch_t *m = merch_new(name, desc, price);
    ioopm_hash_table_insert(merch_ht, string_elem(m->name), ptr_elem(m));
}

/* L */
void list_merch(void) {
    size_t n;
    merch_t **arr = merch_sorted(&n);

    for (size_t i = 0; i < n; i++) 
    {

        printf("%s | %s | %d\n", arr[i]->name, arr[i]->desc, arr[i]->price);

        if ((i + 1) % 20 == 0 && i + 1 < n) 
        {
            char *answer = ask_question_string(
                "Vill du fortsätta? (n = meny): "
            );

            if (answer[0] == 'n' || answer[0] == 'N')

            {
                free(answer);
                break;

            }

            free(answer);
        }
    }
    free(arr);
    return NULL;
}

/* D */
void *remove_merch(void *arg) {

    char *name = ask_question_string("Namn på varan: ");

    merch_t *m = merch_find(name);

    if (!m)

    {
        puts("Fel: varan finns inte.");
        free(name);
        return NULL;
    }

    char *name = ask_question_string("Vill du ta bort varan? (y/n): ");

    if (answer[0] != 'y' && answer[0] != Y)

    {
        free(answer);
        free(name);
        return NULL;
    }

    /* senare: remove_locations_of(m); remove_from_all_carts(m); */
    elem_t removed;
    ioopm_hash_table_remove(&merch_ht, string_elem(m->name), &removed);
    
    merch_free(m);

    free(answer);
    free(name);
    return NULL;
}

/* E */
void edit_merch(void) {

    char *old_name = ask_question_string("Nuvarande namn: ");
    char *new_name = ask_question_string("Nytt namn: ");
    char *desc = ask_question_string("Ny beskrivning: ");
    int price = ask_question_int("Nytt pris i öre: ");
    char *answer = ask_question_string("Bekräfta ändringen? (y/n): ");

    /* Hitta varan som ska ändras */
    merch_t *m = merch_find(old_name);

    if (!m)
    {
        puts("Fel: varan finns inte.");
        free(old_name);
        free(new_name);
        free(desc);
        free(answer);
        return NULL;
    }

    /* Kontrollera att det nya namnet inte redan används */
    if (strcmp(old_name, new_name) != 0 && merch_find(new_name))
    {
        puts("Fel: namnet används redan.");
        free(old_name);
        free(new_name);
        free(desc);
        free(answer);
        return NULL;
    }

    /* Uppdatera endast om svaret börjar med Y eller y */
    if (answer[0] != 'Y' && answer[0] != 'y')
    {
        free(old_name);
        free(new_name);
        free(desc);
        free(answer);
        return NULL;
    }

    /* Ta bort den gamla nyckeln från hash-tabellen */
    elem_t removed;
    ioopm_hash_table_remove(
        &merch_ht,
        string_elem(m->name),
        &removed
    );

    /* Uppdatera varans uppgifter */
    free(m->name);
    free(m->desc);

    m->name = strdup(new_name);
    m->desc = strdup(desc);
    m->price = price;

    /* Lägg tillbaka varan med den nya nyckeln */
    ioopm_hash_table_insert(
        &merch_ht,
        string_elem(m->name),
        ptr_elem(m)
    );

    free(old_name);
    free(new_name);
    free(desc);
    free(answer);

    return NULL;
}

/* Städning vid avslut */
void cleanup_merch(void) {
    size_t n;
    merch_t **arr = merch_sorted(&n);

    for (size_t i = 0; i < n; i++) merch_free(arr[i]);
    {
        merch_free(arr[i]);
    }

    free(arr);
    ioopm_hash_table_destroy(&merch_ht);

    return NULL;
}

typedef struct loc { char id[4]; merch_t *m; int qty; } loc_t;

static ioopm_hash_table_t *loc_ht;   /* id (string) -> loc_t*, nyckel = l->id, skapas i main med string_hash/string_eq */

static int cmp_loc(const void *a, const void *b) {
    return strcmp((*(loc_t *const *)a)->id, (*(loc_t *const *)b)->id);
}

/* S */
static void show_stock(void) {
    char name[256];
    read_line("Name: ", name, sizeof name);
    merch_t *m = merch_find(name);
    if (!m) { puts("Error: no such merch."); return; }

    loc_t **arr = malloc((ioopm_hash_table_size(loc_ht) + 1) * sizeof *arr);
    size_t k = 0;
    ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(loc_ht);
    for (; !ioopm_hash_table_iterator_at_end(it); ioopm_hash_table_iterator_advance(it)) {
        loc_t *l = ioopm_hash_table_iterator_current_value(it).p;
        if (l->m == m) arr[k++] = l;
    }
    ioopm_hash_table_iterator_destroy(it);

    qsort(arr, k, sizeof *arr, cmp_loc);
    for (size_t i = 0; i < k; i++) printf("%s: %d\n", arr[i]->id, arr[i]->qty);
    if (k == 0) puts("Not in stock.");
    free(arr);
}