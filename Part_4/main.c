#include <stdio.h>
#include "parser.h"
#include "misc_functions.h"
#include "solving_ax_b.h"
#include "csparse.h"

void free_memory(FileData*, MatrixEquation*, List**, SparseItem*);

int compare(void* search_data, void* data){
    // We do not dereference it !
    int* search = (int*)search_data;
    int* try    = (int*)data;

    if(search > try){
        return 1;
    }else if(search == try){
        return 0;
    }else if(search < try){
        return -1;
    }
    return -9;
}

void print_the_system(FileData* file_data){

    printf("\n\n\t\t\t\t\tNET LIST ITEMS\n");
    print_custom_list(file_data->net_list, print_custom_node);

    printf("\n\n\t\t\t\t\tNODE LIST\n");
    print_custom_list(file_data->nodes_list, print_hash_table_list);
}

//void solve_the_circuit(double* A, double* x, double* B){
//
//}

//  SOS DO THIS FIRST!!!!!!!!!!!!!
// TODO:
//      1) Add the csparse library.
//      2) Add a debug/printing mode.
int main() {

    List** hash_table = new_HashTable();
    FileData* file_data = get_data_from_txt("./dataset/final_test.txt", hash_table);


    // A x = B
    MatrixEquation* matrix_equation = initialize_the_matrix_equation(file_data, 0);
    // TODO: This function needs to be altered as well.
    fill_the_matrix(matrix_equation, file_data, hash_table);

    int sparse_matrix_len;
    double arr_density;
    SparseItem* sparse_arr = create_sparse_arr(matrix_equation->A, matrix_equation->len_of_arrays,
                                               &sparse_matrix_len, &arr_density);

    print_the_system(file_data);
    print_sparse_arr(sparse_arr, &sparse_matrix_len);
    print_the_matrix_equation(matrix_equation, &arr_density);
    free_memory(file_data, matrix_equation, hash_table, sparse_arr);
    return 0;
}

void free_memory(FileData* file_data, MatrixEquation* matrix_equation, List** hash_table, SparseItem* sparse_arr){
    free(sparse_arr);
    free_FileData(file_data);
    free_HashTable(hash_table);
    free_MatrixEquation(matrix_equation);
}
