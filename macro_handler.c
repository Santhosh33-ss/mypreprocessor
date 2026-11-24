#include <string.h>
#include <ctype.h>
#include "preprocessor.h"

static Macro macros[MAX_MACROS];
static int macro_count = 0;

int find_macro(const char *word)
{
    for (int i = macro_count - 1; i >= 0; i--)
    {
        if (strcmp(macros[i].name, word) == 0)
            return i;
    }
    return -1;
}

void expand_macro_with_arg(Macro *m, const char *arg, FILE *out)
{
    int i = 0;
    while (m->value[i] != '\0')
    {
        if (isalpha((unsigned char)m->value[i]) || m->value[i] == '_')
        {
            char token[64];
            int t = 0;
            while (isalnum((unsigned char)m->value[i]) || m->value[i] == '_')
                token[t++] = m->value[i++];
            token[t] = '\0';

            if (strcmp(token, m->param) == 0)
                fputs(arg, out);
            else
                fputs(token, out);
        }
        else
        {
            fputc(m->value[i++], out);
        }
    }
}

void handle_define(FILE *fp, int *ch)
{
    if (macro_count >= MAX_MACROS)
        return;

    while (*ch == ' ' || *ch == '\t')
        *ch = fgetc(fp);

    int n = 0;
    while (n < 63 && *ch != EOF && (isalnum((unsigned char)*ch) || *ch == '_'))
    {
        macros[macro_count].name[n++] = *ch;
        *ch = fgetc(fp);
    }
    macros[macro_count].name[n] = '\0';

    if (*ch == '(')
    {
        macros[macro_count].has_param = 1;
        *ch = fgetc(fp);
        while (*ch == ' ' || *ch == '\t')
            *ch = fgetc(fp);

        int p = 0;
        while (p < 63 && *ch != EOF && (isalnum((unsigned char)*ch) || *ch == '_'))
        {
            macros[macro_count].param[p++] = *ch;
            *ch = fgetc(fp);
        }
        macros[macro_count].param[p] = '\0';

        while (*ch != EOF && *ch != ')' && *ch != '\n')
            *ch = fgetc(fp);
        if (*ch == ')')
            *ch = fgetc(fp);
    }
    else
    {
        macros[macro_count].has_param = 0;
        macros[macro_count].param[0] = '\0';
    }

    while (*ch == ' ' || *ch == '\t')
        *ch = fgetc(fp);

    int v = 0;
    while (v < 255 && *ch != EOF && *ch != '\n' && *ch != '\r')
    {
        macros[macro_count].value[v++] = *ch;
        *ch = fgetc(fp);
    }
    macros[macro_count].value[v] = '\0';

    macro_count++;
}

void handle_identifier(FILE *fp, FILE *out, int first_ch)
{
    char word[64];
    int w = 0;
    int ch = first_ch;
    word[w++] = ch;

    while (w < 63 && (ch = fgetc(fp)) != EOF && (isalnum((unsigned char)ch) || ch == '_'))
        word[w++] = ch;
    word[w] = '\0';

    int idx = find_macro(word);
    if (idx != -1)
    {
        if (!macros[idx].has_param)
        {
            fputs(macros[idx].value, out);
            if (ch != EOF)
                fseek(fp, -1, SEEK_CUR);
        }
        else
        {
            while (ch == ' ' || ch == '\t')
                ch = fgetc(fp);

            if (ch == '(')
            {
                char arg[128];
                int a = 0;
                int depth = 1;
                while (a < 127 && (ch = fgetc(fp)) != EOF && depth > 0)
                {
                    if (ch == '(') depth++;
                    else if (ch == ')')
                    {
                        depth--;
                        if (depth == 0) break;
                    }
                    arg[a++] = ch;
                }
                arg[a] = '\0';
                expand_macro_with_arg(&macros[idx], arg, out);
            }
            else
            {
                fputs(word, out);
                if (ch != EOF)
                    fseek(fp, -1, SEEK_CUR);
            }
        }
    }
    else
    {
        fputs(word, out);
        if (ch != EOF)
            fseek(fp, -1, SEEK_CUR);
    }
}
