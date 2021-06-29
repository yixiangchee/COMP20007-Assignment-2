/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "graph.h"
#include "utils.h"
#include "pq.h"

#define INITIALEDGES 32
#define INF 99999 // assume largest diameter relatively small

struct edge;

/* Definition of a graph. */
struct graph {
  int numVertices;
  int numEdges;
  int allocedEdges;
  struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
  int start;
  int end;
};

struct graph *newGraph(int numVertices){
  struct graph *g = (struct graph *) malloc(sizeof(struct graph));
  assert(g);
  /* Initialise edges. */
  g->numVertices = numVertices;
  g->numEdges = 0;
  g->allocedEdges = 0;
  g->edgeList = NULL;
  return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end){
  assert(g);
  struct edge *newEdge = NULL;
  /* Check we have enough space for the new edge. */
  if((g->numEdges + 1) > g->allocedEdges){
    if(g->allocedEdges == 0){
      g->allocedEdges = INITIALEDGES;
    } else {
      (g->allocedEdges) *= 2;
    }
    g->edgeList = (struct edge **) realloc(g->edgeList,
      sizeof(struct edge *) * g->allocedEdges);
    assert(g->edgeList);
  }

  /* Create the edge */
  newEdge = (struct edge *) malloc(sizeof(struct edge));
  assert(newEdge);
  newEdge->start = start;
  newEdge->end = end;

  /* Add the edge to the list of edges. */
  g->edgeList[g->numEdges] = newEdge;
  (g->numEdges)++;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g){
  int i;
  for(i = 0; i < g->numEdges; i++){
    free((g->edgeList)[i]);
  }
  if(g->edgeList){
    free(g->edgeList);
  }
  free(g);
}

/* Finds:
  - Number of connected subnetworks (before outage) (Task 2)
  - Number of servers in largest subnetwork (before outage) (Task 3)
  - SIDs of servers in largest subnetwork (before outage) (Task 3)
  - Diameter of largest subnetworks (after outage) (Task 4)
  - Number of servers in path with largest diameter - should be one more than
    Diameter if a path exists (after outage) (Task 4)
  - SIDs in path with largest diameter (after outage) (Task 4)
  - Number of critical servers (before outage) (Task 7)
  - SIDs of critical servers (before outage) (Task 7)
*/
struct solution *graphSolve(struct graph *g, enum problemPart part,
  int numServers, int numOutages, int *outages){
  struct solution *solution = (struct solution *)
    malloc(sizeof(struct solution));
  assert(solution);
  /* Initialise solution values */
  initaliseSolution(solution);
  if(part == TASK_2){
    /* TASK 2 SOLUTION HERE */
    solution->connectedSubnets = connectedSubnet(g,numServers);
  } else if(part == TASK_3) {
    /* TASK 3 SOLUTION HERE */
    int* subnet = (int*)malloc(numServers*sizeof(int));
    assert(subnet);
    solution->largestSubnet = largestSubnet(g,numServers,subnet);
    solution->largestSubnetSIDs = subnet;
  } else if(part == TASK_4) {
    /* TASK 4 SOLUTION HERE */
    int* postSID = (int*)malloc(numServers*sizeof(int));
    assert(postSID);
    int numPostDiameter = 0;
    solution->postOutageDiameter = 
      postDiameter(g,numServers,postSID,&numPostDiameter, outages, numOutages);
    solution->postOutageDiameterCount = numPostDiameter;
    solution->postOutageDiameterSIDs = postSID;
  } else if(part == TASK_7) {
    /* TASK 7 SOLUTION HERE */
    int* criticalSID = (int*)malloc(numServers*sizeof(int));
    assert(criticalSID);
    solution->criticalServerCount = criticalNodes(g,numServers,criticalSID);
    solution->criticalServerSIDs = criticalSID;
  }
  return solution;
}

/* returns the number of critical nodes in a network */
int criticalNodes(struct graph *g, int numServers, int* criticalSID){

  int numCritical = 0;
  int visited[numServers];
  int critical[numServers];
  int child[numServers];
  int HRA[numServers];
  int parent[numServers];
  int count = 0;
  
  /* initialises count array with 0 */
  for(int i=0; i < numServers; i++){
    visited[i] = critical[i] = child[i] = 0;
    parent[i] = -1;
  }

  /* perform depth-first search from each node */
  for(int v=0; v < numServers; v++){
    /* new root found */
    if(!visited[v]){
      dfsChild(v, visited, g, &count, critical, HRA, child, parent);
    }
  }

  for(int v=0; v < numServers; v++){
    if(critical[v]){
      criticalSID[numCritical++] = v;
    }
  }
  return numCritical;
}

/* performs dfs while finding critical nodes */
void dfsChild(int v, int* visited, struct graph *g, int* count, 
              int* critical,int* HRA, int*child, int* parent){
  *count += 1;
  visited[v] = *count;
  HRA[v] = visited[v];
  
  /* iterate through each edge to find adjacent nodes to search */
  for(int i=0; i<g->numEdges; i++){
    int start = g->edgeList[i]->start;
    int end = g->edgeList[i]->end;
    int w = 0;
    if(start == v || end == v){
      if(start == v){
        w = end;
      } else {
        w = start;
      }

      if(!visited[w]){
        child[v]++;
        parent[w] = v;
        dfsChild(w, visited, g, count, critical, HRA, child, parent);
        HRA[v] = min(HRA[v],HRA[w]);
        if (parent[v] == -1 && child[v] > 1){
          critical[v] = 1;
        }
        if (parent[v] != -1 && HRA[w] >= visited[v]){
          critical[v] = 1;
        }
      } else if (w != parent[v]) {
        HRA[v] = min(HRA[w], HRA[v]);
      }
    }
  }
}

