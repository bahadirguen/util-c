cd build
cmake -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
cmake --build .
