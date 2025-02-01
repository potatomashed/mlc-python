#ifndef MLC_SYM_OP_H_
#define MLC_SYM_OP_H_

#include "./ir.h"
#include <mlc/core/all.h>

namespace mlc {
namespace sym {

struct SymbolicLimits {
  inline static Expr pos_inf_ = Var("pos_inf");
  inline static Expr neg_inf_ = Var("neg_inf");
};

inline Expr pos_inf() { return SymbolicLimits::pos_inf_; }
inline Expr neg_inf() { return SymbolicLimits::neg_inf_; }
inline bool is_pos_inf(const Expr &value) { return value.get() == SymbolicLimits::pos_inf_.get(); }
inline bool is_neg_inf(const Expr &value) { return value.get() == SymbolicLimits::neg_inf_.get(); }

struct Op_ {
  static inline Op left_shift = Op::Get("mlc.sym.left_shift");
  static inline Op right_shift = Op::Get("mlc.sym.shift_right");
  static inline Op bitwise_and = Op::Get("mlc.sym.bitwise_and");
  static inline Op bitwise_or = Op::Get("mlc.sym.bitwise_or");
  static inline Op bitwise_xor = Op::Get("mlc.sym.bitwise_xor");
  static inline Op bitwise_not = Op::Get("mlc.sym.bitwise_not");
  static inline Op if_then_else = Op::Get("mlc.sym.if_then_else");
};

MLC_API Expr add(Expr a, Expr b);
MLC_API Expr sub(Expr a, Expr b);
MLC_API Expr mul(Expr a, Expr b);
MLC_API Expr neg(Expr a);
MLC_API Expr truncdiv(Expr a, Expr b);
MLC_API Expr truncmod(Expr a, Expr b);
MLC_API Expr floordiv(Expr a, Expr b);
MLC_API Expr floormod(Expr a, Expr b);
MLC_API Expr ceildiv(Expr a, Expr b);
MLC_API Expr min(Expr a, Expr b);
MLC_API Expr max(Expr a, Expr b);
MLC_API Expr if_then_else(Expr cond, Expr true_value, Expr false_value);
MLC_API Expr greater(Expr a, Expr b);
MLC_API Expr greater_equal(Expr a, Expr b);
MLC_API Expr less(Expr a, Expr b);
MLC_API Expr less_equal(Expr a, Expr b);
MLC_API Expr equal(Expr a, Expr b);
MLC_API Expr not_equal(Expr a, Expr b);
MLC_API Expr logical_and(Expr a, Expr b);
MLC_API Expr logical_or(Expr a, Expr b);
MLC_API Expr logical_not(Expr a);
MLC_API Expr right_shift(Expr a, Expr b);
MLC_API Expr left_shift(Expr a, Expr b);
MLC_API Expr bitwise_and(Expr a, Expr b);
MLC_API Expr bitwise_or(Expr a, Expr b);
MLC_API Expr bitwise_xor(Expr a, Expr b);
MLC_API Expr bitwise_neg(Expr a);

inline Expr operator+(Expr a, Expr b) { return add(a, b); }
inline Expr operator-(Expr a, Expr b) { return sub(a, b); }
inline Expr operator*(Expr a, Expr b) { return mul(a, b); }
// inline Expr operator/(Expr a, Expr b) { return floordiv(a, b); }
// inline Expr operator%(Expr a, Expr b) { return floormod(a, b); }
inline Expr operator-(Expr a) { return neg(a); }
inline Expr operator<<(Expr a, Expr b) { return left_shift(a, b); }
inline Expr operator>>(Expr a, Expr b) { return right_shift(a, b); }
inline Expr operator>(Expr a, Expr b) { return greater(a, b); }
inline Expr operator>=(Expr a, Expr b) { return greater_equal(a, b); }
inline Expr operator<(Expr a, Expr b) { return less(a, b); }
inline Expr operator<=(Expr a, Expr b) { return less_equal(a, b); }
inline Expr operator==(Expr a, Expr b) { return equal(a, b); }
inline Expr operator!=(Expr a, Expr b) { return not_equal(a, b); }
inline Expr operator&&(Expr a, Expr b) { return logical_and(a, b); }
inline Expr operator||(Expr a, Expr b) { return logical_or(a, b); }
inline Expr operator!(Expr a) { return logical_not(a); }
inline Expr operator&(Expr a, Expr b) { return bitwise_and(a, b); }
inline Expr operator|(Expr a, Expr b) { return bitwise_or(a, b); }
inline Expr operator^(Expr a, Expr b) { return bitwise_xor(a, b); }
inline Expr operator~(Expr a) { return bitwise_neg(a); }

} // namespace sym
} // namespace mlc

#endif // MLC_SYM_OP_H_
