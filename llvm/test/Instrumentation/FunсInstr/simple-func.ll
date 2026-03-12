; RUN: opt -passes=function-instrumentation -S %s | FileCheck %s

declare void @instrument_start()
declare void @instrument_end()

define i32 @foo() {
entry:
  %a = add i32 1, 2
  ret i32 %a
}

; CHECK: call void @instrument_start()
; CHECK: call void @instrument_end()
