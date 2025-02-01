#include <mlc/sym/all.h>

namespace mlc {
namespace sym {

struct OpRegistry {
  Dict<Str, Op> registry;

  Op RegisterOrGet(Str name) {
    auto it = registry.find(name);
    if (it == registry.end()) {
      Op ret(name);
      registry.Set(name, ret);
      return ret;
    }
    return (*it).second;
  }

  static OpRegistry &Global() {
    static OpRegistry inst;
    return inst;
  }
};

Op Op::Get(Str name) { return OpRegistry::Global().RegisterOrGet(name); }

// specialization of constant folders.
Optional<Expr> Add::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(pa->value + pb->value);
  }
  if (pa && pa->value == 0) {
    return b;
  }
  if (pb && pb->value == 0) {
    return a;
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(fa->value + fb->value);
  }
  if (fa && fa->value == 0) {
    return b;
  }
  if (fb && fb->value == 0) {
    return a;
  }
  return Null;
}

Optional<Expr> Sub::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(pa->value - pb->value);
  }
  if (pb && pb->value == 0) {
    return a;
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(fa->value - fb->value);
  }
  if (fb && fb->value == 0) {
    return a;
  }
  return Null;
}

Optional<Expr> Mul::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(pa->value * pb->value);
  }
  if (pa) {
    if (pa->value == 1)
      return b;
    if (pa->value == 0)
      return a;
  }
  if (pb) {
    if (pb->value == 1)
      return a;
    if (pb->value == 0)
      return b;
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(fa->value * fb->value);
  }
  if (fa) {
    if (fa->value == 1)
      return b;
    if (fa->value == 0)
      return a;
  }
  if (fb) {
    if (fb->value == 1)
      return a;
    if (fb->value == 0)
      return b;
  }
  return Null;
}

Optional<Expr> Div::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pb && pb->value == 0) {
    MLC_THROW(ValueError) << "Divide by zero";
  }
  if (pa && pb) {
    return IntImm(pa->value / pb->value);
  }
  if (pa && pa->value == 0) {
    return a;
  }
  if (pb && pb->value == 1) {
    return a;
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fb && fb->value == 0) {
    MLC_THROW(ValueError) << "Divide by zero";
  }
  if (fa && fb) {
    return FloatImm(fa->value / fb->value);
  }
  if (fa && fa->value == 0) {
    return a;
  }
  if (fb && fb->value == 1) {
    return a;
  }
  return Null;
}

Optional<Expr> Mod::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pb && pb->value == 0) {
    MLC_THROW(ValueError) << "Divide by zero";
  }
  if (pa && pb) {
    return IntImm(pa->value % pb->value);
  }
  if (pa && pa->value == 0) {
    return a;
  }
  if (pb && pb->value == 1) {
    return IntImm(0);
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fb && fb->value == 0) {
    MLC_THROW(ValueError) << "Divide by zero";
  }
  if (fa && fb) {
    return FloatImm(std::fmod(fa->value, fb->value));
  }
  if (fa && fa->value == 0) {
    return a;
  }
  if (fb && fb->value == 1) {
    return FloatImm(0.0);
  }
  return Null;
}

Optional<Expr> FloorDiv::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pb && pb->value == 0) {
    MLC_THROW(ValueError) << "Divide by zero";
  }
  if (pa && pb) {
    return IntImm(pa->value / pb->value);
  }
  if (pa && pa->value == 0) {
    return a;
  }
  if (pb && pb->value == 1) {
    return a;
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fb && fb->value == 0) {
    MLC_THROW(ValueError) << "Divide by zero";
  }
  if (fa && fb) {
    return FloatImm(std::floor(fa->value / fb->value));
  }
  if (fa && fa->value == 0) {
    return a;
  }
  if (fb && fb->value == 1) {
    return a;
  }
  return Null;
}

