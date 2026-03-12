; RUN: opt -passes=function-instrumentation -S %s | FileCheck %s

declare void @instrument_start()
declare void @instrument_end()

define i32 @max(i32 %x, i32 %y) {
entry:
  %cmp = icmp sgt i32 %x, %y
  br i1 %cmp, label %then, label %else

then:
  ret i32 %x

else:
  ret i32 %y
}

; CHECK: call void @instrument_start()
; CHECK: br i1
; CHECK-NOT: call void @instrument_end()
; CHECK: call void @instrument_end()
; CHECK: call void @instrument_end()
