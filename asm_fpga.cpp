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


typedef struct {
  char name[MAXSTRLEN];
  int addr;
} label_t;

label_t Label[MAXLABELS];

char str[MAXSTRLEN];
char token[MAXSTRLEN];
int str_index, token_index;

void ReportState()
{
	printf("Code : %d, Data : %d ", code_p, data_p);
}

void AddCommand(int cmd)
{
	Program[code_p] = cmd;
	code_p++;
}

void CompileJmp(int addr)
{
	AddCommand(JMP + (addr & 0xFFFFFFF));
}

void MakeStart()
{
	CompileJmp(code_p);
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

int main() {
	printf("Assembler.\n\r");

	AddCommand(NOP);
	MakeStart();
	ReportState();

	printf("\nParsing demo\n\r");
	str_index = 0;

	strcpy(str, " demo example");
	Parse();
	printf(token);
	printf("\n");
	Parse();
	printf(token);
	printf("\n");


	return 0;
}
