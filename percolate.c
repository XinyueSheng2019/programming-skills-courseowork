#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "arralloc.h"
#include "percolate.h"
#include "percwrite.h"
#include "command_line.h"
#include "uni.h"


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
















