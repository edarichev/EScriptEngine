# Eugen-Script Version 0.1 alpha
Eugen-Script (кратко: escript).
Это встраиваемый движок C-подобного языка подмножества ECMAScript с динамической типизацией для программ на C++.
Его можно использовать для написания скриптов, макросов и решения других задач расширения функциональности основной программы.

Язык по возможности похож на ECMAScript/JavaScript, но всё же он отличается. 
Например, в данный момент нет таких понятий, как `null` и `undefined`, арифметические опреации максимально стараются сохранять тип `Integer`.

## Целевая платформа
Linux, x64, C++17.

## Пример
```C++
// Предположим, в нашей программе есть какая-то таблица:
MySpreadSheet spreadsheet; // объект, с которым нужно работать
EScript engine;            // скриптовый движок

// напишем скрипт:
const std::u32string code1 = UR"(
    function updateCell(row, col, v) {
        spreadsheet.setCellValue(row, col, v);
    }
    updateCell(1, 3, "Hello, world");
    updateCell(1, 2, 12345);
    s = spreadsheet.getCellValue(1, 3);
    s += "!!!!";
    updateCell(0, 0, s);
    )";

// добавляем переменную в таблицу символов, например, под именем "spreadsheet"
engine.attachObject(&spreadsheet, U"spreadsheet");
// выполняем макрос
engine.eval(code1);
// отсоединяем объект, т.к. он был создан статически
engine.detachObject(&spreadsheet);
// проверяем результат
assert(spreadsheet.getCellValue(1, 3) == U"Hello, world");
assert(spreadsheet.getCellValue(1, 2) == U"12345");
assert(spreadsheet.getCellValue(0, 0) == U"Hello, world!!!!");
```


Движок принимает исполняемый скрипт в виде строки `std::u32string`, поэтому вы можете использовать:

* литералы `U"32-битная строка"`;
* литералы `UR"(многострочная 32-битная строка)"`;
* вспомогательную функцию `escript::to_u32string()`;
* другие средства преобразования в `std::u32string`.

## Типы данных
Доступны следуюшие типы данных:

|Тип|Размер|Примечания|
|-|-|-|
|Integer|64 бита|int64_t|
|Real|64 бита|double|
|String|pointer|Внутренне - это строка std::u32string, можно задавать строковый литерал в одинарных, двойных или обратных кавычках|
|Array|pointer|Ассоциативный массив|
|Function|pointer|Функция|
|Object|pointer|Пользовательский объект|

У строк и чисел можно вызывать методы, в том числе и у литералов, числовые литералы при этом должны быть заключены в круглые скобки:
```javascript
x = 387.561;
s = x.toString();
...
s = (387.561).toString();
...
x = 387;
s = x.toString(16); // s == "183" (в 16-й системе)
```

## null и undefined
Пока таких понятий в escript не существует, и их введение будет откладываться.
Если понадобится, то введено будет что-то одно, а не две сущности, выполняющие похожие функции.

## Переменные
Переменные объявляются ключевым словом `var` или вообще без ключевого слова, просто при присваивании.
```javascript
var x = 1;
y = "hello";
```
`var` потребуется, если в вышестоящей области видимости находится переменная с тем же именем. 

Точка с запятой после инструкции обязательна.

## Операции
Доступны обычные операции для C-подобных языков:

`+, -, *, /, %, &, |, ^, ~, &&, ||, ++, --, <<, >>, >>>`

И соответствующие им операторы присваивания

`+=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=, >>>=`

Операция `>>>` - сдвиг беззнакового целого.

Логические операции:

`<, >, ==, <=, >=`


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

### switch/case
В блоках `case` условием выступают любые выражения.
```javascript
x = 'hello, world';
y = 6;
a = [4, 11, 3, ', world'];
switch (x) {
    case a[0] + y:
        x = 1;
        break;
    default:
        x = 15;
        break;
    case 'hello' + a[3]: { // x == 'hello, world'
        x = 2;
        break;
    }
}
```

