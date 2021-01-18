#include "virtual.c"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define NEW_STRING(data) ((String) {.val = data, .size = strlen(data)})
#define NEW_SIZED_STRING(data, buf) ((String) {.val = data, .size = buf})
typedef struct {
  char* val;
  size_t size;
} String;

void string_ltrim(String *a) {
  int buf = 0;
  while (!isalnum(*a->val)) {
	a->val += 1;
	a->size -= 1;
	buf+=1;
	if (buf >= a->size) {
	  fprintf(stderr, "string provided was just whitespace");
	  break;
	}
  }
}
void string_rtrim(String *a) {
  int buf = 0;
  while(buf < a->size && isspace(a->val[a->size - buf - 1])) {
	buf += 1;
  }
  a->size = a->size - buf;
}
void trim(String* a) {
  string_ltrim(a);
  string_rtrim(a);
}

String take_until_value(String* a, char delim) {
  size_t buf = 0;
  while(a->val[buf] != delim && buf < a->size) {
	buf += 1;
  }
  String ret = NEW_SIZED_STRING(a->val, buf);
  if (a->size <= buf) {
	a->size -= buf;
	a->val += buf;
  }
  else {
	a->size -= buf + 1;
	a->val += buf + 1;
  }
  return ret;
}
String peek_until_value(String *a, char delim) {
  size_t buf = 0;
  while(a->val[buf] != delim && buf < a->size) {
	buf += 1;
  }
  String ret = NEW_SIZED_STRING(a->val, buf);
  return ret;
}
void print_string(String* s) {
  for(int i = 0; i < s->size; i++) {
	printf("%c", s->val[i]);
  }
}
int string_eq(String* a, String* b) {
  if (a->size != b->size) return 0;
  for(int i = 0; i < a->size; i++) {
	if(a->val[i] != b->val[i]) return 0;
  }
  return 1;
}
int string_as_num(String* a) {
  char buffer[a->size + 1];
  for(int i = 0; i < a->size; i++) {
	buffer[i] = a->val[i];
  }
  return atoi(buffer);
}

Chunk parse_chunk(String* s, String* funcDef) {
  Chunk c;
  instantiateChunk(&c, funcDef->val);
  String inst = take_until_value(s, '\n');
  trim(&inst);
  int buf_set = 0;
  while(!string_eq(&inst, &NEW_STRING("ret"))) {
	String instruction = take_until_value(&inst, ' ');
	trim(&instruction);
	if(string_eq(&instruction, &NEW_STRING("push"))) {
	  addInstruction(&c, &NEW_INST(OP_CONST, string_as_num(&inst)));
	}
	if(string_eq(&instruction, &NEW_STRING("add"))) {
	  addInstruction(&c, &NEW_INST(OP_ADD,0));
	}
	if(string_eq(&instruction, &NEW_STRING("sub"))) {
	  addInstruction(&c, &NEW_INST(OP_SUB,0));
	}
	if(string_eq(&instruction, &NEW_STRING("mult"))) {
	  addInstruction(&c, &NEW_INST(OP_MULT,0));
	}
	if(string_eq(&instruction, &NEW_STRING("div"))) {
	  addInstruction(&c, &NEW_INST(OP_DIV,0));
	}
	if(string_eq(&instruction, &NEW_STRING("jmp"))) {
	  addInstruction(&c, &NEW_INST(OP_JMP,string_as_num(&inst)));
	}
	if(string_eq(&instruction, &NEW_STRING("br"))) {
	  addInstruction(&c, &NEW_INST(OP_BREAK,0));
	}
	inst = take_until_value(s, '\n');
	trim(&inst);
	if (buf_set == 10) break;
	buf_set += 1;
  }
  addInstruction(&c, &NEW_INST(OP_RETURN, 0));
  printChunk(&c);
  pushChunk(&c);
  return c;
}


void prog(String* st) {
  int chunk_nums = 2;
	String funcDefVal = peek_until_value(st, ':');
	char funcDefCharArr[funcDefVal.size + 1];
	for(int i = 0; i < funcDefVal.size; i++) {
	  funcDefCharArr[i] = funcDefVal.val[i];
	}
	parse_chunk(st, &NEW_STRING(funcDefCharArr));

	funcDefVal = peek_until_value(st, ':');
	for(int i = 0; i < funcDefVal.size; i++) {
	  funcDefCharArr[i] = funcDefVal.val[i];
	}
	parse_chunk(st, &NEW_STRING(funcDefCharArr));

  dumpProgram();
  executeProgram();
}

int main() {
  FILE *f;
  f = fopen("test.neilasm", "r");
  if (f == NULL) {
	fprintf(stderr, "cannot read file");
	exit(1);
  }
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);
  char a[size];
  fread(a, 1, size, f);
  String st = NEW_STRING(a);
  trim(&st);


  /* String funcDefVal = peek_until_value(&st, ':'); */
  /* char funcDefCharArr[funcDefVal.size + 1]; */
  /* for(int i = 0; i < funcDefVal.size; i++) { */
  /*	funcDefCharArr[i] = funcDefVal.val[i]; */
  /* } */
  /* parse_chunk(&st, &NEW_STRING(funcDefCharArr)); */

  /* funcDefVal = peek_until_value(&st, ':'); */
  /* for(int i = 0; i < funcDefVal.size; i++) { */
  /*	funcDefCharArr[i] = funcDefVal.val[i]; */
  /* } */
  /* parse_chunk(&st, &NEW_STRING(funcDefCharArr)); */
  /* dumpProgram(); */
  /* executeProgram(); */
  prog(&st);

}
