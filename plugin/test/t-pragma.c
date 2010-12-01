#pragma myproof test "gery"

int main()
{
#pragma myproof test "caner" //error
#pragma myproof test 12 //error
#pragma myproof test tata //error
#pragma myproof toto //error

#pragma myproof test on //error
#pragma myproof test off //error

#pragma myproof instrumentation state // print state

#pragma myproof instrumentation off // change state

#pragma myproof instrumentation state // print state

#pragma myproof instrumentation on // change state

#pragma myproof instrumentation state // print state

    return 0;
}
