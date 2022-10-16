#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PAGES 4096


int main(int argc, char* argv[]) {

  if(argc != 3){
      printf("usage: addmx file1 file2\n");
      return 1;
  }

  char dimension_matrix1[50], dimension_matrix2[50];
  FILE *file1, *file2;
  if ((file1 = fopen(argv[1], "r")) == NULL) {
      printf("Error: File 1 cannot be opened.");
      exit(1);
  }
  if ((file2 = fopen(argv[2], "r")) == NULL) {
      printf("Error: File 2 cannot be opened.");
      exit(1);
  }

  // reads text until newline is encountered
  fscanf(file1, "%[^\n]", dimension_matrix1);
  fscanf(file2, "%[^\n]", dimension_matrix2);
  if(strcmp(dimension_matrix1,dimension_matrix2) != 0){
    printf("Matrix dimensions are not compatible.");
    exit(1);
  }
  char *token = strtok(dimension_matrix1, "x");
  int lines = atoi(token);
  token = strtok(NULL, "x");
  int columns = atoi(token);


  char matrix_line[(columns*2)-1];
  int index_lin = 0;
  int index_col = 0;
  int (*matrix1)[lines] = mmap(NULL, columns* sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0,0);
  if(matrix1 == MAP_FAILED){
     printf("Mapping Failed\n");
     return 1;
    }
  int (*matrix2)[lines] = mmap(NULL, columns* sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0,0);
  if(matrix2 == MAP_FAILED){
     printf("Mapping Failed\n");
     return 1;
  }
  int (*matrix_result)[lines] = mmap(NULL, columns* sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0,0);
  if(matrix_result == MAP_FAILED){
     printf("Mapping Failed\n");
     return 1;
  }

  for(int i = 0; i< lines; i++){
    for(int k = 0; k< columns; k++){
      matrix_result[i][k] = 0;
    }
  }
  fgets(matrix_line, 128, file1);
  while(fgets(matrix_line, 128, file1) != NULL){
    char *token2 = strtok(matrix_line, " ");
    while(token2 != NULL){
      matrix1[index_lin][index_col] = atoi(token2);
      index_col++;
      token2 = strtok(NULL, " ");
    }
    index_lin++;
    index_col = 0;
  }

  fclose(file1);

  index_lin = 0;
  index_col = 0;

  fgets(matrix_line, 128, file2);
  while(fgets(matrix_line, 128, file2) != NULL){
    char *token2 = strtok(matrix_line, " ");
    while(token2 != NULL){
      matrix2[index_lin][index_col] = atoi(token2);
      index_col++;
      token2 = strtok(NULL, " ");
    }
    index_lin++;
    index_col = 0;
  }


  fclose(file2);

  for(int i = 0; i < columns; i++){
    id_t pid = fork();
    if(pid < 0){
      printf("Error forking\n");
      return 1;
    }
    if(pid == 0){
      for(int k = 0; k< columns; k++){
        if(i == k){ //se o numero do processo for igual ao numero da coluna
          for(int y = 0; y < lines; y++){
            matrix_result[y][k] = matrix1[y][k] + matrix2[y][k]; 
          }
          break;
        }
      }
      exit(EXIT_SUCCESS);
    }
  }

  for(int i = 0; i < columns; i++){
    if(waitpid(-1, NULL, 0) < 0){
      return 1;
    }
  }

  printf("%s \n", dimension_matrix2);
  for(int i = 0; i< lines; i++){
    for(int j = 0; j< columns; j++){
      if(j+1 == columns){
        printf("%i\n", matrix_result[i][j]);
      }else{
        printf("%i ", matrix_result[i][j]);
      }
    }
  }
    return 0;
}
