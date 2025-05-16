//=============================================================================
// FILE:
//    IRPatternSearch.cpp
//
// DESCRIPTION:
//    Visits all functions in a module, prints their names and the number of
//    arguments via stderr. Strictly speaking, this is an analysis pass (i.e.
//    the functions are not modified). However, in order to keep things simple
//    there's no 'print' method here (every analysis pass should implement it).
//
// USAGE:
//    New PM
//      opt -load-pass-plugin=libIRPatternSearch.dylib -passes="hello-world" `\`
//        -disable-output <input-llvm-file>
//
//
// License: MIT
//=============================================================================
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;


static cl::opt<std::string> CheckFuncName(cl::Positional,
                                          cl::desc("Function name to check"),
                                          cl::init("foo"),
                                          cl::value_desc("checkfuncname"));
//-----------------------------------------------------------------------------
// IRPatternSearch implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
namespace {
// https://github.com/dtcxzyw/llvm-opt-benchmark/blob/main/daggrep.cpp
bool matchPattern(Function &F) {
  for (auto &BB : F) {
    for (auto &I : BB) {
        return true;
      }
    }
  }

  return false;
}
// This method implements what the pass does
void visitor(Function &F) {

    if (CheckFuncName == "*" || F.getName() == CheckFuncName) {
      errs() << "[OPT-SO] "<< F.getName() << "\n";
      errs() << "[OPT-SO] number of arguments: " << F.arg_size() << "\n";
    }  
}

// New PM implementation
struct IRPatternSearch : PassInfoMixin<IRPatternSearch> {
  // Main entry point, takes IR unit to run the pass on (&F) and the
  // corresponding pass manager (to be queried if need be)
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
    visitor(F);
    return PreservedAnalyses::all();
  }

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }
};
} // namespace

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getIRPatternSearchPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "IRPatternSearch", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "hello-world") {
                    FPM.addPass(IRPatternSearch());
                    return true;
                  }
                  return false;
                });
          }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize IRPatternSearch when added to the pass pipeline on the
// command line, i.e. via '-passes=hello-world'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getIRPatternSearchPluginInfo();
}
