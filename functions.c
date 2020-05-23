#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // for getting the file size

#include "functions.h"
#include "swinfo.h"

int getFileSize(const char *filename)
{
  struct stat st;
  stat(filename, &st);
  return st.st_size;
}

int getFileSize2(FILE *fp)
{
  fseek(fp, 0, SEEK_END);
  int fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return fsize;
}

void copyNBytes(void *dst, const void *src, int n)
{
  const char *sptr = src;
  char *dptr = dst;

  while (n--) {
    /*
      *dptr = *sptr;
      dptr++; sptr++;
    */
    *dptr++ = *sptr++;
  }
}

void SIGINT_handler()
{
  printf("\n");
  exit(EXIT_SUCCESS);
}

void printHelp()
{
  char helpMsg[] =  "Usage:\n"
                    "        mxview <file>\n"
                    "        mxview -f <file>\n"
                    "    or\n"
                    "        cat <file> | mxview\n"
                    "Options:\n"
                    "     -f, --file <file>                   reading data from a file.\n"
                    "     -p, --patch <config file>           patch the file with the given config file.\n"
                    "     -l, --lines <n>                     print up to n lines.\n"
                    "     -b, --border <s>                    set s as a border (default border: |).\n"
                    "     -v, --version                       print current version.\n"
                    "     -h, --help                          print this message.";

  puts(helpMsg);
  exit(EXIT_SUCCESS);
}

void printVersion()
{
  printf("mxview V%s %s by %s\n", SW_VERSION[0][0], SW_DATE, SW_AUTHOR);
  exit(EXIT_SUCCESS);
}

int charCount(const char *str, const char ch)
{
  int counter = 0;
  const char *ptr = str;

  while (*ptr) {
    if (*ptr == ch) {
      counter++;
    }
    ptr++;
  }

  return counter;
}
