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
};