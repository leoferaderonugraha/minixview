#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  // for isprint()
#include <unistd.h> // for getopt()
#include <signal.h> // for catching the ctrl+c

#include "functions.h"

#define MAX_BUFF 256

int main(int argc, char**argv) {
  int flag;
  char *file_name;
  unsigned char *content;
  unsigned char buffer[MAX_BUFF];

  int offset, line_offset;
  int content_len = 0;
  int bytes_copied;


  if (argc < 2){
    flag = READ_FROM_STDIN;
    content = malloc(sizeof(char) * MAX_BUFF);

    int n = 1;
    offset = 0;
    // catching the ctrl case
    // in case if you're forgot to type -f file_name
    signal(SIGINT, SIGINT_handler);

    while (bytes_copied = read(STDIN_FILENO, buffer, MAX_BUFF)) {
      // if the current buffer it's not enough
      // then reallocate some more
      if (bytes_copied == MAX_BUFF) {
        n++;
        content = realloc(content, n * sizeof(char) * MAX_BUFF);
      }

      cpnb(&content[offset], buffer, MAX_BUFF);
      offset += bytes_copied;
      content_len += bytes_copied;
    }
  }
  else {
    int opt;
    flag = READ_FROM_FILE;

    while ((opt = getopt(argc, argv, ":vhf:")) != -1) {
      switch (opt) {
        case 'f':
          file_name = optarg;
          break;
        case 'h':
          print_help();
          break;
        case 'v':
          print_version();
          break;
        case '?':
          print_help();
          break;
      }
    }

    FILE *fp = fopen(file_name, "rb");
    int file_size = get_file_size(file_name);

    content = malloc(sizeof(char) * file_size);

    if (content == NULL) {
      fprintf(stderr, "Fail to allocating the required memory.\n");
      return 1;
    }

    bytes_copied = 0;
    offset = 0;
    int bytes_to_copy = file_size;

    // read the whole file content and store it to *content
    while (bytes_to_copy > 0) {
      bytes_copied = fread(&buffer, 1, 256, fp);
      bytes_to_copy -= bytes_copied;

      cpnb(&content[offset], buffer, bytes_copied);
      offset += bytes_copied;
    }
    fclose(fp);
  }

  // set offset back to zero
  offset = 0;
  line_offset = 0;
  if (flag == READ_FROM_FILE) {
    content_len = get_file_size(file_name);
  }

  // display the content in a hex value
  for(int stat = 0, total_ch = 0, total_space = 0; (content_len - offset) > 0;) {
    // printing its offset
    printf("%08x:    ", line_offset);

    for (int i = 0; i < 8 && !stat; i++) {
      for (int j = 0; j < 2 && !stat; j++) {
        if ((offset + j + 1) >= content_len) stat = 1;

        total_ch++;
        printf("%02x", content[offset + j]);
      }

      offset += 2;
      printf(" ");
      total_space++;
    }

    //fprintf(stderr, "%d\n", total_ch);

    // for align
    int align = total_ch * 2 + total_space;
    int max_print_ch = 40;
        max_print_ch += 4;  // align for 4 spaces

    for (; align < max_print_ch; align++)  printf(" ");
    printf("┇");

    // preview the hex code as a printable character
    for (int cnt = 0, ch, pos; cnt < total_ch; cnt++) {
      pos = offset - total_ch + cnt - (total_ch % 2);
      ch = content[pos];
      printf("%c", (isprint(ch)) ? ch : '.');
    }

    line_offset += total_ch;
    total_ch = 0;
    total_space = 0;
    printf("\n");
  }

  free(content);
}
