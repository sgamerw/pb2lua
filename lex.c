//
//  lex.c
//  PBtoLUA
//
//  Created by WuZheng on 15/10/28.
//  Copyright © 2015年 WuZheng. All rights reserved.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "lex.h"

static int s_line_num = 0;
static FILE *s_file;

#define currIsNewline(c)	(c == '\n' || c == '\r')

void
set_stream(FILE *f)
{
    s_file = f;
}

char
next()
{
    char c = fgetc(s_file);
    return c;
}

void
prev(char c)
{
    ungetc(c, s_file);
}

void inclinenumber()
{
    s_line_num++;
}

void
get_token(Token *token)
{
    int out_pos = 0;
    char current_char = next();
    token->kind = BAD_TOKEN;
    while (1) {
        if (out_pos >= MAX_TOKEN_SIZE) {
            printf("token too long.\n");
            return;
        }
        switch (current_char) {
            case '{': {
                if (token->kind == BAD_TOKEN) {
                    token->kind = LEFT_PAREN_TOKEN;
                } else {
                    prev(current_char);
                }
                return;
            }
            case '}': {
                if (token->kind == BAD_TOKEN) {
                    token->kind = RIGHT_PAREN_TOKEN;
                } else {
                    prev(current_char);
                }
                return;
            }
            case '=': {
                if (token->kind == BAD_TOKEN) {
                    token->kind = EQUAL_TOKEN;
                } else {
                    prev(current_char);
                }
                return;
            }
            case '/': { /* '/' or '//' comment */
                if (token->kind != BAD_TOKEN) {
                    prev(current_char);
                    return;
                }
                current_char = next();
                if (current_char == '/') {
                    current_char = next();
                    while (!currIsNewline(current_char) && current_char != EOF) {
                        current_char = next();
                    }
                } else if (current_char == '*') {
                    current_char = next();
                    while (1) {
                        while (current_char != '*') {
                            current_char = next();
                        }
                        current_char = next();
                        if (current_char == '/') {
                            current_char = next();
                            break;
                        }
                    }
                } else {
                    printf("unknow symbol '/%c'.", current_char);
                    return;
                }
                break;
            }
            case ';': {
                if (token->kind == BAD_TOKEN) {
                    token->kind = SEMICOLON_TOKEN;
                } else {
                    prev(current_char);
                }
                return;
            }
            case '\n': case '\r': { /* line breaks */
                if (token->kind == BAD_TOKEN) {
                    inclinenumber();
                    current_char = next();
                    break;
                } else {
                    return;
                }
            }
            case ' ': case '\f': case '\t': case '\v': {
                if (token->kind == BAD_TOKEN) {
                    current_char = next();
                    break;
                } else {
                    return;
                }
            }
            case EOF: {
                if (token->kind == BAD_TOKEN) {
                    token->kind = END_OF_FILE_TOKEN;
                } else {
                    prev(current_char);
                }
                return;
            }
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9': {
                if (token->kind == STRING_TOKEN) {
                    token->kind = STRING_TOKEN;
                    token->str[out_pos] = current_char;
                    out_pos++;
                    token->str[out_pos] = '\0';
                } else if (token->kind == BAD_TOKEN || token->kind == NUMBER_TOKEN) {
                    token->kind = NUMBER_TOKEN;
                    token->str[out_pos] = current_char;
                    out_pos++;
                    token->str[out_pos] = '\0';
                    sscanf(token->str, "%d", &token->value);
                }
                current_char = next();
                break;
            }
            default: {
                token->kind = STRING_TOKEN;
                token->str[out_pos] = current_char;
                out_pos++;
                token->str[out_pos] = '\0';
                current_char = next();
                break;
            }
        }
    }
}