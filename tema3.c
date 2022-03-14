#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "queue.c" // biblioteca pentru implementare si opreatii cu coada
#include <limits.h>

typedef struct node_t {
 char *nume;     // numele actorului
 int n;         // numarul actorului in vectorul de actori
 struct node_t **neigh; // lista de succesori ai unui nod
 int nr_neigh; // numarul de vecini
} node_t; 

typedef struct {
    int from;
    int to;
} edge_t;

typedef struct {
    int nr_noduri; //numar noduri
    node_t **v;  // lista de adiacenta/vector de pointeri la liste de succesori
} graph_t;

void initGraph (graph_t g, int n) {    // funcție de inițializare a grafului
    g.nr_noduri = n; // număr de noduri
    g.v = (node_t**) calloc(n+1, sizeof(node_t*));// inițializeaza pointeri cu 0
}


////////////////////////////////////////// TASK 1 /////////////////////////////////////////
void dfs_recursive(graph_t g, int source, int *visited, int *count, char **v) {   // functie care parcurge graful in adancime recursiv
  int i,j;                                                                       // si retine in "count" numarul de noduri vizitate
  visited[source] = 1;
  v[(*count)] = (char*)malloc((strlen(g.v[source]->nume)+1)*sizeof(char));
  strcpy(v[(*count)], g.v[source]->nume);
  (*count)++;
  for (i = 0; i < g.nr_noduri; i++){
    int ok = 0;
    for (j = 0; j < g.v[source]->nr_neigh; j++)  
        if ( g.v[source]->neigh[j]->n == i)
           ok = 1;     
    if ( ok == 1 && visited[i] == 0){
        dfs_recursive(g, i, visited, count, v);
    }
  }
}

int cmp_char(const void *s1, const void *s2) {  // functie care compara doua siruri de caractere
    char** sir1 = (char**) s1;
    char** sir2 = (char**) s2;
    return strcmp(*sir1, *sir2);
}


///////////////////////////////// TASK 2 /////////////////////////////////////////////////
int bfs_iterative(graph_t g, int source, int dest) {     // functie care parcurge graful in latime iterativ din nodul source pana in nodul dest
    dequeue_t *q = alloc_deq();
    int *visited = calloc(g.nr_noduri,sizeof(int));
    int *dist = malloc(g.nr_noduri*sizeof(int));
    int x, y;
    dist[source] = 0;
    visited[source] = 1; 
    push_back(q, source);
    while ( q->front != NULL) {
        front(q, &x);
        pop_front(q);  // scoate elementul in coada
        for (y = 0; y < g.nr_noduri; y++){
            int ok = 0;
            int j;
            for (j = 0; j < g.v[x]->nr_neigh; j++)  
                if ( g.v[x]->neigh[j]->n == y){
                    ok = 1; break;
                }    
            if ( ok == 1  && visited[y] == 0){
                visited[y] = 1; 
                dist[y] = dist[x] + 1;
                push_back(q, y);  // pune elementul in coada

                if( y == dest){  // a ajuns la nodul destinatie
                    int z = dist[y];
                    free_queue(q);
                    free(q);
                    free(visited);
                    free(dist);
                    return z;
                }
                  
            }
        }
    }
    free_queue(q);
    free(q);
    free(visited);
    free(dist);
    return -1;
}


///////////////////////////////// TASK 3 /////////////////////////////////////////////////
int min( int a, int b){  // functia determina minimul dintre doua elemente
    if (a > b)
       return b;
    return a;
}

char** save(graph_t g, int x, int y, char **r, int *k){ // functia retine numele a doi actori in ordine alfabetica
   
    if ( strcmp(g.v[x]->nume, g.v[y]->nume) < 0){
        char *s = calloc(strlen(g.v[x]->nume)+strlen(g.v[y]->nume)+1,sizeof(char));
        strncpy(s, g.v[x]->nume, strlen(g.v[x]->nume)-1);
        strcat( s, " ");
        strcat( s,  g.v[y]->nume);
        if( *k != 0)
           r = realloc(r, ((*k)+1)*sizeof(char*));
        r[(*k)] = (char*)calloc(strlen(s)+1,sizeof(char));
        strcpy(r[(*k)], s);
        (*k)++;
        free(s);
    }else
    {
        char *s = calloc(strlen(g.v[x]->nume)+strlen(g.v[y]->nume)+1,sizeof(char));
        strncpy(s, g.v[y]->nume, strlen(g.v[y]->nume)-1);
        strcat( s, " ");
        strcat( s,  g.v[x]->nume);
        if( *k != 0)
           r = realloc(r, ((*k)+1)*sizeof(char*));
        r[(*k)] = (char*)calloc(strlen(s)+1,sizeof(char));
        strcpy(r[(*k)], s);
        (*k)++;
        free(s);
    }
   return r;
}

