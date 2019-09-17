%{
#include "tokens.h"
%}
%%
(?-s:#.*) ; // Line comment

use/[^[:alnum:]_]               return USE;
load/[^[:alnum:]_]              return LOAD;
fn/[^[:alnum:]_]                return FUNCTION;
return/[^[:alnum:]_]            return RETURN;
namespace/[^[:alnum:]_]         return NAMESPACE;
run/[^[:alnum:]_]               return RUN;
extends/[^[:alnum:]_]           return EXTENDS;

if/[^[:alnum:]_]                return IF;
else/[^[:alnum:]_]              return ELSE;
for/[^[:alnum:]_]               return FOR;
rpt/[^[:alnum:]_]               return REPEAT;
while/[^[:alnum:]_]             return WHILE;

break/[^[:alnum:]_]             return BREAK;
continue/[^[:alnum:]_]          return CONTINUE;

class/[^[:alnum:]_]             return CLASS;
struct/[^[:alnum:]_]            return STRUCT;

abstract/[^[:alnum:]_]          return FUNCTION_MOD;
loc?ked/[^[:alnum:]_]           return FUNCTION_MOD;
private/[^[:alnum:]_]           return ACCESS_MOD;
protected/[^[:alnum:]_]         return ACCESS_MOD;
internal/[^[:alnum:]_]          return ACCESS_MOD;
public/[^[:alnum:]_]            return ACCESS_MOD;

(0x|0b|0o)?[0-9]+/[^f]?         return INTEGER;
(0x|0b|0o)?[0-9]+(\.[0-9])?f?   return FLOAT;
[A-Za-z][A-Za-z0-9_]*           return IDENTIFIER;

\"[^\"\n]*\"                    return STRING;

[&$%]?:?\<([[:alnum:]_]|::)*\>  return TYPE;

"{" return OPEN_BRACE;
"}" return CLOS_BRACE;
"(" return OPEN_PARENTHESIS;
")" return CLOS_PARENTHESIS;
"[" return OPEN_SQUARE;
"]" return CLOS_SQUARE;

"++"|--|"**"|"//" return OPERATOR_POST_UN;
[!~] return OPERATOR_PRE_UN;
\<=|\>=|"<<"|">>"|==|!=|&&|"||"|[+\-*/%<>=:.&\|\^] return OPERATOR_BIN;
;|\u037e return SEMICOLON;

<<EOF>> return END;
\n return NEWLINE;
[ \t]* return WHITESPACE;
. return UNKNOWN;
