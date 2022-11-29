#include <string.h>
#include <math.h>
#include "net_node_class.h"

//#define SIMPLE_HASH
#define DJB2_HASH
//#define DEBUG
//#define QUAD_PROBING_HASH

#define HASH_TABLE_SIZE 17

// djb2
//this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c.
// another version of this algorithm (now favored by bernstein) uses
// xor: hash(i) = hash(i - 1) * 33 ^ str[i];
// the magic of number 33 (why it works better than many other constants, prime or not) has never been adequately explained.
#ifdef DJB2_HASH
int hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;

    for (int i = 0; i < strlen(str); i++){
        c = str[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    hash = hash % HASH_TABLE_SIZE;

#ifdef DEBUG
    printf("str: %s hash: %lu\n", str, hash);
#endif

    return (int) hash;
}
#endif

// If we get NULL back means there is not such node in that list
// so simple return True aka 1
int is_it_new_node(const char* str, List* hash_list){
    // We search the list of with the id of the node we got.
    // This Node struct is completely different from the NodeInfo.
    // compare_node_names function is in "net_node_class"
    Node* node = find_node(hash_list, (void*) str, compare_node_names);
    if (node == NULL)  return 1;
    else               return 0;
}

#ifdef SIMPLE_HASH
int hash_function(char* str){
    int raw_value = 0;
    for(int i = 0; i < strlen(str); i++){
        raw_value += str[i];
    }
    int hash_value = raw_value % HASH_TABLE_SIZE;

#ifdef DEBUG
    printf("str: %s hash: %d\n", str, hash_value);
#endif

    return hash_value;
}
#endif

#ifdef QUAD_PROBING_HASH
int hash_function(char* str){
    unsigned int g = 33;
    unsigned long long raw_hash = 0;

    for(int i = 0; i < strlen(str); i++)
        raw_hash += (unsigned long long) pow(g, i) * str[i];

    unsigned int hash = raw_hash % HASH_TABLE_SIZE;

#ifdef DEBUG
    printf("str: %s hash: %u\n", str, hash);
#endif
    return (int) hash;
}
#endif

List** new_HashTable(){
    List** hash_table = (List**) malloc(sizeof(List*) * HASH_TABLE_SIZE);
    if (!hash_table) { perror("Malloc failed!"); exit(-1);}

    for(int i = 0; i < HASH_TABLE_SIZE; i++){
        // This creates a dynamic double linked list
        hash_table[i] = new_List("NodeInfo");
    }

    return hash_table;
}

void free_HashTable(List** hashTable){
    for(int i = 0; i < HASH_TABLE_SIZE; i++)
        free_list(hashTable[i]);
    free(hashTable);
}

