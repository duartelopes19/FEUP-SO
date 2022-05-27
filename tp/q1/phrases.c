#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1000

int main(int argc, char* argv[]) {
  char buffer[BUFFER_SIZE];
  FILE* stream;

  if(strcmp(argv[1],"-l")!=0){
    stream = fopen(argv[1], "r");
  } else {
    stream = fopen(argv[2], "r");
  }
  
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
  int count=1;
  if(strcmp(argv[1],"-l")!=0){
    printf("[1] ");
  }

  while (1) {
    memset(buffer, 0, BUFFER_SIZE);
    block_size = fread(buffer, 1, BUFFER_SIZE - 1, stream);

    
    for(int i=0; buffer[i]!=0; i++){
      char ch = buffer[i];
      if(ch=='\n') continue;
      if(strcmp(argv[1],"-l")!=0){
      printf("%c", buffer[i]);
      }
      if(ch == '!'||ch == '?'||ch == '.'){
        count++;
        if(strcmp(argv[1],"-l")!=0){
        printf("\n");
        printf("[");
        printf("%i", count);
        printf("]");
        if(buffer[i+1]!=' '){
          printf(" ");
        }
        }
      }
    }

    if(block_size != BUFFER_SIZE - 1) {
      break;
    }

  }

  if(strcmp(argv[1],"-l")==0){
    printf("%i",count);
  }

  printf("\n");

  return EXIT_SUCCESS;
}
