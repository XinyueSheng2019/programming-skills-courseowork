#include <stdio.h>
#include <stdlib.h>

#include "arralloc.h"
#include "percolate.h"
#include "uni.h"


int main()
{
  int L;
  L=20;
  float rho;
  rho = 0.40;
  int seed;
  seed = 1564;
  int MAX;
  MAX = L * L;
  int** map; 
  //set an initial map, and then send the address of this map to every function for further usages.
  map = (int**)arralloc(sizeof(int), 2, L+2, L+2);
  rinit(seed);
  printf("Parameters are rho=%f, L=%d, seed=%d\n", rho, L, seed);
  
  create_map(rho,L,(int**)map);
  do_loop(L,(int**)map);
  result_of_percolate(L,(int**)map);
  print_datafile(L,(int**)map);
  Print_Imagefile(L,MAX,(int**)map);
  free(map);
  
}

//every square in this map with the extension length and width L+2,is set to be 0.
void init_extent_map(int **map, int L)
{
  int i,j;
  for (i = 0; i < L+2; i++)
  {
    for (j = 0; j < L+2; j++)
    {
      map[i][j] = 0;
    }
  }
}

/*
by using random number(0~1) and comparing it with rho, the number and the coords of unfilled squares are set.
And then those unfilled squares are set to 1.
*/
int set_unfilled_squares(int **map, int L, float rho)
{
  int num_of_unfilled = 0;
  float r;
  int i,j;
  for (i = 1; i <= L; i++)
  {
    for (j = 1; j <= L; j++)
    {
      r = random_uniform();
      if(r > rho)
      {
        num_of_unfilled++;
        map[i][j] = 1;
      }
    }
  }
  return num_of_unfilled;
}

//calculate the number of unfilled squares, and then change each of the unfilled squares with a unique positive integer.
void set_unique_num(int **map, int L)
{
  int i,j;
  int unique_num=0;
  for (i = 1; i <= L; i++)
  {
    for (j = 1; j <= L; j++)
    {
      if (map[i][j] != 0)
      {
        unique_num++;
        map[i][j] = unique_num;
      }
    }
  }
}


//This function means to define the initial map. The rho and L are set by the user. 
void create_map(float rho,int L,int **map)
{
  int i,j;
  int num_of_unfilled;
  
  init_extent_map((int**)map, L);
  num_of_unfilled = set_unfilled_squares((int**)map, L, rho);
  printf("rho = %f, actual density = %f\n",rho, 1.0 - ((double)num_of_unfilled)/((double) L*L) );
  set_unique_num((int**)map, L);  
}

/*
Loop over all the squares in the grid many times, 
and during each pass of the loop we replace each square with the maximum of its four neighbours.
In all cases, we can ignore the filled (grey) squares. 
The large numbers gradually fill the gaps so that each cluster eventually contains a single, unique number.
*/

//replace each square with the maximum of its four neighbours
int replace_square(int **map, int L)
{
  int i, j, old_num;
  int num_of_changes = 0;
  for (i = 1; i <= L; i++)
  {
    for (j = 1; j <= L; j++)
    {
      if (map[i][j] != 0)
      {
        old_num = map[i][j];
        if (map[i-1][j] > map[i][j]) map[i][j] = map[i-1][j];
        if (map[i+1][j] > map[i][j]) map[i][j] = map[i+1][j];
        if (map[i][j-1] > map[i][j]) map[i][j] = map[i][j-1];
        if (map[i][j+1] > map[i][j]) map[i][j] = map[i][j+1];
        if (map[i][j] != old_num) num_of_changes++;    
      }
    }
  }
  return num_of_changes;
}

//Loop over all the squares in the grid many times
void do_loop(int L,int **map)
{ 
  int loop = 1, num_of_changes = 1;

  while (num_of_changes > 0)
  {
    num_of_changes = replace_square((int**)map, L);
    printf("Number of changes on loop %d is %d\n", loop, num_of_changes);
    loop++;
  }
  
}

/*
Calculate and print the results: whether clusters percolate the map, 
if cluster does percolate, print the number of clusters which percolate the map;
if cluster does not percolate, print this result.
*/

void result_of_percolate(int L,int **map)
{
  int top_row, bottom_row;
  int num_of_percclusters=0, perc_success = 0;

  for (top_row = 1; top_row <= L; top_row++)
  {
    if (map[top_row][L] > 0)
    {
      for (bottom_row = 1; bottom_row <= L; bottom_row++)
      {
        if (map[top_row][L] == map[bottom_row][1])
        {
          perc_success = 1;
          num_of_percclusters = map[top_row][L];
        }
      }
    }
  }
  if (perc_success)
  {
    printf("Cluster DOES percolate. Cluster number: %d\n", num_of_percclusters);
  }
  else
  {
    printf("Cluster DOES NOT percolate\n");
  }
}

