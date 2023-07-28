#include <iostream>
#include "escript.h"

using namespace std;
using namespace escript;

int main()
{
    EScript engine;
    std::u32string code = U"i = 1 + 2;";
    engine.eval(code);
    return 0;
}
