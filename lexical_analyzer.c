#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define DEC_DIGITS "0123456789"
#define HEX_DIGITS "0123456789abcdefABCDEF"
#define BIN_DIGITS "01"


typedef struct token {
    char * type;
    int columnNo;
    int rowNo;
    char * lexeme;
}tokenStruct;

int isBracket(const char * str) {
    if (strlen(str) == 1) {
        if (str[0] == '(' || str[0] == ')' || str[0] == '[' || str[0] == ']' || str[0] == '{' || str[0] == '}') {
            return 1;
        }
    }
    return 0;
}

int isDecimal(const char * str) {
    int i = 0;
    if (str[i] == '+' || str[i] == '-') {
        i++;
        if (str[i] == '\0') {
            return 0;
        }
    }
    while (str[i] >= '0' && str[i] <= '9') {
        i++;
    }
    if (str[i] == '\0') {
        return 1;
    }
    return 0;
}

int isHexaDecimal(const char * str) {
    int i = 0;
    if (strncmp(str, "0x", 2) != 0) {
        return 0;
    }
    i += 2;
    int has_hex_digits = 0;
    while (str[i] != '\0' && strchr(HEX_DIGITS, str[i]) != NULL) {
        has_hex_digits = 1;
        i++;
    }
    if (!has_hex_digits || str[i] != '\0') {
        return 0;
    }
    return 1;
}
int isBinary(const char * str) {
    int i = 0;
    if (strncmp(str, "0b", 2) != 0) {
        return 0;
    }
    i += 2;
    int has_bin_digits = 0;
    while (str[i] != '\0' && strchr(BIN_DIGITS, str[i]) != NULL) {
        has_bin_digits = 1;
        i++;
    }
    if (!has_bin_digits || str[i] != '\0') {
        return 0;
    }
    return 1;
}

int isFloatingExp1(const char * str) {
    int i = 0;
    int hasDot = 0;
    int hasE = 0;
    int hasNumberAfterE = 0;

    if (str[i] == '-' || str[i] == '+') {
        i++;
        if (str[i] == '\0') {
            return 0;
        }
    }

    while (str[i] != '\0') {
        if (str[i] == '.') {
            if (hasDot || hasE || (str[i + 1] == '\0' || strchr(DEC_DIGITS, str[i + 1]) == NULL)) {
                return 0;
            }
            hasDot = 1;
        } else if (str[i] == 'e' || str[i] == 'E') {
            if (hasE || i == 0 || str[i + 1] == '\0' || (str[i + 1] != '+' && str[i + 1] != '-' && strchr(DEC_DIGITS, str[i + 1]) == NULL)) {
                return 0;
            }
            hasE = 1;
            if (str[i + 1] == '-' || str[i + 1] == '+') {
                i++;
            }
            if (str[i + 1] == '\0') {
                return 0;
            }
        } else if (strchr(DEC_DIGITS, str[i]) == NULL) {
            return 0;
        } else if (hasE) {
            hasNumberAfterE = 1;
        }

        i++;
    }

    return (hasNumberAfterE || !hasE);
}

int isFloatingExp2(const char * str) {
    int i = 0;
    if (str[i] == '+' || str[i] == '-') {
        i++;
    }
    int found_digit = 0;
    while (str[i] >= '0' && str[i] <= '9') {
        found_digit = 1;
        i++;
    }
    if (!found_digit) {
        return 0;
    }
    if (str[i] == 'e' || str[i] == 'E') {
        i++;
        if (str[i] == '+' || str[i] == '-') {
            i++;
        }
        found_digit = 0;
        while (str[i] >= '0' && str[i] <= '9') {
            found_digit = 1;
            i++;
        }
        if (!found_digit || str[i] != '\0') {
            return 0;
        }
        return 1;
    }
    if (str[i] != '\0') {
        return 0;
    }
    return 1;
}
int isBoolean(const char * str) {
    if (strcmp(str, "true") == 0 || strcmp(str, "false") == 0)
        return 1;
    else
        return 0;
}
int isChar(const char * str) {
    if (str[0] != '\'')
        return 0;
    if (str[1] == '\\' && str[2] == '\'' && str[3] == '\'')
        return 1;
    if (str[1] > 31 && str[1] < 127 && str[2] == '\'')
        return 1;
    return 0;
}
int isString(char * str) {
    int i;
    if (str[0] != '"' || str[strlen(str) - 1] != '"') {
        return 0;
    }

    for (i = 1; i < strlen(str) - 1; i++) {
        if (str[i] == '\\') {
            if (str[i + 1] != '"' && str[i + 1] != '\\' && str[i + 1] != 'n' && str[i + 1] != 'r' && str[i + 1] != 't') {
                return 0;
            }
            i++;
        } else if (str[i] < ' ' || str[i] > '~') {
            return 0;
        }
    }
    return 1;
}
int isKeyword(char * str) {
    if ((strcmp(str, "define") == 0) || (strcmp(str, "let") == 0) || (strcmp(str, "cond") == 0) ||
        (strcmp(str, "if") == 0) || (strcmp(str, "begin") == 0))
        return 1;

    return 0;
}
int isIdentifier(char * str) {
    if ((strcmp(str, "define") == 0) || (strcmp(str, "let") == 0) || (strcmp(str, "cond") == 0) ||
        (strcmp(str, "if") == 0) || (strcmp(str, "begin") == 0))
        return 0;
    if(str[0] == '+' || str[0] == '-' || str[0] == '.'){
        if(str[1] == '\0')
            return 1;
        return 0;
    }
    int i;
    int len = strlen(str);
    if (len == 0) {
        return 0;
    }
    if (!islower(str[0]) && str[0] != '!' && str[0] != '*' && str[0] != '/' && str[0] != ':' &&
        str[0] != '<' && str[0] != '=' && str[0] != '>' && str[0] != '?') {
        return 0;
    }
    for (i = 1; i < len; i++) {
        if (!islower(str[i]) && !isdigit(str[i]) && str[i] != '.' && str[i] != '+' && str[i] != '-') {
            return 0;
        }
    }
    return 1;
}
int isNumLiteral(char * str) {
    if (isDecimal(str) || isHexaDecimal(str) || isBinary(str) || isFloatingExp1(str) || isFloatingExp2(str))
        return 1;
    else
        return 0;
}
char * checkToken(char * str) {

    if (isBracket(str) == 1) {
        if (strcmp(str, "(") == 0)
            return "LEFTPAR";
        else if (strcmp(str, ")") == 0)
            return "RIGHTPAR";
        else if (strcmp(str, "[") == 0)
            return "LEFTSQUAREB";
        else if (strcmp(str, "]") == 0)
            return "RIGHTSQUAREB";
        else if (strcmp(str, "{") == 0)
            return "LEFTCURLYB";
        else if (strcmp(str, "}") == 0)
            return "RIGHTCURLYB";
    }
    if (isNumLiteral(str) == 1) {
        return "NUMBER";
    }
    if (isBoolean(str) == 1) {
        return "BOOLEAN";
    }
    if (isChar(str) == 1) {
        return "CHAR";
    }
    if (isString(str) == 1) {
        return "STRING";
    }
    if (isKeyword(str) == 1) {
        if (strcmp(str, "define") == 0)
            return "DEFINE";
        else if (strcmp(str, "let") == 0)
            return "LET";
        else if (strcmp(str, "cond") == 0)
            return "COND";
        else if (strcmp(str, "if") == 0)
            return "IF";
        else if (strcmp(str, "begin") == 0)
            return "BEGIN";
    }
    if (isIdentifier(str) == 1) {
        return "IDENTIFIER";
    }
    return "ERROR";
}

