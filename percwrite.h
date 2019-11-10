struct cluster
{
  int id;
  int size;
};
void print_datafile(int L, int **map, char* datafile);
void print_digitmap(int L, FILE *input_file, int **map);
void print_imagefile(int L, int MAX, int **map, char* percfile);
void create_clusterlist(int *MAX, int L, int **map, int *rank, int *num_of_clusters, int *maxsize);
void calculate_cluster_size(int L, struct cluster *clustlist, int **map, int *rank);
void percsort(struct cluster *list, int n);
static int clustcompare(const void *p1, const void *p2);
void set_cluster_num(int *maxsize, struct cluster *clustlist, int *num_of_clusters, int *rank, int *MAX, int L);
void judge_MAX(int *MAX, int *num_of_clusters);
void print_MAX(int *MAX, FILE *input_file, int L);
void convert_cluster_to_colour(int **map, int L, int *MAX, int *rank, FILE *input_file);

