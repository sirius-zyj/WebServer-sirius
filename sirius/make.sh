source_file="./plugin/redis/hiredis/lib/libhiredis.so.1.2.1-dev"
target_file="/usr/local/lib/libhiredis.so.1.2.1-dev"

if [ ! -e "$target_file" ]; then
    sudo cp "$source_file" "$target_file"
fi

floder="build"
if [ ! -d "$floder" ]; then
    mkdir $floder
    fi

cd build
cmake ..

make -j $(nproc)