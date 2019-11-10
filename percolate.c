#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>

#include "arralloc.h"
#include "percolate.h"
#include "uni.h"


int main(int argc, char* argv[])
{
  int L, seed, MAX, opt;
  float rho;
  char* datafile = "map";
  char* imagefile = "map";
  char* format_dat = ".dat";
  char* format_pgm = ".pgm";
  char s1[100];
  char *m_file = s1;
  char *datafile_name = &(*m_file);
  L    = 20;
  rho  = 0.400000;
  seed = 1564;
  MAX  = L*L;

  while((opt = getopt(argc,argv,":l:r:s:m:d:g:"))!=-1)
  {
    switch(opt)
    {
      case 'l':
        L = atoi(optarg);
        break;
      case 'r':
        rho = atof(optarg);
        break;
      case 's':
        seed = atoi(optarg);
        break;
      case 'm':
        MAX = atoi(optarg);
        break;
      case 'd':
        datafile = optarg;
        break;
      case 'g':
        imagefile = optarg;
        break;

    }
  }
  /*judge the validation of parameters*/
    if (L < 1)
    {
      printf("map width and height should be no less than 1. \n");
      return 0;
    }
    if (rho < 0 || rho > 1)
    {
      printf("The value of ρ should be between 0 and 1.\n ");
      return 0;
    }
    if (seed < 0 || seed > 900000000)
    {
      printf("The value of seed should be between 0 and 900000000. \n");
      return 0;
    }
    if (MAX < 0 || MAX > L * L)
    {
      printf("The value of MAX should be between 0 and L*L. \n");
      return 0;
    }
   

    while(*datafile != '\0')
    {
      *m_file = *datafile;
      datafile++;
      m_file++;
    }
    while(*format_dat != '\0')
    {
      *m_file = *format_dat;
      format_dat++;
      m_file++;
    }
    *m_file = '\0';
    datafile = &(*m_file);

    char s2[100];
    m_file = &(*s2);
    char *imagefile_name = &(*m_file);
    while(*imagefile != '\0')
    {
      *m_file = *imagefile;
      imagefile++;
      m_file++;
    }
    while(*format_pgm != '\0')
    {
      *m_file = *format_pgm;
      format_pgm++;
      m_file++;
    }
    *m_file = '\0';
    imagefile = &(*m_file);


  printf("**************************************\nHere are parameters:\n(Unset parameters have been given original values.)\nL   : %d\nrho : %f\nseed: %d\nMAX : %d\ndatafile_name : %s\nimagefile_name: %s\n**************************************\n", L, rho, seed, MAX, datafile_name, imagefile_name);
  
  percolate_processing(rho, L, MAX, seed, datafile_name, imagefile_name);

  return 0;

}

void percolate_processing(float rho, int L, int MAX, int seed, char* datafile_name, char* imagefile_name)
{
  /*set an initial map, and then send the address of this map to every function for further usages.*/

  int** map; 
  map = (int**)arralloc(sizeof(int), 2, L+2, L+2);
  rinit(seed);
  create_map(rho,L,(int**)map);
  do_loop(L,(int**)map);
  result_of_percolate(L,(int**)map);
  print_datafile(L,(int**)map, datafile_name);
  print_imagefile(L,MAX,(int**)map, imagefile_name); 
  free(map);
}


void create_map(float rho, int L, int **map)
{
  /* This function means to define the initial map. The rho and L are set by the user. */

  int i,j;
  int num_of_unfilled;
  
  init_extent_map((int**)map, L);
  num_of_unfilled = set_unfilled_squares((int**)map, L, rho);
  printf("rho = %f, actual density = %f\n",rho, 1.0 - ((double)num_of_unfilled)/((double) L*L) );
  set_unique_num((int**)map, L);  
}

void init_extent_map(int **map, int L)
{
  /* Every square in this map with the extension length and width L+2, is set to 0.*/

  int i,j;

  for (i = 0; i < L+2; i++)
  {
    for (j = 0; j < L+2; j++)
    {
      map[i][j] = 0;
    }
  }
}

