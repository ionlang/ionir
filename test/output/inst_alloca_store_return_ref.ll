; ModuleID = 'test'
source_filename = "test"

define void @foobar() {
entry:
  %0 = alloca i32
  store i32 1, i32* %0
  ret i32* %0
}
