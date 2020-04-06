#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "functions.h"

int get_file_size(char *fname) {
  struct stat st;
  stat(fname, &st);
  return st.st_size;
}

int get_file_size2(FILE *fp) {
  fseek(fp, 0, SEEK_END);
  int fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return fsize;
}

void cpnb(void *dst, void *src, int n) {
  char *sptr = src;
  char *dptr = dst;

  while (n--) {
    /*
      *dptr = *sptr;
      dptr++; sptr++;
    */
    *dptr++ = *sptr++;
  }
}

void SIGINT_handler() {
  printf("\n");
  exit(1);
}
