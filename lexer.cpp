#include<iostream>
#include<fstream>
#include<string>
#include<cctype>

using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    CHAR_LITERAL,

    ASSIGN,
    EQUAL,

    LESS,
    LESS_EQUAL,

    GREATER,
    GREATER_EQUAL,

    NOT,
    NOT_EQUAL,

    PLUS,
    INCREMENT,

    MINUS,
    DECREMENT,

    MULTIPLY,
    DIVIDE,

    SEMICOLON,
    COMMA,

    LPAREN,
    RPAREN,

    LBRACE,
    RBRACE,

    END_OF_FILE,
    ERROR

};

struct Token
{
    TokenType type;
    string lexeme;
};

string TokeName(TokenType type)
{
    switch(type)
    {
        case KEYWORD:
            return "KEYWORD";

        
        case IDENTIFIER:
            return "IDENTIFIER";
        case INTEGER:
            return "INTEGER";
        case CHAR_LITERAL:
            return "CHAR_LITERAL";
        case ASSIGN:
            return "ASSIGN";
        case EQUAL:
            return "EQUAL";
        case LESS:
            return "LESS";
        case LESS_EQUAL:
            return "LESS_EQUAL";
        case GREATER:
            return "GREATER";
        case GREATER_EQUAL:
            return "GREATER_EQUAL";
        case NOT:
            return "NOT";
        case NOT_EQUAL:
            return "NOT_EQUAL";
        case PLUS:
            return "PLUS";
        case INCREMENT:
            return "INCREMENT";
        case MINUS:
            return "MINUS";
        case DECREMENT:
            return "DECREMENT";
        case MULTIPLY:
            return "MULTIPLY";
        case DIVIDE:
            return "DIVIDE";
        case SEMICOLON:
            return "SEMICOLON";
        case COMMA:
            return "COMMA";
        case LPAREN:
            return "LPAREN";
        case RPAREN:
            return "RPAREN";
        case LBRACE:
            return "LBRACE";
        case RBRACE:
            return "RBRACE";
        case END_OF_FILE:
            return "END_OF_FILE";
        case ERROR:
            return "ERROR";
            return "KEYWORD";
    }
}

bool isKeyword(string word)
{
    if (word == "if" || word == "else" || word == "while" || word == "return" || word == "int" || word == "char")
    {
         return true;    
    }   

    return false;
}


class Lexer
{
private:
    string source;
    int position;

    Lexer(string input) : source(input), position(0) 
    {}


    char getChar()
    {
        if (position>= (int)source.length())
            return '\0';

        return source[position++];
    }


    void Retract()
    {
        if (position > 0)
            position--;
    }

    void SkipWhitespace()
    {
        while (isspace(source[position]))
            position++;
    }

