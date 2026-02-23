// RUN: %clang -cc1 -load %llvmshlibdir/PrintUserTypeInfo%pluginext -plugin print-user-type-info %s 2>&1 | %filecheck %s

struct Human {
  unsigned int age;
  unsigned int height;

  virtual void sleep() = 0;
  virtual void eat() = 0;
};

struct Engineer : public Human {
  unsigned int salary;

  void sleep() override {}
  void eat() override {}
  void work() {}
};

// CHECK: Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ age (unsigned int|public)
// CHECK-NEXT: | |_ height (unsigned int|public)
// CHECK: |_Methods
// CHECK-NEXT: | |_ sleep (void (void)|public|virtual|pure)
// CHECK-NEXT: | |_ eat (void (void)|public|virtual|pure)
// CHECK-NEXT: | |_ operator= (Human &(const Human &)|public)
// CHECK-NEXT: | |_ operator= (Human &(Human &&)|public)
// CHECK-NEXT: | |_ ~Human (void (void)|public)
// CHECK: Engineer -> Human
// CHECK-NEXT: |_Fields
// CHECK-NEXT: | |_ salary (unsigned int|public)
// CHECK: |_Methods
// CHECK-NEXT: | |_ sleep (void (void)|public|override)
// CHECK-NEXT: | |_ eat (void (void)|public|override)
// CHECK-NEXT: | |_ work (void (void)|public)
// CHECK-NEXT: | |_ operator= (Engineer &(const Engineer &)|public)
// CHECK-NEXT: | |_ operator= (Engineer &(Engineer &&)|public)
// CHECK-NEXT: | |_ ~Engineer (void (void)|public)
