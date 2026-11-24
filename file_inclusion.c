#include <string.h>
#include "preprocessor.h"

void copy_header_file(const char *header_name, FILE *out)
{
    FILE *hdr = fopen(header_name, "r");
    if (hdr == NULL)
    {
        char sys_path[256];
        strcpy(sys_path, "/usr/include/");
        strcat(sys_path, header_name);
        hdr = fopen(sys_path, "r");
    }

    if (hdr == NULL)
    {
        printf("Warning: cannot find header file %s\n", header_name);
        return;
    }

    int ch;
    while ((ch = fgetc(hdr)) != EOF)
        fputc(ch, out);
    fputc('\n', out);
    fclose(hdr);
}

void handle_include(FILE *fp, FILE *out, int *ch)
{
    while (*ch == ' ' || *ch == '\t')
        *ch = fgetc(fp);

    if (*ch == '<' || *ch == '"')
    {
        char end_char = (*ch == '<') ? '>' : '"';
        char header_name[256];
        int h = 0;

        while (h < 255 && (*ch = fgetc(fp)) != EOF && *ch != end_char && *ch != '\n')
            header_name[h++] = *ch;
        header_name[h] = '\0';

        while (*ch != EOF && *ch != '\n')
            *ch = fgetc(fp);

        copy_header_file(header_name, out);
    }
}
