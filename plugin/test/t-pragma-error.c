#pragma myproof test "string"

int main()
{
#pragma myproof test "another string" //warning ignoring
#pragma myproof test 12 //warning ignoring
#pragma myproof test tata //warning ignoring
#pragma myproof toto //warning ignoring

#pragma myproof instrumente foo ffff // two NAME instead one
#pragma myproof instrumente (func1, func2,,) // no CLOSE_PARA after last NAME
#pragma myproof instrumente (func1, func2 // no CLOSE_PARA

    return 0;
}