void dfsB(graph_t g, int x, int timp, int *idx, int *low, int *p,  char ***r, int *k){ // functia determina muchiile care pot fi eliminate 
                                                                                      //pentru a crea noi componente conexe
    idx[x] = timp;
    low[x] = timp;
    timp = timp+1;
    int i;
    for (i = 0; i < g.nr_noduri; i++){
        int ok = 0;
        int j;
        for (j = 0; j < g.v[x]->nr_neigh; j++)  
            if ( g.v[x]->neigh[j]->n == i){ // daca nodul x este vecin cu i
                ok = 1; break;
            }   
        if( ok ==1 &&  p[x] != i ){
          if( idx[i] == -1 ){
              p[i] = x;
              dfsB(g, i, timp, idx, low, p, r, k);
              low[x] = min(low[x], low[i]);
              if( low[i] > idx[x])
                *r = save(g, x, i, *r, k); // muchia este retinuta
          }else
          {
              low[x] =  min(low[x], low[i]);
          }
        }
    }
 }

void punti( graph_t g, char ***r, int *k){
    
    int idx[g.nr_noduri];
    int low[g.nr_noduri];
    int p[g.nr_noduri];
    int timp = 0;
    int i;
    for (i = 0; i < g.nr_noduri; i++){ // initializeaza vectorii
        idx[i] = -1;
        low[i] = INT_MAX;
        p[i] = -1;
    }
    for (i = 0; i < g.nr_noduri; i++){ // pentru fiecare nod din graf
        if( idx[i] == -1)
            dfsB(g, i, timp, idx, low, p, r, k);
    }
}

