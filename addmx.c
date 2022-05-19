#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 64

int num_algorism(int num) {
  char str[32];
  memset(str, 0, 32);
  sprintf(str, "%d", num);

  return strlen(str);
}

int main(int argc, char* argv[]) {
  FILE* stream;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);

  stream = fopen(argv[1], "r");
  if(stream == NULL) {
      perror("ERROR");
      return EXIT_FAILURE;
  }

  // Reading matrix dimensions and creating the matrix
  fscanf(stream, "%[^\n]", buffer);
  int nRows1 = atoi(buffer);
  int nCols1 = atoi(strchr(buffer, 'x') + 1);
  int matrix1[nCols1][nRows1];
  memset(buffer, 0, BUFFER_SIZE);  

  // Filling the matrix
  for (int i = 0; i < nRows1; i++)
  {
    fscanf(stream, "%[^\n]", buffer);
    int v = 0;
    for(int j = 0; j < nCols1; j++){
      int num = atoi(buffer + v);
      v += num_algorism(num) + 1;
      matrix1[j][i] = num; 
    }
  }
  

  printf("The size of the matrix 1 is %d x %d\n", nRows1, nCols1);

  if(fclose(stream) != 0){
      return EXIT_FAILURE;
  }

  stream = fopen(argv[2], "r");
  if(stream == NULL) {
      perror("ERROR");
      return EXIT_FAILURE;
  }

  fscanf(stream, "%[^\n]", buffer);
  int nRows2 = atoi(buffer);
  int nCols2 = atoi(strchr(buffer, 'x') + 1);
  int matrix2[nCols2][nRows2];
  memset(buffer, 0, BUFFER_SIZE); 

  // Filling the matrix
  for (int i = 0; i < nRows2; i++)
  {
    fscanf(stream, "%[^\n]", buffer);
    int v = 0;
    for(int j = 0; j < nCols2; j++){
      int num = atoi(buffer + v);
      v += num_algorism(num) + 1;
      matrix1[j][i] = num; 
    }
  }

  printf("The size of the matrix 2 is %d x %d\n", nRows2, nCols2);

  if(fclose(stream) != 0){
      return EXIT_FAILURE;
  }

  if(nRows1 != nRows2 || nCols1 != nCols2){
      printf("Imcompatible matrices\n");
      return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
