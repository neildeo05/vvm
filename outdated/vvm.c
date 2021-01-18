#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#define PROGRAM_SIZE 1024
#define STACK_SIZE 1024
#define Word uint8_t
#define Value int64_t


typedef enum {
			  OP_RETURN,
			  OP_CONST,
			  OP_ADD,
			  OP_SUB,
			  OP_JMP,
			  OP_BREAK,
} OP_CODE;

typedef struct {
  Word instruction;
  Value operand;
} Instruction;

Instruction program[PROGRAM_SIZE];
Value stack[STACK_SIZE];
size_t program_size = 0;
size_t stackp = 0;
bool halt = false;


Value pop() {
  stackp--;
  Value val = stack[stackp];
  return val;
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

void pushInstruction(Instruction inst) {
  program[program_size] = inst;
  program_size++;
}

Instruction instruction(Word inst, Value op) {
  Instruction i = {.instruction = inst, .operand = op};
  return i;
}
void printInstruction(Instruction *i) {
  printf("%d, %lld\n", i->instruction, i->operand);
}
void showProgram() {
  printf("[ | ");
  for(int i = 0; i < program_size; i++) {
	printInstruction(&program[i]);
	printf(" | ");
  }
  printf("]\n");
}

void executeProgram() {
  uint32_t ip = 0;
  int checker = 0;
  while (!halt && checker != 10) {
	printf("ip => %d\n", ip);
	switch(program[ip].instruction) {
	case OP_CONST:
	  printf("OP_const\n");
	  push(program[ip].operand);
	  ip++;
	  break;
	case OP_ADD:
	  printf("OP_add\n");
	  push(pop() + pop());
	  ip++;
	  break;
	case OP_SUB: {
	  printf("OP_sub\n");
	  Value a = pop();
	  Value b = pop();
	  push(b-a);
	  ip++;
	}
	  break;
	case OP_JMP: {
	  printf("OP_jmp\n");
	  if (program[ip].operand > program_size) ip += 1;
	  else ip = program[ip].operand;
	}
	  break;
	case OP_BREAK: {
	  printf("OP_break\n");
	  halt = true;
	}
	case OP_RETURN: {
	  printf("OP_ret\n");
	  printf("returned value => %lld\n", pop());
	  showStack();
	  halt = true;
	}
	}

	checker++;
  }
  if (checker == 10) printf("infinite loop");
  if (halt == true) printf("halt == true");
}
int main() {
  //TODO: Add support for true, false, and conditionals
  pushInstruction(instruction(OP_CONST, 3));        // 0
  pushInstruction(instruction(OP_CONST, 4));        // 1
  pushInstruction(instruction(OP_ADD, 0));			// 2
  pushInstruction(instruction(OP_CONST, 4));        // 3
  pushInstruction(instruction(OP_JMP, 7));			// 4
  pushInstruction(instruction(OP_CONST, 2));        // 5
  pushInstruction(instruction(OP_BREAK, 0));        // 6
  pushInstruction(instruction(OP_JMP, 6));          // 7
  pushInstruction(instruction(OP_RETURN, 0));       // 8
  executeProgram();
}

/*
 OP_RETURN,
 OP_CONST,
 OP_ADD,
 OP_SUB,
 OP_JMP,
 OP_BREAK,
*/
