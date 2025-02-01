import mlc.dataclasses as mlcd

from . import ir


@mlcd.py_class("mlc.sym.IterMapExpr")
class IterMapExpr(ir.Expr):
    pass


@mlcd.py_class("mlc.sym.IterMark")
class IterMark(ir.Expr):
    source: ir.Expr
    extent: ir.Expr
