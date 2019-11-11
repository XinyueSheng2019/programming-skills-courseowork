#include "command_line.h"
#include "percolate.h"

struct cluster
{
  int id;
  int size;
};

struct output_related
{
  FILE *fp1;
  FILE *fp2;
  int *rank;
  int num_of_clusters, maxsize;
  int colour;
};

void print_datafile(struct grid_related* grid, struct command_line* p, struct output_related *output);
void print_digitmap(struct grid_related* grid, struct command_line* p, struct output_related *output);
void print_imagefile(struct grid_related* grid, struct command_line* p, struct output_related *output);
void create_clusterlist(struct grid_related* grid, struct command_line* p, struct output_related *output);
void calculate_cluster_size(struct cluster *clustlist, struct grid_related* grid, struct command_line* p, struct output_related *output);
void percsort(struct cluster *list, int n);
static int clustcompare(const void *p1, const void *p2);
void set_cluster_num(struct cluster *clustlist, struct command_line* p, struct output_related *output);
void judge_MAX(struct command_line* p, struct output_related *output);
void print_MAX(struct command_line* p, struct output_related *output);
void convert_cluster_to_colour(struct grid_related* grid, struct command_line* p, struct output_related *output);

