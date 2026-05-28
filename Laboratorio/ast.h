#ifndef AST_H
#define AST_H

#include <string>
#include <unordered_map>
#include <list>
#include <ostream>

using namespace std;

struct ReturnException {
    int value;
    explicit ReturnException(int v) : value(v) {}
};

struct BreakException {};

class Visitor;
class Body;

enum BinaryOp {
    PLUS_OP, MINUS_OP, MUL_OP, DIV_OP, POW_OP, AND_OP, OR_OP
};

enum CBinaryOp {
    LESSTHAN_OP, LESSEQUAL_OP, EQUALEQUAL_OP
};

class Exp {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    static string binopToChar(BinaryOp op);
    static string CbinopToChar(CBinaryOp op);
};

class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    int accept(Visitor* visitor) override;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();
};

class CBinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    CBinaryOp op;
    bool negated = false;
    int accept(Visitor* visitor) override;
    CBinaryExp(Exp* l, Exp* r, CBinaryOp op);
    ~CBinaryExp();
};

class NumberExp : public Exp {
public:
    int value;
    int accept(Visitor* visitor) override;
    NumberExp(int v);
    ~NumberExp();
};

class IdExp : public Exp {
public:
    string value;
    int accept(Visitor* visitor) override;
    IdExp(string v);
    ~IdExp();
};

class SqrtExp : public Exp {
public:
    Exp* value;
    int accept(Visitor* visitor) override;
    SqrtExp(Exp* v);
    ~SqrtExp();
};

class NotExp : public Exp {
public:
    Exp* expr;
    int accept(Visitor* visitor) override;
    NotExp(Exp* e);
    ~NotExp();
};

class FunCallExp : public Exp {
public:
    string nombre;
    list<Exp*> args;
    int accept(Visitor* visitor) override;
    FunCallExp(string n);
    ~FunCallExp();
};

class Stmt {
public:
    virtual void accept(Visitor* visitor) = 0;
    virtual ~Stmt() = 0;
};

class PrintStmt : public Stmt {
public:
    Exp* exp;
    void accept(Visitor* visitor) override;
    PrintStmt(Exp* e);
    ~PrintStmt();
};

class AsignStmt : public Stmt {
public:
    string variable;
    Exp* exp;
    void accept(Visitor* visitor) override;
    AsignStmt(string v, Exp* e);
    ~AsignStmt();
};

class IncrementStmt : public Stmt {
public:
    string variable;
    void accept(Visitor* visitor) override;
    IncrementStmt(string v);
    ~IncrementStmt();
};

class ReturnStmt : public Stmt {
public:
    Exp* exp;
    void accept(Visitor* visitor) override;
    ReturnStmt(Exp* e = nullptr);
    ~ReturnStmt();
};

class BreakStmt : public Stmt {
public:
    void accept(Visitor* visitor) override;
    BreakStmt();
    ~BreakStmt();
};

class IfStmt : public Stmt {
public:
    Exp* condicion;
    Body* cuerpodelif;
    Body* cuerpodelelse;
    bool hayelse = false;
    void accept(Visitor* visitor) override;
    IfStmt(Exp* e);
    ~IfStmt();
};

class WhileStmt : public Stmt {
public:
    Exp* condicion;
    Body* cuerpo;
    void accept(Visitor* visitor) override;
    WhileStmt(Exp* e);
    ~WhileStmt();
};

class VarDec {
public:
    string tipo;
    list<string> chicharron;
    void accept(Visitor* visitor);
    VarDec();
    ~VarDec();
};

class Body {
public:
    list<VarDec*> vdlist;
    list<Stmt*> slist;
    void accept(Visitor* visitor);
    Body();
    ~Body();
};

struct Param {
    string tipo;
    string nombre;
    Param(string t, string n) : tipo(t), nombre(n) {}
};

class FunDec {
public:
    string retorno;
    string nombre;
    list<Param*> params;
    Body* cuerpo;
    FunDec();
    ~FunDec();
};

class Programa {
public:
    list<VarDec*> globalVars;
    list<FunDec*> funciones;
    void accept(Visitor* visitor);
    ~Programa();
    Programa();
};

#endif
