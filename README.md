# EScriptEngine Version 0.1 alpha
Это встраиваемый движок ECMAScript-подобного языка с динамической типизацией для программ на C++.

Его можно использовать для написания скриптов, макросов и задач для расширения функциональности основной программы.

Пример:
```C++
const std::string macro1 =
R"(
    function updateCell(row, col, v) {
        spreadsheet.setCellValue(row, col, v);
    }
    updateCell(1, 3, "Hello, world");
    updateCell(1, 2, 12345);
    s = spreadsheet.getCellValue(1, 3);
    s += "!!!!";
    updateCell(0, 0, s);
)";
    const std::u32string code1 = to_u32string(macro1);
    MySpreadSheet spreadsheet; // объект, с которым нужно работать
    EScript engine;            // скриптовый движок
    // добавляем переменную в таблицу символов, например, под именем "spreadsheet"
    engine.attachObject(&spreadsheet, U"spreadsheet");
    // выполняем макрос macro1
    engine.eval(code1);
    // отсоединяем объект, т.к. он был создан статически
    engine.detachObject(&spreadsheet);
    // проверяем результат
    assert(spreadsheet.getCellValue(1, 3) == U"Hello, world");
    assert(spreadsheet.getCellValue(1, 2) == U"12345");
    assert(spreadsheet.getCellValue(0, 0) == U"Hello, world!!!!");
```
## Типы данных
Доступны следуюшие типы данных:

|Тип|Размер|Примечания|
|-|-|-|
|Integer|64 бита|int64_t|
|Real|64 бита|double|
|String||Внутренне - это строка std::u32string, можно задавать строковый литерал в одинарных, двойных или обратных кавычках|
|Array||Ассоциативный массив|
|Function||Функция|
|Object||Пользовательский объект|

## Переменные
Переменные объявляются ключевым словом `var` или вообще без ключевого слова, просто при присваивании.
```javascript
var x = 1;
y = "hello";
```
Точка с запятой после инструкции обязательна.
## Отладочная печать
Объект для отладочной печати - `console`, как и в JavaScript:
```javascript
z = 123.45;
console.log("z=", z);
```
Чтобы перенаправить логи в другой поток, отличный от `std::cout`, явно укажите это:
```C++
    const std::string macro1 = R"(
        z = 123.45;
        console.log("z=", z);
        )";
    const std::u32string code1 = to_u32string(macro1);
    std::stringstream ss; // например, в строковый поток
    EScript engine;
    engine.setOutStream(ss); // установить выходной поток
    engine.eval(code1);
    std::cout << ss.rdbuf();
    ss.clear();
    ss.str(""); // сброс
```
Объект `console` ведёт себя как и в JavaScript (или, точнее, как в nodejs), вставляя пробел между выводимыми значениями и символ переноса строки в конце.

# Функции
Функции объявляются с помощью ключевого слова `function`:
```C++
    const std::string macro1 = R"(
function doSomething(msg, x, y) {
    result = msg + (x + y);
    return result;
}

x = doSomething("Result=", 123, 456);
console.log(x);
)";
    const std::u32string code1 = to_u32string(macro1);
    EScript engine;
    engine.eval(code1);
```
Вы можете использовать рекурсию:
```C++
    const u32string code1 =
U"function factorial(i) { "
"    if (i == 0)"
"        return 1;"
    "return i * factorial(i - 1);"
"}"
"y = factorial(10);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3628800, record->data));
```


















