#ifndef MLC_SYM_IR_H_
#define MLC_SYM_IR_H_

#include <mlc/core/all.h>

namespace mlc {
namespace sym {
struct OpObj {
  MLCAny _mlc_header;
  ::mlc::Str name;
  explicit OpObj(::mlc::Str name) : _mlc_header{}, name(name) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, OpObj, ::mlc::Object, "mlc.sym.Op");
}; // struct OpObj

struct Op : public ::mlc::ObjectRef {
  MLC_API static Op Get(::mlc::Str name);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Op, OpObj, ::mlc::ObjectRef)
      .Field("name", &OpObj::name)
      .StaticFn("__init__", ::mlc::InitOf<OpObj, ::mlc::Str>);

}; // struct Op
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct ExprObj {
  MLCAny _mlc_header;
  explicit ExprObj() : _mlc_header{} {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, ExprObj, ::mlc::Object, "mlc.sym.Expr");
}; // struct ExprObj

struct Expr : public ::mlc::ObjectRef {
  Expr(int64_t value);
  Expr(double value);
  Expr(float value) : Expr(static_cast<double>(value)) {}
  Expr(int32_t value) : Expr(static_cast<int64_t>(value)) {}
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Expr, ExprObj, ::mlc::ObjectRef).StaticFn("__init__", ::mlc::InitOf<ExprObj>);
}; // struct Expr
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct RangeObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr min;
  ::mlc::sym::Expr extent;
  explicit RangeObj(::mlc::sym::Expr min, ::mlc::sym::Expr extent) : _mlc_header{}, min(min), extent(extent) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, RangeObj, ::mlc::Object, "mlc.sym.Range");
}; // struct RangeObj

struct Range : public ::mlc::ObjectRef {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Range, RangeObj, ::mlc::ObjectRef)
      .Field("min", &RangeObj::min)
      .Field("extent", &RangeObj::extent)
      .StaticFn("__init__", ::mlc::InitOf<RangeObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Range
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct VarObj {
  MLCAny _mlc_header;
  ::mlc::Str name;
  explicit VarObj(::mlc::Str name) : _mlc_header{}, name(name) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, VarObj, ::mlc::sym::ExprObj, "mlc.sym.Var");
}; // struct VarObj

struct Var : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Var, VarObj, ::mlc::sym::Expr)
      .Field("name", &VarObj::name)
      .StaticFn("__init__", ::mlc::InitOf<VarObj, ::mlc::Str>);
}; // struct Var
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct SizeVarObj {
  MLCAny _mlc_header;
  ::mlc::Str name;
  explicit SizeVarObj(::mlc::Str name) : _mlc_header{}, name(name) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, SizeVarObj, ::mlc::sym::VarObj, "mlc.sym.SizeVar");
}; // struct SizeVarObj

struct SizeVar : public ::mlc::sym::Var {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, SizeVar, SizeVarObj, ::mlc::sym::Var)
      .Field("name", &SizeVarObj::name)
      .StaticFn("__init__", ::mlc::InitOf<SizeVarObj, ::mlc::Str>);
}; // struct SizeVar
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct IntImmObj {
  MLCAny _mlc_header;
  int64_t value;
  explicit IntImmObj(int64_t value) : _mlc_header{}, value(value) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, IntImmObj, ::mlc::sym::ExprObj, "mlc.sym.IntImm");
}; // struct IntImmObj

struct IntImm : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, IntImm, IntImmObj, ::mlc::sym::Expr)
      .Field("value", &IntImmObj::value)
      .StaticFn("__init__", ::mlc::InitOf<IntImmObj, int64_t>);
}; // struct IntImm
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct FloatImmObj {
  MLCAny _mlc_header;
  double value;
  explicit FloatImmObj(double value) : _mlc_header{}, value(value) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, FloatImmObj, ::mlc::sym::ExprObj, "mlc.sym.FloatImm");
}; // struct FloatImmObj

struct FloatImm : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, FloatImm, FloatImmObj, ::mlc::sym::Expr)
      .Field("value", &FloatImmObj::value)
      .StaticFn("__init__", ::mlc::InitOf<FloatImmObj, double>);
}; // struct FloatImm
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct StringImmObj {
  MLCAny _mlc_header;
  ::mlc::Str value;
  explicit StringImmObj(::mlc::Str value) : _mlc_header{}, value(value) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, StringImmObj, ::mlc::sym::ExprObj, "mlc.sym.StringImm");
}; // struct StringImmObj

