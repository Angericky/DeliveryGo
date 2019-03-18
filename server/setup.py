
import os, sys

from distutils.core import setup, Extension
from distutils import sysconfig

cpp_args = ['-std=c++11','-mmacosx-version-min=10.7','-stdlib=libc++']


ext_modules = [
    Extension(
    'deliveryGo',
        ['algo/main.cpp','binding.cpp'],
        include_dirs=['pybind11/include'],
    language='c++',
    extra_compile_args = cpp_args,
    ),
]

setup(
    name='deliveryGo',
    version='0.0.1',
    author='wxw',
    author_email='DeliveryGo@gamil.com',
    description='DeliveryGo',
    ext_modules=ext_modules,
)