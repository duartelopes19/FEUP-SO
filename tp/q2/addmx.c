#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 64

int num_algorism(int num) {
  char str[32];
  memset(str, 0, 32);
  sprintf(str, "%d", num);

  return strlen(str);
}

int main(int argc, char* argv[]) {
  if(argc==1) {
    printf("usage: addmx file1 file2\n");
    return EXIT_SUCCESS;
  }

  FILE* stream;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);

  stream = fopen(argv[1], "r");

  // checking for errors
  if(stream == NULL) {
      perror("ERROR");
      return EXIT_FAILURE;
  } else {
      fseek (stream, 0, SEEK_END);
      if (ftell(stream) == 0) {
          printf("%s is empty!\n", argv[1]);
          return EXIT_SUCCESS;
      }
      fseek(stream, 0, SEEK_SET);
  }

  // reading matrix dimensions
  fscanf(stream, "%[^\n]", buffer);
  getc(stream);
  int nRows1 = atoi(buffer);
  int nCols1 = atoi(strchr(buffer, 'x') + 1);

  // using mmap()
  int* m_ptr;
  m_ptr = mmap(NULL, 3*nRows1*nCols1*sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);

  if (m_ptr == MAP_FAILED) {
    perror("Error");
    return EXIT_FAILURE;
  }

  // matrix pointers
  int* pmat1 = m_ptr;
  int* pmat2 = m_ptr + (nRows1 * nCols1);
  int* pmatRes = m_ptr + (2 * nRows1 * nCols1);

  // filling the matrix1
  for (int i = 0; i < nRows1; i++) {
    memset(buffer, 0, BUFFER_SIZE);
    fscanf(stream, "%[^\n]", buffer);
    getc(stream);
    char* temp = buffer;
    for(int j = 0; j < nCols1; j++){
      int num = atoi(temp);
      char str[32];
      memset(str, 0, 32);
      sprintf(str, "%d", num);
      temp = strstr(temp, str);
      temp += num_algorism(num) + 1;
      // accessing matrix1 in the maped memory
      *(pmat1 + (j * nRows1 + i)) = num; 
    }
  }
 
  if(fclose(stream) != 0){
      return EXIT_FAILURE;
  }

  stream = fopen(argv[2], "r");
  if(stream == NULL) {
      perror("ERROR");
      return EXIT_FAILURE;
  } else {
      fseek (stream, 0, SEEK_END);
      if (ftell(stream) == 0) {
          printf("%s is empty!\n", argv[2]);
          return EXIT_SUCCESS;
      }
      fseek(stream, 0, SEEK_SET);
  }

  fscanf(stream, "%[^\n]", buffer);
  getc(stream);
  int nRows2 = atoi(buffer);
  int nCols2 = atoi(strchr(buffer, 'x') + 1);
  
  // checking if operation is valid
  if(nRows1 != nRows2 || nCols1 != nCols2) {
      printf("Imcompatible matrices\n");
      return EXIT_SUCCESS;
  }

  for (int i = 0; i < nRows2; i++)
  {
    memset(buffer, 0, BUFFER_SIZE);
    fscanf(stream, "%[^\n]", buffer);
    getc(stream);
    char* temp = buffer;
    for(int j = 0; j < nCols2; j++){
      int num = atoi(temp);
      char str[32];
      memset(str, 0, 32);
      sprintf(str, "%d", num);
      temp = strstr(temp, str);
      temp += num_algorism(num) + 1;
      // accessing matrix2 in the maped memory
      *(pmat2 + (j * nRows2 + i)) = num;
    }
  }

  if(fclose(stream) != 0) {
      return EXIT_FAILURE;
  }

  pid_t pids[nCols2];
  int id, col;
  int n = nCols2;
  
  /* Start children. */
  for (int i = 0; i < n; ++i) {
    if ((pids[i] = fork()) < 0) {
      perror("fork");
      abort();
    }
    if(pids[i]==0) {
      id = getpid();
      col = i;
      break;
    }
  }

  /* Calculate collums in each child process */
  if (id == getpid()) {
    for(int j = 0; j < nRows2; j++) {
      *(pmatRes + (col * nRows2 + j)) = *(pmat1 + (col * nRows2 + j)) + *(pmat2 + (col * nRows2 + j));
    }
    exit(0);
  }

  /* Wait for children to exit. */
  int status;
  while (n > 0) {
    if(wait(&status)==-1) {
      perror("wait");
    }
    --n;
  }

  printf("%dx%d\n",nRows2,nCols2);
  for (int i = 0; i < nRows2; i++)
  {
    for (int j = 0; j < nCols2; j++)
    {
      printf("%d ", *(pmatRes + (j * nRows2 + i)));
    }
    printf("\n");
  }

  return EXIT_SUCCESS;
}
