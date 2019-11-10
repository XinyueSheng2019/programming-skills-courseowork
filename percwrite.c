#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "arralloc.h"
#include "percwrite.h"
#include "uni.h"



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