Optional<Expr> FloorMod::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pb && pb->value == 0) {
    MLC_THROW(ValueError) << "Divide by zero";
  }
  if (pa && pb) {
    return IntImm(pa->value % pb->value);
  }
  if (pa && pa->value == 0) {
    return a;
  }
  if (pb && pb->value == 1) {
    return IntImm(0);
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fb && fb->value == 0) {
    MLC_THROW(ValueError) << "Divide by zero";
  }
  if (fa && fb) {
    return FloatImm(std::fmod(fa->value, fb->value));
  }
  if (fa && fa->value == 0) {
    return a;
  }
  if (fb && fb->value == 1) {
    return FloatImm(0.0);
  }
  return Null;
}

Optional<Expr> Min::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(std::min(pa->value, pb->value));
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(std::min(fa->value, fb->value));
  }
  if (a.get() == b.get()) {
    return a;
  }
  return Null;
}

Optional<Expr> Max::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(std::max(pa->value, pb->value));
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(std::max(fa->value, fb->value));
  }
  if (a.get() == b.get()) {
    return a;
  }
  return Null;
}

Optional<Expr> GT::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(pa->value > pb->value);
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(fa->value > fb->value);
  }
  return Null;
}

Optional<Expr> GE::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(pa->value >= pb->value);
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(fa->value >= fb->value);
  }
  return Null;
}

Optional<Expr> LT::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(pa->value < pb->value);
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(fa->value < fb->value);
  }
  return Null;
}

Optional<Expr> LE::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(pa->value <= pb->value);
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(fa->value <= fb->value);
  }
  return Null;
}

Optional<Expr> EQ::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(pa->value == pb->value);
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(fa->value == fb->value);
  }
  return Null;
}

Optional<Expr> NE::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pb) {
    return IntImm(pa->value != pb->value);
  }
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  const FloatImmObj *fb = b.TryCast<FloatImmObj>();
  if (fa && fb) {
    return FloatImm(fa->value != fb->value);
  }
  return Null;
}

Optional<Expr> And::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pa->value) {
    return b;
  }
  if (pa && !pa->value) {
    return a;
  }
  if (pb && pb->value) {
    return a;
  }
  if (pb && !pb->value) {
    return b;
  }
  return Null;
}

Optional<Expr> Or::TryConstFold(Expr a, Expr b) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const IntImmObj *pb = b.TryCast<IntImmObj>();
  if (pa && pa->value) {
    return a;
  }
  if (pa && !pa->value) {
    return b;
  }
  if (pb && pb->value) {
    return b;
  }
  if (pb && !pb->value) {
    return a;
  }
  return Null;
}

Optional<Expr> Not::TryConstFold(Expr a) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  if (pa) {
    return IntImm(!(pa->value));
  }
  return Null;
}

Expr add(Expr a, Expr b) {
  if (Optional<Expr> ret = Add::TryConstFold(a, b); ret.defined())
    return ret.value();
  return Add(a, b);
}

Expr sub(Expr a, Expr b) {
  if (Optional<Expr> ret = Sub::TryConstFold(a, b); ret.defined())
    return ret.value();
  return Sub(a, b);
}

Expr mul(Expr a, Expr b) {
  if (Optional<Expr> ret = Mul::TryConstFold(a, b); ret.defined())
    return ret.value();
  return Mul(a, b);
}

Expr neg(Expr a) {
  const IntImmObj *pa = a.TryCast<IntImmObj>();
  const FloatImmObj *fa = a.TryCast<FloatImmObj>();
  if (pa)
    return IntImm(-pa->value);
  if (fa)
    return FloatImm(-fa->value);
  return sub(IntImm(0), a);
}

Expr truncdiv(Expr a, Expr b) { // truncdiv
  if (Optional<Expr> ret = Div::TryConstFold(a, b); ret.defined())
    return ret.value();
  return Div(a, b);
}

Expr truncmod(Expr a, Expr b) {
  if (Optional<Expr> ret = Mod::TryConstFold(a, b); ret.defined())
    return ret.value();
  return Mod(a, b);
}

