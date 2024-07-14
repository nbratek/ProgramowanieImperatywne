#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = {0};

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp(const void *a, const void *b) {
    int va = *(int *) a;
    int vb = *(int *) b;
    if (count[va] == count[vb]) return va - vb;
    return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di(const void *a, const void *b) {
    int va = *(int *) a;
    int vb = *(int *) b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    int state = 0;
    char c;

    *nl = *nw = *nc = 0;

    while ((c = getchar()) != EOF) {
        (*nc)++;

        if (c == NEWLINE) {
            (*nl)++;
        }

        if (c == ' ' || c == '\t' || c == NEWLINE) {
            state &= ~IN_WORD;
        } else if (!(state & IN_WORD)) {
            state |= IN_WORD;
            (*nw)++;
        }
    }
}

void char_count(int char_no, int *n_char, int *cnt) {
    int index[MAX_CHARS];
    char c;

    for (int i = 0; i < MAX_CHARS; i++) {
        count[i] = 0;
        index[i] = i;
    }

    while ((c = getchar()) != EOF) {
        if (c >= FIRST_CHAR && c < LAST_CHAR) {
            count[c - FIRST_CHAR]++;
        }
    }

    qsort(index, MAX_CHARS, sizeof(int), cmp);

    *n_char = index[char_no - 1] + FIRST_CHAR;
    *cnt = count[index[char_no - 1]];
}

void bigram_count(int bigram_no, int bigram[]) {
    int index[MAX_BIGRAMS];
    char c, prev_c = 0;

    for (int i = 0; i < MAX_BIGRAMS; i++) {
        count[i] = 0;
        index[i] = i;
    }

    while ((c = getchar()) != EOF) {
        if (prev_c >= FIRST_CHAR && prev_c < LAST_CHAR && c >= FIRST_CHAR && c < LAST_CHAR) {
            count[(prev_c - FIRST_CHAR) * MAX_CHARS + (c - FIRST_CHAR)]++;
        }
        prev_c = c;
    }

    qsort(index, MAX_BIGRAMS, sizeof(int), cmp_di);

    bigram[0] = index[bigram_no - 1] / MAX_CHARS + FIRST_CHAR;
    bigram[1] = index[bigram_no - 1] % MAX_CHARS + FIRST_CHAR;
    bigram[2] = count[index[bigram_no - 1]];
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    int c, prev_c = 0;
    int state = 0;

    *line_comment_counter = 0;
    *block_comment_counter = 0;
    int in_line_comment = 0;
    int in_block_comment = 0;
    prev_c = getchar();

    while((c = getchar()) != EOF) {
        if (in_line_comment) {
            if (c == '\n') {
                in_line_comment = 0;
            }
        } else if (in_block_comment) {
            if (prev_c == '*' && c == '/') {
                in_block_comment = 0;
            }
        } else if (prev_c == '/' && c == '*') {
            (*block_comment_counter)++;
            in_block_comment = 1;
            prev_c = c;
            c = getchar();
        } else if (prev_c == '/' && c == '/'){
            (*line_comment_counter)++;
            in_line_comment = 1;
            prev_c = c;
            c = getchar();
        }
        prev_c = c;
    }
}

#define MAX_LINE 128

int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int) strtol(line, NULL, 10);
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int bigram[3];

    to_do = read_int();
    switch (to_do) {
        case 1: // wc()
            wc(&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_int();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // bigram_count()
            char_no = read_int();
            bigram_count(char_no, bigram);
            printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

