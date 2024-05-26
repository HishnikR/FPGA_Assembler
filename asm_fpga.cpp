#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CODESIZE 16384
#define DATASIZE 4096

#define MAXLABELS 1024
#define MAXSTRLEN 256

int code_p, data_p;

int Program[CODESIZE];
int Data[DATASIZE];

#define NOP 0
#define JMP 0xF0000000
#define CALL 0xE0000000

#define OPALU 1

#define CMDADD 1
#define CMDSUB 2
#define CMDAND 3
#define CMDOR 4
#define CMDXOR 5

#define CMDLITL 9
#define CMDLITH 10

int optype, opcode, dest, srca, srcb, literal;

/*

[31:28] - optype
[27:24] - cmd

assign we_mem = cmd_reg[21];
assign aw_select = cmd_reg[19:16];
assign opa_select = cmd[7:4];
assign opb_select = cmd[3:0];

*/

#define OFF_OPTYPE 28
#define OFF_CMD 24
#define OFF_DEST 16
#define OFF_SRCA 4
#define OFF_SRCB 0

typedef struct {
  char name[MAXSTRLEN];
  int addr;
} label_t;

label_t Label[MAXLABELS];

char str[MAXSTRLEN];
char token[MAXSTRLEN];
size_t str_index, token_index;

void ReportState()
{
	printf("Code : %d, Data : %d ", code_p, data_p);
}

void Dump()
{
	printf("\nProgram:\n");
	for (int i = 0; i < code_p; i++) {
		printf("Addr = %d, cmd = %x \n", i, Program[i]);
	}
}

void CompileCommand(int cmd)
{
	Program[code_p] = cmd;
	code_p++;
}



bool IsBlank(char ch)
{
	return (ch < 33);
}

void Parse()
{
    for (int i = 0; i < MAXSTRLEN; i++)
    {
        token[i] = 0;
    }
	token_index = 0;
	while ((str_index < strlen(str)) && (IsBlank(str[str_index]))) {
		str_index++;
	}
	while ((str_index < strlen(str)) && (!IsBlank(str[str_index]))) {
		token[token_index] = str[str_index];
		str_index++;
		token_index++;
	}
}

void CompileJmp(int addr)
{
	CompileCommand(JMP + (addr & 0xFFFFFFF));
}

void CompileLit()
{
	Parse();
	dest = atoi(token);
	Parse();
	literal = atoi(token);
    CompileCommand(optype << OFF_OPTYPE | CMDLITL << OFF_CMD | dest << OFF_DEST | (literal & 0xFFFF));
    CompileCommand(optype << OFF_OPTYPE | CMDLITH << OFF_CMD | dest << OFF_DEST | ((literal >> 16) & 0xFFFF));

}

void Compile3Addr()
{
	Parse();
	dest = atoi(token);
	Parse();
	srca = atoi(token);
	Parse();
	srcb = atoi(token);
    CompileCommand(optype << OFF_OPTYPE | opcode << OFF_CMD | dest << OFF_DEST | srca << OFF_SRCA | srcb << OFF_SRCB);
}

void Compile2Addr()
{
	Parse();
	dest = atoi(token);
	Parse();
	srca = atoi(token);
    CompileCommand(optype << OFF_OPTYPE | opcode << OFF_CMD | dest << OFF_DEST | srca << OFF_SRCA);
}

void MakeStart()
{
	Program[0] = JMP + (code_p & 0xFFFFFFF);
}
void ParseLine()
{
	str_index = 0;
	Parse();

	if (strcmp(token, "LOAD") == 0)
	  {
		  optype = OPALU;
		  CompileLit();
	  }

	if (strcmp(token, "ADD") == 0)
	  {
		  optype = OPALU;
		  opcode = CMDADD;
		  Compile3Addr();
	  }

	if (strcmp(token, "SUB") == 0)
	  {
		  optype = OPALU;
		  opcode = CMDSUB;
		  Compile3Addr();
	  }

	if (strcmp(token, "AND") == 0)
	  {
		  optype = OPALU;
		  opcode = CMDAND;
		  Compile3Addr();
	  }

	if (strcmp(token, "OR") == 0)
	  {
		  optype = OPALU;
		  opcode = CMDOR;
		  Compile3Addr();
	  }

	if (strcmp(token, "XOR") == 0)
	  {
		  optype = OPALU;
		  opcode = CMDXOR;
		  Compile3Addr();
	  }
}

int main() {
	printf("Assembler.\n\r");

	code_p = 1; // reserve place for JMP

	strcpy(str, "LOAD 2 85");
	ParseLine();
	strcpy(str, "LOAD 3 15");
	ParseLine();
	strcpy(str, "ADD 1 2 3");
	ParseLine();
	strcpy(str, "SUB 3 4 5");
	ParseLine();

	MakeStart();
	ReportState();
	Dump();


	return 0;
}