Expr floordiv(Expr a, Expr b) {
  if (Optional<Expr> ret = FloorDiv::TryConstFold(a, b); ret.defined())
    return ret.value();
  return FloorDiv(a, b);
}

Expr floormod(Expr a, Expr b) {
  if (Optional<Expr> ret = FloorMod::TryConstFold(a, b); ret.defined())
    return ret.value();
  return FloorMod(a, b);
}

Expr ceildiv(Expr a, Expr b) {
  Expr numerator = a + b - IntImm(1);
  if (Optional<Expr> ret = FloorDiv::TryConstFold(numerator, b); ret.defined())
    return ret.value();
  return FloorDiv(numerator, b);
}

Expr min(Expr a, Expr b) {
  // inf-aware simplificaiton
  if (is_pos_inf(a))
    return b;
  if (is_neg_inf(a))
    return a;
  if (is_pos_inf(b))
    return a;
  if (is_neg_inf(b))
    return b;
  if (auto ret = Min::TryConstFold(a, b); ret.defined())
    return ret.value();
  return Min(a, b);
}

Expr max(Expr a, Expr b) {
  // inf-aware simplificaiton
  if (is_pos_inf(a))
    return a;
  if (is_neg_inf(a))
    return b;
  if (is_pos_inf(b))
    return b;
  if (is_neg_inf(b))
    return a;
  if (auto ret = Max::TryConstFold(a, b); ret.defined())
    return ret.value();
  return Max(a, b);
}

// if_then_else
Expr if_then_else(Expr cond, Expr true_value, Expr false_value) {
  if (const IntImmObj *op = cond.TryCast<IntImmObj>()) {
    if (op->value != 0) {
      return true_value;
    } else {
      return false_value;
    }
  }
  return Call(Op_::if_then_else, List<Expr>{cond, true_value, false_value});
}

Expr greater(Expr a, Expr b) {
  if (auto ret = GT::TryConstFold(a, b); ret.defined())
    return ret.value();
  return GT(a, b);
}

Expr greater_equal(Expr a, Expr b) {
  if (auto ret = GE::TryConstFold(a, b); ret.defined())
    return ret.value();
  return GE(a, b);
}

Expr less(Expr a, Expr b) {
  if (auto ret = LT::TryConstFold(a, b); ret.defined())
    return ret.value();
  return LT(a, b);
}

Expr less_equal(Expr a, Expr b) {
  if (auto ret = LE::TryConstFold(a, b); ret.defined())
    return ret.value();
  return LE(a, b);
}

Expr equal(Expr a, Expr b) {
  if (auto ret = EQ::TryConstFold(a, b); ret.defined())
    return ret.value();
  return EQ(a, b);
}

Expr not_equal(Expr a, Expr b) {
  if (auto ret = NE::TryConstFold(a, b); ret.defined())
    return ret.value();
  return NE(a, b);
}

Expr logical_and(Expr a, Expr b) {
  if (auto ret = And::TryConstFold(a, b); ret.defined())
    return ret.value();
  return And(a, b);
}

Expr logical_or(Expr a, Expr b) {
  if (auto ret = Or::TryConstFold(a, b); ret.defined())
    return ret.value();
  return Or(a, b);
}

Expr logical_not(Expr a) {
  if (auto ret = Not::TryConstFold(a); ret.defined())
    return ret.value();
  return Not(a);
}

Expr right_shift(Expr a, Expr b) {
  if (const IntImmObj *pa = a.TryCast<IntImmObj>()) {
    if (const IntImmObj *pb = b.TryCast<IntImmObj>()) {
      if (pb->value < 0) {
        MLC_THROW(ValueError) << "Shift amount must be non-negative, but got: " << pb->value;
      }
      return IntImm(pa->value >> pb->value);
    }
  }
  return Call(Op_::right_shift, List<Expr>{a, b});
}

