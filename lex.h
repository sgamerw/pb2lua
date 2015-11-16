//
//  lex.h
//  PBtoLUA
//
//  Created by WuZheng on 15/10/28.
//  Copyright © 2015年 WuZheng. All rights reserved.
//

#ifndef lex_h
#define lex_h

typedef enum {
    BAD_TOKEN,
    NUMBER_TOKEN,
    STRING_TOKEN,
    LEFT_PAREN_TOKEN,
    RIGHT_PAREN_TOKEN,
    END_OF_FILE_TOKEN,
    EQUAL_TOKEN,
    SEMICOLON_TOKEN,
} TokenKind;

#define MAX_TOKEN_SIZE (100)

typedef struct {
    TokenKind   kind;
    int         value;
    char        str[MAX_TOKEN_SIZE];
} Token;

void set_stream(FILE *f);
void get_token(Token *token);
#endif /* lex_h */
