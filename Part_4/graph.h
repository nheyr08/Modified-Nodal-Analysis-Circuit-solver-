#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"


enum boolean {Undirected = 0, Directed = 1};


typedef struct Graph_Node{
    List* edge_list;
    unsigned int id;
    void* data;
}Graph_Node;

typedef struct Edge{
    int weight;
    Graph_Node* source;
    Graph_Node* destination;
}Edge;

typedef struct Visited_Node{
    int cost;
    bool visited;
    Graph_Node* node;
}Visited_Node;

typedef struct Graph{
    int size;
    enum boolean is_directed;
    int type;
    char type_in_str[50];
    List* edge_list;
    int** adj_matrix;
//    Graph_Node* starting_node;
    List* node_list;
}Graph;


Graph* new_Graph (enum boolean is_directed, void* type){

    Graph* graph = (Graph*) malloc(sizeof(Graph));
    if (!graph) { perror("Malloc failed!"); exit(-2); }

    graph->adj_matrix    = NULL;
    graph->edge_list     = new_List("Edges");
    graph->size          = 0;
    graph->is_directed   = is_directed;
    strcpy(graph->type_in_str, type);

    if(strcmp(type, "int") == 0){
        graph->type = 0;
    }else if (strcmp(type, "char*") == 0){
        graph->type = 1;
    }else{
        graph->type = -1;
    }

    graph->node_list = new_List(type);

    return graph;
}

void free_graph(Graph* graph){
    free_list(graph->edge_list);
    free_list(graph->node_list);
    free(graph->adj_matrix);
    free(graph);
}

void print_all_edges(Graph* graph){

   Node* tmp = graph->node_list->head_node;
   Graph_Node* graph_node;
    Node* edgeListTmp;
    Edge* edge_g_node;

    printf("\t\t\t\t   EDGE LIST \n");
    printf("\t\t\t\t (w = weight) \n");
    printf("\t\t (then is the Node name/value) \n");
    // For every nodes adj list
    while(tmp != NULL){
        graph_node = tmp->data;

        // Graph Node's edge List. From that we take to head node
        // and then we iterate until that list makes it to the end.
        edgeListTmp = graph_node->edge_list->head_node;

        // If there is any edges then iterate until you find them all
        if(edgeListTmp != NULL) {
            if (graph->type == 0)
                printf("Edge list Node[%ld]:", (long) graph_node->data);
            else if (graph->type == 1)
                printf("Edge list Node[%s]:", graph_node->data);

            while (edgeListTmp != NULL) {
                edge_g_node = edgeListTmp->data;

                // Based on the graph type we print the correct way.
                // Only char* and int are supported in printing.
                if (graph->type == 0)
                    printf(" --w(%d)--(%ld)->", edge_g_node->weight, (long) edge_g_node->destination->data);
                else if (graph->type == 1)
                    printf(" --w(%d)--(%s)->",edge_g_node->weight, (char*) edge_g_node->destination->data);

                edgeListTmp = edgeListTmp->next;
            }
            // To skip unwanted lines
            if (graph_node->edge_list->head_node != NULL) printf("\n");
        }
        tmp = tmp->next;
    }

}

void print_graph(Graph* graph){

    Node* tmp = graph->node_list->head_node;
    Graph_Node* tmp_graph_node;
    int count = 0;
    while(tmp != NULL){
        tmp_graph_node = tmp->data;
        if (graph->type == 0)
            printf("Node(%d): %ld\n", count, (long)tmp_graph_node->data);
        else if (graph->type == 1)
            printf("Node(%d): %s\n", count, (char*)tmp_graph_node->data);

        tmp = tmp->next;
        count++;
    }

}
Graph_Node* find_graph_node_with_id(Graph* graph, int id){
    Node* tmp = graph->node_list->head_node;
    Graph_Node* tmp_graph_node = NULL;

    while(tmp != NULL){
        tmp_graph_node = tmp->data;
        if(id == tmp_graph_node->id)
            break;
        tmp = tmp->next;
    }
    return tmp_graph_node;
}
Edge* find_edge_with_ids(Graph* graph, int source_id, int dest_id){
    Graph_Node* source = find_graph_node_with_id(graph, source_id);
    Graph_Node* dest   = find_graph_node_with_id(graph, dest_id);

    Node* tmp = graph->edge_list->head_node;
    Edge* tmp_edge = NULL;

    while(tmp != NULL){
        tmp_edge = (Edge*) tmp->data;
        if(tmp_edge->source == source && tmp_edge->destination == dest)
            break;
        tmp = tmp->next;
    }

    return tmp_edge;
}