// shift left
Expr left_shift(Expr a, Expr b) {
  if (const IntImmObj *pa = a.TryCast<IntImmObj>()) {
    if (const IntImmObj *pb = b.TryCast<IntImmObj>()) {
      if (pb->value < 0) {
        MLC_THROW(ValueError) << "Shift amount must be non-negative, but got: " << pb->value;
      }
      return IntImm(pa->value << pb->value);
    }
  }
  return Call(Op_::left_shift, List<Expr>{a, b});
}

// bitwise and
Expr bitwise_and(Expr a, Expr b) {
  if (const IntImmObj *pa = a.TryCast<IntImmObj>()) {
    if (const IntImmObj *pb = b.TryCast<IntImmObj>()) {
      return IntImm(pa->value & pb->value);
    }
  }
  return Call(Op_::bitwise_and, List<Expr>{a, b});
}

// bitwise_or
Expr bitwise_or(Expr a, Expr b) {
  if (const IntImmObj *pa = a.TryCast<IntImmObj>()) {
    if (const IntImmObj *pb = b.TryCast<IntImmObj>()) {
      return IntImm(pa->value | pb->value);
    }
  }
  return Call(Op_::bitwise_or, List<Expr>{a, b});
}

// bitwise_xor
Expr bitwise_xor(Expr a, Expr b) {
  if (const IntImmObj *pa = a.TryCast<IntImmObj>()) {
    if (const IntImmObj *pb = b.TryCast<IntImmObj>()) {
      return IntImm(pa->value ^ pb->value);
    }
  }
  return Call(Op_::bitwise_xor, List<Expr>{a, b});
}

Expr bitwise_neg(Expr a) {
  if (const IntImmObj *pa = a.TryCast<IntImmObj>()) {
    return IntImm(~pa->value);
  }
  return Call(Op_::bitwise_not, List<Expr>{a});
}

// ExprVisitor

template <typename T, typename F> inline void VisitArray(const List<T> &arr, F fvisit) {
  for (int64_t i = 0; i < arr.size(); i++) {
    fvisit(arr[i]);
  }
}

void ExprVisitor::VisitExpr_(const VarObj *) {}
void ExprVisitor::VisitExpr_(const SizeVarObj *op) { this->VisitExpr_(reinterpret_cast<const VarObj *>(op)); }
void ExprVisitor::VisitExpr_(const IntImmObj *) {}
void ExprVisitor::VisitExpr_(const FloatImmObj *) {}
void ExprVisitor::VisitExpr_(const StringImmObj *) {}
void ExprVisitor::VisitExpr_(const CastDTypeObj *op) { this->VisitExpr(op->value); }
void ExprVisitor::VisitExpr_(const AddObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const SubObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const MulObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const DivObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const ModObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const FloorDivObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const FloorModObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const MinObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const MaxObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const EQObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const NEObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const LTObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const LEObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const GTObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const GEObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const AndObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const OrObj *op) {
  this->VisitExpr(op->a);
  this->VisitExpr(op->b);
}
void ExprVisitor::VisitExpr_(const NotObj *op) { this->VisitExpr(op->a); }
void ExprVisitor::VisitExpr_(const SelectObj *op) {
  this->VisitExpr(op->cond);
  this->VisitExpr(op->true_value);
  this->VisitExpr(op->false_value);
}
void ExprVisitor::VisitExpr_(const RampObj *op) {
  this->VisitExpr(op->base);
  this->VisitExpr(op->stride);
  this->VisitExpr(op->lanes);
}
void ExprVisitor::VisitExpr_(const BroadcastObj *op) {
  this->VisitExpr(op->value);
  this->VisitExpr(op->lanes);
}
void ExprVisitor::VisitExpr_(const LetObj *op) {
  this->VisitExpr(op->var);
  this->VisitExpr(op->value);
  this->VisitExpr(op->body);
}
void ExprVisitor::VisitExpr_(const CallObj *op) {
  VisitArray(op->args, [this](const Expr &e) { this->VisitExpr(e); });
}
void ExprVisitor::VisitExpr_(const ShuffleObj *op) {
  VisitArray(op->indices, [this](const Expr &e) { this->VisitExpr(e); });
  VisitArray(op->vectors, [this](const Expr &e) { this->VisitExpr(e); });
}

