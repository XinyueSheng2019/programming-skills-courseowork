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
  
  Create_map(rho,L,(int**)map);
  Do_loop(L,(int**)map);
  Percolate_result(L,(int**)map);
  Print_Datafile(L,(int**)map);
  Print_Imagefile(L,MAX,(int**)map);
  free(map);
  
}


/*
This function means to define the initial map. 
The rho and L are set by the user. 
*/
void Create_map(float rho,int L,int **map)
{
  int num_of_unfilled=0;
  int unique_num=0;
  float r;
  int i,j;
  //every square in this map with the extension length and width L+2,is set to be 0.
  for (i = 0; i < L+2; i++)
  {
    for (j = 0; j < L+2; j++)
    {
      map[i][j] = 0;
    }
  }
  //by using random number(0~1) and comparing them with rho, unfilled squares are set to 1.
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

  printf("rho = %f, actual density = %f\n",rho, 1.0 - ((double)num_of_unfilled)/((double) L*L) );
  
  //calculate the number of unfilled squares, and then change each of the unfilled squares with a unique positive integer.
  for (int i = 1; i <= L; i++)
  {
    for (int j = 1; j <= L; j++)
    {
      if (map[i][j] != 0)
      {
        unique_num++;
        map[i][j] = unique_num;
      }
    }
  }
}

/*
Loop over all the squares in the grid many times, 
and during each pass of the loop we replace each square with the maximum of its four neighbours.
In all cases, we can ignore the filled (grey) squares. 
The large numbers gradually fill the gaps so that each cluster eventually contains a single, unique number.
*/
void Do_loop(int L,int **map)
{
  int loop, nchange, old;
  loop = 1;
  nchange = 1;
  while (nchange > 0)
  {
    nchange = 0;
    for (int i = 1; i <= L; i++)
    {
      for (int j = 1; j <= L; j++)
      {
        if (map[i][j] != 0)
        {
          old = map[i][j];
          if (map[i-1][j] > map[i][j]) map[i][j] = map[i-1][j];
          if (map[i+1][j] > map[i][j]) map[i][j] = map[i+1][j];
          if (map[i][j-1] > map[i][j]) map[i][j] = map[i][j-1];
          if (map[i][j+1] > map[i][j]) map[i][j] = map[i][j+1];
          if (map[i][j] != old) nchange++;
          
        }
      }
    }
    printf("Number of changes on loop %d is %d\n", loop, nchange);
    loop++;
  }

}

/*
Calculate and print the results:whether clusters percolate the map, 
if cluster does percolate, print the number of clusters which percolate the map;
if cluster does not percolate, print this result.
*/
void Percolate_result(int L,int **map)
{
  int itop, ibot;
  int num_of_percclusters=0, percs = 0;

  for (itop = 1; itop <= L; itop++)
  {
    if (map[itop][L] > 0)
    {
      for (ibot = 1; ibot <= L; ibot++)
      {
        if (map[itop][L] == map[ibot][1])
        {
          percs = 1;
          num_of_percclusters = map[itop][L];
        }
      }
    }
  }
  if (percs)
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
void Print_Datafile(int L,int **map)
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
  rank = (int*)arralloc(sizeof(int), 1, L * L);//initial the rank
  FILE *fp;

  Create_clusterlist(&MAX, L, (int**)map, rank, &ncluster, &maxsize);

  percfile = "map.pgm";
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

  for (int j = L; j >= 1; j--)
  {
    for (int i = 1;i <= L; i++)
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

*/
void Create_clusterlist(int *MAX, int L, int **map, int *rank, int *ncluster, int *maxsize)
{
  struct cluster *clustlist;
  clustlist = (struct cluster*)arralloc(sizeof(struct cluster), 1, L*L);
  for (int i = 0; i < L*L; i++)
  {
    rank[i] = -1;
    clustlist[i].size = 0;
    clustlist[i].id   = i+1;
  }
  for (int i = 1;i <= L; i++)
  {
    for (int j = 1; j <= L; j++)
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
  for (int i = 0; i < *ncluster; i++)
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











