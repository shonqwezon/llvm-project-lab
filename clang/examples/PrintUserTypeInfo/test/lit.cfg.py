import os
import lit.formats

config.name = "PrintUserTypeInfo"
config.test_format = lit.formats.ShTest()
config.suffixes = [".c", ".cpp", ".test"]

llvm_build_dir = os.environ.get("LLVM_BUILD_DIR", os.getcwd())
llvm_tools_dir = os.path.join(llvm_build_dir, "bin")
llvm_lib_dir   = os.path.join(llvm_build_dir, "lib")

config.substitutions.append(("%clang", os.path.join(llvm_tools_dir, "clang++")))
config.substitutions.append(("%llvmshlibdir", llvm_lib_dir))
config.substitutions.append(("%filecheck", os.path.join(llvm_tools_dir, "FileCheck")))
config.substitutions.append(("%pluginext", ".so"))
