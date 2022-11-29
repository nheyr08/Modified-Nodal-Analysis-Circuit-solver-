#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define MAGENTA  printf("\x1b[31m")
#define CYAN     printf("\x1b[36m")
#define RED      printf("\x1b[35m")
#define GREEN    printf("\x1b[32m")
#define ORANGE   printf("\x1b[33m")
#define WHITE    printf("\x1b[0m")
#define PURPLE   printf("\x1b[34m")

#define BG_LADI  printf("\x1b[40m")
#define BG_PINK   printf("\x1b[41m")
#define BG_ORAGNE   printf("\x1b[43m")
#define BG_GREEN   printf("\x1b[42m")
#define BG_PURPLE  printf("\x1b[44m")
#define BG_RED   printf("\x1b[45m")
#define BG_CYAN   printf("\x1b[46m")
#define BG_GRAY   printf("\x1b[47m")
#define BG_WHITE   printf("\x1b[7m")

#define RESET    printf("\x1b[0m")


//#ifndef ASKHSH2_PROG3_ASK2_H
//#define ASKHSH2_PROG3_ASK2_H
//#endif //ASKHSH2_PROG3_ASK2_H

int printf_colored(int color, char* str, ...){

    // arg color here is the return of the printf in the defines.

    va_list arg;
    va_start(arg, str);
    int done = vfprintf(stdout, str, arg);
    va_end(arg);


    RESET; // Reset the color.
    return done;
}

void size_of_spaces(double n, int* spaces){

    int right_spaces = 12;
    int left_spaces = 3;
    char res[20];
    sprintf(res, "%lf", n);
    res[strlen(res) - 1] = '\0';
    spaces[0] = left_spaces + (int) strlen(res);
    spaces[1] = right_spaces - (int) strlen(res);
}

void print_matrix_density(const double arr_density){
    printf_colored(GREEN, "Array density: ");
    printf_colored(RED, "%.2lf\n", arr_density);
}

void print_sparse_arr(SparseItem* sparse_matrix, const int* matrix_size){
    printf_colored(CYAN, "\n\n\t\t\t\tSPARSE MATRIX\n");
    for(int i = 0; i < *matrix_size; i++){
        printf_colored(MAGENTA, "[");
        printf_colored(GREEN, "%d", i);
        printf_colored(MAGENTA, "]");
        printf_colored(CYAN, " = ");
        printf_colored(PURPLE, "row ");
        printf_colored(ORANGE, "%d", sparse_matrix[i].row);
        printf_colored(PURPLE, " col ");
        printf_colored(ORANGE, "%d", sparse_matrix[i].col);
        printf_colored(PURPLE, " val ");
        printf_colored(ORANGE, "%lf\n" , *sparse_matrix[i].val);

    }
}

void print_the_matrix_equation(MatrixEquation* matrix_equation, const double* arr_density){
    printf_colored(MAGENTA, "\n\n\t\t\t\t\t\tA ");
    printf_colored(ORANGE, "x");
    printf(" = ");
    printf_colored(CYAN, "B\n");
    print_matrix_density(*arr_density);
    for(int i = 0; i < matrix_equation->len_of_arrays; i++){
        for(int j = 0; j < matrix_equation->len_of_arrays; j++){
            printf_colored(MAGENTA, "[");
            int spaces[2];
            size_of_spaces(matrix_equation->A[i][j], spaces);
            printf(" %*.3lf%*c ", spaces[0], matrix_equation->A[i][j], spaces[1], ' ');
            printf_colored(MAGENTA, "]");
        }
        printf_colored(ORANGE, "        [");
        int spaces[2];
        size_of_spaces(matrix_equation->x[i], spaces);
        printf(" %*.3lf%*c", spaces[0], matrix_equation->x[i], spaces[1], ' ');
        printf_colored(ORANGE, "]");

        printf_colored(CYAN, "        [");
        size_of_spaces(matrix_equation->B[i], spaces);
        printf(" %*.3lf%*c", spaces[0], matrix_equation->B[i], spaces[1], ' ');
        printf_colored(CYAN, "]\n");
    }
}

void print_custom_node(void* data){
    NetItem* net_node = (NetItem*) data;
    printf("Type: ");
    printf_colored(CYAN, "%s", net_node->type);
    printf("    High-Node: ");
    printf_colored(ORANGE, "%s", net_node->left_node);
    printf("    Low-Node: ");
    printf_colored(ORANGE, "%s", net_node->right_node);
    printf("    Value: ");
    printf_colored(RED, "%lf\n", net_node->value);
}

void print_hash_table_list(void* data){
    NodeInfo* node_info = (NodeInfo*) data;

    printf("Node: ");
    printf_colored(CYAN, "%s", node_info->name);
    printf("    Main-id: ");
    printf_colored(ORANGE, "%d", node_info->main_table_pos);
    printf("    Hash-Table-id: ");
    printf_colored(ORANGE, "%d\n", node_info->hash_table_pos);
}

void print_coords(int* coords){
    printf_colored(ORANGE, "X: ");
    printf_colored(CYAN, "%d  ", coords[0]);
    printf_colored(ORANGE, " Y: ");
    printf_colored(CYAN, "%d\n", coords[1]);
}

int input_int(int lowerBound, int higherBound){
    int input, count = 0;

    do{
        if(count > 0) printf_colored(RED, "Invalid input!\n");
        else count++;
        scanf("%d", &input);
    }while(input < lowerBound || input > higherBound);

    return input;
}

void get_sub_string(char *target, const char *source, int from, int to){
    int i, j = 0;
    for(i = from, j = 0; i <= to; i++, j++){
        target[j] = source[i];
    }
    // assign NULL at the end of string
    target[j] = '\0';
}


char** separate_str(char* str, const char* delim){
    char **args = malloc(64 * sizeof(char*));
    char *token;
    int position = 0;

    token = strtok(str, delim);
    while(token != NULL){
        args[position] = token;
        position++;
        token = strtok(NULL, delim);
    }

    args[position] = NULL;

    return args;
}

void remove_spaces_from(char* ptr, int pos){
    int j = 0;
    for(int i = pos; ptr[i]; i++)
        if(ptr[i] != ' ')
            ptr[j++] = ptr[i];
    ptr[j] = '\0';
}