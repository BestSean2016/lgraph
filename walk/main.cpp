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


igraph_bool_t bfs_callback(const igraph_t *graph,
               igraph_integer_t vid,
               igraph_integer_t pred,
               igraph_integer_t succ,
               igraph_integer_t rank,
               igraph_integer_t dist,
               void *extra) {
  printf(" %li", (long int) vid);
  char strid[32];
  snprintf(strid, 32, "%d", vid);
  SETVAS((igraph_t *)graph, "NodeLabel", vid, strid);
  return 0;
}

int walk_1() {
  igraph_t g;

  igraph_vector_t order, rank, father, pred, succ, dist;
  igraph_i_set_attribute_table(&igraph_cattribute_table);



  // igraph_ring(&g, 10, IGRAPH_UNDIRECTED, 0, 1);
  //igraph_erdos_renyi_game(&g, IGRAPH_ERDOS_RENYI_GNP, 100, 0.2,
  //            IGRAPH_DIRECTED, IGRAPH_NO_LOOPS);
  //igraph_grg_game(&g, 100, sqrt(2) / 2, 1, 0, 0);
  igraph_tree(&g, 20, 3, IGRAPH_TREE_IN);


  igraph_vector_init(&order, 0);
  igraph_vector_init(&rank, 0);
  igraph_vector_init(&father, 0);
  igraph_vector_init(&pred, 0);
  igraph_vector_init(&succ, 0);
  igraph_vector_init(&dist, 0);

  igraph_bfs(&g, /*root=*/0, /*roots=*/ 0, /*neimode=*/ IGRAPH_ALL,
         /*unreachable=*/ 1, /*restricted=*/ 0,
         &order, &rank, &father, &pred, &succ, &dist,
         /*callback=*/ bfs_callback, /*extra=*/ 0);

  // igraph_vector_print(&order);
  // igraph_vector_print(&rank);
  // igraph_vector_print(&father);
  // igraph_vector_print(&pred);
  // igraph_vector_print(&succ);
  // igraph_vector_print(&dist);

  igraph_vector_destroy(&order);
  igraph_vector_destroy(&rank);
  igraph_vector_destroy(&father);
  igraph_vector_destroy(&pred);
  igraph_vector_destroy(&succ);
  igraph_vector_destroy(&dist);

  //igraph_write_graph_gml(&g, stdout, 0, "in");
  igraph_write_graph_graphml(&g, stdout, 0);
  igraph_destroy(&g);


  return 0;
}

void print_vector(igraph_vector_t *v) {
  long int i, l=igraph_vector_size(v);
  for (i=0; i<l; i++) {
    printf(" %li", (long int) VECTOR(*v)[i]);
  }
  printf("\n");
}

int print_free_vector_ptr(igraph_vector_ptr_t *v) {
  long int i, l=igraph_vector_ptr_size(v);
  printf("---\n");
  for (i=0; i<l; i++) {
    print_vector((igraph_vector_t*)VECTOR(*v)[i]);
  }
  printf("===\n");
  return 0;
}


int write_graphml(const char* filepath, igraph_t* g) {
  FILE* fd = fopen(filepath, "w");
  igraph_write_graph_graphml(g, fd, 0);
  fclose(fd);
  return (0);
}

