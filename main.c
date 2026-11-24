#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "preprocessor.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./mypreprocessor filename.c\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("no data is present in %s\n", argv[1]);
        return 1;
    }

    char new_file[256];
    strcpy(new_file, argv[1]);
    char *dot = strrchr(new_file, '.');
    if (dot != NULL)
        strcpy(dot, ".i");
    else
        strcat(new_file, ".i");

    FILE *out = fopen(new_file, "w");
    if (out == NULL)
    {
        printf("Error: could not create %s\n", new_file);
        fclose(fp);
        return 1;
    }

    int ch;
    int in_string = 0;

    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '"')
        {
            in_string = !in_string;
            fputc(ch, out);
            continue;
        }

        if (!in_string && ch == '/')
        {
            handle_comment(fp, out);
            continue;
        }

        if (!in_string && ch == '#')
        {
            char directive[20];
            int i = 0;

            while (i < 19 && (ch = fgetc(fp)) != EOF && ch != ' ' && ch != '\t' && ch != '<' && ch != '"' && ch != '\n')
                directive[i++] = ch;
            directive[i] = '\0';

            if (strcmp(directive, "include") == 0)
            {
                handle_include(fp, out, &ch);
            }
            else if (strcmp(directive, "define") == 0)
            {
                handle_define(fp, &ch);
            }
            else
            {
                fputc('#', out);
                fputs(directive, out);
                if (ch != EOF)
                    fputc(ch, out);
            }
            continue;
        }

        if (!in_string && (isalpha((unsigned char)ch) || ch == '_'))
        {
            handle_identifier(fp, out, ch);
            continue;
        }

        fputc(ch, out);
    }

    fclose(fp);
    fclose(out);
    return 0;
}
