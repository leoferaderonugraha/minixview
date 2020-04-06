#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "functions.h"
#include "swinfo.h"

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

void print_version() {
  printf("mxview V%s %s by %s\n", SW_VERSION, SW_DATE, SW_AUTHOR);
  exit(0);
}

void print_help() {
  char help_msg[] = \
    "Usage:\n"\
    "        mxview -f file_name\n"\
    "    or\n"\
    "        cat file_name | mxview\n"\
    "Options:\n"\
    "    -f file_name        reading data from a file.\n"\
    "    -v                  print current version.\n"
    "    -h                  print this message.";

  puts(help_msg);
  exit(0);
}