int graph_union() {

  igraph_t left, right, uni;
  igraph_vector_t v;
  igraph_vector_ptr_t glist;
  igraph_vector_t edge_map1, edge_map2;
  igraph_vector_ptr_t edgemaps;
  long int i;

  igraph_vector_init(&edge_map1, 0);
  igraph_vector_init(&edge_map2, 0);

  igraph_vector_init_int_end(&v, -1, 0,1, 1,2, 2,2, 2,3, -1);
  igraph_create(&left, &v, 0, IGRAPH_DIRECTED);
  igraph_vector_destroy(&v);

  igraph_vector_init_int_end(&v, -1, 0,1, 1,2, 2,2, 2,4, -1);
  igraph_create(&right, &v, 0, IGRAPH_DIRECTED);
  igraph_vector_destroy(&v);

  igraph_union(&uni, &left, &right, &edge_map1, &edge_map2);
  igraph_write_graph_edgelist(&uni, stdout);
  write_graphml("1.graphml", &uni);
  igraph_vector_print(&edge_map1);
  igraph_vector_print(&edge_map2);

  igraph_destroy(&uni);
  igraph_destroy(&left);
  igraph_destroy(&right);
  igraph_vector_destroy(&edge_map1);
  igraph_vector_destroy(&edge_map2);

  /* Empty graph list */
  igraph_vector_ptr_init(&glist, 0);
  igraph_vector_ptr_init(&edgemaps, 0);
  igraph_union_many(&uni, &glist, &edgemaps);
  if (!igraph_is_directed(&uni) || igraph_vcount(&uni) != 0) {
    return 1;
  }
  print_free_vector_ptr(&edgemaps);
  igraph_vector_ptr_destroy(&glist);
  igraph_destroy(&uni);

  /* Non-empty graph list */
  igraph_vector_ptr_init(&glist, 10);
  for (i=0; i<igraph_vector_ptr_size(&glist); i++) {
    VECTOR(glist)[i]=calloc(1, sizeof(igraph_t));
    igraph_vector_init_int_end(&v, -1, 0,1, 1,0, -1);
    igraph_create((igraph_t*)VECTOR(glist)[i], &v, 0, IGRAPH_DIRECTED);
    igraph_vector_destroy(&v);
  }

  igraph_union_many(&uni, &glist, &edgemaps);
  igraph_write_graph_edgelist(&uni, stdout);
  write_graphml("2.graphml", &uni);
  for (i=0; i<igraph_vector_ptr_size(&glist); i++) {
    igraph_destroy((igraph_t*)VECTOR(glist)[i]);
    free(VECTOR(glist)[i]);
  }
  print_free_vector_ptr(&edgemaps);
  igraph_vector_ptr_destroy(&glist);
  igraph_destroy(&uni);

  /* Another non-empty graph list */
  igraph_vector_ptr_init(&glist, 10);
  for (i=0; i<igraph_vector_ptr_size(&glist); i++) {
    VECTOR(glist)[i]=calloc(1, sizeof(igraph_t));
    igraph_vector_init_int_end(&v, -1, i,i+1, 1,0, -1);
    igraph_create((igraph_t*)VECTOR(glist)[i], &v, 0, IGRAPH_DIRECTED);
    igraph_vector_destroy(&v);
  }

  igraph_union_many(&uni, &glist, &edgemaps);
  igraph_write_graph_edgelist(&uni, stdout);
  write_graphml("3.graphml", &uni);

  for (i=0; i<igraph_vector_ptr_size(&glist); i++) {
    igraph_destroy((igraph_t*)VECTOR(glist)[i]);
    free(VECTOR(glist)[i]);
  }
  print_free_vector_ptr(&edgemaps);
  igraph_vector_ptr_destroy(&glist);
  igraph_destroy(&uni);

  /* Undirected graph list*/
  igraph_vector_ptr_init(&glist, 10);
  for (i=0; i<igraph_vector_ptr_size(&glist); i++) {
    VECTOR(glist)[i]=calloc(1, sizeof(igraph_t));
    igraph_vector_init_int_end(&v, -1, i,i+1, 1,0, -1);
    igraph_create((igraph_t*)VECTOR(glist)[i], &v, 0, IGRAPH_UNDIRECTED);
    igraph_vector_destroy(&v);
  }

  igraph_union_many(&uni, &glist, &edgemaps);
  igraph_write_graph_edgelist(&uni, stdout);
  write_graphml("4.graphml", &uni);

  for (i=0; i<igraph_vector_ptr_size(&glist); i++) {
    igraph_destroy((igraph_t*)VECTOR(glist)[i]);
    free(VECTOR(glist)[i]);
  }
  print_free_vector_ptr(&edgemaps);
  igraph_vector_ptr_destroy(&glist);
  igraph_destroy(&uni);

  igraph_vector_ptr_destroy(&edgemaps);

  return 0;
}


