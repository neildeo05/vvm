#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#define PROGRAM_SIZE 1024
#define CHUNK_SIZE 1024
#define STACK_SIZE 1024
#define Word uint8_t
#define Value int64_t
#define NEW_INST(inst, op) ((Instruction) {.instruction = inst, .operand = op})
typedef enum {
			  OP_RETURN,
			  OP_CONST,
			  OP_NEGATE,
			  OP_ADD,
			  OP_SUB,
			  OP_MULT,
			  OP_DIV,
			  OP_JMP,
			  OP_JMP_IF,
			  OP_EQ,
			  OP_G,
			  OP_GE,
			  OP_L,
			  OP_LE,
			  OP_NE,
			  OP_NOT,
			  OP_BREAK,
			  OP_SWP,
			  OP_DUP,
			  OP_CALL,
	} OP_CODE;

typedef struct {
  Word instruction;
  Value operand;
} Instruction;

typedef struct {
  Instruction insts[CHUNK_SIZE];
  const char* name;
  size_t chunk_size;
  bool halt;
} Chunk;

Chunk* program[PROGRAM_SIZE];
Value stack[STACK_SIZE];
size_t program_size = 0;
size_t stackp = 0;
bool CMP;
Value pop() {
  stackp--;
  Value val = stack[stackp];
  return val;
}
Value peek() {
  return stack[stackp - 1];
}
void push(Value item) {
  stack[stackp] = item;
  stackp++;
}

void showStack() {
  printf("[");
  for (int i = 0; i < stackp; i++) {
	printf(" %lld ", stack[i]);
  }
  printf("]\n");
}

void pushChunk(Chunk* c) {
  program[program_size] = c;
  program_size++;
}

void printInstruction(Instruction *i) {
  printf("%d, %lld\n", i->instruction, i->operand);
}

void printChunk(Chunk* chunk) {
  printf("%s => [ | ", chunk->name);
  for(int i = 0; i < chunk->chunk_size; i++) {
	printf("%d, %lld", chunk->insts[i].instruction, chunk->insts[i].operand);
	printf(" | ");
  }
  printf("]\n");
}
void instantiateChunk(Chunk* chunk, char* name) {
  chunk->chunk_size = 0;
  chunk->name = name;
  chunk->halt = false;
}
void addInstruction(Chunk* chunk, Instruction* inst) {
  chunk->insts[chunk->chunk_size] = *inst;
  chunk->chunk_size++;

}
void dumpProgram() {
  printf("[\n");
  for(int i = 0; i < program_size; i++) {
	printChunk(program[i]);
  }
  printf("]\n");
}

Value executeChunk(Chunk* chunk) {
  int ip = 0;
  int bounds_checker = 0;
  while(!chunk->halt && bounds_checker != 20) {
	switch(chunk->insts[ip].instruction) {
	case OP_CONST:
	  push(chunk->insts[ip].operand);
	  ip+=1;
	  showStack();
	  break;
	case OP_ADD:
	  push(pop() + pop());
	  ip+=1;
	  showStack();
	  break;
	case OP_SUB: {
	  int a = pop();
	  int b = pop();
	  push(b - a);
	  ip+=1;
	  showStack();
	}
	  break;
	case OP_MULT:
	  push(pop() * pop());
	  ip+=1;
	  showStack();
	  break;
	case OP_DIV: {
	  int a = pop();
	  int b = pop();
	  if (a == 0) a = 1;
	  push(b/a);
	  ip+=1;
	  showStack();
	}
	  break;
	case OP_NEGATE: {
	  push(-1 * pop());
	  ip+=1;
	}
	  break;
	case OP_JMP: {
	  if(chunk->insts[ip].operand > chunk->chunk_size) {
		ip +=1;
	  }
	  ip = chunk->insts[ip].operand;
	}
	  break;
	case OP_JMP_IF: {
	  if(CMP == 1) {
		ip = chunk->insts[ip].operand;
	  }
	  else ip+=1;
	}
	  break;
	case OP_EQ: {
	  Value a = peek();
	  CMP = chunk->insts[ip].operand == a;
	  ip += 1;
	}
	  break;
	case OP_NE: {
	  CMP = chunk->insts[ip].operand != peek();
	  ip+=1;
	}
	  break;
	case OP_G: {
	  Value a = peek();
	  CMP = chunk->insts[ip].operand > a;
	  ip+=1;
	}
	  break;
	case OP_GE: {
	  Value a = peek();
	  CMP = chunk->insts[ip].operand >= a;
	  ip+=1;
	}
	  break;
	case OP_L: {
	  Value a = peek();
	  CMP = chunk->insts[ip].operand < a;
	  ip+=1;
	}
	  break;
	case OP_LE: {
	  Value a = peek();
	  CMP = chunk->insts[ip].operand <= a;
	  printf("CMP => %d\n", CMP);
	  ip+=1;
	}
	  break;
	case OP_NOT: {
	  push(!pop());
	  ip+=1;
	}
	  break;
	case OP_CALL: {
	  printf("CALLING:\n");
	  program[chunk->insts[ip].operand]->halt = false;
	  printf("called returned value = %lld\n", executeChunk(program[chunk->insts[ip].operand]));
	  ip+=1;
	}
	  break;
	case OP_SWP: {
	  Value a = stackp - 1;
	  Value b = stackp - 1 - chunk->insts[ip].operand;
	  Value tmp = stack[a];
	  stack[a] = stack[b];
	  stack[b] = tmp;
	  showStack();
	  ip+=1;
	}
	  break;
	case OP_DUP: {
	  push(stack[stackp - 1 - chunk->insts[ip].operand]);
	  ip+=1;
	}
	  break;
	case OP_BREAK: {
	  chunk->halt = true;
	  ip+=1;
	}
	  break;
	case OP_RETURN: {
	  showStack();
	  chunk->halt = true;
	  ip+=1;
	}
	  break;
	}
	bounds_checker++;
  }
  if (chunk->halt != true) {
	printf("no return value");
	return -1;
  }
  return pop();
}
void executeProgram() {
  for(int i = 0; i < program_size; i++) {
	printf("%lld\n", executeChunk(program[i]));
	printf("------------------------------\n");
  }
}

