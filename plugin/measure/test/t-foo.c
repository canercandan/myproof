void foo( void )
{
    myproof_measure_start("foo");
    myproof_measure_stop();
}

int main(void)
{
    foo();

    return 0;
}
