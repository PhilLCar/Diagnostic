#include <stdio.h>
#include <stdlib.h>
#include <diagnostic.h>

int main(void) {
  printf("Diagnostic test!\n");

  CHECK_MEMORY

  int *test = malloc(1 * sizeof(int));

  STOP_WATCHING

  *test = 0;

  return *test;
}