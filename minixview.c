#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "functions.h"


int main(int argc, char**argv) {
  if (argc < 2){
    printf("%s: file_name\n", argv[0]);
    return 1;
  }

  char *file_name = argv[1];
  FILE *fp = fopen(file_name, "rb");
  int file_size = get_file_size(file_name);


  unsigned char *content = malloc(sizeof(char) * file_size);
  unsigned char buffer[256];

  if (content == NULL) {
    printf("Fail to allocating the required memory.");
    return 1;
  }

  int copied = 0;
  int offset = 0;
  int bytes_to_copy = file_size;

  // read the whole file content and store it to *content
  while (bytes_to_copy > 0) {
    copied = fread(&buffer, 1, 256, fp);
    bytes_to_copy -= copied;

    cpnb(&content[offset], buffer, copied);
    offset += copied;
  }

  // set offset back to zero
  offset = 0;
  int line_offset = 0;

  // display the content in a hex value
  for(int stat = 0, total_ch = 0, total_space = 0; (file_size - offset) > 0;) {
    // printing its offset
    printf("%08x:    ", line_offset);

    for (int i = 0; i < 8 && !stat; i++) {
      for (int j = 0; j < 2 && !stat; j++) {
        if ((offset + j + 1) >= file_size) stat = 1;

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
  fclose(fp);
}
