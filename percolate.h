struct cluster
{
  int id;
  int size;
};

void percolate_processing(float rho, int L, int MAX, int seed, char* datafile_name, char* imagefile_name);
void create_map(float rho,int L,int **map);
void init_extent_map(int **map, int L);
int set_unfilled_squares(int **map, int L, float rho);
void set_unique_num(int **map, int L);
void do_loop(int L,int **map);
int replace_square(int **map, int L);
void result_of_percolate(int L,int **map);
void judge_percolate(int *num_of_percclusters, int *perc_success, int **map, int L);
void print_datafile(int L,int **map, char* datafile);
void print_digitmap(int L,FILE *input_file, int **map);
void print_imagefile(int L,int MAX,int **map, char* percfile);
void create_clusterlist(int *MAX, int L, int **map, int *rank, int *num_of_clusters, int *maxsize);
void calculate_cluster_size(int L, struct cluster *clustlist, int **map, int *rank);
void percsort(struct cluster *list, int n);
static int clustcompare(const void *p1, const void *p2);
void set_cluster_num(int *maxsize, struct cluster *clustlist, int *num_of_clusters, int *rank, int *MAX, int L);
void judge_MAX(int *MAX, int *num_of_clusters);
void print_MAX(int *MAX, FILE *fp, int L);
void convert_cluster_to_colour(int **map, int L, int *MAX, int *rank, FILE *fp);