Допускается проход из одного блока `case` или `default` в следующий после него `case/default`, т.е. `break` не является обязательным.

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
Массивы могут содержать числа, строки. 
Массив можно использовать как ассоциативный. 
Следующие примеры обращения к элементам допустимы:
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

Обратиться к элементу массива можно при объявлении:
```javascript
x = [[1,2,3], [4,5,6]][1][1]; // x == 5
```

После вызова функции:
```javascript
function fa() {
    a = [1,2,3,4,5];
    return a;
}

x = fa()[2]; // x == 3
```

Массивы могут быть любой размерности. Например, двумерный:
```javascript
a = [[1,2,3], [4,5,6]];
a[1][2] = 12;
x = a[1][2];
```
или трёхмерный:
```javascript
a =
[
    [
        [1,2,3],
        [4,5,6]
    ],
    [
        [7,8,9],
        [10,11,12]
    ]
];
a[1][0][2] = 512; // меняем 9 на 512
x = a[1][0][2];
```

Технически это означает, что в массиве содержатся другие массивы.

Поддерживаются следующие функции: 
`fill`, `filter`, `get`, `length` (get-свойство), `join`, `pop`, `push`, `reverse`, `set`, `shift`, `slice`, `sort`, `splice`, `unshift`

Функция `filter` принимает в качестве параметра функцию:
```javascript
function fnGreater(x) {
    return x > 3;
}

a = [1,2,3,4,5,6,7];
b = a.filter(fnGreater); // --> b == [4,5,6,7]
```

### Пример работы с массивом: сортировка
Пример взят [отсюда](https://stackoverflow.com/questions/5185864/javascript-quicksort).

Заметим, что все приведения к целому убраны: движок пытается по возможности сохранить тип Integer, тогда как при делении JavaScript переводит всё в вещественные.
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

## Строки

Строки можно объявить в 'однарных', "двойных" или \`обратных кавычках\`.

Однажды созданная строка не изменяется, при операциях создаются новые строки. Однако, одинаковые строки при случае могут повторно использоваться.

Поддерживаются следующие методы: 
`at, charAt, charCodeAt, endsWith, get_length, includes, indexOf, lastIndexOf, padEnd, padStart, repeat, replace, replaceAll, slice, split, startsWith, substring, toLowerCase, toUpperCase, trim, trimEnd, trimStart`
и операторы сравнения: `<, <=, ==, >=, >`.

У литерала можно вызывать методы:

```javascript
s = 'hello, 🌏'.substring(1, 3);
```

Поскольку современный Интернет немыслим без картинок-символов Unicode старше 0xFFFF, внутреннее представление строки - 32 бита. Поэтому вам не придётся возиться с кодовыми точками. Например, длина этой строки равна 8, а не 9, как если бы мы работали в стандарной 16-битовой реализации:

```javascript
x = `hello, 🌏`.length; // == 8
```
Обратиться к символам строки можно так:
```javascript
s = 'hello';
a = s[1];
b = s.at(0);
c = s.charAt(1);
d = s.charCodeAt(2);
e = 'other'[4];
console.log(a, b, c, d, e);
// вывод: e 104 e 108 r
```

Заменить символ на конкретной позиции:

```javascript
s = 'Hello';
s[1] = 'a'; // Hello -> Hallo
```
Это ещё одно отличие от JavaScript. 
Эта операция не приведёт к изменению строки, если выполнить это, например, в nodejs.

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

Ещё способ:

```javascript
testFunc = function(x) {
    return 2 * x;
};

y = testFunc(7);
```

Пример рекурсивного вызова:
```C++
    const u32string code1 =
UR"(function factorial(i) { 
    if (i == 0)
        return 1;
    return i * factorial(i - 1);
}
y = factorial(10);)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto y = mainTable->find(U"y");
    auto record = engine.getObjectRecord(y);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(3628800, record->data));
