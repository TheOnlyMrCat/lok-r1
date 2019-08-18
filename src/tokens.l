%{
#include "tokens.h"
%}
%%
use return USE;
fn return FUNCTION;
library return PUBLIC;
return return RETURN;
namespace return NAMESPACE;

(0x|0b|0o)?[0-9](i(8|16|32|64|128|256|512))? return INTEGER;
(0x|0b|0o)?[0-9](\.[0-9])?(f(16|32|64|128))? return FLOAT;
[A-Za-z]\w* return IDENTIFIER;

"\"[^\"]\"" return STRING;

"<\w*>" return TYPE;

"{|}" return BRACE;
\(|\) return PARENTHESIS;
[|] return SQUARE;

"!" return OPERATOR_UN;
"<=|>=|::|[+*/%<>=:]" return OPERATOR_BIN;

"$" return END;
\n yylineno++;
\s {}//Ignore
. return UNKNOWN;
