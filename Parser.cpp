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
        void match(TokenType t)
    {
        if (cur.type == t) 
        {
            advance();
        }
        else throw string("Syntax error: expected " + TokenName(t) + " but found " + found());
    }

    void matchKw(const string &w)
    {
        if (isKw(w)) 
        {
            advance();
        }
        else throw string("Syntax error: expected '" + w + "' but found " + found());
    }

    bool isType() const   
    { 
        return isKw("int") || isKw("char"); 
    }
    bool isRetType() const 
    { 
        return isType() || isKw("void"); 
    }

    bool isRelOp() const
    {
        return cur.type == LESS || cur.type == LESS_EQUAL ||
               cur.type == GREATER || cur.type == GREATER_EQUAL ||
               cur.type == EQUAL || cur.type == NOT_EQUAL;
    }

    bool startsExpr() const
    {
        return cur.type == LPAREN || cur.type == IDENTIFIER ||
               cur.type == INTEGER || cur.type == CHAR_LITERAL ||
               cur.type == NOT || cur.type == MINUS ||
               cur.type == INCREMENT || cur.type == DECREMENT;
    }

    bool startsStmt() const
    {
        return isType() || isKw("if") || isKw("while") || isKw("return") ||
               isKw("in") || isKw("out") ||
               cur.type == IDENTIFIER || cur.type == LBRACE ||
               cur.type == SEMICOLON || cur.type == INCREMENT ||
               cur.type == DECREMENT;
    }
    struct Scope
    {
        Parser &p;
        Scope(Parser &parser, const string &name) : p(parser)
        {
            p.out << string(p.depth * 2, ' ') << name << "\n";
            p.depth++;
        }
        ~Scope() { p.depth--; }
    };
public:
    Parser(Lexer &lx, ofstream &o) : lexer(lx), cur{END_OF_FILE, ""}, out(o), depth(0) {}

    void parse()
    {
        advance();
        Program();
    }

    void Program()
    {
        Scope s(*this, "Program");
        FuncList();
        if (cur.type != END_OF_FILE)
            throw string("Syntax error: unexpected " + found() + " after last function");
    }

    void FuncList()
    {
        Scope s(*this, "FuncList");
        Func();
        while (isRetType())
        {
            Func();
        }   
    }

    void Func()
    {
        Scope s(*this, "Func");
        RetType();
        match(IDENTIFIER);
        match(LPAREN);
        ParamList();
        match(RPAREN);
        Block();
    }

    void RetType()
    {
        Scope s(*this, "RetType");
        if (isRetType()) advance();
        else throw string("Syntax error: expected return type (int, char, void) but found " + found());
    }

    void Type()
    {
        Scope s(*this, "Type");
        if (isType()) advance();
        else throw string("Syntax error: expected type (int, char) but found " + found());
    }


}
