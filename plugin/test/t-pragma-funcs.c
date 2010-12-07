#pragma myproof instrumente foo
#pragma myproof instrumente (func1, func2, func3)

void foo(){}
void func1(){}
void func2(){}

int main()
{
    foo(); func1(); func2();
    return 0;
}
