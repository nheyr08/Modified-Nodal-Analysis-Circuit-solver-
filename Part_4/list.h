// To use this library especially
// with a custom struct as data in Nodes
// you have to write a compare function!!

// e.g for a
//            List of Integers
// int compare(void* search_data, void* data){
//    // We do not dereference it !
//    int* search = (int*)search_data;
//    int* try    = (int*)data;
//
//    if(search > try){
//        return 1;
//    }else if(search == try){
//        return 0;
//    }else if(search < try){
//        return -1;
//    }
// }
#include <stdlib.h>
#include <string.h>

//#define DEBUG
#define TYPE(a_var) __typeof__(a_var)

typedef struct Node{
    void* data;
    int id; // helpful for debugging
    struct Node* prev;
    struct Node* next;
}Node;

typedef struct List {
    Node* head_node;
    Node* tail_node;
    int size;
    int type;
}List;

// You can declare a type so later on to print based on that choice
List* new_List(void* type){
    List* list = (List*) malloc(sizeof(List));
    if (!list) { perror("Malloc failed!\n"); exit(-2); }
    list->head_node = (Node*) malloc(sizeof(Node));
    list->tail_node = (Node*) malloc(sizeof(Node));

    if(list->head_node == NULL || list->tail_node == NULL){
        printf("\nMALLOC FAILED!\n");
        exit(-3);
    }

    list->head_node = NULL;
    list->tail_node = NULL;
    list->size      = 0;

    if(strcmp(type, "int") == 0){
        list->type = 0;
    }else if (strcmp(type, "char*") == 0){
        list->type = 1;
    }else{
        list->type = -1;
    }

    return list;
}

void print_custom_list(List* list, void (*print_custom_node)(void*)){
    Node* tmp = list->head_node;
    while(tmp != NULL){
        (*print_custom_node)(tmp->data);
        tmp = tmp->next;
    }
}

void print_backwards_custom_list(List* list, void (*print_custom_node)(void*)){
    Node* tmp = list->tail_node;
    while(tmp != NULL){
        (*print_custom_node)(tmp->data);
        tmp = tmp->prev;
    }
}

//void print_list(List* list){
//    Node* tmp = list->head_node;
//    int count = 0;
//    while(tmp != NULL){
//        switch (list->type){
//            case 0:
//                printf("Node(%d): %ld\n", count, (long)tmp->data);
//                break;
//            case 1:
//                printf("Node(%d): %s\n", count, tmp->data);
//                break;
//        }
//        tmp = tmp->next;
//        count++;
//    }
//
//}
//
//void print_backwards(List* list){
////  TODO: test later!
////   WTF is this ? lol
//
////    Node* tmp = list->head_node;
////    while(tmp->next != NULL){
////        tmp = tmp->next;
////    }
//    Node* tmp = list->tail_node;
//    int count = --list->size;
//    // We do not dereference it. Because it contains a value not a reference!!
//    while(tmp != NULL){
//        switch (list->type){
//            case 0:
//                printf("Node(%d): %ld\n", count, (long)tmp->data);
//                break;
//            case 1:
//                printf("Node(%d): %s\n", count, tmp->data);
//                break;
//        }
//        tmp = tmp->prev;
//        count--;
//    }
//}

void free_list(List* list){
    Node* tmp = list->head_node;
    // The list is empty or you lost the head node lol
    if (tmp == NULL) return;
    while(tmp->next != NULL){
        free(tmp->prev);
        tmp = tmp->next;
    }
    free(tmp->prev);
}

Node* find_node(List* list, void* search_data, int (*compare)(void*, void*)){
    Node* tmp = list->head_node;

    while(tmp != NULL){
        if((*compare)(search_data, tmp->data) == 0)
            break;
        tmp = tmp->next;
    }

#ifdef DEBUG
    if(tmp == NULL) printf("\nCouldn't find that element\n");
#endif
    return tmp;
}

Node* find_node_with_index(List* list, int index){
    Node* tmp = list->head_node;

    int count = 0;
    while(tmp != NULL){
        if(count >= index) break;
        tmp = tmp->next;
        count++;
    }

#ifdef DEBUG
    if(tmp == NULL) printf("\nCouldn't find node based on that index\n");
#endif

    return tmp;
}

void remove_n(List* list, Node* tmp){
    if(tmp == list->head_node){
        tmp->next->prev = NULL;
        list->head_node = tmp->next;
    }else if(tmp == list->tail_node){
        tmp->prev->next = NULL;
        list->tail_node = tmp->prev;
    }else if(tmp != NULL){
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
    }else return;

    free(tmp);
    list->size--;
}

// Instead of trying to fix every pointer we just swap
// the pointers of the content of the nodes. This should
// be much faster as well not sure if its limiting tho.
void swap_nodes(Node* node_one, Node* node_two){
    int tmp_id;
    void* tmp_data;

    tmp_id = node_one->id;
    node_one->id = node_two->id;
    node_two->id = tmp_id;
    tmp_data= node_one->data;
    node_one->data = node_two->data;
    node_two->data = tmp_data;
}

// O(n^2) Insertion Sort
void sort_list(List* list, int (*compare)(void*, void*)){
    Node* iterator = list->head_node;
    Node* max, *tmp;

    while(iterator != NULL){
        max = iterator;
        tmp = list->head_node;
        while(tmp != NULL){
            if((*compare)(max->data, tmp->data) == -1)
                swap_nodes(max, tmp);
            tmp = tmp->next;
        }
        iterator = iterator->next;
    }
}

void remove_node_with_index(List* list, int index){
    Node* tmp = find_node_with_index(list, index);
    remove_n(list, tmp);
}

void remove_node(List* list, void* search_data, int (*compare)(void*, void*) ) {
    Node *tmp = find_node(list, search_data, compare);
    remove_n(list, tmp);
}

Node* append_list(List* list, void* data) {
    Node* node = (Node*) malloc(sizeof(Node));
    if (!node) { perror("Malloc failed!\n"); exit(-2); }

    Node* tmp = list->head_node;
    if(tmp != NULL){

        while(tmp->next != NULL)
            tmp = tmp->next;

        node->prev = tmp;
        node->next = NULL;
        tmp->next  = node;
        node->id   = list->size;
        node->data = data;
        list->tail_node = node;
    }else{
        list->head_node = node;
        list->head_node->next = NULL;
        list->head_node->prev = NULL;
        list->head_node->data = data;
        list->head_node->id   = 0;
    }

    list->size++;
    return node;
}