// ExprMutator
template <typename T, typename F> inline List<T> MutateArray(const List<T> &arr, F fvisit) {
  bool changed = false;
  List<T> result;
  result.reserve(arr.size());
  for (int64_t i = 0; i < arr.size(); i++) {
    T a = arr[i];
    T b = fvisit(a);
    if (a.get() != b.get()) {
      changed = true;
    }
    result.push_back(b);
  }
  return changed ? result : arr;
}

Expr ExprMutator::VisitExpr_(const VarObj *op) { return Expr(op); }
Expr ExprMutator::VisitExpr_(const SizeVarObj *op) { return this->VisitExpr_(reinterpret_cast<const VarObj *>(op)); }
Expr ExprMutator::VisitExpr_(const IntImmObj *op) { return Expr(op); }
Expr ExprMutator::VisitExpr_(const FloatImmObj *op) { return Expr(op); }
Expr ExprMutator::VisitExpr_(const StringImmObj *op) { return Expr(op); }
Expr ExprMutator::VisitExpr_(const CastDTypeObj *op) {
  Expr value = this->VisitExpr(op->value);
  if (value.get() == op->value.get()) {
    return Expr(op);
  } else {
    return CastDType(value, op->dtype);
  }
}
#define MLC_SYM_EXPR_MUTATOR_BIN_OP(ObjRef)                                                                            \
  Expr ExprMutator::VisitExpr_(const ObjRef::TObj *op) {                                                               \
    Expr a = this->VisitExpr(op->a);                                                                                   \
    Expr b = this->VisitExpr(op->b);                                                                                   \
    if (a.get() == op->a.get() && b.get() == op->b.get()) {                                                            \
      return Expr(op);                                                                                                 \
    } else {                                                                                                           \
      return ObjRef(a, b);                                                                                             \
    }                                                                                                                  \
  }
MLC_SYM_EXPR_MUTATOR_BIN_OP(Add)
MLC_SYM_EXPR_MUTATOR_BIN_OP(Sub)
MLC_SYM_EXPR_MUTATOR_BIN_OP(Mul)
MLC_SYM_EXPR_MUTATOR_BIN_OP(Div)
MLC_SYM_EXPR_MUTATOR_BIN_OP(Mod)
MLC_SYM_EXPR_MUTATOR_BIN_OP(FloorDiv)
MLC_SYM_EXPR_MUTATOR_BIN_OP(FloorMod)
MLC_SYM_EXPR_MUTATOR_BIN_OP(Min)
MLC_SYM_EXPR_MUTATOR_BIN_OP(Max)
MLC_SYM_EXPR_MUTATOR_BIN_OP(EQ)
MLC_SYM_EXPR_MUTATOR_BIN_OP(NE)
MLC_SYM_EXPR_MUTATOR_BIN_OP(LT)
MLC_SYM_EXPR_MUTATOR_BIN_OP(LE)
MLC_SYM_EXPR_MUTATOR_BIN_OP(GT)
MLC_SYM_EXPR_MUTATOR_BIN_OP(GE)
MLC_SYM_EXPR_MUTATOR_BIN_OP(And)
MLC_SYM_EXPR_MUTATOR_BIN_OP(Or)
#undef MLC_SYM_EXPR_MUTATOR_BIN_OP

