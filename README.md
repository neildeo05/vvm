# Mini Virtual Machine in C


Running programs in C:

```c
Chunk c;
instantiateChunk(&c, "test");
addInstruction(&c, &NEW_INST(OP_CONST, 10));
addInstruction(&c, &NEW_INST(OP_CONST, 10));
addInstruction(&c, &NEW_INST(OP_ADD, 0));
pushChunk(&c);
executeProgram();
```
Running programs by writing neilasm code:
```asm
main:
  push 10
  push 11
  add
  jmp 6
  push 40
  br
  push 20
  push 30
  jmp 4
  ret
```


# Definitions
 - Chunk: a section of bytecode that is separate from the other parts of the program
 - Call: an instruction that calls another chunk from one chunk
 - Jump: an instruction that jumps to another position in the same chunk
 - CMP: a flag that is set whenever a comparison is made (EQ, GE, G, LE, L)
 - JMP_IF: an instruction that jumps to another position in the same chunk if the CMP flag is set
 - HALT: an instruction that stops the execution of the virtual machine. In neilasm, the corresponding instruction is `br`

### NOTICE: This project is under active development. Use at your own risk.
