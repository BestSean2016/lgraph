#include <igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_attributes(const igraph_t *g) {

  igraph_vector_t gtypes, vtypes, etypes;
  igraph_strvector_t gnames, vnames, enames;
  long int i;

  igraph_vector_t vec;
  igraph_strvector_t svec;
  long int j;

  igraph_vector_init(&gtypes, 0);
  igraph_vector_init(&vtypes, 0);
  igraph_vector_init(&etypes, 0);
  igraph_strvector_init(&gnames, 0);
  igraph_strvector_init(&vnames, 0);
  igraph_strvector_init(&enames, 0);

  igraph_cattribute_list(g, &gnames, &gtypes, &vnames, &vtypes, &enames,
                         &etypes);

  /* Graph attributes */
  for (i = 0; i < igraph_strvector_size(&gnames); i++) {
    printf("%s=", STR(gnames, i));
    if (VECTOR(gtypes)[i] == IGRAPH_ATTRIBUTE_NUMERIC) {
      igraph_real_printf(GAN(g, STR(gnames, i)));
      putchar(' ');
    } else {
      printf("\"%s\" ", GAS(g, STR(gnames, i)));
    }
  }
  printf("\n");

  for (i = 0; i < igraph_vcount(g); i++) {
    long int j;
    printf("Vertex %li: ", i);
    for (j = 0; j < igraph_strvector_size(&vnames); j++) {
      printf("%s=", STR(vnames, j));
      if (VECTOR(vtypes)[j] == IGRAPH_ATTRIBUTE_NUMERIC) {
        igraph_real_printf(VAN(g, STR(vnames, j), i));
        putchar(' ');
      } else {
        printf("\"%s\" ", VAS(g, STR(vnames, j), i));
      }
    }
    printf("\n");
  }

  for (i = 0; i < igraph_ecount(g); i++) {
    long int j;
    printf("Edge %li (%i-%i): ", i, (int)IGRAPH_FROM(g, i),
           (int)IGRAPH_TO(g, i));
    for (j = 0; j < igraph_strvector_size(&enames); j++) {
      printf("%s=", STR(enames, j));
      if (VECTOR(etypes)[j] == IGRAPH_ATTRIBUTE_NUMERIC) {
        igraph_real_printf(EAN(g, STR(enames, j), i));
        putchar(' ');
      } else {
        printf("\"%s\" ", EAS(g, STR(enames, j), i));
      }
    }
    printf("\n");
  }

  /* Check vector-based query functions */
  igraph_vector_init(&vec, 0);
  igraph_strvector_init(&svec, 0);

  for (j = 0; j < igraph_strvector_size(&vnames); j++) {
    if (VECTOR(vtypes)[j] == IGRAPH_ATTRIBUTE_NUMERIC) {
      igraph_cattribute_VANV(g, STR(vnames, j), igraph_vss_all(), &vec);
      for (i = 0; i < igraph_vcount(g); i++) {
        igraph_real_t num = VAN(g, STR(vnames, j), i);
        if (num != VECTOR(vec)[i] && (!isnan(num) || !isnan(VECTOR(vec)[i]))) {
          exit(51);
        }
      }
    } else {
      igraph_cattribute_VASV(g, STR(vnames, j), igraph_vss_all(), &svec);
      for (i = 0; i < igraph_vcount(g); i++) {
        const char *str = VAS(g, STR(vnames, j), i);
        if (strcmp(str, STR(svec, i))) {
          exit(52);
        }
      }
    }
  }

  for (j = 0; j < igraph_strvector_size(&enames); j++) {
    if (VECTOR(etypes)[j] == IGRAPH_ATTRIBUTE_NUMERIC) {
      igraph_cattribute_EANV(g, STR(enames, j),
                             igraph_ess_all(IGRAPH_EDGEORDER_ID), &vec);
      for (i = 0; i < igraph_ecount(g); i++) {
        igraph_real_t num = EAN(g, STR(enames, j), i);
        if (num != VECTOR(vec)[i] && (!isnan(num) || !isnan(VECTOR(vec)[i]))) {
          exit(53);
        }
      }
    } else {
      igraph_cattribute_EASV(g, STR(enames, j),
                             igraph_ess_all(IGRAPH_EDGEORDER_ID), &svec);
      for (i = 0; i < igraph_ecount(g); i++) {
        const char *str = EAS(g, STR(enames, j), i);
        if (strcmp(str, STR(svec, i))) {
          exit(54);
        }
      }
    }
  }

  igraph_strvector_destroy(&svec);
  igraph_vector_destroy(&vec);

  igraph_strvector_destroy(&enames);
  igraph_strvector_destroy(&vnames);
  igraph_strvector_destroy(&gnames);
  igraph_vector_destroy(&etypes);
  igraph_vector_destroy(&vtypes);
  igraph_vector_destroy(&gtypes);

  return 0;
}

