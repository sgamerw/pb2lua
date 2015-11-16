//
//  main.c
//  PBtoLUA
//
//  Created by WuZheng on 15/10/27.
//  Copyright © 2015年 WuZheng. All rights reserved.
//

#include <stdio.h>
#include "parser.h"

const char * filePath = "/Users/wuzheng/Documents/PBtoLUA/PBtoLUA/pf.proto";

int
main(int argc, const char * argv[]) {
    FILE *f_in;
    if ((f_in= fopen(filePath, "r")) != NULL) {
        set_stream(f_in);
        char *luaFile = "/Users/wuzheng/Documents/PBtoLUA/PBtoLUA/Proto_pf.lua";
        remove(luaFile);
        FILE *f_out = fopen(luaFile, "a");
        if (f_out != NULL) {
            parse(f_out);
            fclose(f_out);
        }
        fclose(f_in);
    }
    return 0;
}