Graph_Node* find_graph_node(Graph* graph, void* search_data, int compare(void*, void*)){
    Node* tmp = graph->node_list->head_node;
    Graph_Node* tmp_graph_node = NULL;

    while(tmp != NULL){
        tmp_graph_node = tmp->data;
        if(compare(search_data, tmp_graph_node->data) == 0)
            break;
        tmp = tmp->next;
    }
    if(tmp == NULL){
        graph->type == 0
        ? printf("\nCouldn't find that element: %ld\n", (long)search_data)
        : printf("\nCouldn't find that element: %s\n", (char*)search_data);
    }
    return tmp_graph_node;
}
void create_adj_matrix(Graph* graph){
    int** adj_list = (int**) malloc(graph->size * sizeof(int*));
    if(!adj_list) { perror("Malloc failed!\n"); exit(-2); }
    for(int i = 0; i < graph->size; i++){
        adj_list[i] = (int*) malloc(graph->size * sizeof(int));
        if(!adj_list[i]) { perror("Malloc failed!\n"); exit(-2); }
    }

    Node* tmp = graph->edge_list->head_node;
    Edge* tmp_edge;
    while(tmp != NULL){
        tmp_edge = tmp->data;

        int source = tmp_edge->source->id;
        int dest   = tmp_edge->destination->id;
        adj_list[source][dest] = tmp_edge->weight;

        tmp = tmp->next;
    }

    graph->adj_matrix = adj_list;
}
#define INFINITY 999999
int dijkstra_helper(Graph* graph, Visited_Node nodes[]){
    int id = -1, min = INFINITY;

    for(int i = 0; i < graph->size; i++){
        if(nodes[i].cost < min && nodes[i].visited == false){
            min = nodes[i].cost;
            id = i;
        }
    }

    return id;
}

