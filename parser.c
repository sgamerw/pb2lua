//
//  parser.c
//  PBtoLUA
//
//  Created by WuZheng on 15/10/28.
//  Copyright © 2015年 WuZheng. All rights reserved.
//

#include "parser.h"
#include <string.h>

static FILE *s_out_file;
static int st_look_ahead_token_exists;
static Token st_look_ahead_token;

void
my_get_token(Token *token)
{
    if (st_look_ahead_token_exists) {
        *token = st_look_ahead_token;
        st_look_ahead_token_exists = 0;
    } else {
        get_token(token);
    }
}

void
unget_token(Token *token)
{
    st_look_ahead_token = *token;
    st_look_ahead_token_exists = 1;
}

void
parse_primary_expression()
{
    
}

void
parse_term()
{
    
}

Var_Exsit
readRRO(const char *str)
{
    if (0 == strcmp(str, "required")) {
        return REQUIRED;
    } else if (0 == strcmp(str, "repeated")) {
        return REPEATED;
    } else if (0 == strcmp(str, "optional")) {
        return OPTIONAL;
    } else {
        return ERROR;
    }
}

int
isInnerType(char *type)
{
    char *innerType[4] = {"int32", "string", "int64", "float"};
    for (int i = 0; i < 4; i++) {
        if (0==strcmp(type, innerType[i])) {
            return 1;
        }
    }
    return 0;
}

void
outType(MSG msg)
{
    char stream[1024];
    if (isInnerType(msg.var_type)) {
        sprintf(stream, "\t\tself:printItem(\"%s\")\n", msg.var_name);
    } else {
        sprintf(stream, "\t\tLCPB_%s.new(self._msg.%s):printTitle():print()\n", msg.var_type, msg.var_name);
    }
    fputs(stream, s_out_file);
}

void
outRRO(MSG msg)
{
    if (msg.exsit == REQUIRED) {
        outType(msg);
    } else if (msg.exsit == REPEATED) {
        char stream[1024];
        sprintf(stream, "\t\tfor _, data in ipairs(self._msg.%s or {}) do\n", msg.var_name);
        fputs(stream, s_out_file);
        if (!isInnerType(msg.var_type)) {
            sprintf(stream, "\t\t\tLCPB_%s.new(data):printTitle():print()\n\t\tend\n", msg.var_type);
        } else {
            sprintf(stream, "\t\t\tself:printItem(\"%s\")\n\t\tend\n", msg.var_name);
        }
        fputs(stream, s_out_file);
    } else if (msg.exsit == OPTIONAL) {
        outType(msg);
    }
}

MSG
parse_expression()
{
    MSG msg;
    Token token;
    my_get_token(&token);
    if (token.kind == STRING_TOKEN) {
        msg.exsit = readRRO(token.str);
        if (msg.exsit != ERROR) {
            my_get_token(&token);
            if (token.kind == STRING_TOKEN) {
                strcpy(msg.var_type, token.str);
                my_get_token(&token);
                if (token.kind == STRING_TOKEN) {
                    strcpy(msg.var_name, token.str);
                    my_get_token(&token);
                    if (token.kind == EQUAL_TOKEN) {
                        my_get_token(&token);
                        if (token.kind == NUMBER_TOKEN) {
                            msg.index = token.value;
                            my_get_token(&token);
                            if (token.kind == SEMICOLON_TOKEN) {
                                outRRO(msg);
                                return msg;
                            } else {
                                printf("; error.\n");
                            }
                        } else {
                            printf("index error.\n");
                        }
                    } else {
                        printf("= error.\n");
                    }
                } else {
                    printf("var name error.\n");
                }
            } else {
                printf("type error.\n");
            }
        } else {
            printf("r,r,o error\n");
        }
    } else {
        printf("expression error.\n");
    }
    return msg;
}

void
parse_msg()
{
    Token token;
    my_get_token(&token);
    char cls_name[MAX_TOKEN_SIZE];
    if (token.kind == STRING_TOKEN) {
        strcpy(cls_name, token.str);
        my_get_token(&token);
        if (token.kind == LEFT_PAREN_TOKEN) {
            my_get_token(&token);
            char title[1024];
            sprintf(title, "LCPB_%s = class(\"LCPB_%s\", LCProtoBase)\nfunction LCPB_%s:ctor(msgInput)\n\tlocal msg = msgInput or {}\n\tLCPB_%s.super.ctor(self, protoPack, \"%s\", msg)\n\tself.print = function()\n", cls_name, cls_name, cls_name, cls_name, cls_name);
            fputs(title, s_out_file);
            if (token.kind == RIGHT_PAREN_TOKEN) {
                fputs("\tend\nend\n\n", s_out_file);
                return;
            } else {
                while (token.kind != RIGHT_PAREN_TOKEN) {
                    unget_token(&token);
                    parse_expression();
                    my_get_token(&token);
                }
                fputs("\tend\nend\n\n", s_out_file);
                return;
            }
        } else {
            printf("no { next to cls name.\n");
            return;
        }
    } else {
        printf("no cls name next to message.\n");
        return;
    }
}

void
parse(FILE *f_out)
{
    s_out_file = f_out;
    Token token;
    my_get_token(&token);
    if (!(token.kind == STRING_TOKEN && 0 == strcmp(token.str, "message"))) {
        printf("pb file not begin with \"message\"");
    }
    while (token.kind == STRING_TOKEN && 0 == strcmp(token.str, "message")) {
        parse_msg();
        my_get_token(&token);
    }
}