/*
Create a txt datafile with a user-defined name.
Print the final map with digits in this file.
*/

void print_datafile(int L,int **map)
{
  char* datafile;
  //user defines the name of datafile.
  datafile = "map.dat"; 
  printf("Opening file <%s>\n", datafile);
  
  FILE *fp;
  fp = fopen(datafile, "w");

  printf("Writing data ...\n");

  //print the final map.
  for (int j = L; j >= 1; j--)
  {
    for (int i = 1;i <= L; i++)
    {
      fprintf(fp, " %4d", map[i][j]);
    }
    fprintf(fp,"\n");
  }

  printf("...done\n");
  fclose(fp);
  printf("File closed\n");

}

/*
Create an imagefile with a user-defined name.
Print the final map with different gray colours in this file.
*/
void Print_Imagefile(int L,int MAX,int **map)
{
  //need a struct:ncluster,maxsize,rank
  char* percfile;
  int colour;
  int *rank;
  int ncluster,maxsize;
  int i,j;
  //user defines the name of imagefile
  percfile = "map.pgm";

  rank = (int*)arralloc(sizeof(int), 1, L * L);//initial the rank
  
  FILE *fp;

  Create_clusterlist(&MAX, L, (int**)map, rank, &ncluster, &maxsize);

  printf("Opening file <%s>\n", percfile);

  fp = fopen(percfile, "w");

  printf("Map has %d clusters, maximum cluster size is %d\n",ncluster, maxsize);
  
  if (MAX == 1)
  {
    printf("Displaying the largest cluster\n");
  }
  else if (MAX == ncluster)
  {
    printf("Displaying all clusters\n");
  }
  else
  {
    printf("Displaying the largest %d clusters\n", MAX);
  }

  printf("Writing data ...\n"); 
  
  fprintf(fp, "P2\n");
  
  if (MAX > 0)
  {
    fprintf(fp, "%d %d\n%d\n", L, L, MAX);
  }
  else
  {
    fprintf(fp, "%d %d\n%d\n", L, L, 1);
  }

  for (j = L; j >= 1; j--)
  {
    for (i = 1;i <= L; i++)
    {
      colour = map[i][j];
      if (map[i][j] > 0)
      {
        colour = rank[map[i][j]-1];

        if (colour >= MAX)
        {
          colour = MAX;
        }
      }
      else
      {
        colour = MAX;
      }

      fprintf(fp, " %4d", colour);
    }

    fprintf(fp,"\n");
  }

  printf("...done\n");
  fclose(fp);
  printf("File closed\n");
  free(rank);
}

/*
Create the clusterlist.
*/
void Create_clusterlist(int *MAX, int L, int **map, int *rank, int *ncluster, int *maxsize)
{
  int i,j;
  struct cluster *clustlist;
  clustlist = (struct cluster*)arralloc(sizeof(struct cluster), 1, L*L);
  //calculate the size of cluster
  for (i = 0; i < L*L; i++)
  {
    rank[i] = -1;
    clustlist[i].size = 0;
    clustlist[i].id   = i+1;
  }
  for (i = 1;i <= L; i++)
  {
    for (j = 1; j <= L; j++)
    {
      if (map[i][j] != 0)
      {
        ++(clustlist[map[i][j]-1].size);
      }
    }
  }

  percsort(clustlist, L*L);
  *maxsize = clustlist[0].size;
  for (*ncluster=0; *ncluster < L*L && clustlist[*ncluster].size > 0; (*ncluster)++);
  if (*MAX > *ncluster)
  {
    *MAX = *ncluster;
  }
  for (i = 0; i < *ncluster; i++)
  {
    rank[clustlist[i].id - 1] = i;
  }
  free(clustlist);
  
}
static int clustcompare(const void *p1, const void *p2)
{

  int size1, size2, id1, id2;

  size1 = ((struct cluster *) p1)->size;
  size2 = ((struct cluster *) p2)->size;

  id1   = ((struct cluster *) p1)->id;
  id2   = ((struct cluster *) p2)->id;

  if (size1 != size2)
  {
    return(size2 - size1);
  }
  else
  {
    return(id2 - id1);
  }

}

void percsort(struct cluster *list, int n) 
{
  qsort(list, (size_t) n, sizeof(struct cluster), clustcompare);
}











