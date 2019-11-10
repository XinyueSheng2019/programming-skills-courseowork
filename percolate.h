

struct command_line
{
  int L, seed, MAX, opt;
  float rho;
  char* datafile;
  char* imagefile;
  char* format_dat;
  char* format_pgm; 
};

struct percolation_related
{
  int loop;
  int num_of_changes, old_num;
  int num_of_percclusters, perc_success;
  int top_row, bottom_row;  
};

struct output_related
{
  int *rank;
  int num_of_clusters, maxsize;
  int colour;
};

struct grid_related
{
  int** map; 
  int num_of_unfilled;
  int unique_num;
  float r;
};


int judge_validation_of_command_lines(int L, int rho, int seed, int MAX);
void percolate_processing(float rho, int L, int MAX, int seed, char* datafile_name, char* imagefile_name);
void create_map(float rho, int L, int **map);
void init_extent_map(int **map, int L);
int  set_unfilled_squares(int **map, int L, float rho);
void set_unique_num(int **map, int L);
void do_loop(int L, int **map);
int  replace_square(int **map, int L);
void result_of_percolate(int L,int **map);
void judge_percolate(int *num_of_percclusters, int *perc_success, int **map, int L);
