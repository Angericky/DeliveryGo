### complie the pybind11 file
## Requirement
    python 3.6
    tkinker 
    numpy

## Run
    If you are using MacOSx,you can just run 
        $ python game.py

    If you are using Win or linux,you need to compile pybind11 first
        For reference, please check http://pybind11.readthedocs.io/en/stable/
    Then run following code to link python and C++:
        $ python setup.py build_ext -i
    Finally run the backend:
        $ python route.py

