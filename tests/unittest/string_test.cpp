#include "stdafx.h"
#include "string_test.h"

void String_Test::run()
{
    initTestCase();
    test_literal();
    test_callMethodOfStringLiteral();
    test_callMethodOfBQStringLiteral();
    test_plus();
    test_length();
    test_at();
    test_charCodeAt();
    test_charAt();
    test_endsWith();
    test_includes();
    test_indexOf();
    test_lastIndexOf();
    test_padEnd();
    test_padStart();
    test_repeat();
    test_replace();
    test_replaceAll();
    test_slice();
    test_split();
    test_startsWith();
    test_substring();
    test_toLowerCase();
    test_toUpperCase();
    test_trim();
    test_trimEnd();
    test_trimStart();
    test_comparisonOperators();
    test_accessItemLiteralGet();
    test_accessItemVariableGet();
    test_accessItemVariableSet();
    cleanupTestCase();
}

void String_Test::initTestCase()
{

}

void String_Test::cleanupTestCase()
{

}

void String_Test::test_literal()
{
    const u32string code1 = U"s = \"hello\";";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"hello");
}

void String_Test::test_plus()
{
    const u32string code1 = U"s = \"hello\" + ', world';";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"hello, world");
}

void String_Test::test_length()
{
    const u32string code1 = U"s = \"hello\"; i = s.length;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(5, record->data));
}

void String_Test::test_at()
{
    const u32string code1 = U"s = \"hello\"; i = s.at(1);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64('e', record->data));
}

void String_Test::test_charCodeAt()
{
    const u32string code1 = U"s = \"hello\"; i = s.charCodeAt(1);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64('e', record->data));
}

void String_Test::test_charAt()
{
    const u32string code1 = U"s = \"hello\"; i = s.charAt(3);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"l");
}

void String_Test::test_endsWith()
{
    const u32string code1 = U"x = `hello, 🌏`.endsWith('🌏');";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    const u32string code2 = U"x = `hello, 🌏`.endsWith('str');";
    engine.eval(code2);
    x = mainTable->find(U"x");
    record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));
}

void String_Test::test_includes()
{
    const u32string code1 = U"x = `hello, 🌏`.includes('ello');";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    const u32string code2 = U"x = `hello, 🌏`.includes('Welt');";
    engine.eval(code2);
    x = mainTable->find(U"x");
    record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));
}

void String_Test::test_indexOf()
{
    const u32string code1 = U"x = `hello, 🌏`.indexOf('llo');";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(2, record->data));

    const u32string code2 = U"x = `hello, 🌏`.indexOf('Welt');";
    engine.eval(code2);
    x = mainTable->find(U"x");
    record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(-1, record->data));
}

void String_Test::test_lastIndexOf()
{
    const u32string code1 = U"x = `hello, 🌏, well`.lastIndexOf('ell');";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(11, record->data));

    const u32string code2 = U"x = `hello, 🌏, well`.lastIndexOf('Welt');";
    engine.eval(code2);
    x = mainTable->find(U"x");
    record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(-1, record->data));
}

void String_Test::test_padEnd()
{
    const u32string code1 = U"str1 = 'Breaded Mushrooms'; str1 = str1.padEnd(25, '.');";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto str1 = mainTable->find(U"str1");
    auto record = engine.getObjectRecord(str1);
    assert(record->type == SymbolType::String);
    auto strValue = (StringObject*)record->data;
    assert(*strValue == U"Breaded Mushrooms........");

    const u32string code2 = U"str2 = '200'; str2 = str2.padEnd(5);";
    engine.eval(code2);
    auto str2 = mainTable->find(U"str2");
    record = engine.getObjectRecord(str2);
    assert(record->type == SymbolType::String);
    strValue = (StringObject*)record->data;
    assert(*strValue == U"200  ");

    const u32string code3 = U"str3 = '200'; str3 = str3.padEnd(2);";
    engine.eval(code3);
    auto str3 = mainTable->find(U"str3");
    record = engine.getObjectRecord(str3);
    assert(record->type == SymbolType::String);
    strValue = (StringObject*)record->data;
    assert(*strValue == U"200");

    const u32string code4 = U"str4 = 'xyz'; str4 = str4.padEnd(20, '1234567');";
    engine.eval(code4);
    auto str4 = mainTable->find(U"str4");
    record = engine.getObjectRecord(str4);
    assert(record->type == SymbolType::String);
    strValue = (StringObject*)record->data;
    assert(*strValue == U"xyz12345671234567123");
}

