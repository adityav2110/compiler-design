#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_IDENTIFIER_LENGTH 64

const char *keywords[] = {"int", "float", "return", "if", "else", "while", "for", "do", "break", "continue", "void", "main"};
const char *delimiters = " ,;(){}[]";
const char *operators = "+-/%=";

int isKeyword(char *str) {
    for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isDelimiter(char ch) {
    return strchr(delimiters, ch) != NULL;
}

int isOperator(char ch) {
    return strchr(operators, ch) != NULL;
}

void removeComments(FILE *input, FILE *output) {
    char ch, next_ch;
    while ((ch = fgetc(input)) != EOF) {
        if (ch == '/') {
            next_ch = fgetc(input);
            if (next_ch == '/') {
                while ((ch = fgetc(input)) != EOF && ch != '\n');
            } else if (next_ch == '*') {
                while ((ch = fgetc(input)) != EOF) {
                    if (ch == '*' && (next_ch = fgetc(input)) == '/') {
                        break;
                    }
                }
            } else {
                fputc(ch, output);
                fputc(next_ch, output);
            }
        } else {
            fputc(ch, output);
        }
    }
}

void analyze(FILE *input) {
    char buffer[MAX_IDENTIFIER_LENGTH];
    char ch;
    int i;

    while ((ch = fgetc(input)) != EOF) {
        if (isspace(ch)) continue;
        if (isDelimiter(ch)) {
            printf("Delimiter: %c\n", ch);
        } else if (isOperator(ch)) {
            printf("Operator: %c\n", ch);
        } else if (isdigit(ch)) {
            buffer[0] = ch;
            i = 1;
            while (isdigit(ch = fgetc(input))) {
                buffer[i++] = ch;
            }
            buffer[i] = '\0';
            ungetc(ch, input);
            printf("Constant: %s\n", buffer);
        } else if (isalpha(ch) || ch == '_') {
            buffer[0] = ch;
            i = 1;
            while (isalnum(ch = fgetc(input)) || ch == '_') {
                if (i < MAX_IDENTIFIER_LENGTH - 1) {
                    buffer[i++] = ch;
                }
            }
            buffer[i] = '\0';
            ungetc(ch, input);
            if (isKeyword(buffer)) {
                printf("Keyword: %s\n", buffer);
            } else {
                printf("Identifier: %s\n", buffer);
            }
        }
    }
}

int main() {
    FILE *input = fopen("input.txt", "r");
    FILE *temp = tmpfile();

    if (!input || !temp) {
        printf("Error opening file.\n");
        return 1;
    }

    removeComments(input, temp);
    rewind(temp);

    analyze(temp);

    fclose(input);
    fclose(temp);
    return 0;
}
