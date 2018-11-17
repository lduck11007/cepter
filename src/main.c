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
	TOKEN_INT = 128,		// ascii if tokenkind < 128
	TOKEN_NAME,
	TOKEN_UNARY,
	TOKEN_BINEXP,
	TOKEN_EOF,
} TokenKind;

typedef struct Token {
	TokenKind kind;
	const char *start;
	const char *end;
	union {
		uint64_t val;
		const char *name;
	};
} Token;

typedef struct InternStr {
    size_t len;
    const char *str;
} InternStr;

//globals
Token token;
char *stream;
InternStr *interns;

const char *str_intern_range(const char *start, const char *end){
	size_t len = end - start;
	for(size_t i = 0; i < buf_len(interns); i++){
		if(interns[i].len == len && strncmp(interns[i].str, start, len) == 0){
			return interns[i].str;
		}
	}
	char *str = malloc(len + 1);
	memcpy(str, start, len);
	str[len] = 0; //null byte
	buf_push(interns, ((InternStr){len, str}));
	return str;
}

const char *str_intern(const char *str) {		//only works with null-byte terminated strings
	return str_intern_range(str, str + strlen(str));
}

void next_token(){
	if(*stream == 0){
		token.kind = TOKEN_EOF;
		return;
	}
    token.start = stream;
    switch (*stream) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
        uint64_t val = 0;
            while(isdigit(*stream)){
                val *= 10;
                val += *stream++ - '0';
            }
            token.kind = TOKEN_INT;
            token.val = val;
            break;
        }
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z': 
        case '_': {
            const char *start = stream++;
            while(isalnum(*stream) || *stream == '_'){
                stream++;
            }
            token.kind = TOKEN_NAME;
            token.name = str_intern_range(token.start, stream);
            break;
        }
		case '+':
		case '-': { 
				if(*(stream+1) == '='){
					const char *start = stream++;
					token.kind = TOKEN_BINEXP;
					token.name = str_intern_range(token.start, ++stream);
				} else {
					token.kind = *stream++;
					break;
				}
			break;
		}

        default:
            token.kind = *stream++;
            break;
    }
    token.end = stream;
}

void print_token(Token token){
    printf("%d TOKEN: ", token.kind);
    if (token.kind < 128) {
        printf("'%c'\n", token.kind);
    } else {
    switch (token.kind) {
        case TOKEN_INT:
            printf("TOKEN INT: %lu\n", token.val);
            break;
        case TOKEN_NAME:
            printf("TOKEN NAME: %.*s\n", (int)(token.end - token.start), token.start);
            break;
		case TOKEN_BINEXP:
            printf("TOKEN BINEXP: %.*s\n", (int)(token.end - token.start), token.start);
            break;
		case TOKEN_EOF:
			printf("END OF FILE\n");
			break;
        default:
            printf("OTHER TOKEN '%c'\n", token.kind);
            break;
        }
    }
}

void parse_test(){
	next_token();
	
	while(token.kind != TOKEN_EOF){
		print_token(token);
		next_token();
	}
}

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
	*(stream+fsize) = 0;
	printf("%d\n", *(stream+fsize));
	parse_test();

}