void addElement(tokenStruct ** array, int * size, tokenStruct newToken) {
    * array = (tokenStruct * ) realloc( * array, ( * size + 1) * sizeof(tokenStruct));
    ( * array)[ * size] = newToken;
    ( * size) ++;
}
void printTokens(tokenStruct * array,const int * size) {
    FILE *file;
    file = fopen("output.txt", "w");

    if (file == NULL) {
        printf("Error: could not open file output.txt\n");
        exit(0);
    }

    int i;
    for (i = 0; i < * size; i++) {
        if(strcmp(array[i].type,"ERROR") == 0)
        {
            printf("LEXICAL ERROR [%d:%d]: Invalid token `%s'", array[i].rowNo, array[i].columnNo, array[i].lexeme);
            fprintf(file,"LEXICAL ERROR [%d:%d]: Invalid token `%s'", array[i].rowNo, array[i].columnNo, array[i].lexeme);
            fclose(file);
            exit(0);
        }
        else
            {
            printf("%s %d:%d\n", array[i].type, array[i].rowNo, array[i].columnNo);
            fprintf(file, "%s %d:%d\n", array[i].type, array[i].rowNo, array[i].columnNo);
            }
    }
    fclose(file);
}
void addCharToToken(char **token, char c, int *token_len) {
    (*token_len)++;
    *token = realloc(*token, (*token_len + 1) * sizeof(char));
    (*token)[*token_len - 1] = c;
    (*token)[*token_len] = '\0';
}
void processFile(const char * filename) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return;
    }
    tokenStruct t;
    tokenStruct * tokenArray = NULL;
    int size = 0;
    int row = 1;
    int col = 0;

    while (1) {
        char c = (char) fgetc(fp);
        col++;
        while (c == '\n') {
            c = (char) fgetc(fp);
            row++;
            col = 1;
        }
        while (isspace(c)) {
            c = (char) fgetc(fp);
            col++;
        }
        if (c == '~') {
            while(c != '\n')
                c = (char) fgetc(fp);
            ungetc(c,fp);
            continue;
        }

        if (c == EOF) {
            break;
        }

        char *token = NULL;
        int token_len = 0;


        if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
            addCharToToken(&token,c,&token_len);
        } else if (c != '"') {
            while (!isspace(c) && c != EOF &&
                   !(c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}')
                    ) {
                addCharToToken(&token,c,&token_len);
                c = (char) fgetc(fp);
                col++;
            }
            ungetc(c, fp);
            col--;
        } else{
            addCharToToken(&token,c,&token_len);
            c = (char) fgetc(fp);
            col++;

            while (c != '\"' && c != EOF) {
                if (c == '\\') {
                    addCharToToken(&token,c,&token_len);
                    c = (char) fgetc(fp);
                    col++;

                    if (c != EOF) {
                        addCharToToken(&token,c,&token_len);
                    }
                } else {
                    addCharToToken(&token,c,&token_len);
                }

                c = (char) fgetc(fp);
                col++;
            }
            if (c == '\"') {
                addCharToToken(&token,c,&token_len);
            }
        }
        t.rowNo = row;
        t.columnNo = col - token_len + 1;
        t.type = (checkToken(token));
        t.lexeme = token;
        addElement(&tokenArray, &size, t);
        free(token);
    }
    printTokens(tokenArray, & size);
    free(tokenArray);
    fclose(fp);
}

int main() {
    char filename[100];

    printf("Enter the name of the input file:");
    scanf("%s", filename);
    processFile(filename);

    return 0;
}

