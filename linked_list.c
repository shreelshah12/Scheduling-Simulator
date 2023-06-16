#include <stdlib.h>
#include "linked_list.h"

// Creates and returns a new list
// If compare is NULL, list_insert just inserts at the head
list_t* list_create(compare_fn compare)
{
    list_t* list = malloc(sizeof(list_t));
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;

    list->compare = compare;

    return list;
}

// Destroys a list
void list_destroy(list_t* list)
{
    
   list_node_t* nn = list->head;
   for (int i = 0; i < list->count; i++)
   {
       list_node_t* next_node = nn->next;
       free(nn);
       nn = next_node;
   }
   free(list);


    
    
}

// Returns head of the list
list_node_t* list_head(list_t* list)
{
    /* IMPLEMENT THIS */
    return list->head;
    return NULL;
}

// Returns tail of the list
list_node_t* list_tail(list_t* list)
{
    /* IMPLEMENT THIS */
    return list->tail;
    
}

// Returns next element in the list
list_node_t* list_next(list_node_t* node)
{
    /* IMPLEMENT THIS */
    return node->next;
    
}

// Returns prev element in the list
list_node_t* list_prev(list_node_t* node)
{
    /* IMPLEMENT THIS */
    return node->prev;
    
}

// Returns end of the list marker
list_node_t* list_end(list_t* list)
{
    /* IMPLEMENT THIS */
    // Returns end of the list marker
    return list->tail;
}

// Returns data in the given list node
void* list_data(list_node_t* node)
{
    /* IMPLEMENT THIS */
    return node->data;
}

// Returns the number of elements in the list
size_t list_count(list_t* list)
{
    /* IMPLEMENT THIS */
    return list->count;

    //return 0;
}

// Finds the first node in the list with the given data
// Returns NULL if data could not be found
list_node_t* list_find(list_t* list, void* data)
{
    /* IMPLEMENT THIS */
    for (list_node_t* new_node = list->head; new_node != NULL; new_node = new_node->next)
    {
        if (new_node->data == data){
            return new_node;
        }

    }
    return NULL;    
}

// Inserts a new node in the list with the given data
// Returns new node inserted
list_node_t* list_insert(list_t* list, void* data)
{
    /* IMPLEMENT THIS */
    list_node_t* nn = malloc(sizeof(list_node_t));
    nn->data = data;
    list->count++;


    // If list is empty
    if (list->head == NULL)
    {
        list->head = nn;
        list->tail = nn;
        nn->next = NULL;
        nn->prev = NULL;
        return nn;
    }
    // if list is not empty and compare is NULL
    else if (list->compare == NULL)
    {
        nn->next = list->head;
        nn->prev = NULL;
        list->head->prev = nn;
        list->head = nn;
        return nn;
    }

    else{
        list_node_t* curr = list->head;
        // new_node->data = data;
        while (curr != NULL)
        {
            if (list->compare(data, curr->data) == -1) 
            {
                // insert new_node before the current node
                nn->next = curr;
                nn->prev = curr->prev;
                curr->prev = nn;
                if (nn->prev == NULL)
                {
                    list->head = nn;
                }
                else
                {
                    nn->prev->next = nn;
                }
                return nn;
            }
            else
            {
                //insert new_node after the current node
                if (curr->next == NULL)
                {
                    nn->next = NULL;
                    nn->prev = curr;
                    curr->next = nn;
                    list->tail = nn;
                    return nn;
                }
            }
            
            curr = curr->next;
        }
        return nn;
        
    }
}

// Removes a node from the list and frees the node resources
void list_remove(list_t* list, list_node_t* node)
{
    /* IMPLEMENT THIS */
    if (node == NULL)
    {
        return;
    }
    //if list has one node
    if (list->head == list->tail)
    {
        list->head = NULL;
        list->tail = NULL;
        list->count--;  
        free(node);
    }
    //if node is head
    else if (node == list->head)
    {
        list->head = node->next;
        list->head->prev = NULL;
        list->count--;
        free(node);
        
    }
    //if node is tail
    else if (node == list->tail)
    {
        list->tail = node->prev;
        list->tail->next = NULL;
        list->count--;
        free(node);
    }
    //if node is in the middle
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        list->count--;
        free(node);
        
    }
}

    
        
