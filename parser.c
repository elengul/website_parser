#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

//#define MAX_ERROR 0x1000

//static int compile_regex

void
fgetline(FILE *in, char **line, int buf_size)
{
    char buffer[buf_size];
    int ctr = 0;
    for(;;) {
        buffer[ctr] = fgetc(in);
        if (buffer[ctr] == '\n') {
            printf("Line break at character %d\n", ctr);
            break;
        }
        if (ctr == (buf_size-1))
            break;
        ctr++;
    }
    *line = malloc(ctr * sizeof(**line));
    for (int i = 0; i < ctr; i++)
        (*line)[i] = buffer[i];
}

int main(int argc, char **argv)
{
    //regex_t regex;
    FILE *in = fopen("data/test.html", "r");
    char *line;
    int buf_size = 512;
    int num_lines = 10;
    if (argc > 1)
        buf_size = atoi(argv[1]);
    if (argc > 2)
        num_lines = atoi(argv[2]);
    for (int i = 0; i < num_lines; i++) {
        fgetline(in, &line, buf_size);
        printf("%s\n", line);
        free(line);
    }
    fclose(in);
    return 0;
}
