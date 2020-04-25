#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdnoreturn.h>

noreturn void usage(const char *prog) {
    fprintf(stderr, 
        "Usage: %s file_name\n"
        "Process the 136M Keystroke dataset.\n", prog);
    exit(EXIT_FAILURE);
}

noreturn void die(const char *prompt) {
    perror(prompt);
    exit(EXIT_FAILURE);
}

#define MAXBUF 1048576
FILE* fd_input;
char line_buf[MAXBUF], *line_ptr;

int input_line() {
    if (fgets(line_buf, sizeof line_buf, fd_input)) {
        int len = strlen(line_buf);
        if (line_buf[len - 1] == '\n') line_buf[len - 1] = '\0';
        line_ptr = line_buf;
        return len;
    } else {
        return 0;
    }
}

char* next_token() {
    if (line_ptr == NULL) return NULL;
    char *next = strchr(line_ptr, '\t');
    char *ret = line_ptr;
    if (next) *next = '\0';
    line_ptr = next + 1;
    return ret;
}

char sentence[MAXBUF];
int bksp;
char buf[MAXBUF], *ptr = buf;

void commit(int cid) {
    *ptr = '\0';
    printf("%d\t%s\t%s\n", cid, sentence, buf);
    ptr = buf;
    bksp = 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) usage(argv[0]);
    fd_input = fopen(argv[1], "r");
    if (!fd_input) die("fopen");
    input_line();
    int cid = 0;
    while (input_line()) {
        line_ptr = line_buf;

        next_token(); // uid
        int next_cid = atoi(next_token()); // sid
        if (next_cid != cid) {
            if (cid) commit(cid);
            cid = next_cid;
        }
        strcpy(sentence, next_token()); // sentence
        next_token();   // user input

        next_token(); // keystroke id
        next_token(); // press time (ms)
        next_token(); // release time (ms)
        char *letter = next_token();
        if (strlen(letter) == 0) {
            continue;
        } else if (strlen(letter) > 1) {
            if (strcmp(letter, "BKSP") == 0) {
                bksp++;
            } else if (strcmp(letter, "SHIFT") == 0) {
            } else if (strcmp(letter, "CAPS_LOCK") == 0) {
            } else if (strcmp(letter, "CTRL") == 0) {
            } else if (strcmp(letter, "ALT") == 0) {
            } else {
                fprintf(stderr, "Illegal keystroke '%s'\n", letter);
                exit(EXIT_FAILURE);
            }
        } else if (isprint(letter[0])) {
            if (bksp) {
                bksp--;
            } else {
                *ptr++ = letter[0];
            }
        } else {
            fprintf(stderr, "Mysterious character '%d'\n", (unsigned char)(letter[0]));
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
