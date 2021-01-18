#include "array.h"
#include <stdio.h>
void* allocate(uint8_t* ptr, size_t old, size_t new) {
  old = old * sizeof(uint8_t);
  new = new * sizeof(uint8_t);
  void* result = realloc(ptr, new);
  if (result == NULL) exit(1);
  return result;
}
void grow(Array* a) {
  if (a->capacity < 8) {
	a->capacity = 8;
	a->start = (uint8_t*) allocate(a->start, a->capacity, 8);
  }
  else {
	a->capacity *= 2;
	a->start = (uint8_t*) allocate(a->start, a->capacity, a->capacity * 2);
  }
}
void instantiate(Array* a) {
  a->len = 0;
  a->capacity = 1;
  a->start = NULL;
}

void append(Array* a, uint8_t instruction) {
  if (a->len + 1 >= a->capacity) {
	grow(a);
  }
  a->start[a->len] = instruction;
  a->len++;
}
void print1(Array* a) {
  printf("This array has length, %d and capacity %d\n", a->len, a->capacity);
  printf("[");
  for(int i = 0; i < a->len; i++) {
	printf(" %d ", a->start[i]);
  }
  printf("]\n");
}

void freeArray(Array *a) {
  a->len = 0;
  a->capacity = 0;
  free(a->start);

}