int read_gml(const char *filepath) {
  igraph_t g;
  FILE *ifile;

  ifile = fopen(filepath, "r");
  if (ifile == 0) {
    printf("Can not open file %s\n", filepath);
    return 10;
  }

  igraph_read_graph_gml(&g, ifile);
  fclose(ifile);

  if (igraph_is_directed(&g)) {
    printf("directed\n");
  } else {
    printf("undirected\n");
  }

  igraph_write_graph_edgelist(&g, stdout);
  printf("-----------------\n");

  igraph_write_graph_gml(&g, stdout, 0, "test suite");
  igraph_destroy(&g);

  return 0;
}

int get_attr(const char *filepath) {
  igraph_t g;
  FILE *ifile;
  igraph_vector_t gtypes, vtypes, etypes;
  igraph_strvector_t gnames, vnames, enames;
  int i;

  /* turn on attribute handling */
  igraph_i_set_attribute_table(&igraph_cattribute_table);

  ifile = fopen(filepath, "r");
  if (ifile == 0) {
    printf("Can not open file %s\n", filepath);
    return 10;
  }

  igraph_read_graph_gml(&g, ifile);
  fclose(ifile);

  igraph_vector_init(&gtypes, 0);
  igraph_vector_init(&vtypes, 0);
  igraph_vector_init(&etypes, 0);
  igraph_strvector_init(&gnames, 0);
  igraph_strvector_init(&vnames, 0);
  igraph_strvector_init(&enames, 0);

  igraph_cattribute_list(&g, &gnames, &gtypes, &vnames, &vtypes, &enames,
                         &etypes);

  /* List attribute names and types */
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


  igraph_strvector_destroy(&enames);
  igraph_strvector_destroy(&vnames);
  igraph_strvector_destroy(&gnames);
  igraph_vector_destroy(&etypes);
  igraph_vector_destroy(&vtypes);
  igraph_vector_destroy(&gtypes);

  print_attributes(&g);
  igraph_destroy(&g);

  return 0;
}


int attr2() {

      igraph_t g;
      igraph_vector_t y;

      /* turn on attribute handling */
      igraph_i_set_attribute_table(&igraph_cattribute_table);

      /* Create a graph, add some attributes and save it as a GraphML file */
      igraph_famous(&g, "Petersen");
      SETGAS(&g, "name", "Petersen's graph");
      SETGAN(&g, "vertices", igraph_vcount(&g));
      SETGAN(&g, "edges", igraph_ecount(&g));

      igraph_vector_init_seq(&y, 1, igraph_vcount(&g));
      SETVANV(&g, "id", &y);
      igraph_vector_destroy(&y);

      SETVAS(&g, "name", 0, "foo");
      SETVAS(&g, "name", 1, "foobar");
      SETVAS(&g, "text", 0, "foo");
      SETVAS(&g, "text", 1, "foobar");

      igraph_vector_init_seq(&y, 1, igraph_ecount(&g));
      SETEANV(&g, "id", &y);
      igraph_vector_destroy(&y);

      SETEAS(&g, "name", 0, "FOO");
      SETEAS(&g, "name", 1, "FOOBAR");
      SETEAS(&g, "text", 0, "FOO");
      SETEAS(&g, "text", 1, "FOOBAR");

      igraph_write_graph_gml(&g, stdout, 0, "");
      //igraph_write_graph_graphml(&g, stdout, /*prefixattr=*/ 1);

      igraph_destroy(&g);

      return 0;
}

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    printf("Usage attr-1 <file path>\n");
    return 0;
  }

  /// read_gml(argv[1]);
  /// get_attr(argv[1]);
  attr2();

  return (0);
}
