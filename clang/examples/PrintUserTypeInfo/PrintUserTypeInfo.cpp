#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;

namespace {

std::string getAccessString(AccessSpecifier accessSpecifier) {
    switch (accessSpecifier) {
        case AS_public: return "public";
        case AS_protected: return "protected";
        case AS_private: return "private";
        case AS_none: return "none";
    }
    return "unknown";
}

std::string getMethodQualifiers(CXXMethodDecl *methodDeclaration) {
    std::string qualifiers;
    if (methodDeclaration->isVirtualAsWritten())
        qualifiers += "|virtual";
    if (methodDeclaration->isPureVirtual())
        qualifiers += "|pure";
    if (methodDeclaration->size_overridden_methods() > 0)
        qualifiers += "|override";
    return qualifiers;
}

class ASTVisitor : public RecursiveASTVisitor<ASTVisitor> {
public:
    explicit ASTVisitor(ASTContext &ctx) : context(ctx) { }
    bool VisitCXXRecordDecl(CXXRecordDecl *declaration) {

        // Ignore forward declaration and compiler structures
        if (!declaration->isThisDeclarationADefinition() || declaration->isImplicit())
            return true;
        if (!context.getSourceManager().isWrittenInMainFile(declaration->getLocation()))
            return true;

        llvm::errs() << declaration->getNameAsString();

        if (declaration->getNumBases() > 0) {
            llvm::errs() << " -> ";

            // Traverse parent classes
            for (auto it = declaration->bases_begin(); it != declaration->bases_end(); it++) {
                auto *baseType = it->getType()->getAsCXXRecordDecl();
                if (baseType) {
                    llvm::errs() << baseType->getNameAsString();
                    if (std::next(it) != declaration->bases_end())
                        llvm::errs() << ", ";
                }
            }
        }
        llvm::errs() << "\n";


        llvm::errs() << "|_Fields\n";
        for (auto *field : declaration->fields()) {
            llvm::errs() << "| |_ " << field->getNameAsString()
                         << " (" << field->getType().getAsString()
                         << "|" << getAccessString(field->getAccess()) << ")\n";
        }

        llvm::errs() << "\n|_Methods\n";
        for (auto *method : declaration->methods()) {
            llvm::errs() << "| |_ " << method->getNameAsString()
                         << " (" << method->getType().getAsString()
                         << "|" << getAccessString(method->getAccess())
                         << getMethodQualifiers(method) << ")\n";
        }

        llvm::errs() << "\n";
        // Continue traversing
        return true;
    }

private:
    ASTContext &context;
};

// AST Context Handler
class PrintUserTypeConsumer : public ASTConsumer {
public:
    explicit PrintUserTypeConsumer(ASTContext &context) : astVisitor(context) { }
    virtual void HandleTranslationUnit(ASTContext &context) override {
        astVisitor.TraverseDecl(context.getTranslationUnitDecl());
    }

private:
    ASTVisitor astVisitor;
};

// Entry point of plugin
class PrintUserTypeAction : public PluginASTAction {
protected:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance &CI, llvm::StringRef InFile) override {
        return std::make_unique<PrintUserTypeConsumer>(CI.getASTContext());
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override {
        for (unsigned i = 0, e = args.size(); i != e; ++i) {
            llvm::errs() << "PrintUserTypeAction arg = " << args[i] << "\n";
        }
        if (!args.empty() && args[0] == "help")
            PrintHelp(llvm::errs());

        return true;
    }

    void PrintHelp(llvm::raw_ostream& ros) {
        ros << "Help for PrintUserTypeInfo plugin goes here\n";
    }
};

}

// Registry entry point
static FrontendPluginRegistry::Add<PrintUserTypeAction>
X("print-user-type-info", "Print user types info");
