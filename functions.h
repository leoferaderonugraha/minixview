#ifndef FUNCTIONS
#define FUNCTIONS

int get_file_size(char *fname);
int get_file_size2(FILE *fp);

// cpnb = copy n byte(s)
void cpnb(void *dst, void *src, int n);

#endif
