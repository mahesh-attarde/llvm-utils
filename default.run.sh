#! /bin/bash
export LLVM_DIR=??/llvm-project/build/install/
$LLVM_DIR/bin/opt -load-pass-plugin build/lib??.so -passes=hello-world -disable-output test/??.ll