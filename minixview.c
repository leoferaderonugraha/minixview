#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>    // for isprint()
#include <unistd.h>   // for getopt()
#include <signal.h>   // for catching the ctrl+c / SIGINT
#include <getopt.h>   // command parsing
#include <stdbool.h>  // use boolean for readibility

#include "functions.h"
#include "patcher/patcher.h"

#define MAX_BUFFER 256


int main(int argc, char**argv)
{
  int optionsIndex = 0;
  struct option longOptions[] = {
    {"file",    required_argument,  NULL, 'f'},
    {"lines",   required_argument,  NULL, 'l'},
    {"border",  required_argument,  NULL, 'b'},
    {"patch",   required_argument,  NULL, 'p'},
    {"version", no_argument,        NULL, 'v'},
    {"help",    no_argument,        NULL, 'h'},
    {NULL,      0,                  NULL,   0}
  };

  /* maybe we would to use mutiple chars for border? */
  char *border = "|";
  int flag;
  char *fileName = NULL;
  unsigned char *content;
  unsigned char buffer[MAX_BUFFER];

  int offset, lineOffset;
  int contentLen = 0;
  int bytesCopied;

  bool printByLines = false;
  bool stop = false;
  int nLines;

  char *configFile;
  bool patching = false;

  if (argc < 2) {
    flag = READ_FROM_STDIN;
    content = malloc(sizeof(unsigned char) * MAX_BUFFER);

    int n = 1;
    offset = 0;
    // catching the ctrl case
    // in case if you're forgot to type -f fileName
    signal(SIGINT, SIGINT_handler);

    while ((bytesCopied = read(STDIN_FILENO, buffer, MAX_BUFFER)) > 0) {
      // if the current buffer it's not enough
      // then reallocate some more
      if (bytesCopied == MAX_BUFFER) {
        n++;
        unsigned char *tmp = realloc(content, sizeof(unsigned char) * MAX_BUFFER * n); // ++n

        if (tmp != NULL) {
          content = tmp;
        } else {
          free(content);
          fprintf(stderr, "Error on reallocating some memory.\n");
          exit(EXIT_FAILURE);
        }
      }

      copyNBytes(&content[offset], buffer, MAX_BUFFER);
      offset += bytesCopied;
      contentLen += bytesCopied;
    }
  } else {
    int opt;
    flag = READ_FROM_FILE;

    while ((opt = getopt_long(argc, argv, ":f:p:l:b:vh", longOptions, &optionsIndex)) != -1) {
      switch (opt) {
        case 'f':
          fileName = optarg;
          break;
        case 'p':
          patching = true;
          configFile = optarg;
          break;
        case 'l':
          nLines = strtol(optarg, NULL, 10);
          printByLines = (nLines > 0);
          break;
        case 'b':
          border = optarg;
          break;
        case ':':
          fprintf(stderr, "Please specify the option argument.\n");
          exit(EXIT_FAILURE);
          break;
        case 'v':
          printVersion();
          break;
        case 'h':
        case '?':
          printHelp();
          break;
      }
    }

    // preventing from reading a file
    // without specifying its name
    if (fileName == NULL && optind) {
      printHelp();
    } else if (fileName != NULL && patching) {
      if ((patchFile(fileName, configFile)) == PATCH_SUCCESS) {
        printf("Patching success.\n");
        printf("%d byte(s) has been written.\n", getWrittenBytes());
        exit(EXIT_SUCCESS);
      } else {
        fprintf(stderr, "Patching failed.\n");
        exit(EXIT_FAILURE);
      }
    }

    FILE *fp = fopen(fileName, "rb");

    // failed to open the file
    if (fp == NULL) {
      fprintf(stderr, "Couldn't open the file.\n");
      exit(EXIT_FAILURE);
    }

    int fileSize = getFileSize(fileName);
    content = malloc(sizeof(unsigned char) * fileSize);

    if (content == NULL) {
      fprintf(stderr, "Fail to allocating the required memory.\n");
      exit(EXIT_FAILURE);
    }

    int bytesToCopy = fileSize;
    bytesCopied = 0;
    offset = 0;

    // read the whole file content and store it to *content
    while (bytesToCopy > 0) {
      bytesCopied = fread(&buffer, 1, 256, fp);
      bytesToCopy -= bytesCopied;

      copyNBytes(&content[offset], buffer, bytesCopied);

      offset += bytesCopied;
    }

    fclose(fp);
  }

  // set offset back to zero
  offset = 0;
  lineOffset = 0;

  // check if it's reading from a file
  if (flag == READ_FROM_FILE) {
    contentLen = getFileSize(fileName);
  }

  // display the content in a hex value
  for(int reachEOF = 0, totalChar = 0, totalSpace = 0; (contentLen - offset) > 0 && !stop;) {
    if (printByLines) {
      if (nLines > 0) {
        nLines--;
      } else {
        stop = true;
        break;
      }
    }

    // printing its offset
    printf("%08x:    ", lineOffset);

    // print the hex code
    for (int i = 0; i < 8 && !reachEOF; i++) {
      for (int j = 0; j < 2 && !reachEOF; j++) {
        // stop the loop if it has reached the EOF
        if ((offset + j + 1) >= contentLen) {
          reachEOF = 1;
        }
        totalChar++;
        printf("%02x", content[offset + j]);
      }

      offset += 2;
      printf(" ");
      totalSpace++;
    }

    int align = totalChar * 2 + totalSpace;

    // 40 is max printed character
    // and 4 is for creating space
    int maxPrintChar = 44;

    // aligning with whitespace
    for (; align < maxPrintChar; align++) {
      printf(" ");
    }

    // printing the border
    printf("%s", border);

    // preview the hex code as a printable character
    for (int i = 0, ch, position; i < totalChar; i++) {
      // get the exact position of the character
      position = offset - totalChar + i - (totalChar % 2);
      ch = content[position];
      printf("%c", (isprint(ch)) ? ch : '.');
    }

    lineOffset += totalChar;
    totalChar = 0;
    totalSpace = 0;
    printf("\n");
  }

  free(content);
}
