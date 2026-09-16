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