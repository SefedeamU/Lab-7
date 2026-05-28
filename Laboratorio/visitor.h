#ifndef VISITOR_H
#define VISITOR_H

#include "ast.h"
#include <list>
#include <unordered_map>
#include "environment.h"

class BinaryExp;
class CBinaryExp;
class NumberExp;
class IdExp;
class SqrtExp;
class NotExp;
class FunCallExp;

class Visitor {
public:
    virtual int  visit(BinaryExp* exp)     = 0;
    virtual int  visit(CBinaryExp* exp)    = 0;
    virtual int  visit(NumberExp* exp)     = 0;
    virtual int  visit(IdExp* exp)         = 0;
    virtual int  visit(SqrtExp* exp)       = 0;
    virtual int  visit(NotExp* exp)        = 0;
    virtual int  visit(FunCallExp* exp)    = 0;
    virtual void visit(AsignStmt* stm)     = 0;
    virtual void visit(PrintStmt* stm)     = 0;
    virtual void visit(IncrementStmt* stm) = 0;
    virtual void visit(ReturnStmt* stm)    = 0;
    virtual void visit(BreakStmt* stm)     = 0;
    virtual void visit(IfStmt* stm)        = 0;
    virtual void visit(WhileStmt* stm)     = 0;
    virtual void visit(VarDec* stm)        = 0;
    virtual void visit(Body* stm)          = 0;
    virtual void visit(Programa* prog)     = 0;
};

class PrintVisitor : public Visitor {
public:
    int  visit(BinaryExp* exp)     override;
    int  visit(CBinaryExp* exp)    override;
    int  visit(NumberExp* exp)     override;
    int  visit(IdExp* exp)         override;
    int  visit(SqrtExp* exp)       override;
    int  visit(NotExp* exp)        override;
    int  visit(FunCallExp* exp)    override;
    void visit(AsignStmt* stm)     override;
    void visit(PrintStmt* stm)     override;
    void visit(IncrementStmt* stm) override;
    void visit(ReturnStmt* stm)    override;
    void visit(BreakStmt* stm)     override;
    void visit(IfStmt* stm)        override;
    void visit(WhileStmt* stm)     override;
    void visit(VarDec* stm)        override;
    void visit(Body* b)            override;
    void visit(Programa* prog)     override;
    void imprimir(Programa* program);
};

class EVALVisitor : public Visitor {
public:
    Environment<int> memoria;
    unordered_map<string, FunDec*> funciones;

    int  visit(BinaryExp* exp)     override;
    int  visit(CBinaryExp* exp)    override;
    int  visit(NumberExp* exp)     override;
    int  visit(IdExp* exp)         override;
    int  visit(SqrtExp* exp)       override;
    int  visit(NotExp* exp)        override;
    int  visit(FunCallExp* exp)    override;
    void visit(AsignStmt* stm)     override;
    void visit(PrintStmt* stm)     override;
    void visit(IncrementStmt* stm) override;
    void visit(ReturnStmt* stm)    override;
    void visit(BreakStmt* stm)     override;
    void visit(IfStmt* stm)        override;
    void visit(WhileStmt* stm)     override;
    void visit(VarDec* stm)        override;
    void visit(Body* b)            override;
    void visit(Programa* prog)     override;
    void interprete(Programa* program);
};

#endif