    Token getNextToken()
    {
        int state=0;
        string lexeme="";

        char ch;

        while (true)
        {
            switch (state)
            {
                case 0:
                {
                    SkipWhitespace();

                    ch = getChar();

                    if (ch == '\0')
                    {
                        return {END_OF_FILE, ""};
                    }



                    if (isalpha(ch) || ch == '_')
                    {
                        lexeme += ch;

                        state = 1;
                    }


                    else if (isdigit(ch))
                    {
                        lexeme += ch;

                        state = 3;
                    }

                    else if (ch == '=')
                    {
                        lexeme += ch;

                        state = 5;
                    }

                    else if (ch == '<')
                    {
                        lexeme += ch;

                        state = 8;
                    }

                    else if (ch == '>')
                    {
                        lexeme += ch;

                        state = 11;
                    }

                    else if (ch == '!')
                    {
                        lexeme += ch;

                        state = 14;
                    }

                    else if (ch == '+')
                    {
                        lexeme += ch;

                        state = 17;
                    }

                    else if (ch == '-')
                    {
                        lexeme += ch;

                        state = 20;
                    }

                    else if (ch == '*')
                    {
                        lexeme += ch;

                        state = 23;
                    }

                    else if (ch == '\'')
                    {
                        lexeme += ch;

                        state = 25;
                    }

                    else if (ch == '/')
                    {
                        lexeme += ch;

                        return {DIVIDE, lexeme};
                    }

                    else if (ch == ';' ||
                             ch == ',' ||
                             ch == '(' ||
                             ch == ')' ||
                             ch == '{' ||
                             ch == '}')
                    {
                        lexeme += ch;

                        state = 29;
                    }


                    else
                    {
                        lexeme += ch;

                        return {ERROR, lexeme};
                    }

                    break;
                }
            case 1:
                {
                    ch = getChar();

                    if (isalpha(ch) || isdigit(ch) || ch == '_')
                    {
                        lexeme += ch;

                        state = 1;
                    }
                    else
                    {
                        Retract();

                        state = 2;
                    }

                    break;
                }
            case 2:
                {
                    if (isKeyword(lexeme))
                        return {KEYWORD, lexeme};

                    return {IDENTIFIER, lexeme};
                }
             case 3:
                {
                    ch = getChar();

                    if (isdigit(ch))
                    {
                        lexeme += ch;

                        state = 3;
                    }
                    else
                    {
                        Retract();

                        state = 4;
                    }

                    break;
                }
            case 4:
                {
                    return {INTEGER, lexeme};
                }

            case 5:
                {
                    ch=getChar();
                    if(ch=='=')
                    {
                        lexeme+=ch;
                        state=6;
                    }
                    else
                    {
                        Retract();
                        state=7;
                    }
            case 6:
                {
                    return{LESS_EQUAL,lexeme};
                }

            case 7:
                {
                    return{LESS,lexeme};
                }
            
            case 8:
                {
                    ch=getChar();
                    if(ch=='=')
                    {
                        lexeme+=ch;
                        state=9;
                    }
                    else
                    {
                        Retract();
                        state=10;
                    }
                    break;
                }
            case 9:
                {
                    return{EQUAL,lexeme};
                }

            case 10:
                {
                    return {ASSIGN,lexeme};
                }
            case 11:
                {
                    ch = getChar();

                    if (ch == '>')
                    {
                        lexeme += ch;

                        state = 12;
                    }
                    else
                    {
                        Retract();

                        state = 13;
                    }

                    break;
                }
                
            case 12:
                {
                    return {GREATER_EQUAL, lexeme};
                }
            case 13:
            {   {
                    return {GREATER, lexeme};
                }
            }
            case 14:
                {
                    ch = getChar();

                    if (ch == '=')
                    {
                        lexeme += ch;

                        state = 15;
                    }
                    else
                    {
                        Retract();

                        state = 16;
                    }

                    break;
                }

                case 15:
                {
                    return {NOT_EQUAL, lexeme};
                }



                case 16:
                {
                    return {NOT, lexeme};
                }


                case 17:
                {
                    ch = getChar();

                    if (ch == '+')
                    {
                        lexeme += ch;

                        state = 18;
                    }
                    else
                    {
                        Retract();
                        state = 19;
                    }

                    break;
                }



                case 18:
                {
                    return {INCREMENT, lexeme};
                }



                case 19:
                {
                    return {PLUS, lexeme};
                }



                case 20:
                {
                    ch = getChar();

                    if (ch == '-')
                    {
                        lexeme += ch;

                        state = 21;
                    }
                    else
                    {
                        Retract();

                        state = 22;
                    }

                    break;
                }

                case 21:
                {
                    return {DECREMENT, lexeme};
                }
                
                case 22:
                {
                    return {MINUS, lexeme};
                }


                // ==================================================
                // STATE 23
                // *
                // ==================================================

                case 23:
                {
                    state = 24;

                    break;
                }


                // ==================================================
                // STATE 24 *
                // *
                // ==================================================

                case 24:
                {
                    return {MULTIPLY, lexeme};
                }

            }

        }
    }
};