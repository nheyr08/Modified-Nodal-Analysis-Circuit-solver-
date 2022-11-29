#ifndef ERGASIAPROSOMOIWSHSKYKLWMATWN_NET_NODE_CLASS_H
#define ERGASIAPROSOMOIWSHSKYKLWMATWN_NET_NODE_CLASS_H
#include "list.h"

// This should be pointing to a val at A table e.g:
// A[0][3]
// row: 0
// col: 3

typedef struct SparseItem{
    unsigned int row;
    unsigned int col;
    double* val;  // Just so we won't be copying the values we point to the existing ones.
}SparseItem;

typedef struct NetItem {
    //NodeType type;
    char* type;
    char* left_node;
    char* right_node;
    float value;
}NetItem;

typedef struct NodeInfo {
    char* name;
    int main_table_pos;
    int hash_table_pos;
}NodeInfo;

typedef struct FileData {
    List* net_list;
    List* nodes_list;
    List* volt_list;
}FileData;

typedef struct MatrixEquation {
    double** A;
    double* x;
    double* B;
    int len_of_arrays;
}MatrixEquation;

void free_FileData(FileData* file_data){
    free_list(file_data->net_list);
    free_list(file_data->nodes_list);
    free_list(file_data->volt_list);
    free(file_data);
}

SparseItem* create_sparse_arr(double** A, int matrix_len, int* sparse_arr_len, double* arr_density){
    SparseItem* sparse_arr = (SparseItem*) malloc(sizeof(SparseItem) * matrix_len);
    if(!sparse_arr){perror("MALLOC ON sparse matrix A FAILED"); exit(-5);}

    int pos = 0;
    for(int i = 0; i < matrix_len; i++){
        for(int j = 0; j < matrix_len; j++){
            if (A[i][j] != 0){
                sparse_arr[pos].row = i;
                sparse_arr[pos].col = j;
                sparse_arr[pos].val = &A[i][j];
                ++pos;
            }
        }
    }
    *arr_density = ((double)pos / (matrix_len * matrix_len)) * 100;
    printf("Sparse Array is ready with density: (%d / %d) * 100 = %.2lf\n", pos, (matrix_len * matrix_len), *arr_density);
    *sparse_arr_len = pos;
    return sparse_arr;
}

// TODO: Use skip_A_matrix to "dynamically" choose whether you simple gonna skip making the A matrix
//       in general or you gonna make it. ( in case of big data you should always skip making the A_matrix).
MatrixEquation* initialize_the_matrix_equation(FileData* file_data, short skip_A_matrix){
    int n = file_data->nodes_list->size;
    int m2 = file_data->volt_list->size;

    MatrixEquation* matrix_equation = (MatrixEquation*) malloc(sizeof(MatrixEquation));
    if (!matrix_equation) { perror("Malloc failed at create_A_arr!\n"); exit(-2);}

    // No need to do n - 1 because we
    // don't add at all at the parsing phase the ground node 0
    // (n - 1) + m2
    int len_of_arrays = n + m2;

    // Initialize the A matrix
    // We use calloc because :
    // The calloc() function then gives all the bits of each element an initial value of 0.
    // https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_73/rtref/calloc.htm
    double** A = (double**) calloc(len_of_arrays, sizeof(double*));
    if (!A) {perror("Calloc failed to create A matrix!\n"); exit(-2);}
    for (int i = 0; i < len_of_arrays; i++){
        A[i] = (double*) calloc(len_of_arrays, sizeof(double));
        if (!A[i]) {printf("Calloc failed to create A[%d]!\n", i); exit(-2);}
    }

    // Initialize the x array
    double* x = (double*) calloc(len_of_arrays, sizeof(double*));
    if (!x) {perror("Calloc failed to create x array!\n"); exit(-2);}

    // Initialize the B array
    double* B = (double*) calloc(len_of_arrays, sizeof(double*));
    if (!B) {perror("Calloc failed to create B array!\n"); exit(-2);}

    matrix_equation->A = A;
    matrix_equation->x = x;
    matrix_equation->B = B;
    matrix_equation->len_of_arrays = len_of_arrays;

    printf("Matrix equation Ax = B is initialized\n");

    return matrix_equation;
}

void free_MatrixEquation(MatrixEquation* matrix_equation){
    for(int i = 0; i < matrix_equation->len_of_arrays; i++)
        free(matrix_equation->A[i]);
    free(matrix_equation->x);
    free(matrix_equation->B);
    free(matrix_equation);
}

int compare_node_names(void* search_name, void* data){
    // We do not dereference it !
    const char* search = (char*) search_name;
    NodeInfo* try = (NodeInfo*) data;

    return strcmp(search, try->name);
}

#endif //ERGASIAPROSOMOIWSHSKYKLWMATWN_NET_NODE_CLASS_H
