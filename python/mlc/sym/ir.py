from typing import Any

import mlc
import mlc.dataclasses as mlcd


@mlcd.py_class("mlc.sym.Expr")
class Expr(mlcd.PyClass):
    pass


@mlcd.py_class("mlc.sym.Range")
class Range(mlcd.PyClass):
    min: Expr
    extent: Expr


@mlcd.py_class("mlc.sym.Op")
class Op(Expr):
    name: str


@mlcd.py_class("mlc.sym.Var")
class Var(Expr):
    name: str


@mlcd.py_class("mlc.sym.SizeVar")
class SizeVar(Var):
    pass


@mlcd.py_class("mlc.sym.IntImm")
class IntImm(Expr):
    value: int


@mlcd.py_class("mlc.sym.FloatImm")
class FloatImm(Expr):
    value: float


@mlcd.py_class("mlc.sym.StringImm")
class StringImm(Expr):
    value: str


@mlcd.py_class("mlc.sym.CastDType")
class CastDType(Expr):
    value: Expr
    dtype: mlc.DataType


@mlcd.py_class("mlc.sym.Add")
class Add(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.Sub")
class Sub(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.Mul")
class Mul(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.Div")
class Div(Expr):  # truncdiv
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.Mod")
class Mod(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.FloorDiv")
class FloorDiv(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.FloorMod")
class FloorMod(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.Min")
class Min(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.Max")
class Max(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.EQ")
class EQ(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.NE")
class NE(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.LT")
class LT(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.LE")
class LE(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.GT")
class GT(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.GE")
class GE(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.And")
class And(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.Or")
class Or(Expr):
    a: Expr
    b: Expr


@mlcd.py_class("mlc.sym.Not")
class Not(Expr):
    a: Expr


@mlcd.py_class("mlc.sym.Select")
class Select(Expr):
    cond: Expr
    true_value: Expr
    false_value: Expr


@mlcd.py_class("mlc.sym.Ramp")
class Ramp(Expr):
    base: Expr
    stride: Expr
    lanes: Expr


@mlcd.py_class("mlc.sym.Broadcast")
class Broadcast(Expr):
    value: Expr
    lanes: Expr


@mlcd.py_class("mlc.sym.Shuffle")
class Shuffle(Expr):
    vectors: list[Expr]
    indices: list[Expr]


@mlcd.py_class("mlc.sym.Let")
class Let(Expr):
    var: Var
    value: Expr
    body: Expr


@mlcd.py_class("mlc.sym.Call")
class Call(Expr):
    op: Any
    args: list[Expr]
