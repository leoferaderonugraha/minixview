#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

int getFileSize(const char *);
int getFileSize2(FILE *);

// ctrl_c (interrupt) signal handler
void SIGINT_handler();

void copyNBytes(void *, const void *, int);

void printHelp();
void printVersion();

enum stats {
  READ_FROM_STDIN,
  READ_FROM_FILE
};

#endif
