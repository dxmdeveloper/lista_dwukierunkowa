/**************************************************************
 * Filename: dbl_list.h                                       *
 * File description: Lista w stylu listy z standardu c++      *
 * File created at: 2022-12-15                                *
 * Author: Maciej Bąbolewski (bm53943)                        *
 **************************************************************/
#ifndef _DBL_LIST_HEADER_
#define _DBL_LIST_HEADER_

#include <stdio.h>

// ---- List structures ----
struct _list_node { 
    char text[100]; 
    struct _list_node * prev_ptr; 
    struct _list_node * next_ptr; 
}; 
 
typedef struct { 
    struct _list_node *firstNode;
    struct _list_node *lastNode;
} List; 
 

/// @brief List list = {0} equivalent 
void list_Init     (List *list);
void list_PushBack (List *list, const char * str); 
void list_PushFront(List *list, const char * str);

/// @return 0 on successful insertion, -1 if given index is not in list range.
int  list_Insert   (List *list, unsigned int index, const char * str);
void list_Remove   (List *list, unsigned int index); 
void list_PopBack  (List *list);
void list_PopFront (List *list);

/// @brief removes all nodes and free memory after them.
void list_Flush    (List *list);

// -- List Iterators --
void list_Iterate  (List *list, void (*iter_callback)(unsigned int iteration, const char * nodeContent));

/// @brief extended version of list_Iterate.
/// @param passFile file passed to callback function.
void list_IterateEx(List *list, void (*iter_callback)(unsigned int iteration, const char * nodeContent, FILE * file), FILE * passFile);

/// @brief reversive list iteration.
void list_RIterate (List *list, void (*iter_callback)(unsigned int iteration, const char * nodeContent));


void list_WriteToFile(List *list, FILE * file);
List list_LoadFromFile (FILE * file);

#endif