struct StringImm : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, StringImm, StringImmObj, ::mlc::sym::Expr)
      .Field("value", &StringImmObj::value)
      .StaticFn("__init__", ::mlc::InitOf<StringImmObj, ::mlc::Str>);
}; // struct StringImm
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct CastDTypeObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr value;
  DLDataType dtype;
  explicit CastDTypeObj(::mlc::sym::Expr value, DLDataType dtype) : _mlc_header{}, value(value), dtype(dtype) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, CastDTypeObj, ::mlc::sym::ExprObj, "mlc.sym.CastDType");
}; // struct CastDTypeObj

struct CastDType : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, CastDType, CastDTypeObj, ::mlc::sym::Expr)
      .Field("value", &CastDTypeObj::value)
      .Field("dtype", &CastDTypeObj::dtype)
      .StaticFn("__init__", ::mlc::InitOf<CastDTypeObj, ::mlc::sym::Expr, DLDataType>);
}; // struct CastDType
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct AddObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit AddObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, AddObj, ::mlc::sym::ExprObj, "mlc.sym.Add");
}; // struct AddObj

struct Add : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Add, AddObj, ::mlc::sym::Expr)
      .Field("a", &AddObj::a)
      .Field("b", &AddObj::b)
      .StaticFn("__init__", ::mlc::InitOf<AddObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Add
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct SubObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit SubObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, SubObj, ::mlc::sym::ExprObj, "mlc.sym.Sub");
}; // struct SubObj

struct Sub : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Sub, SubObj, ::mlc::sym::Expr)
      .Field("a", &SubObj::a)
      .Field("b", &SubObj::b)
      .StaticFn("__init__", ::mlc::InitOf<SubObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Sub
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct MulObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit MulObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, MulObj, ::mlc::sym::ExprObj, "mlc.sym.Mul");
}; // struct MulObj

struct Mul : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Mul, MulObj, ::mlc::sym::Expr)
      .Field("a", &MulObj::a)
      .Field("b", &MulObj::b)
      .StaticFn("__init__", ::mlc::InitOf<MulObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Mul
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct DivObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit DivObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, DivObj, ::mlc::sym::ExprObj, "mlc.sym.Div");
}; // struct DivObj

struct Div : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Div, DivObj, ::mlc::sym::Expr)
      .Field("a", &DivObj::a)
      .Field("b", &DivObj::b)
      .StaticFn("__init__", ::mlc::InitOf<DivObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Div
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct ModObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit ModObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, ModObj, ::mlc::sym::ExprObj, "mlc.sym.Mod");
}; // struct ModObj

struct Mod : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Mod, ModObj, ::mlc::sym::Expr)
      .Field("a", &ModObj::a)
      .Field("b", &ModObj::b)
      .StaticFn("__init__", ::mlc::InitOf<ModObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Mod
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct FloorDivObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit FloorDivObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, FloorDivObj, ::mlc::sym::ExprObj, "mlc.sym.FloorDiv");
}; // struct FloorDivObj

struct FloorDiv : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, FloorDiv, FloorDivObj, ::mlc::sym::Expr)
      .Field("a", &FloorDivObj::a)
      .Field("b", &FloorDivObj::b)
      .StaticFn("__init__", ::mlc::InitOf<FloorDivObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct FloorDiv
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct FloorModObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit FloorModObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, FloorModObj, ::mlc::sym::ExprObj, "mlc.sym.FloorMod");
}; // struct FloorModObj

struct FloorMod : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, FloorMod, FloorModObj, ::mlc::sym::Expr)
      .Field("a", &FloorModObj::a)
      .Field("b", &FloorModObj::b)
      .StaticFn("__init__", ::mlc::InitOf<FloorModObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct FloorMod
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct MinObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit MinObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, MinObj, ::mlc::sym::ExprObj, "mlc.sym.Min");
}; // struct MinObj

struct Min : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Min, MinObj, ::mlc::sym::Expr)
      .Field("a", &MinObj::a)
      .Field("b", &MinObj::b)
      .StaticFn("__init__", ::mlc::InitOf<MinObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Min
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct MaxObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit MaxObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, MaxObj, ::mlc::sym::ExprObj, "mlc.sym.Max");
}; // struct MaxObj

struct Max : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Max, MaxObj, ::mlc::sym::Expr)
      .Field("a", &MaxObj::a)
      .Field("b", &MaxObj::b)
      .StaticFn("__init__", ::mlc::InitOf<MaxObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Max
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct EQObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit EQObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, EQObj, ::mlc::sym::ExprObj, "mlc.sym.EQ");
}; // struct EQObj