Expr ExprMutator::VisitExpr_(const NotObj *op) {
  Expr a = this->VisitExpr(op->a);
  if (a.get() == op->a.get()) {
    return Expr(op);
  } else {
    return Not(a);
  }
}
Expr ExprMutator::VisitExpr_(const SelectObj *op) {
  Expr cond = this->VisitExpr(op->cond);
  Expr true_value = this->VisitExpr(op->true_value);
  Expr false_value = this->VisitExpr(op->false_value);
  if (cond.get() == op->cond.get() && true_value.get() == op->true_value.get() &&
      false_value.get() == op->false_value.get()) {
    return Expr(op);
  } else {
    return Select(cond, true_value, false_value);
  }
}
Expr ExprMutator::VisitExpr_(const RampObj *op) {
  Expr base = this->VisitExpr(op->base);
  Expr stride = this->VisitExpr(op->stride);
  Expr lanes = this->VisitExpr(op->lanes);
  if (base.get() == op->base.get() && stride.get() == op->stride.get() && lanes.get() == op->lanes.get()) {
    return Expr(op);
  } else {
    return Ramp(base, stride, lanes);
  }
}
Expr ExprMutator::VisitExpr_(const BroadcastObj *op) {
  Expr value = this->VisitExpr(op->value);
  Expr lanes = this->VisitExpr(op->lanes);
  if (value.get() == op->value.get() && lanes.get() == op->lanes.get()) {
    return Expr(op);
  } else {
    return Broadcast(value, lanes);
  }
}
Expr ExprMutator::VisitExpr_(const LetObj *op) {
  Expr value = this->VisitExpr(op->value);
  Expr body = this->VisitExpr(op->body);
  if (value.get() == op->value.get() && body.get() == op->body.get()) {
    return Expr(op);
  } else {
    return Let(op->var, value, body);
  }
}
Expr ExprMutator::VisitExpr_(const CallObj *op) {
  List<Expr> args = MutateArray(op->args, [this](const Expr &e) { return this->VisitExpr(e); });
  if (args.get() == op->args.get()) {
    return Expr(op);
  } else {
    return Call(op->op, args);
  }
}
Expr ExprMutator::VisitExpr_(const ShuffleObj *op) {
  List<Expr> indices = MutateArray(op->indices, [this](const Expr &e) { return this->VisitExpr(e); });
  List<Expr> vectors = MutateArray(op->vectors, [this](const Expr &e) { return this->VisitExpr(e); });
  if (indices.get() == op->indices.get() && vectors.get() == op->vectors.get()) {
    return Expr(op);
  } else {
    return Shuffle(indices, vectors);
  }
}

// ExprDeepEqual

