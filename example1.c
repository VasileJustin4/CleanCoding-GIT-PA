#include <stdio.h>
#include <stdlib.h>

// Definire nod in lista de adiacenta
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Structura pentru graf (neorientat)
typedef struct Graph {
    int vertices;         // numarul de noduri (varfuri)
    int* visited;         // vector de vizitare pentru parcurgere DFS/BFS
    Node** adjacencyLists; // vector de liste de adiacenta
} Graph;

// Structura pentru stiva utilizata in DFS (optional)
typedef struct Stack {
    int top;
    int capacity;
    int* array;
} Stack;

// Functie pentru creare nod nou
Node* createNode(int data) {
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Memorie insuficienta pentru nod");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Functie pentru creare graf cu un numar dat de noduri
Graph* createGraph(int vertices) {
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        perror("Memorie insuficienta pentru graf");
        exit(EXIT_FAILURE);
    }
    graph->vertices = vertices;
    graph->visited = malloc(vertices * sizeof(int));
    graph->adjacencyLists = malloc(vertices * sizeof(Node*));
    if (graph->visited == NULL || graph->adjacencyLists == NULL) {
        perror("Memorie insuficienta pentru componentele grafului");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < vertices; i++) {
        graph->adjacencyLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// Functie pentru adaugare muchie (graf neorientat)
void addEdge(Graph* graph, int src, int dest) {
    // Adauga muchie de la src la dest
    Node* newNode = createNode(dest);
    newNode->next = graph->adjacencyLists[src];
    graph->adjacencyLists[src] = newNode;

    // Adauga muchie de la dest la src (graf neorientat)
    newNode = createNode(src);
    newNode->next = graph->adjacencyLists[dest];
    graph->adjacencyLists[dest] = newNode;
}

// Functie pentru inserarea muchiilor din input
void insertEdges(Graph* graph, int edgeCount) {
    int src, dest;
    printf("Introdu %d muchii (format:sursa destinatie):\n", edgeCount);
    for (int i = 0; i < edgeCount; i++) {
        if (scanf_s("%d %d", &src, &dest) != 2) {
            fprintf(stderr, "Input invalid!\n");
            exit(EXIT_FAILURE);
        }
        addEdge(graph, src, dest);
    }
}

// Functia de resetare a vectorului de vizitare
void resetVisited(Graph* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        graph->visited[i] = 0;
    }
}

// Functii pentru stiva (Stack)
Stack* createStack(int capacity) {
    Stack* stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        perror("Memorie insuficienta pentru stiva");
        exit(EXIT_FAILURE);
    }
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = malloc(capacity * sizeof(int));
    if (stack->array == NULL) {
        perror("Memorie insuficienta pentru elementele stivei");
        exit(EXIT_FAILURE);
    }
    return stack;
}

void push(Stack* stack, int value) {
    if (stack->top == stack->capacity - 1) {
        fprintf(stderr, "Stiva este plina!\n");
        return;
    }
    stack->array[++stack->top] = value;
}

int pop(Stack* stack) {
    if (stack->top == -1) {
        fprintf(stderr, "Stiva este goala!\n");
        return -1;
    }
    return stack->array[stack->top--];
}

int isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

// Parcurgerea in adancime (DFS) - recursiv
void DFS(Graph* graph, Stack* stack, int vertex) {
    Node* adjList = graph->adjacencyLists[vertex];
    graph->visited[vertex] = 1;
    printf("%d ", vertex);
    push(stack, vertex);

    while (adjList != NULL) {
        int adjVertex = adjList->data;
        if (graph->visited[adjVertex] == 0) {
            DFS(graph, stack, adjVertex);
        }
        adjList = adjList->next;
    }
}

int main() {
    int numVertices, numEdges, startVertex;

    printf("Cate noduri are graful? ");
    if (scanf_s("%d", &numVertices) != 1) {
        fprintf(stderr, "Input invalid!\n");
        exit(EXIT_FAILURE);
    }

    printf("Cate muchii are graful? ");
    if (scanf_s("%d", &numEdges) != 1) {
        fprintf(stderr, "Input invalid!\n");
        exit(EXIT_FAILURE);
    }

    // Crearea grafului
    Graph* graph = createGraph(numVertices);

    // Inserarea muchiilor
    insertEdges(graph, numEdges);

    // Crearea unei stive pentru DFS (optional, doar pentru demonstratie)
    Stack* stack = createStack(2 * numVertices);

    printf("De unde doriti sa incepeti parcurgerea DFS? ");
    if (scanf_s("%d", &startVertex) != 1) {
        fprintf(stderr, "Input invalid!\n");
        exit(EXIT_FAILURE);
    }

    printf("Parcurgere DFS: ");
    DFS(graph, stack, startVertex);
    printf("\n");

    // Eliberare memorie alocata (un mic exemplu; nu se elibereaza complet tot spatiul)
    free(stack->array);
    free(stack);
    free(graph->visited);
    for (int i = 0; i < numVertices; i++) {
        Node* temp = graph->adjacencyLists[i];
        while (temp != NULL) {
            Node* toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
    free(graph->adjacencyLists);
    free(graph);

    return 0;
}
