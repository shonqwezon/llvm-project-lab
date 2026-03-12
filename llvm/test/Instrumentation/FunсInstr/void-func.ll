; RUN: opt -passes=function-instrumentation -S %s | FileCheck %s

declare void @instrument_start()
declare void @instrument_end()

define void @bar() {
entry:
  ret void
}

; CHECK: call void @instrument_start()
; CHECK: call void @instrument_end()
