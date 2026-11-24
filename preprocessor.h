#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>

#define MAX_MACROS 100

typedef struct {
	char name[64];
	char param[64];
	char value[256];
	int has_param;
} Macro;

// Comment removal 
void handle_comment(FILE *fp, FILE *out);

// File inclusion 
void copy_header_file(const char *header_name, FILE *out);
void handle_include(FILE *fp, FILE *out, int *ch);

// Macro handler 
void handle_define(FILE *fp, int *ch);
int find_macro(const char *word);
void expand_macro_with_arg(Macro *m, const char *arg, FILE *out);
void handle_identifier(FILE *fp, FILE *out, int first_ch);

#endif

