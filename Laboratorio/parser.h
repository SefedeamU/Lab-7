#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "ast.h"

class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;

    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    bool isBodyEnd();

    Programa* parseP();
    FunDec*   parseFunDec();
    Body*     parseBody();
    VarDec*   parseVarDec();
    Stmt*     parsestmt();

    Exp* parseCEXP();
    Exp* parseLogAnd();
    Exp* parseNot();
    Exp* parseComp();
    Exp* parseAdd();
    Exp* parseE();
    Exp* parseT();
    Exp* parseF();

public:
    Parser(Scanner* scanner);
    Programa* parseProgram();
};

#endif
