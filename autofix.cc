#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IRBuilder.h"
using namespace llvm;

namespace {
  struct SkeletonPass : public ModulePass {
    static char ID;
    SkeletonPass() : ModulePass(ID) {}
    std::map<std::string, std::tuple<size_t, int> > mem_addrs;
    virtual bool runOnModule(Module &M) {
     
    errs() << "\nThis is my pass !\n";
    for (Function &F: M) {
    for (BasicBlock &B: F) {
        for (Instruction &I: B) {
            if(CallInst* call_inst = dyn_cast<CallInst>(&I)) {

                /////errs() << "\nAt least one malloc";
            
            //// CallInst *callInst = nullptr;
           /////if(callInst = dyn_cast<CallInst>(&I)){
                bool isMalloc = true;
                Function *Callee = call_inst->getCalledFunction();
                if(!Callee) continue;
                        if(call_inst->getCallingConv() != llvm::CallingConv::C) continue;
                        std::string FuncName = Callee->getName().str() ;
                        isMalloc &= (!FuncName.compare("malloc"));       
                        if(isMalloc == true)
                                errs() << "\nWe have malloc() calls :D";
                        else
                                errs() << "\nWe have no malloc calls()";
            }


        }
      } 
    }
  
    
  
      return false;
    }
  };
}

char SkeletonPass::ID = 0;
static RegisterPass<SkeletonPass> X("skeleton", "leamem check pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerSkeletonPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new SkeletonPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerSkeletonPass);
//// How to run !?
//// opt -load ./build/skeleton/libSkeletonPass.so --skeleton < sm.bc > /dev/null