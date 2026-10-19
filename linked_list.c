#include "linked_list.h"
#include "linked_list_iterator.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

typedef struct node node_t;

struct list
{
    node_t *first; // holds the first node, possibly NULL
    node_t *last; // holds the last node, possibly NULL
    size_t size;
};

struct node
{
    elem_t value; // holds the value
    node_t *next;  // points to the next entry (possibly NULL)
};

/// @brief Destroys a given node in Θ(1) time, freeing its memory.
/// @param node a node
/// @return the value stored in the node
elem_t node_destroy(node_t *node)
{
    elem_t node_value = node->value;
    free(node);
    return node_value;
}

ioopm_list_t *ioopm_list_create()
{
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    return list;
}

/// @brief traverses and destroys a node chain in Θ(n) time
/// @param node a node (can be NULL)
void node_free_chain(node_t *node)
{
    while (node != NULL)
    {
        node_t *removal_node = node;
        node = node->next;
        free(removal_node);
    }
}

void ioopm_list_destroy(ioopm_list_t *list)
{
    node_t *first = list->first;
    node_free_chain(first);
    free(list);
}

node_t *node_create(elem_t value, node_t *next)
{
    node_t *node = calloc(1, sizeof(node_t));
    node->value = value;
    node->next = next;
    return node;
}

size_t ioopm_list_size(ioopm_list_t *list)
{
    return list->size;
}

/// Gets the previous node of a valid index [1,n] for a list of size(n) in Θ(n) time
node_t *list_get_previous_node(ioopm_list_t *list, size_t index)
{
    node_t *node = list->first;
    for (size_t i = 1; i < index; ++i)
    {
        node = node->next;
    }
    return node;
}

/// If index is 0, update list first pointer. If index is last, update list last pointer
void list_node_ensure_first_last(ioopm_list_t *list, size_t index, node_t *node)
{
    if (index == 0)
    {
        list->first = node;
    }
    if (index == (list->size - 1))
    {
        list->last = node;
    }
}

/// Creates a new node and points first and last to the node. (DOES NOT MODIFY SIZE!)
void list_set_only_node(ioopm_list_t *list, elem_t value)
{
    list->first = list->last = node_create(value, NULL);
}

void list_increment_size(ioopm_list_t *list)
{
    ++(list->size);
}

void list_decrement_size(ioopm_list_t *list)
{
    --(list->size);
}

void ioopm_list_prepend(ioopm_list_t *list, elem_t value)
{
    if (ioopm_list_is_empty(list))
    {
        list_set_only_node(list, value);
    }
    else
    {
        node_t *next = list->first;
        list->first = node_create(value, next);
    }
    list_increment_size(list);
}

void ioopm_list_append(ioopm_list_t *list, elem_t value)
{
    if (ioopm_list_size(list) == 0)
    {
        list_set_only_node(list, value);
    }
    else
    {
        node_t *old_last = list->last;
        node_t *new_last = node_create(value, NULL);
        old_last->next = new_last;
        list->last = new_last;
    }
    list_increment_size(list);
}

void ioopm_list_insert(ioopm_list_t *list, size_t index, elem_t value)
{
    if (index == 0)
    {
        ioopm_list_prepend(list, value);
    }
    else
    {
        node_t *previous_node = list_get_previous_node(list, index);
        node_t *new_node = node_create(value, previous_node->next);
        previous_node->next = new_node;
        list_increment_size(list);
        list_node_ensure_first_last(list, index, new_node);
    }
}

elem_t ioopm_list_head(ioopm_list_t *list)
{
    return list->first->value;
}

elem_t ioopm_list_last(ioopm_list_t *list)
{
    return list->last->value;
}

elem_t ioopm_list_get(ioopm_list_t *list, size_t index)
{
    node_t *node = list->first;
    for (size_t i = 0; i < index; ++i)
    {
        node = node->next;
    }
    return node->value;
}

bool ioopm_list_is_empty(ioopm_list_t *list)
{
    return list->size == 0;
}

elem_t ioopm_list_remove(ioopm_list_t *list, size_t index)
{
    if (index == 0)
    {
        node_t *removal_node = list->first;
        node_t *new_first = list->first->next;

        list_node_ensure_first_last(list, index, new_first);
        list_decrement_size(list);

        return node_destroy(removal_node);
    }
    else
    {
        node_t *previous_node = list_get_previous_node(list, index);
        node_t *removal_node = previous_node->next;

        previous_node->next = removal_node->next;

        list_node_ensure_first_last(list, index, previous_node);
        list_decrement_size(list);

        return node_destroy(removal_node);
    }
}


struct list_iterator
{
    node_t *node;
};

ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *l)
{
    ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));
    iter->node = l->first;
    return iter;
}

void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}

bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter)
{
    return iter->node == NULL;
}

void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter)
{
    iter->node = iter->node->next;
}

elem_t ioopm_list_iterator_current(ioopm_list_iterator_t *iter)
{
    return iter->node->value;
}