struct EQ : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, EQ, EQObj, ::mlc::sym::Expr)
      .Field("a", &EQObj::a)
      .Field("b", &EQObj::b)
      .StaticFn("__init__", ::mlc::InitOf<EQObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct EQ
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct NEObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit NEObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, NEObj, ::mlc::sym::ExprObj, "mlc.sym.NE");
}; // struct NEObj

struct NE : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, NE, NEObj, ::mlc::sym::Expr)
      .Field("a", &NEObj::a)
      .Field("b", &NEObj::b)
      .StaticFn("__init__", ::mlc::InitOf<NEObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct NE
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct LTObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit LTObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, LTObj, ::mlc::sym::ExprObj, "mlc.sym.LT");
}; // struct LTObj

struct LT : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, LT, LTObj, ::mlc::sym::Expr)
      .Field("a", &LTObj::a)
      .Field("b", &LTObj::b)
      .StaticFn("__init__", ::mlc::InitOf<LTObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct LT
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct LEObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit LEObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, LEObj, ::mlc::sym::ExprObj, "mlc.sym.LE");
}; // struct LEObj

struct LE : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, LE, LEObj, ::mlc::sym::Expr)
      .Field("a", &LEObj::a)
      .Field("b", &LEObj::b)
      .StaticFn("__init__", ::mlc::InitOf<LEObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct LE
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct GTObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit GTObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, GTObj, ::mlc::sym::ExprObj, "mlc.sym.GT");
}; // struct GTObj

struct GT : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, GT, GTObj, ::mlc::sym::Expr)
      .Field("a", &GTObj::a)
      .Field("b", &GTObj::b)
      .StaticFn("__init__", ::mlc::InitOf<GTObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct GT
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct GEObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit GEObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, GEObj, ::mlc::sym::ExprObj, "mlc.sym.GE");
}; // struct GEObj

struct GE : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, GE, GEObj, ::mlc::sym::Expr)
      .Field("a", &GEObj::a)
      .Field("b", &GEObj::b)
      .StaticFn("__init__", ::mlc::InitOf<GEObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct GE
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct AndObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit AndObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, AndObj, ::mlc::sym::ExprObj, "mlc.sym.And");
}; // struct AndObj

struct And : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, And, AndObj, ::mlc::sym::Expr)
      .Field("a", &AndObj::a)
      .Field("b", &AndObj::b)
      .StaticFn("__init__", ::mlc::InitOf<AndObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct And
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct OrObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  ::mlc::sym::Expr b;
  explicit OrObj(::mlc::sym::Expr a, ::mlc::sym::Expr b) : _mlc_header{}, a(a), b(b) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, OrObj, ::mlc::sym::ExprObj, "mlc.sym.Or");
}; // struct OrObj

struct Or : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a, Expr b);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Or, OrObj, ::mlc::sym::Expr)
      .Field("a", &OrObj::a)
      .Field("b", &OrObj::b)
      .StaticFn("__init__", ::mlc::InitOf<OrObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Or
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct NotObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr a;
  explicit NotObj(::mlc::sym::Expr a) : _mlc_header{}, a(a) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, NotObj, ::mlc::sym::ExprObj, "mlc.sym.Not");
}; // struct NotObj

struct Not : public ::mlc::sym::Expr {
  MLC_API static Optional<Expr> TryConstFold(Expr a);
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Not, NotObj, ::mlc::sym::Expr)
      .Field("a", &NotObj::a)
      .StaticFn("__init__", ::mlc::InitOf<NotObj, ::mlc::sym::Expr>);
}; // struct Not
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct SelectObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr cond;
  ::mlc::sym::Expr true_value;
  ::mlc::sym::Expr false_value;
  explicit SelectObj(::mlc::sym::Expr cond, ::mlc::sym::Expr true_value, ::mlc::sym::Expr false_value)
      : _mlc_header{}, cond(cond), true_value(true_value), false_value(false_value) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, SelectObj, ::mlc::sym::ExprObj, "mlc.sym.Select");
}; // struct SelectObj

struct Select : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Select, SelectObj, ::mlc::sym::Expr)
      .Field("cond", &SelectObj::cond)
      .Field("true_value", &SelectObj::true_value)
      .Field("false_value", &SelectObj::false_value)
      .StaticFn("__init__", ::mlc::InitOf<SelectObj, ::mlc::sym::Expr, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Select
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct RampObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr base;
  ::mlc::sym::Expr stride;
  ::mlc::sym::Expr lanes;
  explicit RampObj(::mlc::sym::Expr base, ::mlc::sym::Expr stride, ::mlc::sym::Expr lanes)
      : _mlc_header{}, base(base), stride(stride), lanes(lanes) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, RampObj, ::mlc::sym::ExprObj, "mlc.sym.Ramp");
}; // struct RampObj

