typedef struct graphAdjMat {
    int numNodes;
    double **mat;
} graphAdjMat_t;

typedef struct node {
    int data;
    struct node *next;
    struct node *prev;
} node_t;

typedef struct list {
    node_t *head;
    node_t *tail;
    int len;
} list_t;

typedef struct currentStore {
    int store;
    double cost;
    int bitmask;
} currStore_t;

typedef struct nodeQueue {
    currStore_t *info;
    struct nodeQueue *next, *prev;
} nodeQueue_t;

typedef struct stack {
    nodeQueue_t *head, *tail;
    int len;
} queue_t;

typedef list_t stack_t;

void enqueue(queue_t *queue, currStore_t *data) {
    nodeQueue_t *nou = malloc(sizeof(nodeQueue_t));
    nou->next = nou->prev = NULL;
    nou->info = data;
    if(queue->len==0) {
        queue->head=nou;
        queue->tail=nou;
        queue->len++;
    }
    else {
        nou->next=queue->head;
        queue->head->prev=nou;
        queue->head=nou;
        queue->len++;
    }
    return;
}

currStore_t *dequeue(queue_t *queue) {
    currStore_t *x;
    nodeQueue_t *aux=queue->tail;
    if(queue->len==0) {
        return NULL;
    }
    else if(queue->len==1) {
        x=queue->tail->info;
        free(aux);
        queue->head=NULL;
        queue->tail=NULL;
        queue->len=0;
        return x;
    }
    else {
        x=queue->tail->info;
        queue->tail=aux->prev;
        queue->tail->next=NULL;
        free(aux);
        queue->len--;
        return x;
    }
}

