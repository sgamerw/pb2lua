//
//  parser.h
//  PBtoLUA
//
//  Created by WuZheng on 15/10/28.
//  Copyright © 2015年 WuZheng. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include <stdio.h>
#include "lex.h"

#define STACK_SIZE (256)

//typedef struct {
//    Token stack[STACK_SIZE];
//    int top;
//} parse_stack;

typedef enum {
    ERROR = 0,
    REQUIRED,
    REPEATED,
    OPTIONAL,
} Var_Exsit;

typedef struct {
    char *cls_name;
    Var_Exsit exsit;
    char var_type[MAX_TOKEN_SIZE];
    char var_name[MAX_TOKEN_SIZE];
    int index;
} MSG;

void parse(FILE *f_out);
#endif /* parser_h */
