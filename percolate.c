#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "arralloc.h"
#include "percolate.h"
#include "percwrite.h"
#include "command_line.h"
#include "uni.h"


void percolate_processing(struct command_line* p)
{
  /*set an initial map, and then send the address of this map to every function for further usages.*/
  struct grid_related grid_info;
  struct grid_related *grid = NULL;
  grid = &grid_info;
  struct percolation_related percolate_info;
  struct percolation_related *percolate = NULL;
  percolate = &percolate_info;
  struct output_related output_info;
  struct output_related *output = NULL;
  output = &output_info;

  grid->map = (int**)arralloc(sizeof(int), 2, p->L+2, p->L+2);
  
  rinit(p->seed);
  create_map(grid, p);
  do_loop(percolate, grid, p);
  result_of_percolate(percolate, grid, p);
  print_datafile(grid, p, output);
  print_imagefile(grid,p, output); 
  free(grid->map);
}


void create_map(struct grid_related* grid, struct command_line* p)
{
  /* This function means to define the initial map. The rho and L are set by the user. */

  int i,j;
  // int num_of_unfilled;
  
  init_extent_map(grid, p);
  grid->num_of_unfilled = set_unfilled_squares(grid, p);
  printf("num of unfilled: %d\n",grid->num_of_unfilled);
  printf("rho = %f, actual density = %f\n",p->rho, 1.0 - ((double)grid->num_of_unfilled)/((double)(p->L)*(p->L)));
  set_unique_num(grid, p);  
}

void init_extent_map(struct grid_related* grid, struct command_line* p)
{
  /* Every square in this map with the extension length and width L+2, is set to 0.*/

  int i,j;

  for (i = 0; i < p->L+2; i++)
  {
    for (j = 0; j < p->L+2; j++)
    {
      grid->map[i][j] = 0;
    }
  }
}

int set_unfilled_squares(struct grid_related* grid, struct command_line* p)
{
  /* By using random number(0~1) and comparing it with rho, the number and the coords of unfilled squares are set.
And those unfilled squares are set to 1.*/

  int i,j;
  grid->num_of_unfilled = 0;
  for (i = 1; i <= p->L; i++)
  {
    for (j = 1; j <= p->L; j++)
    {
      grid->r = random_uniform();
      if(grid->r > p->rho)
      {
        grid->num_of_unfilled++;
        grid->map[i][j] = 1;
      }
    }
  }
  return grid->num_of_unfilled;
}

void set_unique_num(struct grid_related* grid, struct command_line* p)
{
  /* Calculate the number of unfilled squares, 
  and then change each of the unfilled squares with a unique positive integer.*/

  int i,j;
  grid->unique_num = 0;

  for (i = 1; i <= p->L; i++)
  {
    for (j = 1; j <= p->L; j++)
    {
      if (grid->map[i][j] != 0)
      {
        grid->unique_num++;
        grid->map[i][j] = grid->unique_num;
      }
    }
  }
}


void do_loop( struct percolation_related *percolate, struct grid_related* grid, struct command_line* p)
{ 
/* Loop over all the squares in the map many times, and during each pass of the loop we replace each square 
with the maximum of its four neighbours. In all cases, we can ignore the filled (grey) squares. 
The large numbers gradually fill the gaps so that each cluster eventually contains a single, unique number.*/

  percolate->loop = 1;
  percolate->num_of_changes = 1;

  while (percolate->num_of_changes > 0)
  {
    percolate->num_of_changes = replace_square(percolate, grid, p);
    printf("Number of changes on loop %d is %d\n", percolate->loop, percolate->num_of_changes);
    percolate->loop++;
  }
  
}

int replace_square(struct percolation_related *percolate, struct grid_related* grid, struct command_line* p)
{
  /* Replace each square with the maximum of its four neighbours*/

  int i, j;
  percolate->num_of_changes = 0;

  for (i = 1; i <= p->L; i++)
  {
    for (j = 1; j <= p->L; j++)
    {
      if (grid->map[i][j] != 0)
      {
        percolate->old_num = grid->map[i][j];
        if (grid->map[i-1][j] > grid->map[i][j]) grid->map[i][j] = grid->map[i-1][j];
        if (grid->map[i+1][j] > grid->map[i][j]) grid->map[i][j] = grid->map[i+1][j];
        if (grid->map[i][j-1] > grid->map[i][j]) grid->map[i][j] = grid->map[i][j-1];
        if (grid->map[i][j+1] > grid->map[i][j]) grid->map[i][j] = grid->map[i][j+1];
        if (grid->map[i][j] != percolate->old_num)  percolate->num_of_changes++;    
      }
    }
  }
  return  percolate->num_of_changes;
}



void result_of_percolate(struct percolation_related *percolate, struct grid_related* grid, struct command_line* p)
{
/* Calculate and print the results: whether clusters percolate the map, 
if cluster does percolate, print the number of clusters which percolate the map;
if cluster does not percolate, print this result.*/

  percolate->num_of_percclusters = 0;
  percolate->perc_success = 0;

  judge_percolate(percolate,grid,p);

  if (percolate->perc_success)
  {
    printf("Cluster DOES percolate. Cluster number: %d\n", percolate->num_of_percclusters);
  }
  else
  {
    printf("Cluster DOES NOT percolate\n");
  }

}

void judge_percolate(struct percolation_related *percolate, struct grid_related* grid, struct command_line* p)
{
  /* Judge whether there is at least a cluster which percolates the map. */

  for (percolate->top_row = 1; percolate->top_row <= p->L; percolate->top_row++)
  {
    if (grid->map[percolate->top_row][p->L] > 0)
    {
      for (percolate->bottom_row = 1; percolate->bottom_row <= p->L; percolate->bottom_row++)
      {
        if (grid->map[percolate->top_row][p->L] == grid->map[percolate->bottom_row][1])
        {
          percolate->perc_success = 1;
          percolate->num_of_percclusters = grid->map[percolate->top_row][p->L];
        }
      }
    }
  }
}
















