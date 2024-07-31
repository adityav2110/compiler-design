#include <stdio.h>

#define MAX_LINE_LENGTH 1024

int isCommentLine(char *line, int *inMultiLineComment) {
    // Check for single-line comment
    if (line[0] == '/' && line[1] == '/') {
        return 1;
    }

    // Check for multi-line comment start
    if (line[0] == '/' && line[1] == '*') {
        *inMultiLineComment = 1;
        return 1;
    }

    // Check if we are currently inside a multi-line comment
    if (*inMultiLineComment) {
        // Check for multi-line comment end
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '*' && line[i + 1] == '/') {
                *inMultiLineComment = 0;
                return 1;
            }
        }
        return 1;
    }

    // Check for inline multi-line comment
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == '/' && line[i + 1] == '*') {
            *inMultiLineComment = 1;
            return 1;
        }
    }

    return 0;
}

void removeTrailingNewline(char *line) {
    int i = 0;
    while (line[i] != '\0') {
        if (line[i] == '\n') {
            line[i] = '\0';
            break;
        }
        i++;
    }
}

void readLine(FILE *file, char *line) {
    int i = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF && ch != '\n') {
        if (i < MAX_LINE_LENGTH - 1) {
            line[i++] = ch;
        }
    }
    line[i] = '\0';
}

void checkCommentsInFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int lineNumber = 1;
    int inMultiLineComment = 0;

    while (!feof(file)) {
        readLine(file, line);
        removeTrailingNewline(line);

        if (line[0] == '\0') {
            continue;  // Skip empty lines
        }

        if (isCommentLine(line, &inMultiLineComment)) {
            printf("Line %d is a comment: %s\n", lineNumber, line);
        } else {
            printf("Line %d is not a comment: %s\n", lineNumber, line);
        }
        lineNumber++;
    }

    fclose(file);
}

int main() {
    checkCommentsInFile("input1.txt");
    return 0;
}
