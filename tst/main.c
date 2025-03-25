#include <stdio.h>
#include <stdlib.h>

#define MEMORY_WATCH
#include <diagnostic.h>

int main(void) {
  printf("Diagnostic test!\n");

  CHECK_MEMORY

  void *test = malloc(1 * sizeof(int));

  STOP_WATCHING

  return 0;
}