int main(int argc, char **argv)
{
    FILE *in = fopen(argv[2], "r"); 
    char aux[1000];
    fgets(aux, 1000, in);
    int h = atoi(aux);  // numarul de filme ce vor fi citite
    graph_t g;
    g.nr_noduri = 0; // numarul de noduri
    g.v = (node_t**) calloc(1, sizeof(node_t*));// initializare pointeri cu 0
    int i;
    int k = 0;
    for( i = 0; i<h; i++){  // citeste toate filmele
        char titlu[1000];
        fgets(titlu, 1000, in);  // citeste titlul filmului
        char aux[1000];
        fgets(aux, 1000, in);  // citeste numarul de actori
        int nr = atoi(aux);  // numarul de actori din filmul curent
        g.nr_noduri = g.nr_noduri + nr;  // actualizeaza numarul total de actori
        g.v = (node_t**) realloc(g.v, (g.nr_noduri+nr+1)*sizeof(node_t*));
        int j;
        int x = 0; // va retine numarul actorii deja existenti
        int *c;
        c =(int*)calloc(nr,sizeof(int)); // va retine actorii deja existenti
        for( j = g.nr_noduri -nr; j<g.nr_noduri; j++){
            char nume[1000];
            fgets(nume, 1000, in); // citeste numele actorului
            if( i != 0){ // daca NU este primul film citit
                int o;
                int ok = 0;
                for( o = 0; o<g.nr_noduri - nr; o++){  // cauta daca acotul citit exista deja 
                    if( strcmp(g.v[o]->nume, nume) == 0){
                        ok = 1;
                        c[x++] = o;
                    }
                }
                if( ok == 0 ){ // daca actorul este nou, este adaugat in lista
                    node_t *nou = (node_t*) malloc(sizeof(node_t));
                    nou->nume = (char*)malloc((strlen(nume)+1)*sizeof(char));
                    strcpy(nou->nume, nume);
                    nou->n  = j;
                    nou->neigh = (node_t**) calloc(nr,sizeof(node_t*));
                    nou->nr_neigh = 0;
                    g.v[j] = nou;

                }else    // daca a fost gasit cel putin un actor deja existent
                {   g.nr_noduri--;
                    j--;
                    nr--;
                }
            }else // daca este primul film citit, se adauga toti actorii
            {   
                node_t *nou = (node_t*) malloc(sizeof(node_t));
                nou->nume = (char*)malloc((strlen(nume)+1)*sizeof(char));
                strcpy(nou->nume, nume);
                nou->n  = j;
                nou->neigh = (node_t**) malloc((nr+1)*sizeof(node_t*));
                nou->nr_neigh = 0;
                g.v[j] = nou;   
            }
        }

        k=g.nr_noduri -nr ;
        for( j = k; j<g.nr_noduri; j++){  // leaga toti actorii noi din filmul curent intre ei
            int m;
            for( m = k; m<g.nr_noduri; m++){
                if( j != m){
                    g.v[m]->neigh[g.v[m]->nr_neigh] = g.v[j];
                    g.v[m]->nr_neigh++;
                }
            }
        }
        if(i != 0 && x != 0){ // leaga toti actorii noi din filmul curent de cei deja existenti
            for( j = k; j<g.nr_noduri; j++){
                int o;
                for( o = 0; o<x; o++){
                        int aux = c[o];
                        g.v[aux]->neigh = realloc( g.v[aux]->neigh,( g.v[aux]->nr_neigh+2)*sizeof(node_t*));
                        g.v[aux]->neigh[ g.v[aux]->nr_neigh] = g.v[j];
                        g.v[aux]->nr_neigh++;
                        g.v[j]->neigh = realloc(g.v[j]->neigh,(g.v[j]->nr_neigh+2)*sizeof(node_t*));
                        g.v[j]->neigh[g.v[j]->nr_neigh] =  g.v[aux]; 
                        g.v[j]->nr_neigh++;
                }
            }
        }

        int o, m;
        for( o = 0; o<x; o++){  // leaga toti actorii deja existenti intre ei
            for( m = 0; m<x; m++){
                    if( o != m ){
                         int j;
                         int ok = 0;
                         for (j = 0; j < g.v[c[o]]->nr_neigh; j++)  // cauta daca cei doi acoti au deja legatura/sunt vecini
                             if ( g.v[c[o]]->neigh[j]->n == g.v[c[m]]->n){
                                ok = 1; break;
                         }  
                         if(ok == 0) {  // daca cei doi acoti nu sunt vecini, se face legaura
                            g.v[c[o]]->neigh = realloc( g.v[c[o]]->neigh,( g.v[c[o]]->nr_neigh+2)*sizeof(node_t*));
                            g.v[c[o]]->neigh[ g.v[c[o]]->nr_neigh] = g.v[c[m]];
                            g.v[c[o]]->nr_neigh++;
                            g.v[c[m]]->neigh = realloc(g.v[c[m]]->neigh,(g.v[c[m]]->nr_neigh+2)*sizeof(node_t*));
                            g.v[c[m]]->neigh[g.v[c[m]]->nr_neigh] =  g.v[c[o]]; 
                            g.v[c[m]]->nr_neigh++;
                         }
                }
            }
         }
        free(c);
    }

/////////////////////////// TASK 1 /////////////////////////////////////////
if( strcmp(argv[1], "-c1") == 0){
    fclose(in);

    int max = -1;
    int x;
    int j;
    int *visited = (int*)calloc(g.nr_noduri,sizeof(int));
    char **v = (char**)calloc(g.nr_noduri,sizeof(char*));
    for ( j = 0; j < g.nr_noduri; j++) {  // cauta componenta conexa cu cele mai multe elemente
        int count = 0;
        if (visited[j] == 0) {
           dfs_recursive(g, j, visited, &count, v);
           if(count > max){
               max = count;  // retine nodul de la care a pornit cautarea
               x = j;  // retine nodul de la care a pornit cautarea in componenta conexa
           }
           for ( j = 0; j < count; j++)
             free(v[j]);
        }
    }
    free(visited);
    free(v);
    visited = (int*)calloc(g.nr_noduri,sizeof(int));
    v = (char**)calloc(g.nr_noduri,sizeof(char*));
    int count = 0;
    dfs_recursive(g, x, visited, &count, v); 

    qsort(v, count, sizeof(char*), cmp_char); // sorteaza in ordine alfabetica
    FILE *out = fopen(argv[3], "w");
    fprintf(out, "%d", count);
    fputc('\n', out);
    for( j = 0; j<count; j++){
       fputs(v[j], out);
    }
    fclose(out); 
    free(visited);
    for ( j = 0; j < g.nr_noduri; j++)
        free(v[j]);
    free(v);

}

//////////////////////////// TASK 2 ////////////////////////////////
if( strcmp(argv[1], "-c2") == 0){
    char nume1[1000];
    fgets(nume1, 1000, in);  // citeste primul actor
    char nume2[1000];
    fgets(nume2, 1000, in);  // citeste al doilea actor
    fclose(in);
    int src, dest;
    int i, ok1 = 0, ok2 = 0;
    for (i = 0; i < g.nr_noduri; i++){   // gaseste pozitia la care se afla actorii in vectorul de actori
        if(ok1 == 1 && ok2 == 1){  //daca au fost gasiti amandoi
            break;   // se opreste cautarea
        }else{
            if( strcmp(g.v[i]->nume, nume1) == 0){
                src = g.v[i]->n;
                ok1 = 1;   // primul actor a fost gasit
            }
            if( strcmp(g.v[i]-> nume, nume2) == 0){ 
                dest = g.v[i]->n;
                ok2 = 1;   // al doilea actor a fost gasit
            }
        }
    
    }
    printf("%d %d\n", src, dest);
    FILE *out = fopen(argv[3], "w");
    int x;
    x = bfs_iterative(g, src, dest);  // determina gradul de inrudire
    fprintf(out, "%d", x);
    fputc('\n', out);
    fclose(out); 
}


//////////////////////// TASK 3 /////////////////////////////
if( strcmp(argv[1], "-c3") == 0){
    
    char **r = (char**)calloc(1,sizeof(char*));
    int k = 0;
    punti(g, &r, &k);
    qsort(r, k, sizeof(char*), cmp_char);  //sorteaza in ordine alfabetica
    FILE *out = fopen(argv[3], "w");
    fprintf(out, "%d", k);
    fputc('\n', out);
    int j;
    for( j = 0; j<k; j++){
       fputs(r[j], out);
    }
    fclose(out);

    for ( j = 0; j < k; j++)  // elibereaza memoria
        free(r[j]);
    free(r);
}

////////////////////// FREE Memory /////////////////////////////
    int j;
    for( j = 0; j<g.nr_noduri; j++){  //elibereara memoria ramasa ocupata
        free(g.v[j]->nume);
        free(g.v[j]->neigh);
        free(g.v[j]);
    }
    free(g.v);
    return 0;
}