#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef double Value;
typedef enum {
			  OP_RETURN,
			  OP_PLUS,
			  OP_MINUS,
			  OP_MULT,
			  OP_DIV,
			  OP_CONSTANT,
			  OP_NEGATE,
} OpCodes;



typedef struct {
  size_t size;
  size_t capacity;
  Value* ptr;
} Const;

typedef struct {
  size_t size;
  size_t capacity;
  uint8_t* ptr;
} Block; //bytecode block
typedef struct {
  Block* block;
  Const* vals;
  size_t ip;
  Value stack[2048];
  size_t stackP;
} VM;

//Block instructions:
Block block() {
  Block b;
  b.size = 0;
  b.capacity = 0;
  b.ptr = NULL;
  return b;
}
void* allocate(Block* b, size_t newSize) {
  if (newSize == 0) {
	free(b->ptr);
	b->capacity = 0;
	b->size = 0;
	return NULL;
  }
  void* result = realloc(b->ptr, newSize);
  if (!result) {
	fprintf(stderr, "shit...");
	exit(1);
  }
  return result;
}
void grow(Block* b) {
  if (b->capacity < 8) {
	b->capacity = 8;
	b->ptr = allocate(b, 8);
  }
  else {
	b->capacity *= 2;
	b->ptr = allocate(b, b->capacity * 2);
  }
}
void appendInstruction(Block *b, uint8_t val) {
  if (b->size + 1 >= b->capacity) grow(b);
  b->ptr[b->size] = val;
  b->size++;
}
void freeBlock(Block *b) {
  allocate(b, 0);
}
void print1(Block *b) {
  printf("[");
  for (int i = 0; i < b->size; i++) {
	printf(" %d ", b->ptr[i]);
  }
  printf("]\n");
}







//Constant Expressions //////////////////////////////////////////////////////////////////////////////////////
Const construct() {
  Const b;
  b.size = 0;
  b.capacity = 0;
  b.ptr = NULL;
  return b;
}
void* allocateConst(Const* b, size_t newSize) {
  if (newSize == 0) {
	free(b->ptr);
	b->capacity = 0;
	b->size = 0;
	return NULL;
  }
  void* result = realloc(b->ptr, sizeof(Value) * newSize);
  if (!result) {
	fprintf(stderr, "shit...");
	exit(1);
  }
  return result;
}
void growConst(Const* b) {
  if (b->capacity < 8) {
	b->capacity = 8;
	b->ptr = allocateConst(b, 8);
  }
  else {
	b->capacity *= 2;
	b->ptr = allocateConst(b, b->capacity * 2);
  }
}
void appendConst(Const *b, Value val) {
  if (b->size + 1 >= b->capacity) growConst(b);
  b->ptr[b->size] = val;
  b->size++;
}
void freeConst(Const *b) {
  allocateConst(b, 0);
}
void printConst1(Const *b) {
  printf("[");
  for (int i = 0; i < b->size; i++) {
	printf(" %f ", b->ptr[i]);
  }
  printf("]\n");
}
void initVM(VM *vm, Block* block, Const* vals) {
  vm->block = block;
  vm->vals = vals;
}
void freeVM(VM *vm) {
  freeBlock(vm->block);
  freeConst(vm->vals);
}


void addConstant(VM *v, Value a, int offset) {
  appendConst(v->vals, a);
}
void pushStack(VM* v, Value item) {
  v->stack[v->stackP] = item;
  v->stackP++;
}
Value popStack(VM* v) {
  v->stackP--;
  return v->stack[v->stackP];
}
void showStack(VM* v) {
  printf("Stack = [");
  for (int i = 0; i < v->stackP; i++) {
	printf(" %f ", v->stack[i]);
  }
  printf("]\n");
}

void pushInst(VM* v, OpCodes a, Value operand) {
  if(a == OP_CONSTANT) {
	addConstant(v, operand, v->ip);
	appendInstruction(v->block, v->ip);
	print1(v->block);
	printConst1(v->vals);
	pushStack(v, v->vals->ptr[v->block->ptr[v->ip]]);
	showStack(v);
	v->ip+=1;
  }
  else if (a == OP_RETURN) {
	printf("Returned value => %f\n", popStack(v));
  }
  else if (a == OP_PLUS) {
	pushStack(v, (popStack(v) + popStack(v)));
	showStack(v);
  }
  else if (a == OP_MINUS) {
	Value a = (popStack(v));
	Value b = (popStack(v));
	pushStack(v, b - a);
	showStack(v);
  }
  else if (a == OP_MULT) {
	pushStack(v, popStack(v) * popStack(v));
	showStack(v);
  }
  else if (a == OP_DIV) {
	Value a = popStack(v);
	Value b = popStack(v);
	if (a == 0) exit(1);
	else {
	  pushStack(v, b / a);
	  showStack(v);
	}
  }
  else if (a == OP_NEGATE) {
	pushStack(v, -1 * popStack(v));
	showStack(v);
  }

}
void printVM(VM *v) {
  for (int i = 0; i < v->block->capacity; i++) {
	printf("%d\n", v->block->ptr[i]);
  }
  for (int i = 0; i < v->vals->capacity; i++) {
	printf("%f\n", v->vals->ptr[i]);
  }
}

int main() {
  Block b;
  b = block();
  Const c;
  c = construct();
  VM v;
  v.ip = 0;
  initVM(&v, &b, &c);
  //  (((((2+3) + 4) * 4) + 10) * -1)
  pushInst(&v, OP_CONSTANT, 2);
  pushInst(&v, OP_CONSTANT, 3);
  pushInst(&v, OP_PLUS, 255);
  pushInst(&v, OP_CONSTANT, 4);
  pushInst(&v, OP_PLUS, 255);
  pushInst(&v, OP_CONSTANT, 4);
  pushInst(&v, OP_MULT, 255);
  pushInst(&v, OP_NEGATE, 255);
  pushInst(&v, OP_CONSTANT, 5);
  pushInst(&v, OP_RETURN, 255);


  freeVM(&v);




}

