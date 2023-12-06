#pragma once

#include <fwd_stmt.hpp>

#include <const_expr.hpp>
#include <ops.hpp>

#include <memory>
#include <utility> // std::move
#include <variant>
#include <vector>

namespace pas {
namespace ast {

// Expr is not defined yet, we don't know it's size yet.
//   And variant must allocate members inside of it,
//   not on heap or etc.
//   Like the problem is that we need expr inside expr
//   subvariants.
class Expr;
using ExprUP = std::unique_ptr<Expr>;

class DesignatorFieldAccess {
  public: DesignatorFieldAccess() = default;

public:
  DesignatorFieldAccess(std::string ident) : ident_(std::move(ident)) {}

private:
  std::string ident_;
};
class DesignatorArrayAccess {
  public: DesignatorArrayAccess() = default;

public:
  DesignatorArrayAccess(std::vector<ExprUP> expr_list)
      : expr_list_(std::move(expr_list)) {}

private:
  std::vector<ExprUP> expr_list_;
};
class DesignatorPointerAccess {
  public: DesignatorPointerAccess() = default;
};

enum class DesignatorItemKind {
    FieldAccess   = 0,
    ArrayAccess   = 1,
    PointerAccess = 2
};

using DesignatorItem = std::variant<DesignatorFieldAccess, DesignatorArrayAccess, DesignatorPointerAccess>;

class Designator {
  public: Designator() = default;

public:
  Designator(std::string ident,
             std::vector<DesignatorItem> items)
      : ident_(std::move(ident)), items_(std::move(items)) {}

private:
  std::string ident_;
  std::vector<DesignatorItem> items_;
};

enum class FactorKind {
  Identifier = 0,
  Number = 1,
  Bool = 2,
  Nil = 3,
  Designator = 4,
  Expr = 5,
  Negation = 6,
  FuncCall = 7
};

class Negation;
class FuncCall;

using NegationUP = std::unique_ptr<Negation>;
using FuncCallUP = std::unique_ptr<FuncCall>;

using Factor = std::variant<std::string, int, bool, std::monostate, Designator, ExprUP, NegationUP, FuncCallUP>;

class Negation {
  public: Negation() = default;

public:
  Negation(Factor factor)
      : factor_(std::move(factor)) {}

private:
  Factor factor_;
};


class Term {
  public: Term() = default;

public:
  struct Op {
    MultOp op;
    Factor factor;
  };
  Term(Factor start_factor, std::vector<Op> ops)
      : start_factor_(std::move(start_factor)), ops_(std::move(ops)) {
  }

private:
  Factor          start_factor_;
  std::vector<Op> ops_;
};

class SimpleExpr {
  public: SimpleExpr() = default;

public:
  struct Op {
    AddOp op;
    Term term;
  };
  SimpleExpr(std::optional<UnaryOp> unary_op, Term start_term,
               std::vector<Op> ops)
      : unary_op_(std::move(unary_op)), start_term_(std::move(start_term)),
        ops_(std::move(ops)) {
  }

private:
  std::optional<UnaryOp> unary_op_;
  Term start_term_;
  std::vector<Op> ops_;
};

class Expr {
  public: Expr() = default;

public:
  struct Op {
    RelOp rel;
    SimpleExpr expr;
  };
  Expr(SimpleExpr start_expr,
         std::optional<Op> op = std::optional<Op>())
      : start_expr_(std::move(start_expr)), op_(std::move(op)) {
  }

private:
  SimpleExpr        start_expr_;
  std::optional<Op> op_;
};

// These are allowed only in expressions.
//   This is a kind of an expr, because
//   it returns a value.
// We could allow to return void types,
//   but then we'd need to make expressions
//   also be statements. I.e. expression
//   evaluation should become a statement
//   then.
class FuncCall {
  public: FuncCall() = default;

public:
  FuncCall(std::string func_ident, std::vector<Expr> params)
      : func_ident_(std::move(func_ident)), params_(std::move(params)) {}

private:
  std::string func_ident_;
  std::vector<Expr> params_;
};

}
}