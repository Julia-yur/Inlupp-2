#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "hash_table.h"
#include "linked_list_iterator.h"
#include "hash_table_iterator.h"
#include <ctype.h>

typedef struct merch { char *name, *desc; int price; } merch_t;

static ioopm_hash_table_t *merch_ht;   /* namn -> merch_t*, skapas i main med string_hash/string_eq */

static size_t string_hash(elem_t key) {
    size_t h = 5381;
    for (const char *s = key.s; *s; s++) h = h * 33 + (unsigned char)*s;
    return h;
}
static bool string_eq(elem_t a, elem_t b) { return strcmp(a.s, b.s) == 0; }

/* Inläsning */
static void read_line(const char *prompt, char *buf, size_t n) {
    printf("%s", prompt); fflush(stdout);
    if (!fgets(buf, (int)n, stdin)) exit(0);
    buf[strcspn(buf, "\r\n")] = '\0';
}
static int read_int(const char *prompt) {
    char buf[256], *end;
    for (;;) {
        read_line(prompt, buf, sizeof buf);
        long v = strtol(buf, &end, 10);
        if (end != buf && *end == '\0') return (int)v;
        puts("Not an integer, try again.");
    }
}
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
static void act_add_merch(void) {
    char name[256], desc[256];
    read_line("Name: ", name, sizeof name);
    read_line("Description: ", desc, sizeof desc);
    int price = read_int("Price: ");
    if (merch_find(name)) { puts("Error: merch already exists."); return; }
    merch_t *m = merch_new(name, desc, price);
    ioopm_hash_table_insert(merch_ht, string_elem(m->name), ptr_elem(m));
}

/* L */
static void act_list(void) {
    size_t n;
    merch_t **arr = merch_sorted(&n);
    char buf[256];
    for (size_t i = 0; i < n; i++) {
        printf("%s | %s | %d\n", arr[i]->name, arr[i]->desc, arr[i]->price);
        if ((i + 1) % 20 == 0 && i + 1 < n) {
            read_line("Continue? (n = menu): ", buf, sizeof buf);
            if (buf[0] == 'N' || buf[0] == 'n') break;
        }
    }
    free(arr);
}

/* D */
static void act_remove_merch(void) {
    char name[256];
    read_line("Name: ", name, sizeof name);
    bool ok = read_confirm();
    merch_t *m = merch_find(name);
    if (!m) { puts("Error: no such merch."); return; }
    if (!ok) return;
    /* senare: remove_locations_of(m); remove_from_all_carts(m); */
    elem_t removed;
    ioopm_hash_table_remove(merch_ht, string_elem(m->name), &removed);
    merch_free(m);
}

/* E */
static void act_edit_merch(void) {
    char old_name[256], new_name[256], desc[256];
    read_line("Old name: ", old_name, sizeof old_name);
    read_line("New name: ", new_name, sizeof new_name);
    read_line("New description: ", desc, sizeof desc);
    int price = read_int("New price: ");
    bool ok = read_confirm();
    merch_t *m = merch_find(old_name);
    if (!m) { puts("Error: no such merch."); return; }
    if (strcmp(old_name, new_name) && merch_find(new_name)) { puts("Error: name already in use."); return; }
    if (!ok) return;
    elem_t removed;
    ioopm_hash_table_remove(merch_ht, string_elem(m->name), &removed);
    free(m->name); free(m->desc);
    *m = (merch_t){ strdup(new_name), strdup(desc), price };
    ioopm_hash_table_insert(merch_ht, string_elem(m->name), ptr_elem(m));
}

/* Städning vid avslut */
static void cleanup_merch(void) {
    size_t n;
    merch_t **arr = merch_sorted(&n);
    for (size_t i = 0; i < n; i++) merch_free(arr[i]);
    free(arr);
    ioopm_hash_table_destroy(merch_ht);
}

typedef struct loc { char id[4]; merch_t *m; int qty; } loc_t;

static ioopm_hash_table_t *loc_ht;   /* id (string) -> loc_t*, nyckel = l->id, skapas i main med string_hash/string_eq */

static int cmp_loc(const void *a, const void *b) {
    return strcmp((*(loc_t *const *)a)->id, (*(loc_t *const *)b)->id);
}

/* S */
static void act_show_stock(void) {
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

int main(void) {
    merch_ht = ioopm_hash_table_create(string_hash, string_eq);
    loc_ht   = ioopm_hash_table_create(string_hash, string_eq);

    char buf[256];
    for (;;) {
        read_line("> ", buf, sizeof buf);
        switch (toupper((unsigned char)buf[0])) {
            case 'A': act_add_merch();  break;
            case 'L': act_list();       break;
            case 'D': act_remove_merch(); break;
            case 'E': act_edit_merch(); break;
            case 'S': act_show_stock(); break;
            case 'Q':
                if (read_confirm()) {
                    cleanup_merch();
                    ioopm_hash_table_destroy(loc_ht);
                    return 0;
                }
                break;
            default: puts("Unknown action.");
        }
    }
}