#include "./int_operator.h"
#include <mlc/sym/all.h>

namespace mlc {
namespace sym {
namespace {

constexpr int64_t kNegInf = ConstIntBoundObj::kNegInf;
constexpr int64_t kPosInf = ConstIntBoundObj::kPosInf;

struct Entry {
  int64_t min_value;
  int64_t max_value;

  bool is_const(int64_t value) const { return min_value == max_value && min_value == value; }
  bool operator==(const Entry &other) const { return min_value == other.min_value && max_value == other.max_value; }
  bool operator!=(const Entry &other) const { return !(*this == other); }

  static Entry Make(int64_t _min_value, int64_t _max_value) {
    return Entry{(_min_value == kPosInf) ? _min_value - 1 : _min_value,
                 (_max_value == kNegInf) ? _max_value + 1 : _max_value};
  }

  Entry Union(Entry b) const {
    Entry ret;
    ret.min_value = std::min(this->min_value, b.min_value);
    ret.max_value = std::max(this->max_value, b.max_value);
    return ret;
  }

  Entry Intersect(Entry b) const {
    Entry ret;
    ret.min_value = std::max(this->min_value, b.min_value);
    ret.max_value = std::min(this->max_value, b.max_value);
    return ret;
  }

  Entry Negative() const {
    Entry ret;
    if (this->max_value == kPosInf) {
      ret.min_value = kNegInf;
    } else {
      ret.min_value = -this->max_value;
    }
    if (this->min_value == kNegInf) {
      ret.max_value = kPosInf;
    } else {
      ret.max_value = -this->min_value;
    }
    return ret;
  }

  static Entry Everything() { return Entry{kNegInf, kPosInf}; }
};

struct BoundInfo {
  Expr expr;
  Entry bound;