int set_unfilled_squares(int **map, int L, float rho)
{
  /* By using random number(0~1) and comparing it with rho, the number and the coords of unfilled squares are set.
And those unfilled squares are set to 1.*/

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

void set_unique_num(int **map, int L)
{
  /* Calculate the number of unfilled squares, 
  and then change each of the unfilled squares with a unique positive integer.*/

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


void do_loop(int L, int **map)
{ 
/* Loop over all the squares in the map many times, and during each pass of the loop we replace each square 
with the maximum of its four neighbours. In all cases, we can ignore the filled (grey) squares. 
The large numbers gradually fill the gaps so that each cluster eventually contains a single, unique number.*/

  int loop = 1, num_of_changes = 1;

  while (num_of_changes > 0)
  {
    num_of_changes = replace_square((int**)map, L);
    printf("Number of changes on loop %d is %d\n", loop, num_of_changes);
    loop++;
  }
  
}

int replace_square(int **map, int L)
{
  /* Replace each square with the maximum of its four neighbours*/

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



void result_of_percolate(int L,int **map)
{
/* Calculate and print the results: whether clusters percolate the map, 
if cluster does percolate, print the number of clusters which percolate the map;
if cluster does not percolate, print this result.*/

  int num_of_percclusters = 0, perc_success = 0;

  judge_percolate(&num_of_percclusters, &perc_success, (int**)map, L);

  if (perc_success)
  {
    printf("Cluster DOES percolate. Cluster number: %d\n", num_of_percclusters);
  }
  else
  {
    printf("Cluster DOES NOT percolate\n");
  }

}

void judge_percolate(int *num_of_percclusters, int *perc_success, int **map, int L)
{
  /* Judge whether there is at least a cluster which percolates the map. */

  int top_row, bottom_row;

  for (top_row = 1; top_row <= L; top_row++)
  {
    if (map[top_row][L] > 0)
    {
      for (bottom_row = 1; bottom_row <= L; bottom_row++)
      {
        if (map[top_row][L] == map[bottom_row][1])
        {
          *perc_success = 1;
          *num_of_percclusters = map[top_row][L];
        }
      }
    }
  }
}

void print_datafile(int L, int **map, char* datafile)
{
  /* Create a txt datafile with a user-defined name. Print the final map with digits in this file.*/

  printf("Opening file <%s>\n", datafile);
  FILE *fp;
  fp = fopen(datafile, "w");
  printf("Writing data ...\n");
  print_digitmap(L,fp, (int**)map);
  printf("...done\n");
  fclose(fp);
  printf("File closed\n");

}

void print_digitmap(int L, FILE *input_file, int **map)
{
  /* Print digital map.*/
  
  int i,j;
  for (j = L; j >= 1; j--)
  {
    for (i = 1;i <= L; i++)
    {
      fprintf(input_file, " %4d", map[i][j]);
    }
    fprintf(input_file,"\n");
  }
}

void print_imagefile(int L, int MAX, int **map, char* percfile)
{
  /* Create an imagefile with a user-defined name. Print the final map with colours of different gray levels in this file. */

  int *rank;
  int num_of_clusters, maxsize;
  int i,j;

  FILE *fp;
  fp = fopen(percfile, "w");
  printf("Opening file <%s>\n", percfile);

  rank = (int*)arralloc(sizeof(int), 1, L * L);

  create_clusterlist(&MAX, L, (int**)map, rank, &num_of_clusters, &maxsize);
  printf("Map has %d clusters, maximum cluster size is %d\n", num_of_clusters, maxsize);
  judge_MAX(&MAX, &num_of_clusters);
  printf("Writing data ...\n");  
  fprintf(fp, "P2\n");
  print_MAX(&MAX, fp, L);
  convert_cluster_to_colour((int**)map, L, &MAX, rank, fp );
  printf("...done\n");
  fclose(fp);
  printf("File closed\n"); 

  free(rank);
}

void create_clusterlist(int *MAX, int L, int **map, int *rank, int *num_of_clusters, int *maxsize)
{
  /* Create the array of clusters, and calculate the size of each cluster, and then sort them by their sizes.
   Finally, set a unique number for every cluster. */

  int i,j;
  struct cluster *clustlist;
  clustlist = (struct cluster*)arralloc(sizeof(struct cluster), 1, L*L);

  calculate_cluster_size(L, clustlist, (int**)map, rank);
  percsort(clustlist, L*L);
  set_cluster_num(maxsize, clustlist, num_of_clusters, rank, MAX, L);

  free(clustlist);
  
}

void calculate_cluster_size(int L, struct cluster *clustlist, int **map, int *rank)
{
  /* calculate the size of cluster */

  int i,j;
  for (i = 0; i < L*L; i++)
  {
    rank[i] = -1;
    clustlist[i].size = 0;
    clustlist[i].id   = i+1;
  }

  for (i = 1; i <= L; i++)
  {
    for (j = 1; j <= L; j++)
    {
      if (map[i][j] != 0)
      {
        ++(clustlist[map[i][j]-1].size);
      }
    }
  }
}

void percsort(struct cluster *list, int n) 
{
  /* Sort clusters by size. */

  qsort(list, (size_t) n, sizeof(struct cluster), clustcompare);
}

static int clustcompare(const void *p1, const void *p2)
{
  /*compare the size of two clusters*/

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

void set_cluster_num(int *maxsize, struct cluster *clustlist, int *num_of_clusters, int *rank, int *MAX, int L)
{
  /* Set a number for every cluster*/

  int i;

  *maxsize = clustlist[0].size;

  for (*num_of_clusters = 0; *num_of_clusters < L*L && clustlist[*num_of_clusters].size > 0; (*num_of_clusters)++);
  
  if (*MAX > *num_of_clusters)
  {
    *MAX = *num_of_clusters;
  }

  for (i = 0; i < *num_of_clusters; i++)
  {
    rank[clustlist[i].id - 1] = i;
  }

}

void judge_MAX(int *MAX, int *num_of_clusters)
{
  /* By judging the value of MAX, print the number of clusters. */

  if (*MAX == 1)
  {
    printf("Displaying the largest cluster\n");
  }
  else if (*MAX == *num_of_clusters)
  {
    printf("Displaying all clusters\n");
  }
  else
  {
    printf("Displaying the largest %d clusters\n", *MAX);
  }
}

void print_MAX(int *MAX, FILE *input_file, int L)
{
  /* present the MAX value */

  if (*MAX > 0)
  {
    fprintf(input_file, "%d %d\n%d\n", L, L, *MAX);
  }
  else
  {
    fprintf(input_file, "%d %d\n%d\n", L, L, 1);
  }
}

void convert_cluster_to_colour(int **map, int L, int *MAX, int *rank, FILE *input_file )
{
  /* Clusters are converted to colours */

  int i,j;
  int colour;

  for (j = L; j >= 1; j--)
  {
    for (i = 1;i <= L; i++)
    {
      colour = map[i][j];

      if (map[i][j] > 0)
      {
        colour = rank[map[i][j]-1];

        if (colour >= *MAX)
        {
          colour = *MAX;
        }
      }
      else
      {
        colour = *MAX;
      }

      fprintf(input_file, " %4d", colour);
    }

    fprintf(input_file,"\n");
  }
  
}