```

Можно передавать функции как аргументы в другую функцию:
```javascript
function testFunc(x) {
    return 2 * x;
}
func = testFunc;
function fnTest(pFn, x) {
    return pFn(x);
}
y = fnTest(func, 12);
z = fnTest(testFunc, 14);
```

Функции можно возвращать из других функций:
```javascript
factory = function(name) {
    switch (name) {
        case 3:
            return function(x) { return 3 * x; };
        case 5:
            return function(x) { return 5 * x; };
        default:
            break;
    }
    return function(x) { return x; };
};

func = factory(5);
y = func(12); // y==60
```

## Math

У класса Math реализованы следующие методы: `abs, acos, acosh, asin, asinh, atan, atan2, atanh, cbrt, ceil, clz32, cos, cosh, exp, expm1, floor, fround, hypot, imul, log, log10, log1p, log2, max, min, pow, random, round, sign, sin, sinh, sqrt, tan, tanh, trunc`.

## console: Отладочная печать
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

## Объекты автоматизации

### Общая концепция

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
        // здесь показана подробная работа со стеком, но можно и проще (см. ниже)
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

### Создание методов, доступных для вызова из скрипта

Вызов метода объекта автоматизации производится по его имени, поэтому если методов много, лучше сделать их вызов через хэш-таблицу, чем сооружать гигантские блоки if/else. Для этого:
1. Добавьте в ваш класс, производный от `AutomationObject`, объявление указателя на функцию-член:
```C++
typedef void (MyClass::*pFn)(Processor *p);
```
2. Добавьте в класс статическую переменную-хэш таблицу, например:
```C++
// .h:
static std::map<std::u32string, pFn> _fn;

// .cpp:
std::map<std::u32string, MyClass::pFn> MyClass::_fn;
```
3. Добавьте в класс метод, который будет выполнять задачу:
```C++
void MyClass::call_acos(Processor *p)
{
    auto args = loadArguments(p);
    if (args.empty())
        p->pushRealToStack(std::numeric_limits<double>::signaling_NaN());
    else
        p->pushRealToStack(acos(args.top().getRealValue()));
}
```
Вспомогательная функция `loadArguments` вынимает из стека все аргументы и разворачивает их в прямом порядке. 
Стек будет очищен, сколько бы агрументов ни было передано. 
Хотя вынимать агрументы можно вручную, вы можете использовать этот метод как более удобный.

4. Создайте функцию для заполнения хэш-таблицы:
```C++
/*static*/ void MyClass::buildFunctionsMap()
{
    if (!_fn.empty())
        return;
    _fn[U"acos"] = &MyClass::call_acos;
    // добавьте таким же образом все остальные методы
}
```
Эту функцию можно вызвать, например, в конструкторе объекта. При её первом вызове хзш-таблица будет заполнена.

5. Переопределите функцию `call` следующим образом:
```C++
bool MyClass::call(const std::u32string &method, Processor *p)
{
    if (BaseClass::call(method, p))
        return true;
    auto ptrToMethod = _fn.find(method);
    if (ptrToMethod == _fn.end())
        throw std::domain_error("Call of unknown method: MyClass." + to_utf8(method));
    (this->*ptrToMethod->second)(p);
    return true;
}
```
где `BaseClass` определяется, например, так:
```C++
using BaseClass = AutomationObject;
```

Если от класса предполагается наследоваться, то вместо выброса исключения лучше вернуть false.
Исключение же поможет увидеть, что вызыван неверный метод, это на усмотрение.

Теперь метод `acos` будет доступен в скрипте.

### Составные объекты автоматизации

Вы можете использовать более сложные конструкции. Например, пусть наша таблица 
содержит не просто текстовые ячейки, а экземпляры специализированного класса ячейки таблицы `MyCell`.
Чтобы работать с ячейками - 
получать на них ссылки или вызывать методы - класс ячейки тоже должен 
реализовать интерфейс `AutomationObject` (см. полный пример в 
[unittest/automation_test.cpp](https://github.com/edarichev/EScriptEngine/blob/master/tests/unittest/automation_test.cpp)):
```C++
class MyCell : public AutomationObject
{
    std::u32string _value;
public:
    const std::u32string &value() const { return _value; }
    void setValue(const u32string &newValue) { _value = newValue; }
    // остальное пропущено для краткости
}

