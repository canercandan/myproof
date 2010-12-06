#pragma myproof instrumente foo
#pragma myproof instrumente (func1, func2, func3)

void foo(){}
void func1(){}
void func2(){}

//void test( const char *str ) { printf("toto\n"); }
void test( const char *str ) { printf("print me \"%s\"\n", str); }

int main()
{
    foo(); func1(); func2();
    return 0;
}