bool ExprDeepEqual::VisitExpr_(const VarObj *lhs, void *_rhs) {
  VarObj *rhs = static_cast<VarObj *>(_rhs);
  return lhs == rhs;
}
bool ExprDeepEqual::VisitExpr_(const SizeVarObj *lhs, void *_rhs) {
  SizeVarObj *rhs = static_cast<SizeVarObj *>(_rhs);
  return lhs == rhs;
}
bool ExprDeepEqual::VisitExpr_(const IntImmObj *lhs, void *_rhs) {
  IntImmObj *rhs = static_cast<IntImmObj *>(_rhs);
  return lhs->value == rhs->value;
}
bool ExprDeepEqual::VisitExpr_(const FloatImmObj *lhs, void *_rhs) {
  FloatImmObj *rhs = static_cast<FloatImmObj *>(_rhs);
  return lhs->value == rhs->value;
}
bool ExprDeepEqual::VisitExpr_(const StringImmObj *lhs, void *_rhs) {
  StringImmObj *rhs = static_cast<StringImmObj *>(_rhs);
  return lhs->value == rhs->value;
}
bool ExprDeepEqual::VisitExpr_(const CastDTypeObj *lhs, void *_rhs) {
  CastDTypeObj *rhs = static_cast<CastDTypeObj *>(_rhs);
  return ::mlc::base::DataTypeEqual(lhs->dtype, rhs->dtype) && VisitExpr(lhs->value, rhs->value.get());
}
#define MLC_SYM_EXPR_DEEP_EQ_BIN_OP(Obj)                                                                               \
  bool ExprDeepEqual::VisitExpr_(const Obj *lhs, void *_rhs) {                                                         \
    Obj *rhs = static_cast<Obj *>(_rhs);                                                                               \
    return VisitExpr(lhs->a, rhs->a.get()) && VisitExpr(lhs->b, rhs->b.get());                                         \
  }
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(AddObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(SubObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(MulObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(DivObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(ModObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(FloorDivObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(FloorModObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(MinObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(MaxObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(EQObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(NEObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(LTObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(LEObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(GTObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(GEObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(AndObj)
MLC_SYM_EXPR_DEEP_EQ_BIN_OP(OrObj)
#undef MLC_SYM_EXPR_DEEP_EQ_BIN_OP

bool ExprDeepEqual::VisitExpr_(const NotObj *lhs, void *_rhs) {
  NotObj *rhs = static_cast<NotObj *>(_rhs);
  return VisitExpr(lhs->a, rhs->a.get());
}
bool ExprDeepEqual::VisitExpr_(const SelectObj *lhs, void *_rhs) {
  SelectObj *rhs = static_cast<SelectObj *>(_rhs);
  return VisitExpr(lhs->cond, rhs->cond.get()) && VisitExpr(lhs->true_value, rhs->true_value.get()) &&
         VisitExpr(lhs->false_value, rhs->false_value.get());
}
bool ExprDeepEqual::VisitExpr_(const RampObj *lhs, void *_rhs) {
  RampObj *rhs = static_cast<RampObj *>(_rhs);
  return VisitExpr(lhs->base, rhs->base.get()) && VisitExpr(lhs->stride, rhs->stride.get()) &&
         VisitExpr(lhs->lanes, rhs->lanes.get());
}
bool ExprDeepEqual::VisitExpr_(const BroadcastObj *lhs, void *_rhs) {
  BroadcastObj *rhs = static_cast<BroadcastObj *>(_rhs);
  return VisitExpr(lhs->value, rhs->value.get());
}
bool ExprDeepEqual::VisitExpr_(const LetObj *lhs, void *_rhs) {
  LetObj *rhs = static_cast<LetObj *>(_rhs);
  return VisitExpr(lhs->var, rhs->var.get()) && VisitExpr(lhs->value, rhs->value.get()) &&
         VisitExpr(lhs->body, rhs->body.get());
}
bool ExprDeepEqual::VisitExpr_(const CallObj *lhs, void *_rhs) {
  CallObj *rhs = static_cast<CallObj *>(_rhs);
  const OpObj *lhs_op = lhs->op.TryCast<OpObj>();
  const OpObj *rhs_op = rhs->op.TryCast<OpObj>();
  if (lhs_op == nullptr || rhs_op == nullptr) {
    MLC_THROW(InternalError) << "`Call::op` must be `Op` in ExprDeepEqual";
  }
  if (lhs_op != rhs_op)
    return false;
  if (lhs->args.size() != rhs->args.size())
    return false;
  for (int64_t i = 0; i < lhs->args.size(); ++i)
    if (!this->Visit(lhs->args[i], rhs->args[i]))
      return false;
  return true;
}
bool ExprDeepEqual::VisitExpr_(const ShuffleObj *lhs, void *_rhs) {
  ShuffleObj *rhs = static_cast<ShuffleObj *>(_rhs);
  if (lhs->vectors.size() != rhs->vectors.size() || lhs->indices.size() != rhs->indices.size())
    return false;
  for (int64_t i = 0; i < lhs->vectors.size(); ++i)
    if (!this->Visit(lhs->vectors[i], rhs->vectors[i]))
      return false;
  for (int64_t i = 0; i < lhs->indices.size(); ++i)
    if (!this->Visit(lhs->indices[i], rhs->indices[i]))
      return false;
  return true;
}

} // namespace sym
} // namespace mlc