class MySpreadSheet : public AutomationObject
{
    std::vector<std::vector<MyCell> > _cells;
    /* ... */
    void call_cell(Processor *p)
    { // вернуть указатель на ячейку
        auto args = loadArguments(p);
        assert(args.size() == 2);
        int rowIndex = args.top().getIntValue();    // индекс строки
        args.pop();
        int columnIndex = args.top().getIntValue(); // индекс столбца
        // указатель на ячейку мы вернём, и он будет доступен в скрипте
        MyCell *c = &_cells[rowIndex][columnIndex];
        p->pushObjectToStack(c);
    }
    /* ...остальное пропущено для краткости.. */
}
```

Тогда можно применять следующие выражения:
```javascript
c1 = spreadsheet.cell(1, 1);           // можно сначала получить ячейку
c1.text = "Hello";                     // а затем задать её текст
s = c1.text;                           // или получить текст
s = spreadsheet.cell(1, 1).text;       // а можно обратиться и сразу к результату 
spreadsheet.cell(1, 2).text = "World"; // вызова метода, без объявления переменной
```
Аналогично можно у `MyCell` сделать какой-нибудь класс стиля `MyStyle` и задавать туда цвет, шрифт и т.п.,
главное, чтобы класс наследовался от `AutomationObject`.

Хотя вот так делать не стоит:
```javascript
spreadsheet.cell(1, 3).text[1] = "a";
```
поскольку `text` - это отдельно живущий экземпляр того, что вернулось из метода `cell.get_text()`, а не внутреннее 
представление ячейки, которое вовсе и не текстовое, а в виде объекта `MyCell`,
само значение при этом не изменится:
```javascript
spreadsheet.cell(1, 3).text = "Hello";
p = spreadsheet.cell(1, 3).text[1] = "a"; // p == "Hallo", однако:
console.log(spreadsheet.cell(1, 3).text); // выведет "Hello"
```

### Вызов функции скрипта из кода на C++ по её имени

Если нужно вызывать конкретную функцию, определённую в скрипте, нужно знать её адрес.
Для этого необходимо добраться до таблицы символов, где находится запись об 
этой функции. Функции в глобальной области находятся в главной таблице символов,
до остальных можно добраться, используя дерево блоков, например: 
```C++
engine.unit()->block()->blocks()[0]->symbolTable();
```

При вызове функции нужно сохранить значение счётчика команд, куда записан
адрес возврата. Чтобы сразу после вызова прекратить выполнение, нужно передать
заведомо большое число, например, `(size_t)-1`, поскольку выполение будет прервано,
если значение PC больше верхней границы буфера памяти исполняющей машины.
Это не значит, что он будет перебирать память, пока не доберётся до её предела.
Это означает, что инструкция RET установит PC=MAX сразу же после выполнения функии и в момент возврата из неё.

Затем нужно поместить в стек аргументы функции в прямом порядке и последним записать число аргументов,
после чего запустить выполнение движка методом `EScript::run()`.

Делается это следующим образом:

```C++
const u32string code1 = UR"(
function fnMul(x, y) {
return (x + 1) * y;
}
)";
EScript engine;
engine.compile(code1); // или eval, если в скрипте есть какой-то инициализирующий код

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
m.cpu().setPC(fnMul->callAddress());   // в PC ставим адрес функции
StackValue x(SymbolType::Integer, 3);  // готовим аргументы
StackValue y(SymbolType::Integer, 5);
m.cpu().pushToStack(x);    // помещаем аргументы в прямом порядке
m.cpu().pushToStack(y);
m.cpu().pushToStack(2);    // последним - число аргументов, здесь 2 аргумента
engine.run();              // запускаем с текущего PC
StackValue v = m.cpu().popFromStack();  // в стеке всегда одно значение после вызова функции
assert(v.type == SymbolType::Variable); // результат помещается во временную переменную
int64_t result = v.getIntValue();       // извлекаем результат
assert(result == (3 + 1) * 5);
```

### Вызов функции как аргумента метода

Аналогичным образом можно делать объекты автоматизации, методы которых принимают функции в качестве аргументов.
В этом случае значение счётчика сохраняется, и после работы он должен быть восстановлен в прежнее значение.

Например, метод `Array.filter` принимает в качестве аргумента фильтрующую функцию.
Вызов функции скрипта из объекта, который работает внутри этого же скрипта,
может быть обработан так:
```C++
Function *fn = argFunction.getFunction(); // функция передана в качестве аргумента,
assert(fn != nullptr);         // мы его извлекли в переменную argFunction
size_t currentPC = p->PC();    // восстановим PC после перебора
for (auto &pval : _items) {    // допустим, надо применить эту функцию ко всем элементам 
                               // коллекции нашего объекта
    p->setPC((size_t)-1);      // ставим максимальный адрес для остановки выполнения
    p->pushPC();               // инструкция RET извлечёт PC при выходе из функции
    p->setPC(fn->callAddress()); // в PC ставим адрес функции
    StackValue x(pval.type, pval.value64()); // готовим аргумент
    p->pushToStack(x);         // помещаем аргументы в прямом порядке
    p->pushToStack(1);         // последним - число аргументов, здесь 1 аргумент
    p->machine()->run();       // запускаем с текущего PC, выполнене остановится
                               // как только PC получит значение (size_t)-1.
    StackValue v = p->popFromStack(); // после завершения run() в стеке мы всегда получим результат
    bool b = v.getBoolValue(); // предположим, наша функция возвращает логическое значение
    if (b) {
        // выполняем полезную работу
    }
}
p->setPC(currentPC);           // продолжить основную ветвь выполнения, восстановив PC
```
где `p` - переменная типа `Processor*`.

Так передаётся в качестве аргумента строка (показана часть кода), и строка же получается в качестве результата:

```C++
const u32string code1 = UR"(
function fnSubStr(s, from, to) {
    return s.substring(from, to);
}
)";
/* код пропущен: он аналогичен предыдущему примеру */
std::u32string myString = U"Hello, world!"; // готовим данные
int from = 7;
int to = 12;
m.cpu().pushToStack(myString);          // помещаем аргументы в прямом порядке
m.cpu().pushToStack(from);
m.cpu().pushToStack(to);
m.cpu().pushToStack(3);                 // последним - число аргументов, здесь 3 аргумента
engine.run();                           // запускаем с текущего PC
StackValue v = m.cpu().popFromStack();  // в стеке всегда одно значение после вызова функции
assert(v.type == SymbolType::Variable); // результат помещается во временную переменную
auto result = v.getStringValue();       // извлекаем результат
assert(result == U"world");
```

Массивы передаются и возвращаются подобным же образом. Например, функция скрипта принимает массив, заменяет одни значения на другие:

```C++
const u32string code1 = UR"(
function fnArrayReplace(arr, from, to) {
    for (i = 0; i < arr.length; i++) {
        if (arr[i] == from)
            arr[i] = to;
    }
    return arr;
}
)";
/* код пропущен: он аналогичен предыдущему примеру */
Array *arr = new Array();  // создадим новый массив, он будет зарегистрирован в таблице объектов
arr->addRef();             // поэтому его не нужно удалять вручную

arr->add(PValue(5));       // готовим данные: [5, 2, -1, 5]
arr->add(PValue(2));
arr->add(PValue(-1));
arr->add(PValue(5));
int from = 5;              // заменим все 5 на 7
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

```



