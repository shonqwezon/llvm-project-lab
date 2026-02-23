add_llvm_library(PrintUserTypeInfo MODULE
  PrintUserTypeInfo.cpp
  PLUGIN_TOOL clang
)

if(WIN32 OR CYGWIN)
  set(LLVM_LINK_COMPONENTS
    Support
  )
  clang_target_link_libraries(PrintUserTypeInfo PRIVATE
    clangAST
    clangBasic
    clangFrontend
  )
endif()

if(BUILD_TESTING)
  add_lit_testsuite(check-PrintUserTypeInfo
    "Running PrintUserTypeInfo plugin tests"
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    DEPENDS PrintUserTypeInfo
  )
endif()
