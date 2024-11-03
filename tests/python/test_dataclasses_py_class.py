import mlc


@mlc.dataclasses.py_class("mlc.testing.MyPyObj")
class MyPyObj(mlc.PyClass):
    x: int
    y: float
    z: str


def test_py_class_init() -> None:
    obj = MyPyObj(1, y=2.0, z="str")  # type: ignore[call-arg]
    assert obj.x == 1
    assert obj.y == 2.0
    assert obj.z == "str"


def test_py_class_set_attr() -> None:
    obj = MyPyObj(1, y=2.0, z="str")  # type: ignore[call-arg]
    obj.x = 2
    obj.y = 4
    obj.z = ""
    assert obj.x == 2
    assert obj.y == 4.0
    assert obj.z == ""


def test_py_class_repr() -> None:
    obj = MyPyObj(1, y=2.0, z="str")  # type: ignore[call-arg]
    expected = "mlc.testing.MyPyObj(x=1, y=2.0, z='str')"
    assert repr(obj) == expected
    assert str(obj) == expected