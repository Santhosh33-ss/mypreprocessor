#include "preprocessor.h"

void handle_comment(FILE *fp, FILE *out)
{
    int next = fgetc(fp);
    int ch;

    if (next == '/')
    {
        //for single-line comment 
        while ((ch = fgetc(fp)) != EOF && ch != '\n');
        if (ch == '\n')
            fputc('\n', out);
    }
    else if (next == '*')
    {
        //for multi-line comment 
        int prev = 0;
        while ((ch = fgetc(fp)) != EOF)
        {
            if (ch == '\n')
                fputc('\n', out);
            if (prev == '*' && ch == '/')
                break;
            prev = ch;
        }
    }
    else
    {
        fputc('/', out);
        if (next != EOF)
            fseek(fp, -1, SEEK_CUR);
    }
}
