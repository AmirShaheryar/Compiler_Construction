#define main lexer_main   
#include "lexer.cpp"
#undef main
#include <vector>
class Parser
{
private:
    Lexer &lexer;
    Token cur;
    ofstream &out;
    int depth;

    void advance()
    {
        cur = lexer.getNextToken();
        if (cur.type == ERROR)
            throw string("Lexical error: invalid token '" + cur.lexeme + "'");
    }

    bool isKw(const string &w) const
    {
        return cur.type == KEYWORD && cur.lexeme == w;
    }

    string found() const
    {
        if (cur.type == END_OF_FILE) return "end of file";
        return "'" + cur.lexeme + "'";
    }
}
