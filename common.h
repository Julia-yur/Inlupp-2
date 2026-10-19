#pragma once
#include <stddef.h>
#include <stdbool.h>

// Helper macros to create elements of a specific underlying type
#define int_elem(x)   ((elem_t) { .i = (x) })
#define uint_elem(x)   ((elem_t) { .u = (x) })
#define bool_elem(x)  ((elem_t) { .b = (x) })
#define float_elem(x)   ((elem_t) { .f = (x) })
#define ptr_elem(x)   ((elem_t) { .p = (x) })
#define string_elem(x) ((elem_t) { .s = (x) })

/// @brief An union of valid values for the hash table and linked list
typedef union elem elem_t;

/// @brief A function that checks if two elements of a given type are equal
/// @param a the first element
/// @param b the second element
/// @return true if the elements are considered equal, false otherwise
typedef bool ioopm_eq_function(elem_t a, elem_t b);

/// @brief A function that calculates a hash for a given element type
/// @param key the key to calculate the hash for
/// @return the calculated hash
typedef size_t ioopm_hash_function(elem_t key);


/// @brief An union of valid values for the hash table and linked list
union elem
{
  int i;
  unsigned int u;
  bool b;
  float f;
  void *p;
  char *s;
};
