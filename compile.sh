export LLVM_DIR=/iusers/mattarde/upstream/llvm-project/build/install/
mkdir -p build
cd build
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR $PWD/../IRPatternSearch/
make