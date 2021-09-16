#include <llvm/Pass.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/DebugLoc.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Analysis/ValueTracking.h>
using namespace llvm;

int mallocCount = 0;
int freeCount   = 0;
int callocCount      = 0;
int loadCount = 0;
int storeCount = 0;
namespace {
  struct SkeletonPass : public ModulePass {
    static char ID;
    SkeletonPass() : ModulePass(ID) {}
    std::map<std::string, std::tuple<size_t, int> > mem_addrs;

    struct Metadata{
        int64_t line;
        int64_t col;
    };

    virtual bool runOnModule(Module &M) {
        
        auto &context = M.getContext();
        auto Int64Ty = Type::getInt64Ty(context);


    errs() << "\nThis is my pass !\n";
    for (Function &F: M) {
    for (BasicBlock &B: F) {
        for (Instruction &I: B) {
            if(CallInst* call_inst = dyn_cast<CallInst>(&I)) {
                bool isMalloc = true;
                bool isFree = true;
                bool isCalloc = true;
                bool isAlloca = true;
                bool isLoad = true;
                bool isStore = true; 
                Function *Callee = call_inst->getCalledFunction();
                if(!Callee) continue;
                        if(call_inst->getCallingConv() != llvm::CallingConv::C) continue;
                        std::string FuncName = Callee->getName().str() ;
                        Value* address = cast<Value>(call_inst);
                        Value* size = call_inst->getOperand(0);                       
                        if (call_inst->getCalledFunction()->getName() == "malloc") {
                        ///// isMalloc &= (!FuncName.compare("malloc"));
                            std::vector<Value*> args;
                            args.push_back(address);
                            args.push_back(size);
                            ///// args.push_back(ConstantInt::get(Int64Ty, instrMetadata.line, true));
                            ///// errs() << "\nWe have malloc() calls\n";
                            errs() << "\n";
                            errs() << "size of allocation : " << *(call_inst->getOperand(0)) <<"\n";
                            errs() << "address of allocation : " << (address) <<"\n";
                            ///// errs() << args->size;
                            mallocCount++;
                        }
                         if (call_inst->getCalledFunction()->getName() == "free") {    
                            Value* addrs = call_inst->getOperand(0);                                     
                            std::vector<Value*> args;
                            args.push_back(address);
                            args.push_back(size);
                            Value *freeAddr = call_inst->getArgOperand(0);
                            errs() << "\n";
                            errs() << "address of free() : " << freeAddr <<"\n";
                            errs() << "address of free() : " << &addrs <<"\n";
                            freeCount++;
                        }

                        /*  
                        isCalloc &= (!FuncName.compare("calloc"));
                        isAlloca &= (!FuncName.compare("alloca"));
                        isLoad   &= (!FuncName.compare("load"));
                        isStore  &= (!FuncName.compare("store"));
                        */
                        /*
                        if(isFree == true){
                                errs() << "\nWe have free() calls\n";
                                freeCount++;
                        }
                        if(isCalloc == true) {
                                errs() << "\nWe have calloc() calls \n" ;
                                callocCount++ ;
                        }
                        
                        if(isLoad == true){
                                errs() << "\nWe have load() calls";
                        }
                        if(isStore == true){
                                errs() << "\nWe have store() calls ";
                        }
                        */
                    
                    ///// Value *str1Pointer = call_inst->getArgOperand(0);
                    ////// std::string Str1, Str2;
                    ///// StringRef TmpStr;
                    ///// getConstantStringInfo(str1Pointer, TmpStr);
                    ////// errs() << call_inst->getArgOperand(0);
                    ///// errs() << "\n";
            }
        }        
      } 
    }

    /*
     errs() << "Malloc counts : \n";
        errs() << mallocCount;
    errs() << "\nFree Counts :\n";
        errs() << freeCount;    
        if(mallocCount != freeCount){
            errs() << "\n I can say, you have a Use After Free Vulnerability or a Memory leak in parts of your code";
        }
    */
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
