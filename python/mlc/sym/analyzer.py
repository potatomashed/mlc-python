import mlc.dataclasses as mlcd


@mlcd.py_class("mlc.sym.ConstIntBound")
class ConstIntBound(mlcd.PyClass):
    """Constant integer bound: [min_value, max_value]"""

    min_value: int
    max_value: int


@mlcd.py_class("mlc.sym.ModularSet")
class ModularSet(mlcd.PyClass):
    """Range of a linear integer function. Used to specify the possible index values.

        set = { coeff * x + base | x in Z }

    or (when coeff != 0)

        set = { n | n % coeff == base }

    This is useful to decide if the index is divisible by a certain value.
    For example, if index = 0 + 4 * x, then we know it can be divided by 4.
    """

    coeff: int
    base: int
