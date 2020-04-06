#ifndef FUNCTIONS
#define FUNCTIONS


int get_file_size(char *fname);
int get_file_size2(FILE *fp);

// ctrl_c (interrupt) signal handler
void SIGINT_handler();

// cpnb = copy n byte(s)
void cpnb(void *dst, void *src, int n);

void print_help();
void print_version();

enum stats {
  READ_FROM_STDIN,
  READ_FROM_FILE
};

#endif
