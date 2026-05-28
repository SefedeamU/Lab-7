#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <ostream>

using namespace std;

class Token {
public:
    enum Type {
        PLUS, MINUS, MUL, DIV, POW,
        LPAREN, RPAREN, COMA, SEMICOL, COLON,
        PRINT, IF, ELSE, THEN, ENDIF,
        VAR, WHILE, DO, ENDWHILE,
        BOOL, BREAK, AND, OR, NOT,
        SQRT,
        FUN, ENDFUN, RETURN, TRUE, FALSE,
        ASSIGN, PLUSPLUS,
        LESSTHAN, LESSTHANEQUAL, EQUALEQUAL,
        NUM, ERR, ID, END
    };

    Type type;
    string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const string& source, int first, int last);

    friend ostream& operator<<(ostream& outs, const Token& tok);
    friend ostream& operator<<(ostream& outs, const Token* tok);
};

#endif
