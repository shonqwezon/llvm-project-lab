#include "llvm/Transforms/Utils/FuncInstr.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace llvm {

PreservedAnalyses FuncInstrPass::run(Function &func, FunctionAnalysisManager &funcAnalysisManager) {
    if (func.isDeclaration())
        return PreservedAnalyses::all(); // пропускаем extern функции

    Module *module = func.getParent();
    LLVMContext &context = module->getContext();

    FunctionCallee startFunc = module->getOrInsertFunction("instrument_start", Type::getVoidTy(context));
    FunctionCallee endFunc = module->getOrInsertFunction("instrument_end", Type::getVoidTy(context));

    errs() << "Instrumenting function: " << func.getName() << "\n";

    // вставляем instrument_start в начало entry блока
    BasicBlock &entry = func.getEntryBlock();
    IRBuilder<> builder(&*entry.getFirstInsertionPt());
    builder.CreateCall(startFunc);

    // вставляем instrument_end перед каждым return
    for (BasicBlock &basicBlock : func) {
        Instruction *term = basicBlock.getTerminator();
        if (isa<ReturnInst>(term)) {
            IRBuilder<> endBuilder(term);
            endBuilder.CreateCall(endFunc);
        }
    }

    return PreservedAnalyses::all();
}

}
