// RUN: %clang -cc1 -load %llvmshlibdir/PrintUserTypeInfo%pluginext -plugin print-user-type-info %s 2>&1 | %filecheck %s

int global_int;

struct Human {
    unsigned age;
    void eat() {}
};

void foo() {
    int x = 5;
}


// CHECK: Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: |_ age (unsigned int|public)
// CHECK: |_Methods
// CHECK-NEXT: |_ eat (void (void)|public)

// CHECK-NOT: global_int
// CHECK-NOT: int x
