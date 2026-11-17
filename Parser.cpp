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
        void ParamList()
    {
        Scope s(*this, "ParamList");
        if (isType())
        {
            Param();
            while (cur.type == COMMA)
            {
                advance();
                Param();
            }
        }
    }

    void Param()
    {
        Scope s(*this, "Param");
        Type();
        match(IDENTIFIER);
    }

    void Block()
    {
        Scope s(*this, "Block");
        match(LBRACE);
        StmtList();
        match(RBRACE);
    }

    void StmtList()
    {
        Scope s(*this, "StmtList");
        while (startsStmt())
            Stmt();
    }

    void Stmt()
    {
        Scope s(*this, "Stmt");
        if (isType())                    Decl();
        else if (cur.type == IDENTIFIER) AssignStmt();
        else if (isKw("if"))             IfStmt();
        else if (isKw("while"))          WhileStmt();
        else if (isKw("return"))         ReturnStmt();
        else if (isKw("in"))             InputStmt();
        else if (isKw("out"))            OutputStmt();
        else if (cur.type == LBRACE)     Block();
        else if (cur.type == SEMICOLON)  advance();
        else if (cur.type == INCREMENT || cur.type == DECREMENT)
        {
            advance();
            match(IDENTIFIER);
            match(SEMICOLON);
        }
        else throw string("Syntax error: unexpected " + found() + " at start of statement");
    }

    void Decl()
    {
        Scope s(*this, "Decl");
        Type();
        IdList();
        match(SEMICOLON);
    }

    void IdList()
    {
        Scope s(*this, "IdList");
        DeclItem();
        while (cur.type == COMMA)
        {
            advance();
            DeclItem();
        }
    }

    void DeclItem()
    {
        Scope s(*this, "DeclItem");
        match(IDENTIFIER);
        if (cur.type == ASSIGN)      
        {
            advance();
            Expr();
        }
    }

    void AssignStmt()
    {
        Scope s(*this, "AssignStmt");
        match(IDENTIFIER);
        if (cur.type == ASSIGN)
        {
            advance();
            Expr();
            match(SEMICOLON);
        }
        else if (cur.type == INCREMENT || cur.type == DECREMENT)
        {
            advance();
            match(SEMICOLON);
        }
        else if (cur.type == LPAREN)          
        {
            advance();
            ArgList();
            match(RPAREN);
            match(SEMICOLON);
        }
        else throw string("Syntax error: expected '=', '++', '--' or '(' after identifier but found " + found());
    }

    void ArgList()
    {
        Scope s(*this, "ArgList");
        if (startsExpr())
        {
            Expr();
            while (cur.type == COMMA)
            {
                advance();
                Expr();
            }
        }
    }

    void IfStmt()
    {
        Scope s(*this, "IfStmt");
        matchKw("if");
        match(LPAREN);
        Expr();
        match(RPAREN);
        Block();
        if (isKw("else"))            
        {
            Scope e(*this, "ElsePart");
            advance();
            Block();
        }
    }

    void WhileStmt()
    {
        Scope s(*this, "WhileStmt");
        matchKw("while");
        match(LPAREN);
        Expr();
        match(RPAREN);
        Block();
    }

    void ReturnStmt()
    {
        Scope s(*this, "ReturnStmt");
        matchKw("return");
        if (cur.type == SEMICOLON)
            advance();
        else
        {
            Expr();
            match(SEMICOLON);
        }
    }

    void InputStmt()
    {
        Scope s(*this, "InputStmt");
        matchKw("in");
        match(INPUT);
        match(IDENTIFIER);
        while (cur.type == INPUT)
        {
            advance();
            match(IDENTIFIER);
        }
        match(SEMICOLON);
    }

    void OutputStmt()
    {
        Scope s(*this, "OutputStmt");
        matchKw("out");
        match(OUTPUT);
        Expr();
        while (cur.type == OUTPUT)
        {
            advance();
            Expr();
        }
        match(SEMICOLON);
    }

    void Expr()
    {
        Scope s(*this, "Expr");
        AddExpr();
        while (isRelOp())
        {
            advance();
            AddExpr();
        }
    }

    void AddExpr()
    {
        Scope s(*this, "AddExpr");
        Term();
        while (cur.type == PLUS || cur.type == MINUS)
        {
            advance();
            Term();
        }
    }

    void Term()
    {
        Scope s(*this, "Term");
        Factor();
        while (cur.type == MULTIPLY || cur.type == DIVIDE || cur.type == MODULO)
        {
            advance();
            Factor();
        }
    }

    void Factor()
    {
        Scope s(*this, "Factor");
        switch (cur.type)
        {
            case LPAREN:
                advance();
                Expr();
                match(RPAREN);
                break;
            case IDENTIFIER:
                advance();
                if (cur.type == INCREMENT || cur.type == DECREMENT)  
                    advance();
                else if (cur.type == LPAREN)                         
                {
                    advance();
                    ArgList();
                    match(RPAREN);
                }
                break;
            case INTEGER:
            case CHAR_LITERAL:
                advance();
                break;
            case NOT:
            case MINUS:
                advance();
                Factor();
                break;
            case INCREMENT:
            case DECREMENT:
                advance();
                match(IDENTIFIER);
                break;
            default:
                throw string("Syntax error: unexpected " + found() + " in expression");
        }
    }
};