void String_Test::test_padStart()
{
    const u32string code1 = U"str1 = '1234'; str1 = str1.padStart(8, '*');";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto str1 = mainTable->find(U"str1");
    auto record = engine.getObjectRecord(str1);
    assert(record->type == SymbolType::String);
    auto strValue = (StringObject*)record->data;
    assert(*strValue == U"****1234");

    const u32string code2 = U"str2 = '1234'; str2 = str2.padStart(4, '*');";
    engine.eval(code2);
    auto str2 = mainTable->find(U"str2");
    record = engine.getObjectRecord(str2);
    assert(record->type == SymbolType::String);
    strValue = (StringObject*)record->data;
    assert(*strValue == U"1234");
}

void String_Test::test_repeat()
{
    const u32string code1 = U"str1 = 'Hello! '; str1 = str1.repeat(3);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto str1 = mainTable->find(U"str1");
    auto record = engine.getObjectRecord(str1);
    assert(record->type == SymbolType::String);
    auto strValue = (StringObject*)record->data;
    assert(*strValue == U"Hello! Hello! Hello! ");
}

void String_Test::test_replace()
{
    const u32string code1 = U"str1 = 'Hello! '; str1 = str1.replace('He', 'Ha');";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto str1 = mainTable->find(U"str1");
    auto record = engine.getObjectRecord(str1);
    assert(record->type == SymbolType::String);
    auto strValue = (StringObject*)record->data;
    assert(*strValue == U"Hallo! ");

    const u32string code2 = U"str2 = 'Hello! '; str2 = str2.replace('xxx', 'Ha');";
    engine.eval(code2);
    auto str2 = mainTable->find(U"str2");
    record = engine.getObjectRecord(str2);
    assert(record->type == SymbolType::String);
    strValue = (StringObject*)record->data;
    assert(*strValue == U"Hello! ");

    const u32string code3 = U"str3 = 'Hello! Hello!'; str3 = str3.replace('Hell', 'Hall');";
    engine.eval(code3);
    auto str3 = mainTable->find(U"str3");
    record = engine.getObjectRecord(str3);
    assert(record->type == SymbolType::String);
    strValue = (StringObject*)record->data;
    assert(*strValue == U"Hallo! Hello!"); // 1 раз заменить
}

void String_Test::test_replaceAll()
{
    const u32string code1 = U"str1 = 'Hello! Hello!'; str1 = str1.replaceAll('He', 'Ha');";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto str1 = mainTable->find(U"str1");
    auto record = engine.getObjectRecord(str1);
    assert(record->type == SymbolType::String);
    auto strValue = (StringObject*)record->data;
    assert(*strValue == U"Hallo! Hallo!");
}

void String_Test::test_slice()
{
    const u32string codeDecl = U"str = 'The quick brown fox jumps over the lazy dog.';";
    EScript engine;
    engine.eval(codeDecl);
    auto mainTable = engine.unit()->block()->symbolTable();

    const u32string code1 = U"str1 = str.slice(31);";
    engine.eval(code1);
    auto str1 = mainTable->find(U"str1");
    auto record = engine.getObjectRecord(str1);
    assert(record->type == SymbolType::String);
    auto strValue = (StringObject*)record->data;
    assert(*strValue == U"the lazy dog.");

    const u32string code2 = U"str2 = str.slice(4, 19);";
    engine.eval(code2);
    auto str2 = mainTable->find(U"str2");
    record = engine.getObjectRecord(str2);
    assert(record->type == SymbolType::String);
    strValue = (StringObject*)record->data;
    assert(*strValue == U"quick brown fox");

    const u32string code3 = U"str3 = str.slice(-4);";
    engine.eval(code3);
    auto str3 = mainTable->find(U"str3");
    record = engine.getObjectRecord(str3);
    assert(record->type == SymbolType::String);
    strValue = (StringObject*)record->data;
    assert(*strValue == U"dog.");

    const u32string code4 = U"str4 = str.slice(-9, -5);";
    engine.eval(code4);
    auto str4 = mainTable->find(U"str4");
    record = engine.getObjectRecord(str4);
    assert(record->type == SymbolType::String);
    strValue = (StringObject*)record->data;
    assert(*strValue == U"lazy");

    const u32string code5 = U"str5 = str.slice(-9, 41);";
    engine.eval(code5);
    auto str5 = mainTable->find(U"str5");
    record = engine.getObjectRecord(str5);
    assert(record->type == SymbolType::String);
    strValue = (StringObject*)record->data;
    assert(*strValue == U"lazy d");
}

