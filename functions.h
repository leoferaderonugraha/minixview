#ifndef FUNCTIONS
#define FUNCTIONS

int get_file_size(char *fname);
int get_file_size2(FILE *fp);

// cpnb = copy n byte(s)
void cpnb(void *dst, void *src, int n);

<<<<<<< HEAD
enum stats {
  READ_FROM_STDIN,
  READ_FROM_FILE
};

=======
>>>>>>> 23f197833ae9b6786265cef77fc646f40847a162
#endif
