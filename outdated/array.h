#include <stdint.h>
#include <stdlib.h>

typedef struct {
  int len;
  int capacity;
  uint8_t* start;
} Array;


void* allocate(uint8_t* ptr, size_t old, size_t new);
void grow(Array* a);
void instantiate(Array* a);
void append(Array* a, uint8_t instruction);
void print1(Array* a);
void freeArray(Array *a);