int* dijkstra(Graph* graph, Graph_Node* source_node, Graph_Node* dest_node, bool with_print){
    if(!graph->adj_matrix){
        create_adj_matrix(graph);
    }

    // Init path
    int* path = (int*) malloc(graph->size * sizeof(int));
    for(int i = 0; i < graph->size; i++){
        path[i] = -1;
    }

    Visited_Node nodes[graph->size];
    Node* tmp = graph->node_list->head_node;

    // The ancestor works like this:
    // It tell us for every node who was his " ancestor " aka the previous node
    // Example:
    // 0} [ -1 ] <- source node no ancestor
    // 1} [ 2 ]
    // 2} [ 4 ]
    // 3} [ 5 ]
    // 4} [ 9 ] <- dest node  . We know that to get to node 4 we need first
    // 5} [ 11 ]                to go to 9 so we go and see node's 9 previous
    // 6} [ 2 ]                 node and so on.. until we get to the source node.
    // 7} [ 1 ]                 (This is a perfect example for recursion but I avoided it
    //                          because I didn't want to make a new function..)
    int ancestor[graph->size];

    // Initialize our nodes arr
    for(int i = 0; tmp != NULL; i++, tmp = tmp->next){
        ancestor[i] = 0;
        nodes[i].cost    = INFINITY;
        nodes[i].visited = false;
        nodes[i].node    = (Graph_Node*) tmp->data;
    }

    // Because id's are " irrelevant " to the nodes, (i mean
    // that they are more like a timestamp) we known that
    // our source_node would be in the same position in this
    // array just like in our node_list from our graph.
    // so: nodes[source_node->id].node == source_node
    nodes[source_node->id].cost    = 0;
    nodes[source_node->id].visited = true;

    Node* edge_list_node;
    Edge* edge;
    int id;
    Graph_Node* next_node = source_node;
    for(int i = 0; i < graph->size; i++){
        edge_list_node = next_node->edge_list->head_node;

        // Update all costs of neighbours
        while(edge_list_node != NULL){
            edge = (Edge*) edge_list_node->data;

            // If( edge node cost > edge weight + cost to get to the previous node )
            if(nodes[edge->destination->id].cost  > edge->weight + nodes[edge->source->id].cost){
                ancestor[edge->destination->id] = edge->source->id;
                nodes[edge->destination->id].cost = edge->weight + nodes[edge->source->id].cost;
            }

            edge_list_node = edge_list_node->next;
        }

        if((id = dijkstra_helper(graph, nodes)) != -1){
            next_node = nodes[id].node;
            nodes[id].visited = true;
        }else break;
    }

//    // Debugging
//    for(int i = 0; i < graph->size; i++){
//        printf("node: %s  cost: %d ancestor: %s\n", (char*)nodes[i].node->data, nodes[i].cost, (char*)nodes[ancestor[i]].node->data);
//    }


    // Printing the path
    for(int i = 0; i < graph->size; i++){
        if(dest_node->id == i){
            if(with_print){
                printf("Source: %s  ---> Destination: %s  cost: %d\n", (char*)source_node->data, (char*)nodes[i].node->data, nodes[i].cost);
                printf("The path: ");
            }

            int tmp_id = ancestor[dest_node->id];
            for(int k = 0; k < graph->size; k++){
                if(tmp_id == source_node->id){
                    path[k] = source_node->id;
                    //printf(" %s", (char*)nodes[source_node->id].node->data);
                    break;
                }

                path[k] = tmp_id;
                //printf(" %s-->", (char*)nodes[tmp_id].node->data);
                tmp_id = ancestor[tmp_id];
            }

            if(with_print){

                for(int k = graph->size - 1; k >= 0; k--){
                    if(path[k] != -1){
                        printf(" %s-->", (char*)nodes[path[k]].node->data);
                    }
                }
                printf(" %s\n\n", (char*)nodes[dest_node->id].node->data);
            }
        }

    }

    return path;
}
void print_adj_matrix(Graph* graph){

    if(!graph->adj_matrix){
        create_adj_matrix(graph);
    }
    printf("   ");
    for(int i = 0; i < graph->size; i++){
        printf(" v%d", i);
    }
    printf("\n");
    for(int i = 0; i < graph->size; i++){
        printf("v%d  ", i + 1);
        for(int j = 0; j < graph->size; j++){
            printf("%d  ", graph->adj_matrix[i][j]);
        }
        printf("\n");
    }
}
void find_shortest_path(Graph* graph, void* source_node, void* dest_node, int (*compare)(void*, void*)){
    Graph_Node* source = find_graph_node(graph, source_node, compare);
    Graph_Node* dest   = find_graph_node(graph, dest_node, compare);

    dijkstra(graph, source, dest, true);
}
void add_edge(Graph* graph, void* source_node, void* dest_node, int weight, int (*compare)(void*, void*)){

    Graph_Node* source = find_graph_node(graph, source_node, compare);
    Graph_Node* dest   = find_graph_node(graph, dest_node, compare);
    Edge* edge = (Edge*) malloc(sizeof(Edge));
    if(!edge) { perror("Malloc failed!\n"); exit(-2); }
    if (!source || !dest) { printf("Couldn't find one of the nodes!\n"); return; }
   
    edge->source      = source;
    edge->destination = dest;
    edge->weight      = weight;
    
    // We are adding the in edge list the graph nodes
    // as a whole which means:
    // " xGraph_Node->edge_list->head_node "
    // its a type of Graph_Node*
    if (graph->is_directed == Undirected){

       
        // We swap this around and we add it 
        // to the other node as an Edge.
        Edge* edge2 = (Edge*) malloc(sizeof(Edge));
        if(!edge2) { perror("Malloc failed!\n"); exit(-2); }
        edge2->source      = dest;
        edge2->destination = source;
        edge2->weight      = weight;

        add_list_node(dest->edge_list, edge2);
        add_list_node(graph->edge_list, edge2);
    }

    add_list_node(source->edge_list, edge);
    add_list_node(graph->edge_list, edge);
}

void add_graph_node (Graph* graph, void* nodeValue){
    Graph_Node* graph_node = (Graph_Node*) malloc(sizeof(Graph_Node));
    if (!graph_node) { perror("Malloc failed!\n"); exit(-2); }

    graph_node->edge_list = new_List(graph->type_in_str);
    graph_node->data = nodeValue;
    graph_node->id = graph->size;

    graph->size++;
    add_list_node(graph->node_list, graph_node);
}