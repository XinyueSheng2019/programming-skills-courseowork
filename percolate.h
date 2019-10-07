struct cluster
{
  int id;
  int size;
};

void percsort(struct cluster *list, int n);
static int clustcompare(const void *p1, const void *p2);
void Create_map(float rho,int L,int **map);
void Do_loop(int L,int **map);
void Percolate_result(int L,int **map);
void Print_Datafile(int L,int **map);
void Create_clusterlist(int *MAX,int L,int **map,int *rank,int *ncluster,int *maxsize);
void Print_Imagefile(int L,int MAX,int **map);