node_t *init_node(int value) {
    node_t *node = malloc(sizeof(node_t));
    node->data = value;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

list_t *init_list() {
    list_t *q = malloc(sizeof(list_t));
    q->head = NULL;
    q->tail = NULL;
    q->len = 0;
    return q;
}

void push(stack_t *stack, int value) {
    node_t *nod=init_node(value);
    nod->next=stack->head;
    stack->head=nod;
    stack->len++;
}

int pop(stack_t *stack) {
    if(stack->len==0) {
        return 0;
    }
    else {
        node_t *nod=stack->head;
        int x=stack->head->data;
        stack->head=stack->head->next;
        free(nod);
        stack->len--;
        return x;
    }
    return 0;
}

int insert_node(list_t* list, int store) {
    node_t* nou = init_node(store);
    // introducere in lista vida
    if (list->len == 0) {
        list->head = nou;
        list->tail = nou;
        list->len++;
        return 0;
    }
    node_t *aux;
    // introducere la sfarsitul listei
    aux = list->tail;
    nou->prev = aux;
    aux->next = nou;
    list->tail = nou;
    list->len++;
    return 0;
}

void destroyList(list_t *list) {
    node_t *aux;
    while (list->len > 0) {
        aux = list->head;
        list->head = aux->next;
        free(aux);
        list->len--;
    }
    free(list);
}

// functie de initializare a unui graf
graphAdjMat_t *initGraph(int numNodes) {
    graphAdjMat_t *graph = malloc(sizeof(graphAdjMat_t));
    graph->numNodes = numNodes;
    graph->mat = malloc(numNodes*sizeof(double *));
    int i;
    for (i = 0; i < numNodes; i++) {
        graph->mat[i] = calloc(numNodes, sizeof(double));
    }
    return graph;
}

// functie de adaugare a unei muchii
void insert_edge(graphAdjMat_t *graph, int u, int v, double cost) {
    graph->mat[u][v]=cost;
}

// functie ce returneaza indexul elementului minim
int min(double v[], int n, int unvisited[]) {
    double min=INF, index;
    int i;
    for(i=0; i<n; i++) {
        if(v[i] < min && unvisited[i] == -1) {
            min=v[i];
            index = i;
        }
    }
    return index;
}

// functie ce calculeaza distantele minime de la sursa la celelalte noduri
void Dijkstra_Algorithm(graphAdjMat_t *graph, int source, double dist[], int *prev) {
    int *unvisited=malloc(graph->numNodes*sizeof(int));
    int i, v;
    for (v = 0; v < graph->numNodes; v++) {
        dist[v] = INF;
        prev[v] = 0;
        unvisited[v]=-1;
    }
    dist[source] = 0;
    for(i=0; i < graph->numNodes - 1; i++) {
        int u=min(dist, graph->numNodes, unvisited);
        unvisited[u]=0;
        for(v=0; v < graph->numNodes; v++) {
            if(unvisited[v] == -1 && (dist[u] + graph->mat[u][v]) < dist[v]
            && dist[u] != INF && graph->mat[u][v] != 0) {
                dist[v] = dist[u] + graph->mat[u][v];
                prev[v]=u;
            }
        }
    }
    free(unvisited);
}

// functie ce construieste o lista cu calea drumului dintre noduri
list_t *Dijkstra_path(int prev[], int nr, int source, int target) {
    list_t *list = init_list();
    int u = target, ok=0;
        while(ok == 0) {
            if(prev[u] == source) {
                insert_node(list, u);
                insert_node(list, prev[u]);
                ok=1;
            }
            else {
                insert_node(list, u);
            }
            u = prev[u];
        }
    return list;
}

int top(stack_t *stack) {
    if(stack->len==0) {
        return 0;
    }
    else {
        return stack->head->data;
    }
}

// functie ce calculeaza minimul a doua valori
int min_Tarjan(int a, int b) {
    if(a < b) {
        return a;
    }
    else {
        return b;
    }
}

// functie de interschimbare a 2 valori
void swap_numbers(int *a, int *b) {
    int temp=*a;
    *a=*b;
    *b=temp;
}

// functie de introducere in lista sortata
int insert_sorted(list_t *list, int data) {
    node_t *nou = init_node(data);
    if( list->len == 0 ) {
        list->head = nou;
        list->tail = nou;
        list->len++;
        return 0;
    }
    node_t *aux = list->head;
    if(aux->data > nou->data ) {
        nou->next = aux;
        aux->prev = nou;
        list->head = nou;
        list->len++;
        return 0;
    }
    aux = list->tail;
    if(aux->data < nou->data) {
        nou->prev = aux;
        aux->next = nou;
        list->tail = nou;
        list->len++;
        return 0;
    }
    aux = list->head;
    while( aux != NULL && aux->data < nou->data )
        aux=aux->next;
    nou->prev = aux->prev;
    nou->next = aux;
    aux->prev->next = nou;
    aux->prev = nou;
    list->len++;
    return 0;
}

// functie ce determina componentele tare conexe din graf
void Strongly_Connected(int v, graphAdjMat_t *graph, int *disc, int *low, stack_t *stack,
int *in_stack, int *deposits, int d, int *nr_componente, list_t *SCCs) {
    static int time = 0;
    disc[v] = time;
    low[v] = time;
    time++;
    push(stack, v);
    int k, u;
    in_stack[v] = 1;
    // parcurgem fiecare muchie
    for(u = 0; u < graph->numNodes; u++) {
        if(graph->mat[v][u] != 0) {
            // u nu a fost vizitat
            if(disc[u] == -1) {
                Strongly_Connected(u, graph, disc, low, stack, 
                in_stack, deposits, d, nr_componente, SCCs);
                low[v] = min_Tarjan(low[v], low[u]);
            }
            else {
                // back-edge
                if(in_stack[u] == 1) {
                    low[v] = min_Tarjan(low[v], disc[u]);
                }
            }
        }
    }
    if(low[v] == disc[v]) {
        int y, out, ok=0;
        while(top(stack) != v) {
            y = pop(stack);
            in_stack[y] = 0;
            for(k = 0; k < d; k++) {
                if(y == deposits[k]) {
                    out=pop(stack);
                    ok=1;
                }
            }
            if(ok == 0) {
                insert_node(SCCs, y);
            }
            ok=0;
        }
        int x = pop(stack);
        in_stack[x] = 0;
        for(k = 0; k < d; k++) {
            if(x == deposits[k]) {
                out=pop(stack);
                ok=1; 
            }
        }
        if(ok == 0) {
            insert_node(SCCs, x);
            insert_node(SCCs, -1);
            *nr_componente = *nr_componente + 1;
        }
    }
}

// Algoritmul lui Tarjan
void Tarjan_Algorithm(graphAdjMat_t *graph, int *deposits, int d) {
    int *disc=malloc(graph->numNodes*sizeof(int));
    int *low=malloc(graph->numNodes*sizeof(int));
    // vector care retine prezenta in stiva
    int *in_stack=malloc(graph->numNodes*sizeof(int));
    stack_t *stack=init_list();
    int i, j;
    for(i = 0; i < graph->numNodes; i++) {
        disc[i] = -1;
        low[i] = -1;
        in_stack[i]=0;
    }
    int number=0;
    // SCCs contine toate componentele tare conexe
    list_t *SCCs=init_list();
    // sortedSCCs contine toate componentele tare conexe sortate
    list_t *sortedSCCs=init_list();
    for(i = 0; i < graph->numNodes; i++) {
        if(disc[i] == -1) {
            Strongly_Connected(i, graph, disc, low, stack, 
            in_stack, deposits, d, &number, SCCs);
        }
    }
    printf("%d\n", number);
    int max=0, maxSCC=0;
    node_t *aux = SCCs->head;
    while(aux != NULL) {
        list_t *SCC = init_list();
        while(aux->data != -1) {
            insert_sorted(SCC, aux->data);
            maxSCC++;
            aux = aux->next;
        }
        node_t *aux2 = SCC->head;
        while(aux2 != NULL) {
            insert_node(sortedSCCs, aux2->data);
            aux2 = aux2->next;
        }
        if(max < maxSCC)
            max = maxSCC;
        maxSCC = 0;
        insert_node(sortedSCCs, -1);
        destroyList(SCC);
        aux = aux->next;
    }
    // matrix contine componentele tare conexe sortate
    int **matrix=malloc(number*sizeof(int*));
    for(i = 0; i < number; i++) {
        matrix[i]=malloc(max*sizeof(int));
        for(j = 0; j < max; j++) {
            matrix[i][j] = -1;
        }
    }
    aux = sortedSCCs->head;
    // lines contine primul element de pe fiecare linie a matricei
    int *lines=malloc(number*sizeof(int));
    // index contine indicii liniilor matricei
    int *index = malloc(number*sizeof(int));
    int ok = 0, u = 0, v = 0;
    while(aux != NULL) {
        if(aux->data == -1) {
            ok = 1;
            u++;
            v = 0;
        }
        if(ok == 0) {
            matrix[u][v] = aux->data;
            v++;
        }
        ok = 0;
        aux = aux->next;
    }
    for(i = 0; i < number; i++) {
        lines[i] = matrix[i][0];
        index[i] = i;
    }
    // sortam lines si index pentru a stii ordinea sortata
    for(i = 0; i < number - 1; i++) {
        for(j = 0; j < number-i-1; j++) {
            if(lines[j] > lines[j + 1]) {
                swap_numbers(&lines[j], &lines[j+1]);
                swap_numbers(&index[j], &index[j+1]);
            }
        }
    }
    for(i = 0; i < number; i++) {
        for(j = 0; j < max; j++) {
            if(j != max - 1) {
                if(matrix[index[i]][j+1] != -1) {
                    printf("%d ", matrix[index[i]][j]);
                }
                else { 
                    printf("%d", matrix[index[i]][j]);
                    break;
                }
            }
            else {
                if(matrix[index[i]][j] != -1) {
                    printf("%d", matrix[index[i]][j]);
                }
            }
        }
        if(i != number - 1)
            printf("\n");
    }
    for(i = 0; i < number; i ++) {
        free(matrix[i]);
    }
    free(matrix);
    free(lines);
    free(index);
    free(disc);
    free(low);
    free(in_stack);
    destroyList(SCCs);
    destroyList(sortedSCCs);
    destroyList(stack);
}

// functie care face coada sa se comporte ca o coada de prioritati
void get_min_cost(queue_t *q) {
    nodeQueue_t *node=q->tail;
    double min=INF;
    // aflam nodul minim
    nodeQueue_t *min_node;
    while(node != NULL) {
        if(node->info->cost < min) {
            min = node->info->cost;
            min_node = node;
        }
        node = node->prev;
    }
    // interschimbam nodul minim cu ultimul nod
    currStore_t *x = q->tail->info;
    q->tail->info = min_node->info;
    min_node->info = x;
}

// Dijkstra modificat
void  Dijkstra_Modified(graphAdjMat_t *graph, int source, double **distance, 
int *ID, int nr, int *deposits, int d) {
    int power = 1 << graph->numNodes;
    int w, v;
    for (v = 0; v < graph->numNodes; v++) {
        for (w = 0; w < power -1; w++) {
            distance[v][w] = INF; 
        }
    }
    queue_t *q = malloc(sizeof(queue_t));
    q->head = q->tail = NULL;
    q->len = 0;
    distance[source][0]= 0;
    currStore_t *aux = malloc(sizeof(currStore_t)), *u;
    aux->bitmask = 0;
    aux->cost = 0;
    aux->store = source;
    enqueue(q, aux);
    int k;
    while(q->len > 0) {
        // aflam minimul din coada 
        get_min_cost(q);
        u=dequeue(q);
        int id = u->store, newmask = u->bitmask, ok = 0, mask = u->bitmask;
        for(v=0; v < graph->numNodes; v++) {
            if(graph->mat[id][v] != 0) {
                mask=newmask;
                // verificam daca face parte din zona
                for(k = 0; k < nr; k++) {
                    if(ID[k] == v) {
                        newmask |= (1 << v);
                        ok=1;
                    }
                }
                // verificam daca e depozit
                for(k = 0; k < d; k ++)
                    if(deposits[k] && deposits[k] != source)
                        ok=1;
                if(source == v && id != 0) {
                    ok = 1;
                }
                if(distance[id][mask] + graph->mat[id][v] < distance[v][newmask]) {
                    distance[v][newmask] = distance[id][mask] + graph->mat[id][v];
                    aux->store = v;
                    aux->cost = distance[v][newmask];
                    aux->bitmask = newmask;
                    enqueue(q, aux);                    
                }
            }
        }
    }
    free(aux);
    free(q);
}

// functie de eliberare a memoriei unui graf
void freeGraph(graphAdjMat_t **g) {
    if (*g == NULL)
        return;
    int u;
    for (u = 0; u < (*g)->numNodes; u++)
        free((*g)->mat[u]);
    free((*g)->mat);
    free(*g);
    *g = NULL;
}
