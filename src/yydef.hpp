//
//  yydef.hpp
//  Lok
//
//  Created by TheOnlyMrCat on 14/8/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

extern "C" {
    extern int yylex();
    extern char *yytext;
    extern int yylineno;

    extern int yycol;
}
