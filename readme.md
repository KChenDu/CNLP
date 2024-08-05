```zsh
c++ -O3 -Wall -shared -std=c++17 -fPIC $(python3 -m pybind11 --includes) segmentation/segmentation.cpp -o segmentation$(python3-config --extension-suffix)```