%{
#include "tokens.h"
%}
%%
use/[[:^alnum:]_] return USE;
fn/[[:^alnum:]_] return FUNCTION;
library/[[:^alnum:]_] return PUBLIC;
return/[[:^alnum:]_] return RETURN;
namespace/[[:^alnum:]_] return NAMESPACE;
run/[[:^alnum:]_] return RUN;

(0x|0b|0o)?[0-9]/[^f]? return INTEGER;
(0x|0b|0o)?[0-9](\.[0-9])?f? return FLOAT;
[A-Za-z]\w* return IDENTIFIER;

\"[^\"]\" return STRING;

\<[[:alnum:]_]*\> return TYPE;

"{" return OPEN_BRACE;
"}" return CLOS_BRACE;
"(" return OPEN_PARENTHESIS;
")" return CLOS_PARENTHESIS;
"[" return OPEN_SQUARE;
"]" return CLOS_SQUARE;

"++"|--|"**"|"//" return OPERATOR_POST_UN;
[!~] return OPERATOR_PRE_UN;
\<=|\>=|"<<"|">>"|==|!=|&&|"||"|[+\-*/%<>=:.&\|\^] return OPERATOR_BIN;
; return SEMICOLON;

(?-s:#.*) ;
(?s:##.*##) ;

<<EOF>> return END;
\n yylineno++;
[ \t]* return WHITESPACE;
. return UNKNOWN;