void String_Test::test_split()
{
    const std::string macro1 = R"(
str = 'The quick brown fox jumps over the lazy dog.';

words = str.split(' ');
wordsCount = words.length; // 9
words3 = words[3]; // "fox"

chars = str.split('');
charsCount = chars.length; // 44
chars8 = chars[8]; // "k"

strCopy = str.split();
strCopyCount = strCopy.length; // 1
strCopy0 = strCopy[0]; // "The quick brown fox jumps over the lazy dog."
)";
    const u32string code = to_u32string(macro1);
    EScript engine;
    engine.eval(code);
    auto mainTable = engine.unit()->block()->symbolTable();
    // words
    auto words = mainTable->find(U"words");
    auto wordsRecord = engine.getObjectRecord(words);
    assert(wordsRecord->type == SymbolType::Array);

    auto wordsCount = mainTable->find(U"wordsCount");
    auto wordsCountRecord = engine.getObjectRecord(wordsCount);
    assert(wordsCountRecord->type == SymbolType::Integer);
    assert(Compare::equals_int64(9, wordsCountRecord->data));

    auto words3 = mainTable->find(U"words3");
    auto words3Record = engine.getObjectRecord(words3);
    assert(words3Record->type == SymbolType::String);
    auto words3Value = (StringObject*)words3Record->data;
    assert(*words3Value == U"fox");
    // chars
    auto chars = mainTable->find(U"chars");
    auto charsRecord = engine.getObjectRecord(chars);
    assert(charsRecord->type == SymbolType::Array);

    auto charsCount = mainTable->find(U"charsCount");
    auto charsCountRecord = engine.getObjectRecord(charsCount);
    assert(charsCountRecord->type == SymbolType::Integer);
    assert(Compare::equals_int64(44, charsCountRecord->data));

    auto chars8 = mainTable->find(U"chars8");
    auto chars8Record = engine.getObjectRecord(chars8);
    assert(chars8Record->type == SymbolType::String);
    auto chars8Value = (StringObject*)chars8Record->data;
    assert(*chars8Value == U"k");
    // strCopy
    auto strCopy = mainTable->find(U"strCopy");
    auto strCopyRecord = engine.getObjectRecord(strCopy);
    assert(strCopyRecord->type == SymbolType::Array);

    auto strCopyCount = mainTable->find(U"strCopyCount");
    auto strCopyCountRecord = engine.getObjectRecord(strCopyCount);
    assert(strCopyCountRecord->type == SymbolType::Integer);
    assert(Compare::equals_int64(1, strCopyCountRecord->data));

    auto strCopy0 = mainTable->find(U"strCopy0");
    auto strCopy0Record = engine.getObjectRecord(strCopy0);
    assert(strCopy0Record->type == SymbolType::String);
    auto strCopy0Value = (StringObject*)strCopy0Record->data;
    assert(*strCopy0Value == U"The quick brown fox jumps over the lazy dog.");
}

void String_Test::test_startsWith()
{
    const std::string macro1 = R"(
str1 = 'Saturday night plans';
a = str1.startsWith('Sat'); // Expected output: true
b = str1.startsWith('Sat', 3); // Expected output: false
)";
    const u32string code1 = to_u32string(macro1);
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));
}

void String_Test::test_substring()
{
    const u32string code1 = U"s = \"hello\"; i = s.substring(1, 3);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"el");
}

