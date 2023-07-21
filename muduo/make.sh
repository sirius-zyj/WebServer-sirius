floder="build"
if [ ! -d "$floder" ]; then
    mkdir $floder
    fi

cd build
cmake ..

make -j $(nproc)