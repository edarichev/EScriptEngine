/**
 * Это проект для экспериментов
 */
#include <iostream>

using namespace std;

/*
g++ -S main.cpp
    movl	$15, -4(%rbp)
    movl	-4(%rbp), %eax
    movl	%eax, %edi
    call	_Z8testCalli
 */
inline void testCall(int c)
{
    c += c;
}



int main()
{
    int x = 15;
    testCall(x);
    return 0;
}
