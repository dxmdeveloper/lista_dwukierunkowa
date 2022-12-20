/**************************************************************
 * Filename: dbl_list.c                                       *
 * File description: Lista w stylu listy z standardu c++      *
 * File created at: 2022-12-15                                *
 * Author: Maciej Bąbolewski (bm53943)                        *
 **************************************************************/

#include "dbl_list.h"
#include <stdlib.h>

typedef unsigned int uint;

// --- Static function declarations ---
static struct _list_node * _list_GetNode(List *const list, uint index);
static void my_strncpy(char * dest, const char * src, const size_t num);
static void list_WriteToFileClbkFunc(uint i, const char * line, FILE * file);

// --- Global function definitions ---
void listInit(List * list){ 
    list->firstNode = NULL;
} 
 
void list_PushBack(List *list, const char * str){ 
    struct _list_node *newNode =  calloc(1, sizeof(struct _list_node)); 
    my_strncpy(newNode->text, str, 99); 
    if(list->lastNode == NULL){
        list->firstNode = newNode;
    } else { 
        struct _list_node *lastNode = list->lastNode;
        lastNode->next_ptr = newNode; 
        newNode->prev_ptr = lastNode; 
    } 
    list->lastNode = newNode; 
} 
 
void list_PushFront(List *list, const char * str){ 
    struct _list_node *newNode = calloc(1, sizeof(struct _list_node)); 
    my_strncpy(newNode->text, str, 99); 
    if(list->lastNode == NULL){
        list->lastNode = newNode;
    } else { 
        struct _list_node *firstNode = list->firstNode;
        firstNode->prev_ptr = newNode; 
        newNode->next_ptr = firstNode; 
    } 
    list->firstNode = newNode; 
}

int list_Insert(List *list, uint index, const char * str){
    struct _list_node *prevNode = NULL, 
                      *nextNode = NULL; 
 
    // out of range error
    if((prevNode = _list_GetNode(list, index)) == NULL) 
        return -1; 
        
    if((nextNode = prevNode->next_ptr) == NULL){
        list_PushBack(list, str);
        return 0;
    }
    
    struct _list_node * newNode = malloc(sizeof(struct _list_node));
    newNode->prev_ptr = prevNode;
    newNode->next_ptr = nextNode;
    prevNode->next_ptr = newNode;
    nextNode->prev_ptr = newNode;
    my_strncpy(newNode->text, str, 99);
    return 0;
}

void list_PopBack(List *list){
    struct _list_node * lastNode = list->lastNode;
    if(lastNode == NULL) return;
    if(lastNode->prev_ptr == NULL){
        list->firstNode = NULL;
        list->lastNode = NULL;
    } else {
        lastNode->prev_ptr->next_ptr = NULL;
    }
    free(lastNode);
}

void list_PopFront(List *list){
    struct _list_node * firstNode = list->firstNode;
    if(firstNode == NULL) return;
    if(firstNode->next_ptr == NULL){
        list->firstNode = NULL;
        list->lastNode = NULL;
    } else {
        firstNode->next_ptr->prev_ptr = NULL;
    }
    free(firstNode);
} 
 
void list_Remove(List *list, uint index){
    struct _list_node * node = NULL;
    if(index == 0){list_PopFront(list); return;}

    // out of range index
    if((node = _list_GetNode(list, index)) == NULL) 
        return; 
        
    if(node->next_ptr == NULL){list_PopFront(list); return;}
    if(node->prev_ptr == NULL){list_PopBack(list); return;} //unexpected

    node->prev_ptr->next_ptr = node->next_ptr;
    node->next_ptr->prev_ptr = node->prev_ptr;
    free(node);
}
 
void list_Flush(List *list){
    struct _list_node *node     = NULL, 
                      *nextNode = NULL;
    if(list->firstNode == NULL) return;

    node = list->firstNode;
    do {
        nextNode = node->next_ptr;
        free(node);
        node = nextNode;
    } while(node != NULL);

    list->firstNode = NULL;
    list->lastNode = NULL;
}

void list_Iterate(List *list, void (*iter_callback)(uint iteration, const char * nodeContent)){
    struct _list_node * node = list->firstNode;
    uint i = 0;

    while(node != NULL){
        iter_callback(i++, node->text);
        node = node->next_ptr;
    }
}

void list_IterateEx(List *list, void (*iter_callback)(uint iteration, const char * nodeContent, FILE * file), FILE * passFile){
    struct _list_node * node = list->firstNode;
    uint i = 0;

    while(node != NULL){
        iter_callback(i++, node->text, passFile);
        node = node->next_ptr;
    }
}

void list_RIterate(List *list, void (*iter_callback)(uint iteration, const char * nodeContent)){
    struct _list_node * node = list->lastNode;
    uint i = 0;
    while(node != NULL){
        iter_callback(i++, node->text);
        node = node->prev_ptr;
    }
}


void list_WriteToFile(List *list, FILE * file){
    list_IterateEx(list, list_WriteToFileClbkFunc, file);
}

List list_LoadFromFile(FILE * file){
    List newList = {0};
    char line[100] = {0};

    while((fgets(line, 100, file)) != NULL){
        size_t strlen = 0;
        while (strlen < 100 && line[strlen] != '\0'){
            if(line[strlen++] == '\n'){line[--strlen] = '\0'; break;}
        }

        if(strlen > 0) {
            list_PushBack(&newList, line);
        }
    }

    return newList;
}

// ---- Static function definitions -----
static void list_WriteToFileClbkFunc(uint i, const char * line, FILE * file){
    fprintf(file, "%s\n", line);
}

static void my_strncpy(char * dest, const char * src, const size_t num){ 
    size_t i = 0; 
    do { 
        dest[i] = src[0]; 
    } while(src++[0] != '\0' && i++ < num); 
    dest[i] = '\0';
}

static struct _list_node * _list_GetNode(List *const list, uint index){ 
    struct _list_node * node = list->firstNode; 
    if(node == NULL) return NULL; 
     
    for(uint i = 0; i < index && node != NULL; i++){ 
        node = node->next_ptr; 
    } 
     
    return node; 
}
