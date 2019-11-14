#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "arralloc.h"
#include "percwrite.h"
#include "percolate.h"
#include "uni.h"



void print_datafile(struct grid_related* grid, struct command_line* p, struct output_related *output)
{
  /* Create a txt datafile with a user-defined name. Print the final map with digits in this file.*/

  printf("Opening file </%s>\n", p->datafile_name);
  output->fp1 = fopen(p->datafile_name, "w");
  printf("Writing data ...\n");
  print_digitmap(grid,p,output);
  printf("...done\n");
  fclose(output->fp1);
  printf("File closed\n");

}

void print_digitmap(struct grid_related* grid, struct command_line* p, struct output_related *output)
{
  /* Print digital map.*/
  
  int i,j;
  for (j = p->L; j >= 1; j--)
  {
    for (i = 1;i <= p->L; i++)
    {
      fprintf(output->fp1, " %4d", grid->map[i][j]);
    }
    fprintf(output->fp1,"\n");
  }
}

void print_imagefile(struct grid_related* grid, struct command_line* p, struct output_related *output)
{
  /* Create an imagefile with a user-defined name. Print the final map with colours of different gray levels in this file. */

  int i,j;

  output->fp2 = fopen(p->imagefile_name, "w");
  
  printf("Opening file <%s>\n", p->imagefile_name);

  output->rank = (int*)arralloc(sizeof(int), 1, p->L * p->L);

  create_clusterlist(grid, p, output);
  printf("Map has %d clusters, maximum cluster size is %d\n", output->num_of_clusters, output->maxsize);
  judge_MAX(p, output);
  printf("Writing data ...\n");  
  fprintf(output->fp2, "P2\n");
  print_MAX(p, output);
  convert_cluster_to_colour(grid, p, output);
  printf("...done\n");
  fclose(output->fp2);
  printf("File closed\n"); 

  free(output->rank);
}

void create_clusterlist(struct grid_related* grid, struct command_line* p, struct output_related *output)
{
  /* Create the array of clusters, and calculate the size of each cluster, and then sort them by their sizes.
   Finally, set a unique number for every cluster. */

  int i,j;
  struct cluster *clustlist;
  clustlist = (struct cluster*)arralloc(sizeof(struct cluster), 1, p->L*p->L);

  calculate_cluster_size(clustlist, grid, p, output);
  percsort(clustlist, p->L*p->L);
  set_cluster_num(clustlist, p, output);

  free(clustlist);
  
}

void calculate_cluster_size(struct cluster *clustlist, struct grid_related* grid, struct command_line* p, struct output_related *output)
{
  /* calculate the size of cluster */

  int i,j;
  for (i = 0; i < p->L*p->L; i++)
  {
    output->rank[i] = -1;
    clustlist[i].size = 0;
    clustlist[i].id   = i+1;
  }

  for (i = 1; i <= p->L; i++)
  {
    for (j = 1; j <= p->L; j++)
    {
      if (grid->map[i][j] != 0)
      {
        ++(clustlist[grid->map[i][j]-1].size);
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

void set_cluster_num(struct cluster *clustlist, struct command_line* p, struct output_related *output)
{
  /* Set a number for every cluster*/

  int i;

  output->maxsize = clustlist[0].size;

  for (output->num_of_clusters = 0; output->num_of_clusters < p->L*p->L && clustlist[output->num_of_clusters].size > 0; (output->num_of_clusters)++);
  
  if (p->MAX > output->num_of_clusters)
  {
    p->MAX = output->num_of_clusters;
  }

  for (i = 0; i < output->num_of_clusters; i++)
  {
    output->rank[clustlist[i].id - 1] = i;
  }

}

void judge_MAX(struct command_line* p, struct output_related *output)
{
  /* By judging the value of MAX, print the number of clusters. */

  if (p->MAX == 1)
  {
    printf("Displaying the largest cluster\n");
  }
  else if (p->MAX == output->num_of_clusters)
  {
    printf("Displaying all clusters\n");
  }
  else
  {
    printf("Displaying the largest %d clusters\n", p->MAX);
  }
}

void print_MAX(struct command_line* p, struct output_related *output)
{
  /* present the MAX value */

  if (p->MAX > 0)
  {
    fprintf(output->fp2, "%d %d\n%d\n", p->L, p->L, p->MAX);
  }
  else
  {
    fprintf(output->fp2, "%d %d\n%d\n", p->L, p->L, 1);
  }
}

void convert_cluster_to_colour(struct grid_related* grid, struct command_line* p, struct output_related *output)
{
  /* Clusters are converted to colours */

  int i,j;

  for (j = p->L; j >= 1; j--)
  {
    for (i = 1;i <= p->L; i++)
    {
      if (grid->map[i][j] > 0)
      {
        output->colour = output->rank[grid->map[i][j]-1];

        if (output->colour >= p->MAX)
        {
          output->colour = p->MAX;
        }
      }
      else
      {
        output->colour = p->MAX;
      }

      fprintf(output->fp2, " %4d", output->colour);
    }

    fprintf(output->fp2,"\n");
  }
  
}