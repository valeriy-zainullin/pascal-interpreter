#pragma once

#include <const_expr.hpp>
#include <type.hpp>
#include <expr.hpp>

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace pas {
namespace ast {

enum class StmtKind : size_t {
  Assignment = 0,
  ProcCall = 1,
  If = 2,
  Case = 3,
  While = 4,
  Repeat = 5,
  For = 6,
  Memory = 7,
  Empty = 8
};

class Assignment;
class ProcCall;
class IfStmt;
class CaseStmt;
class WhileStmt;
class RepeatStmt;
class ForStmt;
class MemoryStmt;
class StmtSeq;
class EmptyStmt;

using AssignmentUP = std::unique_ptr<Assignment>;
using ProcCallUP   = std::unique_ptr<ProcCall>;
using IfStmtUP     = std::unique_ptr<IfStmt>;
using CaseStmtUP   = std::unique_ptr<CaseStmt>;
using WhileStmtUP  = std::unique_ptr<WhileStmt>;
using RepeatStmtUP = std::unique_ptr<RepeatStmt>;
using ForStmtUP    = std::unique_ptr<ForStmt>;
using MemoryStmtUP = std::unique_ptr<MemoryStmt>;
using StmtSeqUP    = std::unique_ptr<StmtSeq>;
using EmptyStmtUP  = std::unique_ptr<EmptyStmt>;

using Stmt = std::variant<AssignmentUP, ProcCallUP, IfStmtUP, CaseStmtUP, WhileStmtUP,
                          RepeatStmtUP, ForStmtUP, MemoryStmtUP, StmtSeqUP, EmptyStmtUP>;

class StmtSeq {
  public: StmtSeq() = default;

public:
  StmtSeq(std::vector<Stmt> stmts)
      : stmts_(std::move(stmts)) {}

private:
  std::vector<Stmt> stmts_;
};

class EmptyStmt {
  public:
    EmptyStmt() = default;
};

class IfStmt {
  public: IfStmt() = default;

public:
  IfStmt(Expr cond_expr, Stmt if_stmt,
         std::optional<Stmt> else_stmt = std::optional<Stmt>())
      : cond_expr_(std::move(cond_expr)),
        then_stmt_(std::move(if_stmt)), else_stmt_(std::move(else_stmt)) {}

private:
  Expr                cond_expr_;
  Stmt                then_stmt_;
  std::optional<Stmt> else_stmt_;
};

class Case {
  public: Case() = default;

public:
  Case(std::vector<ConstExpr> labels, Stmt then_stmt)
      : labels_(std::move(labels)), then_stmt_(std::move(then_stmt)) {}

private:
  std::vector<ConstExpr> labels_;
  Stmt then_stmt_;
};

class CaseStmt {
  public: CaseStmt() = default;

public:
  CaseStmt(Expr cond_expr, std::vector<Case> cases) : cond_expr_(std::move(cond_expr)), cases_(std::move(cases)) {}

private:
  Expr cond_expr_;
  std::vector<Case> cases_;
};

class WhileStmt {
  public: WhileStmt() = default;

public:
    WhileStmt(Expr cond_expr, Stmt inner_stmt)
        : cond_expr_(std::move(cond_expr)), inner_stmt_(std::move(inner_stmt)) {}
private:
    Expr cond_expr_;
    Stmt inner_stmt_;
};

class RepeatStmt {
    public: RepeatStmt() = default;

  public:
    RepeatStmt(std::vector<Stmt> inner_stmts,
               Expr cond_expr)
        : inner_stmts_(std::move(inner_stmts)),
          cond_expr_(std::move(cond_expr)) {}

private:
    std::vector<Stmt> inner_stmts_;
    Expr cond_expr_;
};

enum class WhichWay {
    To,
    DownTo
};

class ForStmt {
    public: ForStmt() = default;

  public:
    ForStmt(std::string ident, Expr start_val_expr,
            WhichWay dir, Expr finish_val_expr,
            Stmt inner_stmt)
        : ident_(std::move(ident)), start_val_expr_(std::move(start_val_expr_)),
          finish_val_expr_(std::move(finish_val_expr)), inner_stmt_(std::move(inner_stmt)) {
    }

private:
    std::string ident_;
    Expr        start_val_expr_;
    WhichWay    dir_;
    Expr        finish_val_expr_;
    Stmt        inner_stmt_;
};

class MemoryStmt {
    public: MemoryStmt() = default;

  public:
    enum class Kind {
        Dispose = 0,
        New = 1
    };

    // New or Dispose, identifier of pointer typed variable
    //   allocating memory for.
    MemoryStmt(Kind kind, std::string ident)
        : kind_(std::move(kind)), ident_(std::move(ident)) {}

private:
    Kind        kind_;
    std::string ident_;
};

class Assignment {
  public: Assignment() = default;

public:
  Assignment(Designator designator, Expr expr)
      : designator_(std::move(designator)), expr_(std::move(expr)) {}

private:
  Designator designator_;
  Expr expr_;
};
class ProcCall {
  public: ProcCall() = default;

public:
  ProcCall(std::string proc_ident, std::vector<Expr> params)
      : proc_ident_(std::move(proc_ident)), params_(std::move(params)) {}

private:
  std::string proc_ident_;
  std::vector<Expr> params_;
};

}
}