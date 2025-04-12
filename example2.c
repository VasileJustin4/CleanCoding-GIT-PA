#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} NODE;

typedef struct Graph {
    int vertices;          
    int* visited;         
    NODE** adjacencyLists; 
} GPH;

NODE* create_node(int value) {
    NODE* new_node = malloc(sizeof(NODE));
    if (new_node == NULL) {
        fprintf(stderr, "Eroare la alocarea memoriei pentru nod.\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}

GPH* create_graph(int vertices) {
    GPH* graph = malloc(sizeof(GPH));
    if (graph == NULL) {
        fprintf(stderr, "Eroare la alocarea memoriei pentru graf.\n");
        exit(EXIT_FAILURE);
    }
    graph->vertices = vertices;
    graph->adjacencyLists = malloc(vertices * sizeof(NODE*));
    graph->visited = malloc(vertices * sizeof(int));
    if (graph->adjacencyLists == NULL || graph->visited == NULL) {
        fprintf(stderr, "Eroare la alocarea memoriei pentru componentele grafului.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < vertices; i++) {
        graph->adjacencyLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void add_edge(GPH* graph, int src, int dest) {
    NODE* new_node = create_node(dest);
    new_node->next = graph->adjacencyLists[src];
    graph->adjacencyLists[src] = new_node;
    new_node = create_node(src);
    new_node->next = graph->adjacencyLists[dest];
    graph->adjacencyLists[dest] = new_node;
}

void insert_edges(GPH* graph, int num_edges) {
    int src, dest;
    printf("Introduceti %d muchii (format:sursa destinatie):\n", num_edges);
    for (int i = 0; i < num_edges; i++) {
        if (scanf_s("%d %d", &src, &dest) != 2) {
            fprintf(stderr, "Input invalid!\n");
            exit(EXIT_FAILURE);
        }
        add_edge(graph, src, dest);
    }
}

void print_graph(GPH* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        NODE* temp = graph->adjacencyLists[i];
        printf("Nod %d: ", i);
        while (temp != NULL) {
            printf("%d ", temp->data);
            temp = temp->next;
        }
        printf("\n");
    }
}

void reset_visited(GPH* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        graph->visited[i] = 0;
    }
}

void DFS(GPH* graph, int vertex) {
    NODE* adj_list = graph->adjacencyLists[vertex];
    NODE* temp = adj_list;

    graph->visited[vertex] = 1;
    printf("%d ", vertex);

    while (temp != NULL) {
        int connected_vertex = temp->data;
        if (graph->visited[connected_vertex] == 0) {
            DFS(graph, connected_vertex);
        }
        temp = temp->next;
    }
}

int is_empty(NODE* queue) {
    return (queue == NULL);
}

void enqueue(NODE** queue, int data) {
    NODE* new_node = create_node(data);
    if (*queue == NULL) {
        *queue = new_node;
    }
    else {
        NODE* temp = *queue;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

int dequeue(NODE** queue) {
    if (*queue == NULL) {
        fprintf(stderr, "Coada este goala!\n");
        exit(EXIT_FAILURE);
    }
    int data = (*queue)->data;
    NODE* temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

void BFS(GPH* graph, int start) {
    NODE* queue = NULL;
    graph->visited[start] = 1;
    enqueue(&queue, start);

    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);

        NODE* temp = graph->adjacencyLists[current];
        while (temp != NULL) {
            int adj_vertex = temp->data;
            if (graph->visited[adj_vertex] == 0) {
                graph->visited[adj_vertex] = 1;
                enqueue(&queue, adj_vertex);
            }
            temp = temp->next;
        }
    }
}

int main() {
    int nr_of_vertices, nr_of_edges, start_vertex;

    printf("Cate noduri are graful? ");
    if (scanf_s("%d", &nr_of_vertices) != 1) {
        fprintf(stderr, "Input invalid!\n");
        exit(EXIT_FAILURE);
    }

    printf("Cate muchii are graful? ");
    if (scanf_s("%d", &nr_of_edges) != 1) {
        fprintf(stderr, "Input invalid!\n");
        exit(EXIT_FAILURE);
    }

    // Creaza graful si insereaza muchiile
    GPH* graph = create_graph(nr_of_vertices);
    insert_edges(graph, nr_of_edges);
    print_graph(graph);
    // DFS
    printf("De unde plecam in DFS? ");
    if (scanf_s("%d", &start_vertex) != 1) {
        fprintf(stderr, "Input invalid!\n");
        exit(EXIT_FAILURE);
    }
    printf("Parcurgere cu DFS: ");
    DFS(graph, start_vertex);
    printf("\n");

    // Resetam vectorul de vizitare pentru BFS
    reset_visited(graph);

    // BFS
    printf("De unde plecam in BFS? ");
    if (scanf_s("%d", &start_vertex) != 1) {
        fprintf(stderr, "Input invalid!\n");
        exit(EXIT_FAILURE);
    }
    printf("Parcurgere cu BFS: ");
    BFS(graph, start_vertex);
    printf("\n");

    // Eliberare memorie - eliberam listele de adiacenta si vectorul de vizitare
    for (int i = 0; i < graph->vertices; i++) {
        NODE* temp = graph->adjacencyLists[i];
        while (temp != NULL) {
            NODE* to_free = temp;
            temp = temp->next;
            free(to_free);
        }
    }
    free(graph->adjacencyLists);
    free(graph->visited);
    free(graph);

    return 0;
}
