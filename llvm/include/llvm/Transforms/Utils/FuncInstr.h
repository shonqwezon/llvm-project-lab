#ifndef LLVM_TRANSFORMS_UTILS_FUNC_INSTR_H
#define LLVM_TRANSFORMS_UTILS_FUNC_INSTR_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"

namespace llvm {

class FuncInstrPass : public PassInfoMixin<FuncInstrPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

}

#endif
