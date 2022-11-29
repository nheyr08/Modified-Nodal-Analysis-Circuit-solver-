
#ifndef ERGASIAPROSOMOIWSHSKYKLWMATWN_PARSER_H
#define ERGASIAPROSOMOIWSHSKYKLWMATWN_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashing.h"
#include "net_node_class.h"


char* malloc_string(char* str, size_t size){
    str = (char*) malloc(size * sizeof(char) + 1);
    if(!str) { perror("Malloc failed!\n"); exit(-2); }
    else return str;
}

// This function makes a new node and assigns it an id from from the hash_table plus
// and id for the the Ax = B table. The second id is getting calculated from the pos
// they come-in. e.x:  node_A->main_table_id = 0, node_B->main_table_id = 1, etc
// we need the hash_table because we calculate the hash_id inside the function.
void new_node(short var_type, NetItem* temp_item, char* buffer, List** hash_table, List* nodes_list){

    // This gets updated every time we find a new node.
    static int new_node_id = 0;
    // I wanted to put this declaration inside the if block but I am not sure
    // if its going to be accessible and won't be having weird stuff going on.
    NodeInfo* node_info = (NodeInfo*) malloc(sizeof(NodeInfo));
    if (!node_info) {perror("Malloc failed for node_info!\n"); exit(-2);}


    // Find from which side the node is.
    if(var_type == 1){
        temp_item->left_node = malloc_string(temp_item->left_node, strlen(buffer));
        strcpy(temp_item->left_node, buffer);
    }else if(var_type == 2){
        temp_item->right_node = malloc_string(temp_item->right_node, strlen(buffer));
        strcpy(temp_item->right_node, buffer);
    }
    // If the node we found is the ground
    // DO NOT include it as a new node.
    if(strcmp(buffer, "0") == 0)
        return ;
    int hash_id = hash_function(buffer);


    if(is_it_new_node(buffer, hash_table[hash_id]) == 1){
        // We do new_node_id++ because we cant first
        // to assign the value then make it bigger.
        node_info->main_table_pos = new_node_id++;
        node_info->hash_table_pos = hash_id;
        node_info->name = malloc_string(node_info->name, strlen(buffer));
        strcpy(node_info->name, buffer);
        append_list(nodes_list, node_info);
        append_list(hash_table[hash_id], node_info);
    }else{
        free(node_info);
    }

}

void determine_var_type(short var_type, NetItem* new_item, char* buffer){
    // If ptr is a null pointer, realloc() shall be equivalent to malloc() for the specified size.
    // https://stackoverflow.com/questions/4459275/is-a-malloc-needed-before-a-realloc
    // If ptr does not match a pointer returned earlier by calloc(), malloc(), or realloc() or if the
    // space has previously been deallocated by a call to free() or realloc(), the behavior is undefined.
    char* end_ptr;
    switch (var_type){
        case 0:
            new_item->type = malloc_string(new_item->type, strlen(buffer));
            strcpy(new_item->type, buffer);
            break;
        case 3:
            // This is to store the value in double
            new_item->value = strtod(buffer , &end_ptr);
            break;
        default:
            break;
    }
}

void new_volt_item(const char* type, List** hash_table){

    static int volt_item_offset = 0;

    NodeInfo* volt_info = (NodeInfo*) malloc(sizeof(NodeInfo));
    if (!volt_info) {perror("Malloc failed for node_info!\n"); exit(-2);}

    int hash_id = hash_function(type);
    
    volt_info->name = malloc_string(volt_info->name, strlen(type));
    strcpy(volt_info->name, type);

    volt_info->hash_table_pos = hash_id;
    // This will not show the main table pos this is an offset from the main_table_pos
    // because we have to take account all the nodes that exist then we add the volt_items
    volt_info->main_table_pos = volt_item_offset++;
    append_list(hash_table[hash_id], volt_info);
}

FileData* get_data_from_txt(char* file_path, List** hash_table){
    FILE* fp;

    List* net_list = new_List("NetItem");
    List* nodes_list = new_List("NodeInfo");
    List* volt_list = new_List("NetItem");

    fp = fopen(file_path, "r");
    if (fp == NULL) {perror("Can't open given file or it failed!"); exit(-1);}

    NetItem* new_item = (NetItem*) malloc(sizeof(NetItem));
    if (!new_item) {perror("Malloc failed for new_item!\n"); exit(-2);}

    char buffer[255];
    short var_type = 0;

    FileData* file_data = (FileData*) malloc(sizeof(FileData));

    while (fscanf(fp, "%s", buffer) != EOF){
        determine_var_type(var_type, new_item, buffer);
        if (var_type == 1 || var_type == 2)
            new_node(var_type, new_item, buffer, hash_table, nodes_list);

        ++var_type;
        if(var_type >= 4){
            if(new_item->type[0] != 'V')
                append_list(net_list, new_item);
            else{
                new_volt_item(new_item->type, hash_table);
                append_list(volt_list, new_item);
            }
            // Initialize a new Node
            var_type = 0;
            new_item = (NetItem*) malloc(sizeof(NetItem));
            if (!new_item) { perror("Malloc failed for new_item inside fscanf!\n"); exit(-2); }
        }
    }
    printf("\nEnd of file\n");
    // Assign the values that we gonna return
    file_data->volt_list = volt_list;
    file_data->net_list = net_list;
    file_data->nodes_list = nodes_list;
    fclose(fp);
    return file_data;
}

#endif //ERGASIAPROSOMOIWSHSKYKLWMATWN_PARSER_H
