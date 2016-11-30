#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <igraph.h>
using namespace std;

void vector_print(igraph_vector_t *v) {
  long int i;
  for (i=0; i<igraph_vector_size(v); i++) {
    printf(" %li", (long int) VECTOR(*v)[i]);
  }
  printf("\n");
}

int main() {
  int i = 0;
  igraph_t g;
  igraph_vector_t vids, layers, parents;
  igraph_vector_t gtypes, vtypes, etypes;
  igraph_strvector_t gnames, vnames, enames;

  igraph_i_set_attribute_table(&igraph_cattribute_table);

  igraph_vector_init(&gtypes, 0);
  igraph_vector_init(&vtypes, 0);
  igraph_vector_init(&etypes, 0);
  igraph_strvector_init(&gnames, 0);
  igraph_strvector_init(&vnames, 0);
  igraph_strvector_init(&enames, 0);


  // igraph_ring(&g, 10, IGRAPH_UNDIRECTED, 0, 1);
  //igraph_erdos_renyi_game(&g, IGRAPH_ERDOS_RENYI_GNP, 100, 0.2,
  //            IGRAPH_DIRECTED, IGRAPH_NO_LOOPS);
  //igraph_grg_game(&g, 100, sqrt(2) / 2, 1, 0, 0);
  igraph_tree(&g, 100, 3, IGRAPH_TREE_IN);
  igraph_cattribute_list(&g, &gnames, &gtypes, &vnames, &vtypes, &enames,
                         &etypes);

  igraph_write_graph_graphml(&g, stdout, 0);
  printf("Graph attributes: ");
  for (i = 0; i < igraph_strvector_size(&gnames); i++) {
    printf("%s (%i) ", STR(gnames, i), (int)VECTOR(gtypes)[i]);
  }
  printf("\n");
  printf("Vertex attributes: ");
  for (i = 0; i < igraph_strvector_size(&vnames); i++) {
    printf("%s (%i) ", STR(vnames, i), (int)VECTOR(vtypes)[i]);
  }
  printf("\n");
  printf("Edge attributes: ");
  for (i = 0; i < igraph_strvector_size(&enames); i++) {
    printf("%s (%i) ", STR(enames, i), (int)VECTOR(etypes)[i]);
  }
  printf("\n");

  {
    igraph_vector_init(&vids, 0);
    igraph_vector_init(&layers, 0);
    igraph_vector_init(&parents, 0);

    igraph_i_bfs(&g, 0, IGRAPH_IN, &vids, &layers, &parents);

    //vector_print(&vids);
  }

  igraph_destroy(&g);


  igraph_vector_destroy(&vids);
  igraph_vector_destroy(&layers);
  igraph_vector_destroy(&parents);

  igraph_strvector_destroy(&enames);
  igraph_strvector_destroy(&vnames);
  igraph_strvector_destroy(&gnames);
  igraph_vector_destroy(&etypes);
  igraph_vector_destroy(&vtypes);
  igraph_vector_destroy(&gtypes);

  return 0;
}
