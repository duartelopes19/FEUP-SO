#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64

int main(int argc, char* argv[]) {
  char buffer[BUFFER_SIZE];
  FILE* stream;

  stream = fopen(argv[1], "r");
  if(stream == NULL) {
      perror("Error");
      return EXIT_FAILURE;
  } else {
      fseek (stream, 0, SEEK_END);
      if (ftell(stream) == 0) {
          printf("Empty File!\n");
          return EXIT_SUCCESS;
      }
      fseek(stream, 0, SEEK_SET);
  }

  size_t block_size;

  while (1) {
    memset(buffer, 0, BUFFER_SIZE);
    block_size = fread(buffer, 1, BUFFER_SIZE - 1, stream);

    printf("%s", buffer);

    if(block_size != BUFFER_SIZE - 1) {
      printf("\n");
      break;
    }
  }

  return EXIT_SUCCESS;
}