void String_Test::test_toLowerCase()
{
    const u32string code1 = U"s = \"HELLO\"; i = s.toLowerCase();";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"hello");
}

void String_Test::test_toUpperCase()
{
    const u32string code1 = U"s = \"Hello\"; i = s.toUpperCase();";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"HELLO");
}

void String_Test::test_trim()
{
    const u32string code1 = U"s = \"  \t\r\n  Hello   \t\r\n  \"; i = s.trim();";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"Hello");

    const u32string code2 = U"s = `Hello`; i = s.trim();";
    engine.eval(code2);
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::String);
    str = (StringObject*)record->data;
    assert(*str == U"Hello");

    const u32string code3 = U"s = ``; i = s.trim();";
    engine.eval(code3);
    i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::String);
    str = (StringObject*)record->data;
    assert(*str == U"");
}

void String_Test::test_trimEnd()
{
    const u32string code1 = U"s = \"  \t\r\n  Hello\"; i = s.trimEnd();";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"  \t\r\n  Hello");
}

void String_Test::test_trimStart()
{
    const u32string code1 = U"s = \"  \t\r\n  Hello   \r\n\t \"; i = s.trimStart();";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto i = mainTable->find(U"i");
    auto record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"Hello   \r\n\t ");
}

void String_Test::test_callMethodOfStringLiteral()
{
    const u32string code1 = U"s = 'hello, 🌏'.substring(1, 3);";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto s = mainTable->find(U"s");
    auto record = engine.getObjectRecord(s);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"el");
}

void String_Test::test_callMethodOfBQStringLiteral()
{
    const u32string code1 = U"x = `hello, 🌏`.length;";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::Integer);
    assert(Compare::equals_int64(8, record->data));
}

void String_Test::test_comparisonOperators()
{
    const char32_t *code1 = UR"(
a = "Hello" < "hello";
b = "hello" < "HELLO";
c = 'hello' <= 'hello';
d = 'hello' == 'hello';
e = 'Tastatur' >= 'Maus';
f = 'Lautsprecher' >= 'Apfel';
g = 'Apfel' <= 'Lautsprecher';
h = 'Apfel' >= 'Lautsprecher';
i = 'Apfel' != 'Lautsprecher';
j = 'Apfel' != 'Apfel';
)";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto a = mainTable->find(U"a");
    auto record = engine.getObjectRecord(a);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto b = mainTable->find(U"b");
    record = engine.getObjectRecord(b);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));

    auto c = mainTable->find(U"c");
    record = engine.getObjectRecord(c);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto d = mainTable->find(U"d");
    record = engine.getObjectRecord(d);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto e = mainTable->find(U"e");
    record = engine.getObjectRecord(e);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto f = mainTable->find(U"f");
    record = engine.getObjectRecord(f);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto g = mainTable->find(U"g");
    record = engine.getObjectRecord(g);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto h = mainTable->find(U"h");
    record = engine.getObjectRecord(h);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));

    auto i = mainTable->find(U"i");
    record = engine.getObjectRecord(i);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(true, record->data));

    auto j = mainTable->find(U"j");
    record = engine.getObjectRecord(j);
    assert(record->type == SymbolType::Boolean);
    assert(Compare::equals_bool(false, record->data));
}

void String_Test::test_accessItemLiteralGet()
{
    const u32string code1 = U"x = 'hello'[1];";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"e");
}

void String_Test::test_accessItemVariableGet()
{
    const u32string code1 = U"s = 'hello'; x = s[1];";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"e");
}

void String_Test::test_accessItemVariableSet()
{
    const u32string code1 = U"s = 'hello'; s[1] = 'a'; x = s[1];";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"a");
}

void String_Test::test_accessItemParenthExpressionGet()
{
    const u32string code1 = U"x = ('hello')[1];";
    EScript engine;
    engine.eval(code1);
    auto mainTable = engine.unit()->block()->symbolTable();
    auto x = mainTable->find(U"x");
    auto record = engine.getObjectRecord(x);
    assert(record->type == SymbolType::String);
    auto str = (StringObject*)record->data;
    assert(*str == U"e");
}
