#include "stdafx.h"
#include "callscriptfunction_test.h"


void CallScriptFunction_Test::run()
{
    initTestCase();
    test_callFunction();
    test_callFunctionArgString();
    test_callFunctionArgArray();
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

void CallScriptFunction_Test::test_callFunctionArgString()
{
    const u32string code1 = UR"(
function fnSubStr(s, from, to) {
    return s.substring(from, to);
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.compile(code1); // или eval, если есть хоть что-нибудь кроме функций

    // ищем запись в нужной таблице символов
    auto mainTable = engine.unit()->block()->symbolTable();
    auto fnSubStrVar = mainTable->find(U"fnSubStr");
    auto functionRecord = engine.getObjectRecord(fnSubStrVar);
    assert(functionRecord->type = SymbolType::Function);
    Function *fnSubStr = (Function*)functionRecord->data;

    Machine &m = engine.machine();
    m.cpu().setPC((size_t)-1); // ставим недействительный адрес возврата,
                               // выполнение будет только до верхней границы памяти
                               // и поэтому сразу остановится
    m.cpu().pushPC();          // инструкция RET извлечёт PC при выходе из функции
    m.cpu().setPC(fnSubStr->callAddress()); // в PC ставим адрес функции

    std::u32string myString = U"Hello, world!"; // готовим данные
    int from = 7;
    int to = 12;
    m.cpu().pushToStack(myString); // помещаем аргументы в прямом порядке
    m.cpu().pushToStack(from);
    m.cpu().pushToStack(to);
    m.cpu().pushToStack(3);    // последним - число аргументов, здесь 3 аргумента
    engine.run();              // запускаем с текущего PC
    StackValue v = m.cpu().popFromStack();  // в стеке всегда одно значение после вызова функции
    assert(v.type == SymbolType::Variable); // результат помещается во временную переменную
    auto result = v.getStringValue();       // извлекаем результат
    assert(result == U"world");
}

void CallScriptFunction_Test::test_callFunctionArgArray()
{
    const u32string code1 = UR"(
function fnArrayReplace(arr, from, to) {
    for (i = 0; i < arr.length; i++) {
        if (arr[i] == from)
            arr[i] = to;
    }
    return arr;
}
)";
    EScript engine;
    engine.setShowDisassembleListing(false);
    engine.setShowTCode(false);
    engine.compile(code1); // или eval, если есть хоть что-нибудь кроме функций

    // ищем запись в нужной таблице символов
    auto mainTable = engine.unit()->block()->symbolTable();
    auto fnSubStrVar = mainTable->find(U"fnArrayReplace");
    auto functionRecord = engine.getObjectRecord(fnSubStrVar);
    assert(functionRecord->type = SymbolType::Function);
    Function *fnSubStr = (Function*)functionRecord->data;

    Machine &m = engine.machine();
    m.cpu().setPC((size_t)-1); // ставим недействительный адрес возврата,
                               // выполнение будет только до верхней границы памяти
                               // и поэтому сразу остановится
    m.cpu().pushPC();          // инструкция RET извлечёт PC при выходе из функции
    m.cpu().setPC(fnSubStr->callAddress()); // в PC ставим адрес функции

    Array *arr = new Array();  // создадим новый массив, он будет зарегистрирован в таблице объектов
    arr->addRef();             // поэтому его не нужно удалять вручную

    arr->add(PValue(5)); // готовим данные: [5, 2, -1, 5]
    arr->add(PValue(2));
    arr->add(PValue(-1));
    arr->add(PValue(5));
    int from = 5;        // заменим все 5 на 7
    int to = 7;
    m.cpu().pushArrayToStack(arr); // помещаем аргументы в прямом порядке
    m.cpu().pushToStack(from);
    m.cpu().pushToStack(to);
    m.cpu().pushToStack(3);    // последним - число аргументов, здесь 3 аргумента
    engine.run();              // запускаем с текущего PC
    StackValue v = m.cpu().popFromStack();  // в стеке всегда одно значение после вызова функции
    assert(v.type == SymbolType::Variable); // результат помещается во временную переменную
    auto result = v.getArrayValue();        // извлекаем результат -> [7, 2, -1, 5]
    assert(result->get(0).intValue == 7);   // должна заменяться предыдущая 5 на 7
    assert(result->get(1).intValue == 2);
    assert(result->get(2).intValue == -1);
    assert(result->get(3).intValue == 7);   // и здесь тоже 5 на 7
}
