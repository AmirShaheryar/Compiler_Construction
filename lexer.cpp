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

    INPUT,
    OUTPUT,

    END_OF_FILE,
    ERROR

};

struct Token
{
    TokenType type;
    string lexeme;
};

string TokenName(TokenType type)
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
        case INPUT:
            return "INPUT";
        case OUTPUT:
            return "OUTPUT";
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
public:
    Lexer(string input) : source(input),position(0)
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
        while (position < (int)source.length() && isspace((unsigned char)source[position]))
        {
            position++;
        }
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

                    else if (ch == '<')
                    {
                        lexeme += ch;

                        state = 5;
                    }

                    else if (ch == '=')
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
                    else if(ch=='<')
                    {
                        lexeme+=ch;
                        state=31;
                    }
                    else
                    {
                        Retract();
                        state=7;
                    }
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

                    if (ch == '=')
                    {
                        lexeme += ch;

                        state = 12;
                    }
                    else if (ch == '>')
                    {
                        lexeme += ch;

                        state = 32;
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
                {
                    return {GREATER, lexeme};
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


                case 23:
                {
                    state = 24;
                    break;
                }

                case 24:
                {
                    return {MULTIPLY, lexeme};
                }
                
                case 25:
                {
                    ch = getChar();

                    if (ch == '\0')
                    {
                        state = 28;
                    }
                    else
                    {
                        lexeme += ch;

                        state = 26;
                    }

                    break;
                }


                
                case 26:
                {
                    ch = getChar();

                    if (ch == '\'')
                    {
                        lexeme += ch;

                        state = 27;
                    }
                    else
                    {
                        state = 28;
                    }

                    break;
                }


                
                case 27:
                {
                    return {CHAR_LITERAL, lexeme};
                }



                case 28:
                {
                    return {ERROR, lexeme};
                }



                case 29:
                {
                    state = 30;

                    break;
                }


            
                case 30:
                {
                    if (lexeme == ";")
                        return {SEMICOLON, lexeme};

                    if (lexeme == ",")
                        return {COMMA, lexeme};

                    if (lexeme == "(")
                        return {LPAREN, lexeme};

                    if (lexeme == ")")
                        return {RPAREN, lexeme};

                    if (lexeme == "{")
                        return {LBRACE, lexeme};

                    if (lexeme == "}")
                        return {RBRACE, lexeme};

                    return {ERROR, lexeme};
                }

                case 31:
                {
                    return {OUTPUT, lexeme};
                }

                case 32:
                {
                    return {INPUT, lexeme};
                }
                default:
                {
                    return {ERROR, "Invalid State"};
                }
            }
        }
    }
};


int main()
{
    ifstream inputFile("input.txt");

    if (!inputFile)
    {
        cout << "Error: input.txt could not be opened." << endl;
        return 1;
    }

    ofstream outputFile("output.txt");

    if (!outputFile)
    {
        cout << "Error: output.txt could not be created." << endl;
        return 1;
    }

    string source;
    string line;

    while (getline(inputFile, line))
    {
        source += line;
        source += '\n';
    }

    inputFile.close();

    // Create lexer
    Lexer lexer(source);

    // Header
    outputFile << "C++ LEXICAL ANALYZER OUTPUT\n";
    outputFile << "========================================\n\n";
    outputFile << "LEXEME\t\tTOKEN\n";
    outputFile << "----------------------------------------\n";

    while (true)
    {
        Token token = lexer.getNextToken();

        if (token.type == END_OF_FILE)
            break;

        outputFile << token.lexeme << "\t\t"<< TokenName(token.type)<< "\n";
    }

    outputFile.close();

    cout << "Lexical analysis completed successfully!\n";
    cout << "Input  : input.txt\n";
    cout << "Output : output.txt\n";

    return 0;
}