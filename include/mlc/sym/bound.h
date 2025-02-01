#ifndef MLC_SYM_BOUND_H_
#define MLC_SYM_BOUND_H_

#include "./const_fold.h"
#include "./expr_equal.h"
#include "./int_operator.h"
#include "./ir.h"
#include "./op.h"
#include "./pattern_match.h"

namespace mlc {
namespace sym {
struct ConstIntBoundObj {
  MLCAny _mlc_header;
  int64_t min_value;
  int64_t max_value;
  explicit ConstIntBoundObj(int64_t min_value, int64_t max_value)
      : _mlc_header{}, min_value(min_value), max_value(max_value) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, ConstIntBoundObj, ::mlc::Object, "mlc.sym.ConstIntBound");
}; // struct ConstIntBoundObj

struct ConstIntBound : public ::mlc::ObjectRef {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, ConstIntBound, ConstIntBoundObj, ::mlc::ObjectRef)
      .Field("min_value", &ConstIntBoundObj::min_value)
      .Field("max_value", &ConstIntBoundObj::max_value)
      .StaticFn("__init__", ::mlc::InitOf<ConstIntBoundObj, int64_t, int64_t>);
}; // struct ConstIntBound
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct ModularSetObj {
  MLCAny _mlc_header;
  int64_t coeff;
  int64_t base;
  explicit ModularSetObj(int64_t coeff, int64_t base) : _mlc_header{}, coeff(coeff), base(base) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, ModularSetObj, ::mlc::Object, "mlc.sym.ModularSet");
}; // struct ModularSetObj

struct ModularSet : public ::mlc::ObjectRef {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, ModularSet, ModularSetObj, ::mlc::ObjectRef)
      .Field("coeff", &ModularSetObj::coeff)
      .Field("base", &ModularSetObj::base)
      .StaticFn("__init__", ::mlc::InitOf<ModularSetObj, int64_t, int64_t>);
}; // struct ModularSet
} // namespace sym
} // namespace mlc

#endif // MLC_SYM_BOUND_H_
