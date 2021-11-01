#include "iobuffer.h"
#include "tokeniser-extras.h"

// to shorten the code
using namespace std ;
using namespace CS_IO_Buffers ;

namespace Assignment_Tokeniser
{
    // check if a char matches another char or is a member of a character group
    // eg char_isa('3',cg_digit) returns true
    bool char_isa(int ch,int cg)
    {
        // single character groups use their character as their value
        // do not add them to the switch statement

        if ( ch == cg ) return true ;

        switch(cg)
        {
        case cg_wspace:              // characters that start rule wspace
            switch(ch)
            {
            case '\t':
            case '\n':
            case '\r':
            case ' ':
                return true ;
            default:
                return false ;
            }
                                    // complete a switch statement for each of the following
    // * identifier ::= ('a'-'z'|'A'-'Z'|'$') letter*
    //   - letter ::= 'a'-'z'|'A'-'Z'|'0'-'9'|'-'|'$'|'.'
        case cg_identifier:         // characters that start rule identifier
            switch(ch){
                case 'a'...'z':
                case 'A'...'Z':
                case '$':
                    return true ;
                default:
                    return false ;
            }
        case cg_letter:             // characters that start rule letter
            switch(ch){
                case 'a'...'z':
                case 'A'...'Z':
                case '-':
                case '.':
                case '$':
                case '0'...'9':
                    return true ;
                default:
                    return false ;
            }
        case cg_number:             // characters that start rule number
            switch(ch){
                case cg_integer:
                case cg_scientific:
                    return true;
                default:
                    return false;
            }
        case cg_integer:            // characters that start rule integer
            switch(ch){
                case '0':
                case '1'...'9':
                    return true;
                default:
                    return false;
            }
        case cg_scientific:         // characters that start rule scientific
            switch(ch){
                case cg_integer:
                    return true;
                default:
                    return false;
            }
        case cg_digit:              // characters that start rule digit
            switch(ch){
                case '0'...'9':
                    return true ;
                default:
                    return false ;
            }
        case cg_digit19:            // characters that start rule digit19
            switch(ch){
                case '1'...'9':
                    return true ;
                default:
                    return false ;
            }
        case cg_exponent:           // characters that start rule exponent
            switch(ch){
                case cg_eee:
                    return true ;
                default:
                    return false ;
            }
        case cg_eee:                // characters that start rule eee
            switch(ch){
                case 'e':
                case 'E':
                    return true ;
                default:
                    return false ;
            }
        case cg_sign:               // characters that start rule eee
            switch(ch){
                case '+':
                case '-':
                    return true ;
                default:
                    return false ;
            }
        case cg_instring:           // characters that start rule instring
        case cg_telegu:             // characters that start rule telegu
            switch(ch){//' '|'!'|'#'-'~'|telegu
                case L'\u0C00' ... L'\u0C7F':
                    return true;
                default:
                    return false;
            }


        case cg_symbol:             // characters that start rule symbol
            switch(ch){ 
            //'@'|'~'|'='|'-'|'+'|'/'|'*='|'~='|'=='|'<<<'|'<<'|'>>>'|'>>'|'{'|'}'|'('|')'|'['|']'|'.'
                case '@':
                case '~':
                case '=':
                case '-':
                case '+':
                case '/':
                case '*':
                case '<':
                case '>':
                case '{':
                case '}':
                case '(':
                case ')':
                case '[':
                case ']':
                case '.':
                    return true ;
                default:
                    return false ;
            }
        case cg_comment_char:       // characters that start rule comment_char
            switch(ch){/*   eol_comment    ::= '/' eol_suffix
                            eol_suffix   ::= '/' comment_char* '\n'
                            comment_char ::= '\t'|'\r'|' '-'~'|armenian*/
                case cg_armenian:
                    return true;
                default:
                    return false;
            }
        case cg_armenian:           // characters that start rule armenian
            switch(ch){//0x0530 to 0x058F and 0xFB13 to 0xFB17
                case L'\u0530' ... L'\u058F':
                case L'\uFB13' ... L'\uFB17':
                    return true;
                default:
                    return false;
            }


        default:
            return false ;
        }
    }

    // work out the kind of a parsed token, this may call keyword_or_identifier()
    // the spelling is a valid token or "" if at end of input
    TokenKind classify_spelling(string spelling)
    {
        if ( spelling == "" ) return tk_eoi ;
        switch(spelling[0]){
            case '@': return tk_at;
            case '~': return tk_not;//
            case '=': return tk_assign;
            case '+': return tk_add;
            case '-': return tk_sub;
            case '/': return tk_div;
            case '{': return tk_lcb;
            case '}': return tk_rcb;
            case '(': return tk_lrb;
            case ')': return tk_rrb;
            case '[': return tk_lsb;
            case ']': return tk_rsb;
            case '.': return tk_dot;
            //wspace
            case '\t': return tk_tab;
            case '\n': return tk_newline;
            case '\r': return tk_carriage_return;
            case ' ': return tk_space;//
            //identifier
            case 'a' ... 'z': 
            case 'A' ... 'Z': 
            case '$': return keyword_or_identifier(spelling);;
            //integer
            case '0':
            case '1' ... '9': return tk_integer;
            //scientific

            /*case '<': return spelling == "<" ? tk_lt : tk_le;
            case '>': return spelling == ">" ? tk_gt : tk_ge;
            
            case '!': return spelling == "!" ? tk_not : tk_ne;*/
            case '"': return tk_string;
            default: return tk_oops;
        }
        return tk_identifier ;
    }

    // work out the correct spelling to use in the Token object being created by new_token()
    // the spelling is a valid token and kind is its kind
    string correct_spelling(TokenKind kind,string spelling)
    {
        if ( spelling == "" ) return "" ;

        return spelling ;
    }

}