int walk_2() {
    igraph_t g;

    igraph_vector_t order, rank, father, pred, succ, dist, roots;
    igraph_i_set_attribute_table(&igraph_cattribute_table);



    // igraph_ring(&g, 10, IGRAPH_UNDIRECTED, 0, 1);
    //igraph_erdos_renyi_game(&g, IGRAPH_ERDOS_RENYI_GNP, 100, 0.2,
    //            IGRAPH_DIRECTED, IGRAPH_NO_LOOPS);
    //igraph_grg_game(&g, 100, sqrt(2) / 2, 1, 0, 0);
    igraph_tree(&g, 20, 3, IGRAPH_TREE_IN);


    igraph_vector_init(&order, 0);
    igraph_vector_init(&rank, 0);
    igraph_vector_init(&father, 0);
    igraph_vector_init(&pred, 0);
    igraph_vector_init(&succ, 0);
    igraph_vector_init(&dist, 0);
    igraph_vector_init(&roots, 7);
    VECTOR(roots)[0] = 13;
    VECTOR(roots)[1] = 14;
    VECTOR(roots)[2] = 15;
    VECTOR(roots)[3] = 16;
    VECTOR(roots)[4] = 17;
    VECTOR(roots)[5] = 18;
    VECTOR(roots)[6] = 19;

    // igraph_bfs(&g, /*root=*/0, /*&roots*/0, /*neimode=*/ IGRAPH_IN,
    //        /*unreachable=*/ 1, /*restricted=*/ 0,
    //        &order, &rank, &father, &pred, &succ, &dist,
    //        /*callback=*/ bfs_callback, /*extra=*/ 0);
    int igraph_dfs(const igraph_t *graph, igraph_integer_t root,
               igraph_neimode_t mode, igraph_bool_t unreachable,
               igraph_vector_t *order,
               igraph_vector_t *order_out, igraph_vector_t *father,
               igraph_vector_t *dist, igraph_dfshandler_t *in_callback,
               igraph_dfshandler_t *out_callback,
               void *extra);


    igraph_dfs(&g, /*root=*/0, /*neimode=*/ IGRAPH_IN,
           /*unreachable=*/ 1,
           &order, &rank, &father, &dist,
           /*callback=*/ 0, 0, /*extra=*/ 0);

    igraph_vector_print(&order);
    // igraph_vector_print(&rank);
    // igraph_vector_print(&father);
    // igraph_vector_print(&pred);
    // igraph_vector_print(&succ);
    // igraph_vector_print(&dist);

    igraph_vector_destroy(&order);
    igraph_vector_destroy(&rank);
    igraph_vector_destroy(&father);
    igraph_vector_destroy(&pred);
    igraph_vector_destroy(&succ);
    igraph_vector_destroy(&dist);

    igraph_write_graph_gml(&g, stdout, 0, "in");
    //igraph_write_graph_graphml(&g, stdout, 0);
    igraph_destroy(&g);

    return 0;
}


#include <vector>

int walk_3(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage Walk <node number> <branch number> <filepath>\n");
        return 0;
    }

    igraph_t g, gIn, gOut;
    igraph_vector_t y;
    igraph_vector_t edge;
    std::vector<int> e;
    int MAX_NODE = atoi(argv[1]);

    igraph_vector_init(&y, 0);
    igraph_vector_init(&edge, 0);


    igraph_tree(&gIn, MAX_NODE, atoi(argv[2]), IGRAPH_TREE_IN);
    igraph_tree(&gOut, MAX_NODE, atoi(argv[2]), IGRAPH_TREE_OUT);


    igraph_get_edgelist(&gOut, &edge, false);

    for (int64_t i = 0; i < igraph_vector_size(&edge); ++i)
      e.push_back(VECTOR(edge)[i]);

    for (int i = 0; i < MAX_NODE; ++i) {
      igraph_neighbors(&gOut, &y, i, IGRAPH_OUT);
      if (0 == igraph_vector_size(&y)) {
        e.push_back(i);
        e.push_back(i + ((MAX_NODE - i) - 1) * 2 + 1);
      }
    }

    igraph_get_edgelist(&gIn, &edge, false);
    for (int64_t i = 0; i < igraph_vector_size(&edge); ++i) {
      int vid = VECTOR(edge)[i];
      e.push_back(vid + ((MAX_NODE - vid) - 1) * 2 + 1);
    }

    igraph_vector_destroy(&edge);
    igraph_vector_init(&edge, e.size());
    for (int i = 0; i < (int)e.size(); ++i)
      VECTOR(edge)[i] = e[i];

    igraph_create(&g, &edge, MAX_NODE * 2, 1);

    write_graphml(argv[3], &g);

    igraph_destroy(&g);
    igraph_destroy(&gOut);
    igraph_destroy(&gIn);

    igraph_vector_destroy(&edge);
    igraph_vector_destroy(&y);

    return 0;
}

int main(int argc, char* argv[]) {
   //graph_union();

   //walk_2();

    walk_3(argc, argv);

   return 0;
}
