
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

namespace {
  struct AutoFixMemLeaksPass : public FunctionPass {
    static char ID;
    AutoFixMemLeaksPass() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M) {
    llvm::string Res ;
    CallInst *callInst = nullptr;

    for(Module::iterator fi = M.begin(); fi != M.end() ; ++fi){
        for(Function::iterator bi = fi->begin() ; bi != bi->end() ; ++bi){
            for(BasicBlock::iterator it = bi->begin() ; fi->end() ; ++fi){

                Instruction *I = &*it;
                if((callInst = dyn_cast<CallInst>(&I))){
                    bool isMalloc = true;
                    Function *Callee = callInst->getCalledFunction();
                    if(!Callee) continue ;
                    if(!callInst->getCallingConv() != llvm::CallingCov::C) continue;
                    std::string FuncName = Callee->getName.str();
                    isMalloc &= (!FuncName.comapre("malloc"));
                }
            } 
        }
    }
    return true;
}
char SkeletonPass::ID = 0;
static RegisterPass<SkeletonPass> X("autofixmemleakspass", "Fix mem leaks in a module",
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