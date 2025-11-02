#! /bin/bash
export LLVM_DIR=/iusers/mattarde/upstream/llvm-project/build/install/
$LLVM_DIR/bin/opt -load-pass-plugin build/libIRPatternSearch.so -passes=hello-world -disable-output test/hello.ll