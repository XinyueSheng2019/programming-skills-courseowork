struct cluster
{
  int id;
  int size;
};

void percsort(struct cluster *list, int n);
static int clustcompare(const void *p1, const void *p2);
void create_map(float rho,int L,int **map);
void do_loop(int L,int **map);
void result_of_percolate(int L,int **map);
void print_datafile(int L,int **map);
void Create_clusterlist(int *MAX,int L,int **map,int *rank,int *ncluster,int *maxsize);
void Print_Imagefile(int L,int MAX,int **map);

