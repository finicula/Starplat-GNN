#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string.h>
#include <climits>
#include <cmath>
#include <random>
#include "graph_ompv2.hpp"

class env;
extern env environment;

// using namespace std;

class edge
{
public:
  int32_t source;
  int32_t destination;
  float weight;
  int32_t id; /* -unique Id for each edge.
                 -useful in adding properties to edges. */
  int dir;
};

// bool counter=true;
class graph
{
private:
  int32_t nodesTotal;
  int32_t edgesTotal;
  float *edgeLen;
  float *diff_edgeLen;
  float *rev_edgeLen;
  float *diff_rev_edgeLen;
  char *filePath;
  std::map<int32_t, std::vector<edge>> edges;

public:
  int32_t *indexofNodes;     /* stores prefix sum for outneighbours of a node*/
  int32_t *rev_indexofNodes; /* stores prefix sum for inneighbours of a node*/
  int32_t *edgeList;         /*stores destination corresponding to edgeNo.
                               required for iteration over out neighbours */
  int32_t *srcList;          /*stores source corresponding to edgeNo.
                               required for iteration over in neighbours */
  std::vector<edge> graph_edge;
  int32_t *diff_indexofNodes;
  int32_t *diff_edgeList;
  int32_t *diff_rev_indexofNodes;
  int32_t *diff_rev_edgeList;
  int32_t *perNodeCSRSpace;
  int32_t *perNodeRevCSRSpace;
  int32_t *edgeMap;
  std::map<int, int> outDeg;
  std::map<int, int> inDeg;

  graph(char *file);

  std::map<int, std::vector<edge>> getEdges();

  float *getEdgeLen();

  int num_nodes();

  // function to get total number of edges
  int num_edges();

  std::vector<edge> getInOutNbrs(int v);

  edge getEdge(int s, int d);

  // library function to check candidate vertex is in the path from root to dest in SPT.
  bool inRouteFromSource(int candidate, int dest, int *parent);

  bool check_if_nbr(int s, int d);
  int common_nbrscount(int node1, int node2);
  int getOutDegree(int v);

  int getInDegree(int v);
  void addEdge(int src, int dest, int aks);
  void delEdge(int src, int dest);
  void changeWeight(int src, int dest, float weight);
  std::vector<update> parseUpdates(char *updateFile);
  std::vector<update> getDeletesFromBatch(int updateIndex, int batchSize, std::vector<update> updateVec);
  std::vector<update> getAddsFromBatch(int updateIndex, int batchSize, std::vector<update> updateVec);
  void propagateNodeFlags(bool *modified);
  void parseEdges();
  void parseEdgesResidual();
  void parseGraphResidual();
  void parseGraph();
  /******************************|| Dynamic Graph Libraries ||********************************/

  void updateCSRDel(std::vector<update> &batchUpdate, int k, int size);
  void updateCSRAdd(std::vector<update> &batchUpdate, int k, int size);
  std::vector<edge> getNeighbors(int node);
  std::vector<edge> getInNeighbors(int node);
};

class Layer
{
public:
  int32_t num_features;
  float **weights;
  float *bias;
  float *output;
  float *input;
  float *grad_input;
  float **grad_weights;
  float *grad_bias;
  float *grad_output;
  

};

class GNN
{
  graph &g;
  std::vector<Layer> layers;
  std::vector<float> features;
  std::vector<int32_t> labels;
  int32_t feature_size;
  int32_t num_classes;
  char *featFile, *labFile;

public:
  GNN(graph &g, char *featFile, char *labFile);
  graph &getGraph();
  void loadFeatures();
  void loadLabels();
  void gcn_preprocessing();
  void initializeLayers(std::vector<int> neuronsPerLayer, char *initType);
};

class env
{
  char *backend;
  char *algoType;
  char *filename;

public:
  env(char *backend, char *algoType, char *filename);
  char *get_backend();
};