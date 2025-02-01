#ifndef MLC_SYM_EXPR_FUNCTOR_H_
#define MLC_SYM_EXPR_FUNCTOR_H_
#include "./ir.h"

namespace mlc {
namespace sym {

template <typename FType> struct ExprFunctor;

template <typename R, typename... Args> struct ExprFunctor<R(const Expr &n, Args...)> {
  R operator()(const Expr &n, Args... args) { return this->VisitExpr(n, std::forward<Args>(args)...); }
  virtual R VisitExpr(const Expr &n, Args... args) {
    static ::mlc::VTable _vtable = InitVTable();
    return _vtable.operator()<R, const Expr &, void *, Args...>(n, static_cast<void *>(this),
                                                                std::forward<Args>(args)...);
  }
  virtual R VisitExprDefault_(const void *obj, Args...) {
    MLC_THROW(InternalError) << "Do not have a default for: " << static_cast<const Object *>(obj)->GetTypeKey();
  }

protected:
#define MLC_SYM_EXPR_FUNCTOR(TY)                                                                                       \
  virtual R VisitExpr_(const TY *obj, Args... args) { return VisitExprDefault_(obj, std::forward<Args>(args)...); }
#define MLC_SYM_EXPR_VTABLE(VTABLE, TY)                                                                                \
  (VTABLE).template Set<TY>([](TY *obj, void *self, Args... args) {                                                    \
    return static_cast<ExprFunctor *>(self)->VisitExpr_(obj, std::forward<Args>(args)...);                             \
  })
  MLC_SYM_EXPR_FUNCTOR(VarObj)
  MLC_SYM_EXPR_FUNCTOR(SizeVarObj)
  MLC_SYM_EXPR_FUNCTOR(IntImmObj)
  MLC_SYM_EXPR_FUNCTOR(FloatImmObj)
  MLC_SYM_EXPR_FUNCTOR(StringImmObj)
  MLC_SYM_EXPR_FUNCTOR(CastDTypeObj)
  MLC_SYM_EXPR_FUNCTOR(AddObj)
  MLC_SYM_EXPR_FUNCTOR(SubObj)
  MLC_SYM_EXPR_FUNCTOR(MulObj)
  MLC_SYM_EXPR_FUNCTOR(DivObj)
  MLC_SYM_EXPR_FUNCTOR(ModObj)
  MLC_SYM_EXPR_FUNCTOR(FloorDivObj)
  MLC_SYM_EXPR_FUNCTOR(FloorModObj)
  MLC_SYM_EXPR_FUNCTOR(MinObj)
  MLC_SYM_EXPR_FUNCTOR(MaxObj)
  MLC_SYM_EXPR_FUNCTOR(EQObj)
  MLC_SYM_EXPR_FUNCTOR(NEObj)
  MLC_SYM_EXPR_FUNCTOR(LTObj)
  MLC_SYM_EXPR_FUNCTOR(LEObj)
  MLC_SYM_EXPR_FUNCTOR(GTObj)
  MLC_SYM_EXPR_FUNCTOR(GEObj)
  MLC_SYM_EXPR_FUNCTOR(AndObj)
  MLC_SYM_EXPR_FUNCTOR(OrObj)
  MLC_SYM_EXPR_FUNCTOR(NotObj)
  MLC_SYM_EXPR_FUNCTOR(SelectObj)
  MLC_SYM_EXPR_FUNCTOR(RampObj)
  MLC_SYM_EXPR_FUNCTOR(BroadcastObj)
  MLC_SYM_EXPR_FUNCTOR(LetObj)
  MLC_SYM_EXPR_FUNCTOR(CallObj)
  MLC_SYM_EXPR_FUNCTOR(ShuffleObj)
  static ::mlc::VTable InitVTable() {
    ::mlc::VTable ret = ::mlc::Lib::MakeVTable("ExprFunctor");
    MLC_SYM_EXPR_VTABLE(ret, VarObj);
    MLC_SYM_EXPR_VTABLE(ret, SizeVarObj);
    MLC_SYM_EXPR_VTABLE(ret, IntImmObj);
    MLC_SYM_EXPR_VTABLE(ret, FloatImmObj);
    MLC_SYM_EXPR_VTABLE(ret, StringImmObj);
    MLC_SYM_EXPR_VTABLE(ret, CastDTypeObj);
    MLC_SYM_EXPR_VTABLE(ret, AddObj);
    MLC_SYM_EXPR_VTABLE(ret, SubObj);
    MLC_SYM_EXPR_VTABLE(ret, MulObj);
    MLC_SYM_EXPR_VTABLE(ret, DivObj);
    MLC_SYM_EXPR_VTABLE(ret, ModObj);
    MLC_SYM_EXPR_VTABLE(ret, FloorDivObj);
    MLC_SYM_EXPR_VTABLE(ret, FloorModObj);
    MLC_SYM_EXPR_VTABLE(ret, MinObj);
    MLC_SYM_EXPR_VTABLE(ret, MaxObj);
    MLC_SYM_EXPR_VTABLE(ret, EQObj);
    MLC_SYM_EXPR_VTABLE(ret, NEObj);
    MLC_SYM_EXPR_VTABLE(ret, LTObj);
    MLC_SYM_EXPR_VTABLE(ret, LEObj);
    MLC_SYM_EXPR_VTABLE(ret, GTObj);
    MLC_SYM_EXPR_VTABLE(ret, GEObj);
    MLC_SYM_EXPR_VTABLE(ret, AndObj);
    MLC_SYM_EXPR_VTABLE(ret, OrObj);
    MLC_SYM_EXPR_VTABLE(ret, NotObj);
    MLC_SYM_EXPR_VTABLE(ret, SelectObj);
    MLC_SYM_EXPR_VTABLE(ret, RampObj);
    MLC_SYM_EXPR_VTABLE(ret, BroadcastObj);
    MLC_SYM_EXPR_VTABLE(ret, LetObj);
    MLC_SYM_EXPR_VTABLE(ret, CallObj);
    MLC_SYM_EXPR_VTABLE(ret, ShuffleObj);
    return ret;
  }
#undef MLC_SYM_EXPR_FUNCTOR
#undef MLC_SYM_EXPR_VTABLE
};

struct MLC_API ExprVisitor : protected ExprFunctor<void(const Expr &)> {
  using ExprFunctor::operator();
  using ExprFunctor::VisitExpr;

protected:
  void VisitExpr_(const VarObj *op) override;
  void VisitExpr_(const SizeVarObj *op) override;
  void VisitExpr_(const IntImmObj *op) override;
  void VisitExpr_(const FloatImmObj *op) override;
  void VisitExpr_(const StringImmObj *op) override;
  void VisitExpr_(const CastDTypeObj *op) override;
  void VisitExpr_(const AddObj *op) override;
  void VisitExpr_(const SubObj *op) override;
  void VisitExpr_(const MulObj *op) override;
  void VisitExpr_(const DivObj *op) override;
  void VisitExpr_(const ModObj *op) override;
  void VisitExpr_(const FloorDivObj *op) override;
  void VisitExpr_(const FloorModObj *op) override;
  void VisitExpr_(const MinObj *op) override;
  void VisitExpr_(const MaxObj *op) override;
  void VisitExpr_(const EQObj *op) override;
  void VisitExpr_(const NEObj *op) override;
  void VisitExpr_(const LTObj *op) override;
  void VisitExpr_(const LEObj *op) override;
  void VisitExpr_(const GTObj *op) override;
  void VisitExpr_(const GEObj *op) override;
  void VisitExpr_(const AndObj *op) override;
  void VisitExpr_(const OrObj *op) override;
  void VisitExpr_(const NotObj *op) override;
  void VisitExpr_(const SelectObj *op) override;
  void VisitExpr_(const RampObj *op) override;
  void VisitExpr_(const BroadcastObj *op) override;
  void VisitExpr_(const LetObj *op) override;
  void VisitExpr_(const CallObj *op) override;
  void VisitExpr_(const ShuffleObj *op) override;
};

struct MLC_API ExprMutator : protected ExprFunctor<Expr(const Expr &)> {
  using ExprFunctor::operator();
  using ExprFunctor::VisitExpr;

protected:
  Expr VisitExpr_(const VarObj *op) override;
  Expr VisitExpr_(const SizeVarObj *op) override;
  Expr VisitExpr_(const IntImmObj *op) override;
  Expr VisitExpr_(const FloatImmObj *op) override;
  Expr VisitExpr_(const StringImmObj *op) override;
  Expr VisitExpr_(const CastDTypeObj *op) override;
  Expr VisitExpr_(const AddObj *op) override;
  Expr VisitExpr_(const SubObj *op) override;
  Expr VisitExpr_(const MulObj *op) override;
  Expr VisitExpr_(const DivObj *op) override;
  Expr VisitExpr_(const ModObj *op) override;
  Expr VisitExpr_(const FloorDivObj *op) override;
  Expr VisitExpr_(const FloorModObj *op) override;
  Expr VisitExpr_(const MinObj *op) override;
  Expr VisitExpr_(const MaxObj *op) override;
  Expr VisitExpr_(const EQObj *op) override;
  Expr VisitExpr_(const NEObj *op) override;
  Expr VisitExpr_(const LTObj *op) override;
  Expr VisitExpr_(const LEObj *op) override;
  Expr VisitExpr_(const GTObj *op) override;
  Expr VisitExpr_(const GEObj *op) override;
  Expr VisitExpr_(const AndObj *op) override;
  Expr VisitExpr_(const OrObj *op) override;
  Expr VisitExpr_(const NotObj *op) override;
  Expr VisitExpr_(const SelectObj *op) override;
  Expr VisitExpr_(const RampObj *op) override;
  Expr VisitExpr_(const BroadcastObj *op) override;
  Expr VisitExpr_(const LetObj *op) override;
  Expr VisitExpr_(const CallObj *op) override;
  Expr VisitExpr_(const ShuffleObj *op) override;
};

struct MLC_API ExprDeepEqual : protected ExprFunctor<bool(const Expr &, void *other)> {
  static bool Compare(const Expr &lhs, const Expr &rhs) { return ExprDeepEqual().Visit(lhs, rhs); }
  bool Visit(const Expr &lhs, const Expr &rhs) { //
    ExprObj *rhs_obj = const_cast<ExprObj *>(rhs.get());
    return this->VisitExpr(lhs, static_cast<void *>(rhs_obj));
  }
  bool operator()(const Expr &lhs, const Expr &rhs) { return this->Visit(lhs, rhs); }

private:
  bool VisitExpr_(const VarObj *lhs, void *_rhs) override;
  bool VisitExpr_(const SizeVarObj *lhs, void *_rhs) override;
  bool VisitExpr_(const IntImmObj *lhs, void *_rhs) override;
  bool VisitExpr_(const FloatImmObj *lhs, void *_rhs) override;
  bool VisitExpr_(const StringImmObj *lhs, void *_rhs) override;
  bool VisitExpr_(const CastDTypeObj *lhs, void *_rhs) override;
  bool VisitExpr_(const AddObj *lhs, void *_rhs) override;
  bool VisitExpr_(const SubObj *lhs, void *_rhs) override;
  bool VisitExpr_(const MulObj *lhs, void *_rhs) override;
  bool VisitExpr_(const DivObj *lhs, void *_rhs) override;
  bool VisitExpr_(const ModObj *lhs, void *_rhs) override;
  bool VisitExpr_(const FloorDivObj *lhs, void *_rhs) override;
  bool VisitExpr_(const FloorModObj *lhs, void *_rhs) override;
  bool VisitExpr_(const MinObj *lhs, void *_rhs) override;
  bool VisitExpr_(const MaxObj *lhs, void *_rhs) override;
  bool VisitExpr_(const EQObj *lhs, void *_rhs) override;
  bool VisitExpr_(const NEObj *lhs, void *_rhs) override;
  bool VisitExpr_(const LTObj *lhs, void *_rhs) override;
  bool VisitExpr_(const LEObj *lhs, void *_rhs) override;
  bool VisitExpr_(const GTObj *lhs, void *_rhs) override;
  bool VisitExpr_(const GEObj *lhs, void *_rhs) override;
  bool VisitExpr_(const AndObj *lhs, void *_rhs) override;
  bool VisitExpr_(const OrObj *lhs, void *_rhs) override;
  bool VisitExpr_(const NotObj *lhs, void *_rhs) override;
  bool VisitExpr_(const SelectObj *lhs, void *_rhs) override;
  bool VisitExpr_(const RampObj *lhs, void *_rhs) override;
  bool VisitExpr_(const BroadcastObj *lhs, void *_rhs) override;
  bool VisitExpr_(const LetObj *lhs, void *_rhs) override;
  bool VisitExpr_(const CallObj *lhs, void *_rhs) override;
  bool VisitExpr_(const ShuffleObj *lhs, void *_rhs) override;
};

} // namespace sym
} // namespace mlc

#endif // MLC_SYM_EXPR_FUNCTOR_H_
