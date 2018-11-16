#include "main.h"

void fatal(const char *fmt, ...){
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
	exit(1);
}

typedef enum TokenKind {
	TOKEN_LAST_CHAR = 127,
	TOKEN_INT,
	TOKEN_NAME,
} TokenKind;

typedef struct Token {
	TokenKind kind;
} Token;

char *stream;

int main(int argc,char *argv[]){
	if(argc < 2)
		fatal("Usage: cepter <file.ct>");
    FILE *f = fopen(argv[1], "rb");
	if(f == NULL)
		fatal("Cannot open file '%s'", argv[1]);
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	stream = malloc(fsize + 1);
	fread(stream, fsize, 1, f);
	fclose(f);
}