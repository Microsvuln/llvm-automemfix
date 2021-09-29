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

    Value *fp;
    Value *filename;
    Value *perms;
    Value *retval;
    Value *malloc_str;
    Value *free_str;
    Function *F_close, *F_open, *F_printf;

    SkeletonPass() : ModulePass(ID) {}
    std::map<std::string, std::tuple<size_t, int> > mem_addrs;

    struct Metadata{
        int64_t line;
        int64_t col;
    };

    struct Metadata getLineAndCol(Instruction &I){
      struct Metadata instrMetadata = {-1, -1};
      const DebugLoc &debugInfo = I.getDebugLoc();
      if(debugInfo){
        instrMetadata.line = (int64_t) debugInfo.getLine();
        instrMetadata.col  = (int64_t) debugInfo.getCol();
      }
      return instrMetadata;
    }

    virtual bool runOnModule(Module &M) {
        
    auto &context = M.getContext();
    auto Int64Ty = Type::getInt64Ty(context);
    auto Int32PtrTy = Type::getInt32PtrTy(context);
    auto Int8PytTy = Type::getInt8PtrTy(context);
    auto VoidTy = Type::getVoidTy(context);
    Constant* init = M.getOrInsertFunction("Initialize", VoidTy, false);

    std::vector<Type*> mallocArgs;
    mallocArgs.push_back(Int8PtrTy);
    mallocArgs.push_back(Int64Ty);
    mallocArgs.push_back(Int64Ty);
    mallocArgs.push_back(Int64Ty);
    Constant* log_malloc = M.getOrInsertFunction("LogMalloc", FunctionType::get(VoidTy, mallocArgs, false));
    

    
    Function* main = M.getFunction("main");
    BasicBlock* first_bb = &*main->begin();
    BasicBlock::iterator insertion_pt = first_bb->getFirstInsertionPt();

    IRBuilder<>* builder = new IRBuilder<>(context);
    builder->setInsertPoint(&*insertion_pt);
    CallInst* call = builder->CreateCall(init);

    errs() << "\nThis is my pass !\n";
    for (Function &F: M) {
    for (BasicBlock &B: F) {
        for (Instruction &I: B) {
            if(CallInst* call_inst = dyn_cast<CallInst>(&I)) {
                Function* fn = call_inst->getCalledFunction();   
                StringRef fn_name = fn->getName();
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
                            std::vector<Value*> args;
                            args.push_back(address);
                            args.push_back(size);
                            ///// args.push_back(ConstantInt::get(Int64Ty, instrMetadata.line, true));
                            ///// errs() << "\nWe have malloc() calls\n";
                            errs() << "\n";
                            ///// errs() << "size of allocation : " << *(call_inst->getOperand(0)) <<"\n";
                            errs() << "address of allocation : " << (address) <<"\n";
                            ///// errs() << args->size;
                            mallocCount++;
                        }
                        if (call_inst->getCalledFunction()->getName() == "calloc") {                            
                            std::vector<Value*> args;
                            args.push_back(address);
                            args.push_back(size);
                            ///// args.push_back(ConstantInt::get(Int64Ty, instrMetadata.line, true));
                            errs() << "\n";
                            ///// errs() << "size of allocation : " << *(call_inst->getOperand(0)) <<"\n";
                            errs() << "address of allocation : " << (address) <<"\n";
                            ///// errs() << args->size;
                            callocCount++;
                        }
                        if (call_inst->getCalledFunction()->getName() == "realloc") {                            
                            std::vector<Value*> args;
                            args.push_back(address);
                            args.push_back(size);
                            ///// args.push_back(ConstantInt::get(Int64Ty, instrMetadata.line, true));
                            ///// errs() << "\nWe have malloc() calls\n";
                            errs() << "\n";
                            ///// errs() << "size of allocation : " << *(call_inst->getOperand(0)) <<"\n";
                            errs() << "address of allocation : " << (address) <<"\n";
                            ///// errs() << args->size;
                            reallocCount++;
                        }
                         if (call_inst->getCalledFunction()->getName() == "free") {    
                            Value* addrs = call_inst->getOperand(0);  
                            /*
                            for(auto arg = Callee->arg_begin(); arg != Callee->arg_end(); ++arg) {
                              if(auto* ci = dyn_cast<ConstantInt>(arg)){
                                  errs() <<"\nFreeMem" << ci->getValue() << "\n";
                              }                   
                              errs() << *arg;
                            }     
                            */        
                            std::vector<Value*> args;
                            args.push_back(address);
                            args.push_back(size);
                            Value *freeAddr = call_inst->getArgOperand(0);
                            errs() << "\n";
                            errs() << "address of free() : " << freeAddr <<"\n";
                            errs() << "address of free() : " << addrs <<"\n";
                            
                            freeCount++;
                        }

   
                    
            }
        }        
      } 
    }

    
    errs() << "Malloc counts : \n";
    errs() << mallocCount;
    errs() << "\nFree Counts :\n";
    errs() << freeCount;    
    if(mallocCount != freeCount){
        errs() << "\n I can say, you have a Use After Free Vulnerability or a Memory leak in parts of your code";
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
/*
This is my pass !
malloc : 0x55cabb0910c0
i64 %0
address of allocation : 0x55cabb093b50
malloc : 0x55cabb0910c0
i64 %0
address of allocation : 0x55cabb093c80
malloc : 0x55cabb0910c0
i64 %0
address of allocation : 0x55cabb093e20
printf : 0x55cabb093400
i8* %0
free : 0x55cabb096150
i8* %0
address of free() : 0x55cabb096150
free : 0x55cabb0962d0
i8* %0
address of free() : 0x55cabb0962d0
free : 0x55cabb096450
i8* %0
address of free() : 0x55cabb096450
*/