  BoundInfo() {}
  BoundInfo(Expr expr, Entry bound) : expr(expr), bound(bound) {}
};

int64_t InfAwareAdd(int64_t x, int64_t y) {
  if (x == kPosInf) // TODO: check (y != kNegInf)
    return kPosInf;
  if (x == kNegInf) // TODO: check (y != kPosInf)
    return kNegInf;
  if (y == kPosInf || y == kNegInf)
    return y;
  if (WillOverflow<AddObj>(x, y, kNegInf, kPosInf)) {
    if (x > 0)
      return kPosInf;
    return kNegInf;
  }
  return x + y;
}

int64_t InfAwareMul(int64_t x, int64_t y) {
  if (!WillOverflow<MulObj>(x, y, kNegInf, kPosInf))
    return x * y;
  return ((x > 0 && y > 0) || (x < 0 && y < 0)) ? kPosInf : kNegInf;
}

// int64_t InfAwareDiv(int64_t x, int64_t y) {
//   if (y == 0) {
//     MLC_THROW(InternalError) << "Division by zero";
//   }
//   if (x == kPosInf || x == kNegInf) {
//     return y > 0 ? x : -x;
//   }
//   return x / y;
// }

// int64_t InfAwareFloorDiv(int64_t x, int64_t y) {
//   if (y == 0) {
//     MLC_THROW(InternalError) << "Division by zero";
//   }
//   if (x == kPosInf || x == kNegInf) {
//     return y > 0 ? x : -x;
//   }
//   return floordiv(x, y);
// }

// int64_t InfAwareLeftShift(int64_t x, int64_t y) {
//   if (x == kPosInf || x == kNegInf)
//     return x;
//   // Can be replaced with std::bit_width in C++20
//   auto bit_width = [](int64_t as_signed) {
//     uint64_t val = std::abs(as_signed);
//     int num_bits = 0;
//     while (val) {
//       ++num_bits;
//       val >>= 1;
//     }
//     return num_bits;
//   };
//   int x_bits = bit_width(x);
//   if (x_bits + y < 64) {
//     return x << y;
//   } else {
//     return kPosInf;
//   }
// }

// int64_t InfAwareRightShift(int64_t x, int64_t y) {
//   if (x == kPosInf || x == kNegInf)
//     return x;
//   return x >> y;
// }

template <typename F> Entry BinaryOpBoundary(Entry a, Entry b, const F &op) {
  Entry ret;
  // The boundary point must be shihft of the original boundary.
  int64_t v1 = op(a.min_value, b.min_value);
  int64_t v2 = op(a.max_value, b.max_value);
  int64_t v3 = op(a.min_value, b.max_value);
  int64_t v4 = op(a.max_value, b.min_value);
  ret.min_value = std::min(std::min(std::min(v1, v2), v3), v4);
  ret.max_value = std::max(std::max(std::max(v1, v2), v3), v4);
  return ret;
}

} // namespace

struct ConstIntBoundAnalyzer::Impl : public ExprFunctor<Entry(const Expr &)> {
public:
  Entry VisitExpr(const Expr &expr) final; // TODO
  Entry VisitExprDefault_(const void *) final { return Entry::Everything(); }
  Entry VisitExpr_(const IntImmObj *op) final { return Entry::Make(op->value, op->value); }
  Entry VisitExpr_(const AddObj *op) final {
    Entry a = VisitExpr(op->a);
    Entry b = VisitExpr(op->b);
    return Entry{InfAwareAdd(a.min_value, b.min_value), //
                 InfAwareAdd(a.max_value, b.max_value)};
  }
  Entry VisitExpr_(const SubObj *op) final {
    Entry a = VisitExpr(op->a);
    Entry b = VisitExpr(op->b);
    return Entry{InfAwareAdd(a.min_value, -b.max_value), //
                 InfAwareAdd(a.max_value, -b.min_value)};
  }
  Entry VisitExpr_(const MulObj *op) final {
    Entry a = VisitExpr(op->a);
    Entry b = VisitExpr(op->b);
    return BinaryOpBoundary(a, b, InfAwareMul);
  }
  Entry VisitExpr_(const MinObj *op) final {
    Entry a = VisitExpr(op->a);
    Entry b = VisitExpr(op->b);
    return Entry{std::min(a.min_value, b.min_value), //
                 std::min(a.max_value, b.max_value)};
  }
  Entry VisitExpr_(const MaxObj *op) final {
    Entry a = VisitExpr(op->a);
    Entry b = VisitExpr(op->b);
    return Entry{std::max(a.min_value, b.min_value), //
                 std::max(a.max_value, b.max_value)};
  }
  Entry VisitExpr_(const SelectObj *op) final {
    Entry a = VisitExpr(op->true_value);
    Entry b = VisitExpr(op->false_value);
    return a.Union(b);
  }
  Entry VisitExpr_(const VarObj *op) final {
    auto it = var_map_.find(Var(op));
    if (it != var_map_.end()) {
      return it->second;
    } else {
      return Entry::Everything();
    }
  }
  Entry VisitExpr_(const SizeVarObj *op) final {
    auto it = var_map_.find(SizeVar(op));
    if (it != var_map_.end()) {
      return it->second;
    } else {
      return Entry::Make(0, kPosInf);
    }
  }
  Entry VisitExpr_(const DivObj *op) final;      // TODO
  Entry VisitExpr_(const ModObj *op) final;      // TODO
  Entry VisitExpr_(const FloorDivObj *op) final; // TODO
  Entry VisitExpr_(const FloorModObj *op) final; // TODO
  Entry VisitExpr_(const LetObj *op) final {
    auto it = var_map_.find(op->var);
    // if the var has not been binded, update the info.
    if (it == var_map_.end()) {
      var_map_[op->var] = this->VisitExpr(op->value);
      Entry ret = VisitExpr(op->body);
      var_map_.erase(op->var);
      return ret;
    } else {
      return VisitExpr(op->body);
    }
  }
  Entry VisitExpr_(const RampObj *op) final {
    Entry a = VisitExpr(op->base);
    Entry b = VisitExpr(op->base + (op->lanes - 1) * op->stride);
    return a.Union(b);
  }
  Entry VisitExpr_(const BroadcastObj *op) final { return VisitExpr(op->value); }
  Entry VisitExpr_(const CastDTypeObj *op) final; // TODO
  Entry VisitExpr_(const CallObj *op) final;      // TODO
  Entry VisitLeftShift(const CallObj *op);        // TODO
  Entry VisitRightShift(const CallObj *op);       // TODO
  Entry VisitBitwiseAnd(const CallObj *op);       // TODO

  void Bind(const Var &var, const Range &range, bool allow_override) {
    Entry a = VisitExpr(range->min);
    Entry b = VisitExpr(range->extent);
    Entry ret;
    ret.min_value = a.min_value;
    ret.max_value = InfAwareAdd(a.max_value, InfAwareAdd(b.max_value, -1));
    Update(var, ret, allow_override);
  }

  void Update(const Var &var, const ConstIntBound &info, bool allow_override) {
    Update(var, Entry::Make(info->min_value, info->max_value), allow_override);
  }

  void Update(const Var &var, const Entry &info, bool allow_override) {
    if (!allow_override) {
      auto it = var_map_.find(var);
      if (it != var_map_.end() && it->second != info) {
        MLC_THROW(InternalError) << "Trying to update var \'" << var << "\'"
                                 << " with a different const bound: "
                                 << "original=" << ConstIntBound(it->second.min_value, it->second.max_value)
                                 << ", new=" << ConstIntBound(info.min_value, info.max_value);
      }
    }
    var_map_[var] = info;
  }

  // internal variable map
  std::unordered_map<Var, Entry> var_map_;
  // additional bound info
  std::vector<BoundInfo> additional_info_;
  // look up table for memorization
  BoundMapType *bound_{nullptr};
};

} // namespace sym
} // namespace mlc
