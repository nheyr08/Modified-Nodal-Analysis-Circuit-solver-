//#include "hashing.h"


void alter_array_B(double* B, const int* nodes_pos, float net_item_value, char type){
    int left_node_pos = nodes_pos[0];
    int right_node_pos = nodes_pos[1];

    if(type == 'I'){
        // If the left node is not 0 (ground) do: B[right_node_pos] = -1 * I_val
        if(left_node_pos != -1)
            B[left_node_pos] += (-1) * net_item_value;
        
        if(right_node_pos != -1)
            B[right_node_pos] += net_item_value; 
    }else if(type == 'V'){
        int main_pos = nodes_pos[2];
        B[main_pos] += net_item_value;
    }
}

void alter_matrix_A(double** A, const int* nodes_pos, float net_item_value, char type){
    int left_node_pos = nodes_pos[0];
    int right_node_pos = nodes_pos[1];

    if(type == 'R'){
        // -1 means the node is the ground.
        // example -> R1 C B 3.4 -> G1 = 1 / 3.4
        // Matrix A changes:   A[C][C] = G1
        //                     A[B][B] = G1
        //                     A[B][C] = -G1
        //                     A[C][B] = -G1
        if(left_node_pos != -1 && right_node_pos != -1){
            // A[0][0]
            A[left_node_pos][left_node_pos] += 1.00f / net_item_value;
            // A[1][1]
            A[right_node_pos][right_node_pos] += 1.00f / net_item_value;
            // A[0][1]
            A[left_node_pos][right_node_pos] += -1 * (1.00f / net_item_value);
            // A[1][0]
            A[right_node_pos][left_node_pos] += -1 * (1.00f / net_item_value);
        }else if(left_node_pos == -1)
            A[right_node_pos][right_node_pos] += 1.00f / net_item_value;
        else
            A[left_node_pos][left_node_pos] += 1.00f / net_item_value;
    }else if(type == 'V'){
        int volt_pos = nodes_pos[2];
        if(left_node_pos != -1){
            A[left_node_pos][volt_pos] += 1;
            A[volt_pos][left_node_pos] += 1;
        }

        if(right_node_pos != -1){
            A[right_node_pos][volt_pos] -= 1;
            A[volt_pos][right_node_pos] -= 1;
        }
        
    }
}

void row_operation(double* row, float pivot, int make_this_zero, int len_of_matrix){
    float crucial_num = row[make_this_zero] / pivot;
    for(int i = 0; i < len_of_matrix; i++){
        row[i] = row[i] - (crucial_num) / pivot;
        if(i == make_this_zero)
            row[make_this_zero] = 0.0f;
    }
}

void LU_factor(double** A, int len_of_matrix){
    // Rows
    //  i =   A[0][0]
    //        A[1][0];
    //  i =   A[1][1];
    //        A[2][0];
    //        A[2][1];
    // ...
    for(int i = 0; i < len_of_matrix - 1; i++){
        for(int j = 0; j <= i; j++)
            row_operation(A[i + 1], A[i][i], j, len_of_matrix);
    }
}

void solve_with_LU(double** A, int len_of_matrix){
    LU_factor(A, len_of_matrix);
}

void fill_the_matrix(MatrixEquation* matrix_equation, FileData* file_data, List** hash_table){

    // We need to iterate all the net_list and
    // change the equation A x = B accordingly
    Node* tmp = file_data->net_list->head_node;
    while(tmp != NULL){
        // We need to cast to the correct type since the data pointer is void*
        NetItem* net_item = (NetItem*) tmp->data;

        // Get the positions of the nodes in the hash_table.
        int left_n_hash = hash_function(net_item->left_node);
        int right_n_hash = hash_function(net_item->right_node);

        int left_node_pos = -1, right_node_pos = -1;
        
        // This function basically works like this:
        // 1) We give it a list (we pick the correct list based the hash_id which is always the same for specific str if not gg)
        // 2) We pass the name of the node we are looking for.
        // 3) We pass it a comparing function to determine how the comparing will take place.
        Node* tmp_left = find_node(hash_table[left_n_hash], net_item->left_node, compare_node_names);
        Node* tmp_right = find_node(hash_table[right_n_hash], net_item->right_node, compare_node_names);
        //if(tmp_left == NULL && tmp_right == NULL){ perror("Left node AND Right node are NULL!"); exit(-5);}

        // Get the position of the nodes
        if(tmp_left != NULL)
            left_node_pos  = ((NodeInfo*) tmp_left->data)->main_table_pos;
        if(tmp_right != NULL)
            right_node_pos = ((NodeInfo*) tmp_right->data)->main_table_pos;
        
        int nodes_pos[2] = { left_node_pos, right_node_pos };

        // Find which table you gonna alter
        if (net_item->type[0] == 'I')
            alter_array_B(matrix_equation->B, nodes_pos, net_item->value, 'I');
        else if(net_item->type[0] == 'R')
            alter_matrix_A(matrix_equation->A, nodes_pos, net_item->value, 'R');
        tmp = tmp->next;
    }

    tmp = file_data->volt_list->head_node;
    int nodes_list_size = file_data->nodes_list->size;

    while(tmp != NULL){
        NetItem* volt_item = (NetItem*) tmp->data;


        // Get the position of the V item in the table.
        int n_hash = hash_function(volt_item->type);

        // Get the positions of the nodes in the hash_table.
        int left_n_hash = hash_function(volt_item->left_node);
        int right_n_hash = hash_function(volt_item->right_node);

        int left_node_pos = -1, right_node_pos = -1;
        
        Node* tmp_left = find_node(hash_table[left_n_hash], volt_item->left_node, compare_node_names);
        Node* tmp_right = find_node(hash_table[right_n_hash], volt_item->right_node, compare_node_names);
        
        // Get the position of the nodes
        if(tmp_left != NULL)
            left_node_pos  = ((NodeInfo*) tmp_left->data)->main_table_pos;
        if(tmp_right != NULL)
            right_node_pos = ((NodeInfo*) tmp_right->data)->main_table_pos;

        // Even tho we want to find a volt item we still use the same method we used finding nodes
        // since we are simply comparing char*.
        Node* tmp_pos = find_node(hash_table[n_hash], volt_item->type, compare_node_names);
        if(tmp_pos == NULL){ printf("Volt item: '%s' wasn't find something went wrong!", volt_item->type); exit(-5); }

        int main_pos = nodes_list_size + ((NodeInfo*) tmp_pos->data)->main_table_pos;

        int nodes_pos[3] = { left_node_pos, right_node_pos, main_pos };

        alter_matrix_A(matrix_equation->A, nodes_pos, volt_item->value, 'V');
        alter_array_B(matrix_equation->B, nodes_pos, volt_item->value, 'V');

        tmp = tmp->next;
    }

    printf("Equation filled successfully!\n");
    // alter_array_B(matrix_equation->B, nodes_pos, net_item->value, 'V');
    // alter_matrix_A(matrix_equation->A, nodes_pos, net_item->value, 'V');
        
}
