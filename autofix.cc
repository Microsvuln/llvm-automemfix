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

    virtual bool runOnModule(Module &M) {
      ///// errs() << "I saw a function called " << F.getName() << "!\n";
      ///// errs() << "This is the function called : "<< F << "\n"; 
      //
     errs() << "\nThis is my pass !";
            /*
  for (auto &F : M) {
      for(auto& B:F){
              ///// errs() << "Basic block is : " << B << "\n";      
        for(auto& I:B){
                ///// errs() << "Instrunction is : " << I << "\n";
                if(auto* op = dyn_cast<BinaryOperator>(&I)){
                        ///// errs() << *op << "\n";
                        IRBuilder<> builder(op);
                        Value *lhs = op->getOperand(0);
                        Value *rhs = op->getOperand(1);
                        Value *mul = builder.CreateMul(lhs, rhs);

                        for(auto&  U : op->uses()){
                                User* user = U.getUser();
                                user->setOperand(U.getOperandNo(), mul);
                                 
                        }
                        return true;
                }


        }
      }  
  }
  */
      return false;
    }
  };
}

char SkeletonPass::ID = 0;
static RegisterPass<SkeletonPass> X("skeleton", "Skeleton my pass",
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