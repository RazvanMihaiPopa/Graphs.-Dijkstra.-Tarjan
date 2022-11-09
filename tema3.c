#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define INF 99999
#include "tema3.h"
// #include "heap.h"

int main() {
    int n, m, d, i, j;
    scanf("%d%d%d", &n, &m, &d);
    graphAdjMat_t *graph = initGraph(n);
    int u, v;
    double w;
    // se creeaza graful
    for(i = 0; i < m; i++) {
        scanf("%d%d%lf", &u, &v, &w);
        insert_edge(graph, u, v, w);
    }
    // vectorul de depozite
    int *deposits = malloc(d*sizeof(int));
    int nr_tasks, dep;
    for(i = 0; i < d; i++) {
        scanf("%d", &dep);
        deposits[i] = dep;
    }
    scanf("%d", &nr_tasks);
    for(i = 0; i < nr_tasks; i++) {
        char task[2];
        scanf("%s", task);
        if(strcmp(task, "e1") == 0) {
            // s = depozitul din care se incarca marfa
            // k = numarul de magazine ce vor fi aprovizionate
            int s, k;
            scanf("%d%d", &s, &k);
            // ID-urile magazinelor ce vor fi aprovizionate
            int *to_be_supplied = malloc(k*sizeof(int));
            double totalcost=0;
            for(j = 0; j < k; j++) {
                scanf("%d", &to_be_supplied[j]);
                // vector de distante minime de la sursa
                double *dist_to=malloc(sizeof(double)*graph->numNodes);
                // vector in care stocam drumurile din sursa cu costul cel mai mic 
                int *prev_to=malloc(sizeof(int)*graph->numNodes);
                // vector de distante minime de la destinatie
                double *dist_back=malloc(sizeof(double)*graph->numNodes);
                // vector in care stocam drumurile din destinatie cu costul cel mai mic
                int *prev_back=malloc(sizeof(int)*graph->numNodes);
                Dijkstra_Algorithm(graph, s, dist_to, prev_to);
                Dijkstra_Algorithm(graph, to_be_supplied[j], dist_back, prev_back);
                printf("%d\n", to_be_supplied[j]);
                printf("%.1lf %.1lf\n", dist_to[to_be_supplied[j]], dist_back[s]);
                // lista ce contine drumul de la sursa la destinatie
                list_t *path_to=Dijkstra_path(prev_to, graph->numNodes, s, to_be_supplied[j]);
                // lista ce contine drumul de la destinatie la sursa
                list_t *path_back=Dijkstra_path(prev_back, graph->numNodes, to_be_supplied[j], s);
                node_t *aux=path_to->tail;
                while(aux != NULL) {
                    printf("%d ", aux->data);
                    aux=aux->prev;
                }
                aux=path_back->tail->prev;
                while(aux != NULL) {
                    if(aux->prev == NULL) {
                        printf("%d", aux->data);
                    }
                    else {
                        printf("%d ", aux->data);
                    }
                    aux=aux->prev;
                }
                totalcost += dist_to[to_be_supplied[j]] + dist_back[s];
                if(j == k-1)
                    printf("\n%.1lf", totalcost);
                if(j != k-1)
                    printf("\n");
                destroyList(path_to);
                destroyList(path_back);
                free(dist_to);
                free(prev_to);
                free(dist_back);
                free(prev_back);
            }
            free(to_be_supplied);
        }
        if(i != nr_tasks -1) {
            printf("\n");
        }
        if(strcmp(task, "e2") == 0) {
            // Graful fara depozite
            int k,l;
            graphAdjMat_t *graph_without_deposits=initGraph(graph->numNodes);
            for(i = 0; i < graph->numNodes; i++)
                for(j = 0; j < graph->numNodes; j++) {
                    graph_without_deposits->mat[i][j] = graph->mat[i][j];
                }
            for(k = 0; k < graph->numNodes; k++) {
                for(l = 0; l < d; l++) {
                    graph_without_deposits->mat[k][deposits[l]]=0;
                    graph_without_deposits->mat[deposits[l]][k]=0;
                }
            }
            // Algoritmul lui Tarjan afla componentele tare conexe
            Tarjan_Algorithm(graph_without_deposits, deposits, d);
            freeGraph(&graph_without_deposits);
        }
        if(strcmp(task, "e3") == 0) {
            // numarul de zone
            int r, k, index = 0, v;
            scanf("%d", &r);
            // numarul de numere din fiecare zona
            int *nr = malloc(r*sizeof(int));
            double cost = INF;
            for(v = 0; v < r; v++) {
                scanf("%d", &nr[i]);
                // nodurile dintr-o zona
                int *ID = malloc(nr[i]*sizeof(int));
                for(j = 0; j < nr[i]; j++) {
                    scanf("%d", &ID[j]);
                }
                // matrice de distante minime in functie de masca
                double **distance = malloc(sizeof(double*)*graph->numNodes);
                for(j = 0; j < graph->numNodes; j++) {
                    distance[j]=malloc(sizeof(double)*(1<<graph->numNodes + 1));
                }
                // masca in cazul in care sunt vizitate nodurile din ID
                int visitedmask=0;
                for(k = 0; k < nr[i]; k++) {
                    visitedmask = visitedmask | (1 << ID[k]);
                }
                // Aplicam Dijkstra modificat pentru fiecare depozit
                for(k = 0; k < d; k++) {
                    Dijkstra_Modified(graph, deposits[k], distance, ID, nr[i], deposits, d);
                    if(distance[deposits[k]][visitedmask] < cost)
                        cost = distance[deposits[k]][visitedmask];
                }
                if(index != r-1) {
                    printf("%.1lf\n", cost);
                    index++;
                }
                else {
                    printf("%.1lf", cost);
                }
                for(j = 0; j < graph->numNodes; j++) {
                    free(distance[j]);
                }
                free(distance);
                free(ID);
            }   
            free(nr);
        } 
    }
    free(deposits);
    freeGraph(&graph);
}