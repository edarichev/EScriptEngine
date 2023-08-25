#include "stdafx.h"
#include "callscriptfunction_test.h"


void CallScriptFunction_Test::run()
{
    initTestCase();
    test_callFunction();
    cleanupTestCase();
}

void CallScriptFunction_Test::initTestCase()
{

}

void CallScriptFunction_Test::cleanupTestCase()
{

}

void CallScriptFunction_Test::test_callFunction()
{
    const u32string code1 = UR"(
function fnMul(x, y) {
    return (x + 1) * y;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.compile(code1); // или eval, если есть хоть что-нибудь кроме функций

    // ищем запись в нужной таблице символов
    auto mainTable = engine.unit()->block()->symbolTable();
    auto fnMulVar = mainTable->find(U"fnMul");
    auto functionRecord = engine.getObjectRecord(fnMulVar);
    assert(functionRecord->type = SymbolType::Function);
    Function *fnMul = (Function*)functionRecord->data;

    Machine &m = engine.machine();
    m.cpu().setPC((size_t)-1); // ставим недействительный адрес возврата,
                               // выполнение будет только до верхней границы памяти
                               // и поэтому сразу остановится
    m.cpu().pushPC();          // инструкция RET извлечёт PC при выходе из функции
    m.cpu().setPC(fnMul->callAddress()); // в PC ставим адрес функции
    StackValue x(SymbolType::Integer, 3); // готовим аргументы
    StackValue y(SymbolType::Integer, 5);
    m.cpu().pushToStack(x);    // помещаем аргументы в прямом порядке
    m.cpu().pushToStack(y);
    m.cpu().pushToStack(2);    // последним - число аргументов, здесь 2 аргумента
    engine.run();              // запускаем с текущего PC
    StackValue v = m.cpu().popFromStack();  // в стеке всегда одно значение после вызова функции
    assert(v.type == SymbolType::Variable); // результат помещается во временную переменную
    int64_t result = v.getIntValue();       // извлекаем результат
    assert(result == (3 + 1) * 5);
}
