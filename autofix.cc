
#include <llvm/IR/IRBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/Compiler.h>
#include <llvm/Support/raw_ostream.h>

#include <string>

using namespace llvm;


AutoFixMemLeaks::Result AutoFixMemLeaks::runOnModule(Module &M) {
    llvm::string Res ;

    for(Module::iterator fi = M.begin(); fi != M.end() ; ++fi){
        for(Function::iterator bi = fi->begin() ; bi != bi->end() ; ++bi){
            for(BasicBlock::iterator it = bi->begin() ; fi->end() ; ++fi){
                Instruction *I = &*it;
            }
        }
    }
}

PreservedAnalyses AutoFixMemLeaks::run(llvm::Module M, llvm::ModuleAnalysisManager &MAM){
    /// Initialize run 
    return PreservedAnalyses::all();
}

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getInjectFuncCallPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "legacy-autofix-mem-leaks", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "legacy-autofix-mem-leaks") {
                    MPM.addPass(autofixmemleaks());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getInjectFuncCallPluginInfo();
}

//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
char LegacyInjectFuncCall::ID = 0;

// Register the pass - required for (among others) opt
static RegisterPass<LegacyInjectFuncCall>
    X(/*PassArg=*/"legacy-autofix-mem-leaks", /*Name=*/"autofixmemleaks",
      /*CFGOnly=*/false, /*is_analysis=*/false);