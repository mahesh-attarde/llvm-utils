export LLVM_DIR=??/llvm-project/build/install/
mkdir build
cd build
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR $PWD/../??
make