/* calculates the minimum between two numbers */
int min(int a, int b){
  if(a<b){
    return a;
  }
  return b;
}

/* returns the number of connected subnetworks of the graph before outage */
int connectedSubnet(struct graph *g, int numServers){
  int visited[numServers];
  int numComponents = 0;
  int count = 0;
  int lowSID = INT_MAX;

  /* initialises count array with 0 */
  for(int i=0; i < numServers; i++){
    visited[i] = 0;
  }

  /* perform depth-first search from each node */
  for(int v=0; v < numServers; v++){
    /* new component found */
    if(!visited[v]){
      dfsExplore(v,visited,g,&count,&lowSID);
      numComponents ++;
    }
  }
  return numComponents;
}

/* returns number of servers in the largest subnetwork before outage */
int largestSubnet(struct graph *g, int numServers, int* subnet){
  int visited[numServers];
  int count = 0, numComponents = 0;
  int start, end;
  int max = 0;
  int minSID = INT_MAX;

  /* initialises count array with 0 */
  for(int i=0; i < numServers; i++){
    visited[i] = 0;
  }

  /* perform depth-first search from each node */
  for(int v=0; v < numServers; v++){
    /* new component found */
    if(!visited[v]){
      int before = count;
      int lowSID = INT_MAX;
      dfsExplore(v,visited,g,&count,&lowSID);
      numComponents ++;
      
      /* update details if bigger subnetwork found (tiebreak with lower SID) */
      if((count-before) > max || ((count-before) == max && (lowSID < minSID))){
        max = count - before;
        start = before + 1;
        end = count;
        minSID = lowSID;
      }
      //printf("Min SID: %d\n",minSID);
    }
  }

  /* fills and sorts the array with servers from biggest subnetwork */
  int curr = 0;
  for(int v=0; v < numServers; v++){
    if(visited[v] <= end && visited[v] >= start){
      subnet[curr++] = v;
    }
  }
  qsort(subnet, max, sizeof(int), cmpfunc);

  return max;
}

/* comparator function for qsort */
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

/* returns the diameter of largest-diameter subnetwork post-outage,
   number of its servers, and fills an array with the servers in it */
int postDiameter(struct graph *g, int numServers, int* postSID, 
                  int* numPostDiameter, int* outages, int numOutages){
    
    int maxPostDiameter = 0;

    int D[numServers][numServers];
    int P[numServers][numServers];

    /* initialise Distance matrix with inifinity distance */
    for(int i=0; i<numServers; i++){
      for(int j=0; j<numServers; j++){
        D[i][j] = INF;
        P[i][j] = INF;
      }
    }

    /* distance between connected servers is 1, and disconnect outage servers */
    for(int i=0; i<numServers; i++){
      if(isOutage(i,outages,numOutages)){
        continue;
      }
      for(int j=0; j<numServers; j++){
        if(isOutage(j,outages,numOutages)){
          continue;
        }
        for(int k=0; k<g->numEdges; k++){
          if((g->edgeList[k]->start == i && g->edgeList[k]->end == j) ||
          (g->edgeList[k]->start == j && g->edgeList[k]->end == i)){
            D[i][j] = 1;
            P[i][j] = i;
          }
        }
      }
    }

    /* performs floyd's algorithm */
    for(int k=0; k<numServers; k++){
      for(int i=0; i<numServers; i++){
        for(int j=0; j<numServers; j++){
          int alt = D[i][k] + D[k][j];
          if(alt < D[i][j]){
            D[i][j] = alt;
            P[i][j] = P[k][j];
          } else {
            // do nothing
          }
        }
      }
    }

    /* processes table and finds longest diameter */
    int start, end;
    for(int i=0; i<numServers; i++){
      for(int j=0; j<numServers; j++){
        if(i == j || D[i][j] == INF){
          D[i][j]= INF;
        }
        if(D[i][j] > maxPostDiameter &&  D[i][j] != INF){
          maxPostDiameter = D[i][j];
          start = i;
          end = j;
        }
        //printf("%6d ",D[i][j]);
      }
      //printf("\n");
    }

    /* processes Predecessor matrix */
    *numPostDiameter = maxPostDiameter + 1;
    //printf("\n");
    for(int i=0; i<numServers; i++){
      for(int j=0; j<numServers; j++){
        if(i == j|| P[i][j] == INF){
          P[i][j] = INF;
        }
        //printf("%6d ",P[i][j]);
      }
      //printf("\n");
    }
    
    /* backtracks using predecessor matrix */
    int curr = end;
    int n = *numPostDiameter - 1;
    postSID[n--] = end;
    while(curr != start){
      //printf("Curr: %d \n",curr);
      postSID[n--] = P[start][curr];
      curr = P[start][curr];
    }
    
    return maxPostDiameter;
}

/* returns true if server is out */
int isOutage(int server, int* outages, int numOutages){
  for(int i=0; i<numOutages; i++){
    if(server == outages[i]){
      return 1;
    }
  }
  return 0;
}

/* performs depth-first search on graph */
void dfsExplore(int v, int* visited, struct graph *g, int* count, int* lowSID){
  *count += 1;
  visited[v] = *count;
  if(v<(*lowSID)){
    *lowSID = v;
  }
  
  /* iterate through each edge to find adjacent nodes to search */
  for(int i=0; i<g->numEdges; i++){
    int start = g->edgeList[i]->start;
    int end = g->edgeList[i]->end;

    if(start == v && !visited[end]){
      dfsExplore(end,visited,g,count,lowSID);
    } else if (end == v && !visited[start]){
      dfsExplore(start,visited,g,count,lowSID);
    }
  }
}
