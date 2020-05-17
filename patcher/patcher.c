#include <stdio.h>
#include <stdlib.h>

#include "patcher.h"
#include "../functions.h"

#define HEX_BASE 16
#define MAX_BUFFER 512

// for tracking the written bytes by the patching function
static int writtenBytes = 0;

char **strSplit(const char *str, const char delim)
{
  int totalArray = charCount(str, delim) + 1;

  // if delimiter isn't found, just return a NULL
  if (totalArray <= 1) {
    return NULL;
  }

  const char *ptr = str;
  int offsetArray[totalArray];
  char **strArray = malloc(sizeof(char *) * totalArray);

  for (int i = 0, offset = 0; i < totalArray; i++) {
    // find the delimiter and set the offset to its position
    while (*ptr != delim && *ptr != '\0') {
      offset++;
      ptr++;
    }

    offsetArray[i] = offset;

    // skip current delimiter
    ptr++;
  }

  int len;
  int lenCounter = 0;

  for (int i = 0; i < totalArray; i++) {
    // string length
    len = offsetArray[i] - lenCounter;

    strArray[i] = malloc(sizeof(char) * len + 1);

    // copy "len" byte(s) from string
    // from the given offset
    copyNBytes(strArray[i], &str[lenCounter] + i, len);

    // add a NULL to the last position of the string
    strArray[i][len] = '\0';

    lenCounter = offsetArray[i];
  }

  return strArray;
}

void freeStrArray(char **array, int n)
{
  char **ptr = array;

  for (int i = 0; i < n; i++) {
    free(*(ptr + i));
  }

  free(array);
}

void removeNewline(char *s)
{
  char *ptr = s;

  while (*ptr) {
    if (*ptr == '\n') {
      *ptr = '\0';
    }
    ptr++;
  }
}

int stringLength(const char *str)
{
  int length = 0;
  const char *ptr = str;
  while (*ptr) {
    length++;
    ptr++;
  }
  return length;
}

// don't forget to free things when error occured
int patchFile(const char *fileName, const char *configFile)
{
  FILE *fp;
  fp = fopen(fileName, "rb");

  if (fp == NULL) {
    fprintf(stderr, "Error, couldn't open the file.\n");
    return PATCH_FAILED;
  }

  int fileSize = getFileSize2(fp);

  char buffer[MAX_BUFFER];
  char *data = malloc(sizeof(char) * fileSize);

  int bytesCopied;
  int offset = 0;

  while ((bytesCopied = fread(buffer, 1, MAX_BUFFER, fp)) > 0) {
    copyNBytes(&data[offset], buffer, bytesCopied);
    offset += bytesCopied;
  }

  // close the file to reopen with write mode later
  fclose(fp);

  CONFIG *cfg = createConfig(configFile);
  CONFIG *ptr = cfg;

  if (cfg == NULL) {
    fprintf(stderr, "Invalid configuration file.\n");
    free(data);
    return PATCH_FAILED;
  }

  while (ptr) {
    if (ptr->pos > fileSize) {
      fprintf(stderr, "Some offset in the config file are not valid (0x%.8x).\n", ptr->pos);

      free(data);
      freeConfig(cfg);

      return PATCH_FAILED;
    }

    data[ptr->pos] = ptr->byte;
    ptr = ptr->next;
    writtenBytes++;
  }

  // reopen the file for patching
  fp = fopen(fileName, "wb");
  if (fp == NULL) {
    fprintf(stderr, "Error occured when trying to reopen the file.\n");
    free(data);
    // i guess it's no use to close the file,
    // since it's failed to open(?) :/
    return PATCH_FAILED;
  }

  if ((fwrite(data, 1, fileSize, fp)) > 0) {
    freeConfig(cfg);
    free(data);
    fclose(fp);

    return PATCH_SUCCESS;
  } else {
    fprintf(stderr, "Error, couldn't write to the file.\n");
    freeConfig(cfg);
    free(data);
    fclose(fp);

    return PATCH_FAILED;
  }

  //return ((fwrite(data, 1, fileSize, fp)) > 0) ? PATCH_SUCCESS : PATCH_FAILED;
}

// implement linked list
CONFIG *createConfig(const char *fileName)
{
  FILE *fp = fopen(fileName, "r");

  if (fp == NULL) {
    fprintf(stderr, "Couldn't open the config file.\n");
    return NULL;
  }

  //int fileSize = getFileSize(fileName);

  char buffer[MAX_BUFFER];
  CONFIG *head = NULL;
  CONFIG *newNode, *ptr;

  while ((fgets(buffer, MAX_BUFFER, fp))) {
    removeNewline(buffer);
    char **array = strSplit(buffer, ':');

    if (array != NULL) {
      newNode = malloc(sizeof(CONFIG));
      // better use hex rather than decimal for binary patching
      newNode->pos = strtol(array[0], NULL, HEX_BASE);
      newNode->byte = (unsigned char) strtol(array[1], NULL, HEX_BASE);
      newNode->next = NULL;

      if (head == NULL) {
        head = newNode;
      } else {
        ptr = head;

        // skip to the last element
        while (ptr->next) {
          ptr = ptr->next;
        }

        // link the last element with current newNode
        ptr->next = newNode;
      }

      freeStrArray(array, (charCount(buffer, ':') + 1));
    }
  }

  fclose(fp);
  return head;
}

void freeConfig(CONFIG *cfg)
{
  CONFIG *ptr = cfg;
  CONFIG *tmp = NULL;
  while (ptr) {
    tmp = ptr->next;
    free(ptr);
    ptr = tmp;
  }
}

int getWrittenBytes()
{
  return writtenBytes;
}
