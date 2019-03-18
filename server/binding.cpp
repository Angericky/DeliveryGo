#include "pybind11/include/pybind11/pybind11.h"
#include "pybind11/include/pybind11/stl.h"
#include "pybind11/include/pybind11/stl_bind.h"
#include "pybind11/include/pybind11/functional.h"
#include "algo/main.h"

namespace py = pybind11;

PYBIND11_MODULE(deliveryGo,m) {
    m.def("run", &deliveryGo, "Algorithm for path planning",
      py::arg("fpath"));
};


