# EScriptEngine Version 0.1 alpha
Это встраиваемый движок C-подобного языка подмножества ECMAScript с динамической типизацией для программ на C++.

Его можно использовать для написания скриптов, макросов и задач для расширения функциональности основной программы.

## Целевая платформа
Linux, x64, C++17.

## Пример
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
// Предположим, в нашей программе есть какая-то таблица, как LibreOffice.Calc
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
Полный пример - в тестах (`automation_test.cpp`).
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

## Операции
Доступны обычные операции для C-подобных языков:

`+, -, *, /, %, &, |, ^, ~, &&, ||, ++, --, <<, >>, >>>`

И соответствующие им операторы присваивания

`+=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=, >>>=`

Операция `>>>` - сдвиг беззнакового целого.

Логические операции:

`<, >, ==, <=, >=`


## Отладочная печать
Объект для отладочной печати - `console`, как и в JavaScript:
```javascript
z = 123.45;
console.log("z=", z);
```
Чтобы перенаправить вывод в другой поток, отличный от `std::cout`, явно укажите это:
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
Данные выводятся в кодировке UTF-8. Объект `console` ведёт себя как и в JavaScript (или, точнее, как в nodejs), вставляя пробел между выводимыми значениями и символ переноса строки в конце.

## Ветвления
### if/else
if/else используются стандартным образом:
```javascript
i = 0;
if (true) i = 6;
if (false) i = 7; else i = 11;
...
if (false) i = 6;
else if (false) i = 5;
else i = 7;
```
## Циклы
Циклы придерживаются в основном общепринятого для C-подобных языков синтаксиса, можно использовать break/continue.
### while
```javascript
i = 0;
while (i < 10) {
    i = i + 1;
    if (i < 3)
        continue;
    break;
}
```
### do...while
```javascript
i = 0;
do {
    i = i + 1;
    if (i > 5) break;
} while (true);
```
### for
```javascript
x = 1;
for (i = 1; i < 10; i = i + 1) {
    x = x * i;
}
```
## Массивы
Массивы могут содержать числа, строки. Индекс массива - это строка. Даже если используется целое число, оно переводится в строку и служит в качестве ключа. Таким образом, массив является ассоциативным. Следующие примеры обращения к элементам допустимы:
```javascript
a = [];
a[0] = "hello";
var x = a[0];
```

```javascript
a = [];
a["key1"] = 123;
var x = a["key1"];
```

Даже вещественные числа могут быть ключом (хотя для совместимости с кодом JavaScript/ECMAScript это можно не делать, но тем не менее):
```javascript
a = [];
a[10.5] = "hello";
var x = a[10.5];
```
### Пример работы с массивом: сортировка
Пример взят [отсюда](https://stackoverflow.com/questions/5185864/javascript-quicksort).

Заметим, что все округления убраны: движок пытается по возможности сохранить тип Integer, тогда как при делении JavaScript переводит всё в вещественные.
```javascript
var array = [8, 2, 5, 7, 4, 3, 12, 6, 19, 11, 10, 13, 9];

function quickSort(arr, left, right)
{
    var i = left;
    var j = right;
    var tmp;
    pivotidx = (left + right) / 2;
    var pivot = arr[pivotidx];
    /* partition */
    while (i <= j)
    {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j)
        {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }

    /* recursion */
    if (left < j)
        quickSort(arr, left, j);
    if (i < right)
        quickSort(arr, i, right);
    return arr;
}

quickSort(array, 0, array.length -1);
console.log(array);
```

## Функции
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

Можно присвоить функцию переменной:

```javascript
function func1(i) {
    return 123+i;
}
fnRef = func1;
y = fnRef(45);
```

Или так:

```javascript
fnRef = function func1(i) { return 123+i; };
x = func1(7);
y = fnRef(45);
```

Пример рекурсивного вызова:
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
## Объекты автоматизации
Для написания каких-то макросов и выполнения полезной работы нужно создать класс, производный от `escript::AutomationObject`. В частности, сам объект `console` и строка являются производными от этого класса и ничем в работе не отличаются. Хотя строки - это отдельная сущность, хранящаяся в таблице строк отдельно от объектов, работа с ними аналогична работе с любым производным от `AutomationObject`.

Каждый класс, производный от `AutomationObject`, может переопределить метод `call`. "Может", т.к. новых методов у него может и не быть.

В методе `call` мы получаем прямой доступ к процессору и стеку, из которого требуется извлечь аргументы в нужном количестве. Число аргументов в скриптах не гарантируется и не проверяется - метод объекта автоматизации сам решает, сколько параметров ему нужно, и может менять их назначение в зависимости от их количества.

Однако, метод ***обязан вытащить из стека все аргументы и затолкать туда после вызова один результат*** (можно просто поместить 0).

Вот набросок для объекта автоматизации:

```C++
class MySpreadSheet : public AutomationObject
{
    using BaseClass = AutomationObject;
    std::vector<std::vector<std::u32string> > _cells;
    static constexpr const int ROWS = 10;
    static constexpr const int COLUMNS = 5;
public:
    MySpreadSheet() : _cells(ROWS)
    {
        for (size_t row = 0; row < _cells.size(); row++) {
            _cells[row].resize(COLUMNS);
        }
    }
    // AutomationObject interface
public:
    virtual bool call(const u32string &method, Processor *p) override;
    void setCellValue(int row, int col, std::u32string str)
    {
        checkIndexes(row, col);
        _cells[row][col] = std::move(str);
    }
    void setCellValue(int row, int col, const StackValue &value)
    {
        setCellValue(row, col, value.uString());
    }
    const std::u32string getCellValue(int row, int col) const
    {
        checkIndexes(row, col);
        return _cells[row][col];
    }
private:
    void checkIndexes(int rowIndex, int columnIndex) const
    {
        if (rowIndex < 0 || rowIndex >= ROWS)
            throw std::out_of_range("row index is out of range");
        if (columnIndex < 0 || columnIndex >= COLUMNS)
            throw std::out_of_range("column index is out of range");
    }
};

bool MySpreadSheet::call(const u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    if (method == U"getCellValue") {
        auto argCount = p->popFromStack().value; // число аргументов == 2
        assert(argCount == 2);
        auto argColumnIndex = p->popFromStack(); // индекс столбца
        auto argRowIndex = p->popFromStack(); // индекс строки
        int columnIndex = argColumnIndex.getIntValue();
        int rowIndex = argRowIndex.getIntValue();
        std::u32string value = getCellValue(rowIndex, columnIndex);
        StringObject *newString = new StringObject(value);
        p->pushToStack(SymbolType::String, (uint64_t)newString);
        return true;
    }
    throw std::domain_error("Call of unknown method: MySpreadSheet." + to_utf8(method));
}
```

Можно делать не только методы, но и свойства. Технически свойство - это тоже метод. Соглашение таково: 
* свойство для чтения - это метод без параметров, начинающийся с `get_` и возвращающий результат
* свойство для записи - это метод с одним параметром, начинающийся с `set_` и возвращающий произвольный результат, например, 0.

Пример:

```C++
bool Array::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    if (method == U"get_length") {
        // вытащить из стека аргументы
        // число аргументов, здесь должен быть 0, поэтому просто пропустить
        p->popFromStack();
        int64_t p1 = length();
        p->pushToStack(p1); // результат
        return true;
    }
    // прочий код...
}
```
Использование в скрипте:
```javascript
a = [1,2,3];
x = a.length;
```

Однако, ни что не мешает вызвать метод прямо так:
```javascript
a = [1,2,3];
x = a.get_length();
```












