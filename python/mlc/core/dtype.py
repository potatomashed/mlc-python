from __future__ import annotations

import numpy as np

from mlc._cython import DataTypeCode, PyAny, dtype_as_triple, dtype_normalize
from mlc.dataclasses import c_class


@c_class("dtype")
class DataType(PyAny):
    def __init__(self, dtype: str | np.dtype | DataType) -> None:
        self._mlc_init("__init__", dtype_normalize(dtype))

    @property
    def _dtype_triple(self) -> tuple[int, int, int]:
        return dtype_as_triple(self)

    @property
    def code(self) -> DataTypeCode:
        return DataTypeCode(self._dtype_triple[0])

    @property
    def bits(self) -> int:
        return self._dtype_triple[1]

    @property
    def lanes(self) -> int:
        return self._dtype_triple[2]

    def __eq__(self, other: object) -> bool:
        return isinstance(other, DataType) and self._dtype_triple == other._dtype_triple

    def __ne__(self, other: object) -> bool:
        return isinstance(other, DataType) and self._dtype_triple != other._dtype_triple
