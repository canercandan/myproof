#pragma myproof instrumente (func1, func2, func3)
#pragma myproof instrumente (func4, func5)
#pragma myproof instrumente func6
#pragma myproof instrumente (func11, func111)
#pragma myproof instrumente (func21, func22)

void func111() { usleep(100000); }
void func11() { usleep(5000); func111(); }
void func1() { usleep(10000); func11(); }

void func22() { usleep(3000); }
void func21() { usleep(3000); }
void func2() { usleep(1000); func21(); func22(); }

void func3() { usleep(5000); }
void func4() { usleep(1000); }
void func5() { usleep(2000); }
void func6() { usleep(3); }

int main(void)
{
    func1();
    func2();
    func3();
    func4();
    func5();
    func6();

    return 0;
}