struct Ramp : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Ramp, RampObj, ::mlc::sym::Expr)
      .Field("base", &RampObj::base)
      .Field("stride", &RampObj::stride)
      .Field("lanes", &RampObj::lanes)
      .StaticFn("__init__", ::mlc::InitOf<RampObj, ::mlc::sym::Expr, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Ramp
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct BroadcastObj {
  MLCAny _mlc_header;
  ::mlc::sym::Expr value;
  ::mlc::sym::Expr lanes;
  explicit BroadcastObj(::mlc::sym::Expr value, ::mlc::sym::Expr lanes) : _mlc_header{}, value(value), lanes(lanes) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, BroadcastObj, ::mlc::sym::ExprObj, "mlc.sym.Broadcast");
}; // struct BroadcastObj

struct Broadcast : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Broadcast, BroadcastObj, ::mlc::sym::Expr)
      .Field("value", &BroadcastObj::value)
      .Field("lanes", &BroadcastObj::lanes)
      .StaticFn("__init__", ::mlc::InitOf<BroadcastObj, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Broadcast
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct ShuffleObj {
  MLCAny _mlc_header;
  ::mlc::List<::mlc::sym::Expr> vectors;
  ::mlc::List<::mlc::sym::Expr> indices;
  explicit ShuffleObj(::mlc::List<::mlc::sym::Expr> vectors, ::mlc::List<::mlc::sym::Expr> indices)
      : _mlc_header{}, vectors(vectors), indices(indices) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, ShuffleObj, ::mlc::sym::ExprObj, "mlc.sym.Shuffle");
}; // struct ShuffleObj

struct Shuffle : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Shuffle, ShuffleObj, ::mlc::sym::Expr)
      .Field("vectors", &ShuffleObj::vectors)
      .Field("indices", &ShuffleObj::indices)
      .StaticFn("__init__", ::mlc::InitOf<ShuffleObj, ::mlc::List<::mlc::sym::Expr>, ::mlc::List<::mlc::sym::Expr>>);
}; // struct Shuffle
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct LetObj {
  MLCAny _mlc_header;
  ::mlc::sym::Var var;
  ::mlc::sym::Expr value;
  ::mlc::sym::Expr body;
  explicit LetObj(::mlc::sym::Var var, ::mlc::sym::Expr value, ::mlc::sym::Expr body)
      : _mlc_header{}, var(var), value(value), body(body) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, LetObj, ::mlc::sym::ExprObj, "mlc.sym.Let");
}; // struct LetObj

struct Let : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Let, LetObj, ::mlc::sym::Expr)
      .Field("var", &LetObj::var)
      .Field("value", &LetObj::value)
      .Field("body", &LetObj::body)
      .StaticFn("__init__", ::mlc::InitOf<LetObj, ::mlc::sym::Var, ::mlc::sym::Expr, ::mlc::sym::Expr>);
}; // struct Let
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {
struct CallObj {
  MLCAny _mlc_header;
  ::mlc::Any op;
  ::mlc::List<::mlc::sym::Expr> args;
  explicit CallObj(::mlc::Any op, ::mlc::List<::mlc::sym::Expr> args) : _mlc_header{}, op(op), args(args) {}
  MLC_DEF_DYN_TYPE(MLC_EXPORTS, CallObj, ::mlc::sym::ExprObj, "mlc.sym.Call");
}; // struct CallObj

struct Call : public ::mlc::sym::Expr {
  MLC_DEF_OBJ_REF(MLC_EXPORTS, Call, CallObj, ::mlc::sym::Expr)
      .Field("op", &CallObj::op)
      .Field("args", &CallObj::args)
      .StaticFn("__init__", ::mlc::InitOf<CallObj, ::mlc::Any, ::mlc::List<::mlc::sym::Expr>>);
}; // struct Call
} // namespace sym
} // namespace mlc

namespace mlc {
namespace sym {

inline Expr::Expr(int64_t value) : ::mlc::sym::Expr(::mlc::sym::IntImm(value)) {}
inline Expr::Expr(double value) : ::mlc::sym::Expr(::mlc::sym::FloatImm(value)) {}

} // namespace sym
} // namespace mlc

#endif // MLC_SYM_IR_H_
