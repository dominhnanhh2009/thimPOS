rm -rf build
mkdir build
cd build
cmake -DTHIMPOS_IGNORE_KEY_CHECK="${THIMPOS_IGNORE_KEY_CHECK:-OFF}" ..
cmake --build . -j8
