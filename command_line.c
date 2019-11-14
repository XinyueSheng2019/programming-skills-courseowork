#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "command_line.h"
#include "percolate.h"


int main(int argc, char* argv[])
{
 
  struct command_line comm;
  struct command_line *p = NULL;
  p = &comm;
  p->L = 20;
  p->rho = 0.400000;
  p->seed = 1564;
  p->MAX = (p->L)*(p->L);
  p->datafile_name = "map.dat";
  p->imagefile_name = "map.pgm";

  while((p->opt = getopt(argc,argv,":l:r:s:m:d:g:"))!=-1)
  {
    switch(p->opt)
    {
      case 'l':
        p->L = atoi(optarg);
        break;
      case 'r':
        p->rho = atof(optarg); 
        break;
      case 's':
        p->seed = atoi(optarg);
        break;
      case 'm':
        p->MAX = atoi(optarg);
        break;
      case 'd':
        p->datafile_name = optarg;
        break;
      case 'g':
        p->imagefile_name = optarg;
        break;

    }
  }
  valid_result = judge_validation_of_command_lines(p);
  
  if (valid_result == -1) return 0;

  printf("**************************************\nHere are parameters:\n(Unset parameters have been given original values.)\nL   : %d\nrho : %f\nseed: %d\nMAX : %d\ndatafile_name : %s\nimagefile_name: %s\n**************************************\n", p->L, p->rho, p->seed, p->MAX, p->datafile_name, p->imagefile_name);

  // p->datafile_name = "output/" + p->datafile_name;
  percolate_processing(p);

  return 0;

}

int judge_validation_of_command_lines(struct command_line* p)
{
/*judge the validation of parameters*/
  if (p->L < 1)
  {
    printf("map width and height should be no less than 1. \n");
    return -1;
  }
  if (p->rho < 0 || p->rho > 1)
  {
    printf("The value of ρ should be between 0 and 1.\n ");
    return -1;
  }
  if (p->seed < 0 || p->seed > 900000000)
  {
    printf("The value of seed should be between 0 and 900000000. \n");
    return -1;
  }
  if (p->MAX < 0 || p->MAX > p->L * p->L)
  {
    printf("The value of MAX should be between 0 and L*L. \n");
    return -1;
  }
  return 0;
}