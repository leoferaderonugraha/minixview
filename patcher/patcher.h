#ifndef PATCHER_H_
#define PATCHER_H_

enum {
  PATCH_FAILED,
  PATCH_SUCCESS
};

// linked list
typedef struct config {
  int pos;
  int byte;
  struct config *next;
} CONFIG;

// required for splitting a string
int charCount(const char *, const char);
char **strSplit(const char *, const char);
void freeStrArray(char **, int);

// required for patching
int stringLength(const char *);
int patchFile(const char*, const char*);
void removeNewline(char *);
CONFIG *createConfig(const char *);
void freeConfig(CONFIG *);

// misc
int getWrittenBytes();


#endif
