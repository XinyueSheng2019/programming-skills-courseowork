#include "command_line.h"

#ifndef _B_H_
#define _B_H_
struct percolation_related
{
  int loop;
  int num_of_changes, old_num;
  int num_of_percclusters, perc_success;
  int top_row, bottom_row;  
};

struct grid_related
{
  int** map; 
  int num_of_unfilled;
  int unique_num;
  float r;
};

#endif





void create_map(struct grid_related* grid, struct command_line* p);
void init_extent_map(struct grid_related* grid, struct command_line* p);
int set_unfilled_squares(struct grid_related* grid, struct command_line* p);
void set_unique_num(struct grid_related* grid, struct command_line* p);
void do_loop( struct percolation_related *percolate, struct grid_related* grid, struct command_line* p);
int replace_square(struct percolation_related *percolate, struct grid_related* grid, struct command_line* p);
void result_of_percolate(struct percolation_related *percolate, struct grid_related* grid, struct command_line* p);
void judge_percolate(struct percolation_related *percolate, struct grid_related* grid, struct command_line* p);
