#include "stdafx.h"
#include "lexer_test.h"

using namespace std;
using namespace escript;

void Lexer_Test::run()
{
    initTestCase();
    test_allTokens();
    test_realNumbers();
    test_intNumbers();
    test_identifiers();
    test_quotedString();
    test_comments();
    cleanupTestCase();
}

void Lexer_Test::initTestCase()
{

}

void Lexer_Test::cleanupTestCase()
{

}

void Lexer_Test::test_allTokens()
{
    const u32string strOnlyTokens =
            U"break case catch class const continue debugger default \r\n"
            U" delete  do else enum export extends finally for function\r\n"
            U" if  implements import in instanceof interface let \n"
            U" new package private protected public return  static \n"
            U" super switch this throw  try typeof var void while with yield\n"
            U" . ( { [ ) } ] + ++ += - -- -= ^ ^= ! != !== * *= / /=\n"
            U" % %= < <= << <<= > >= >> >>= >>> >>>= = == === & && &=\n"
            U" | || |= ~  ~= ? : ,;";

    const Token tokens[] = {
        Token::Break,              // break
        Token::Case,               // case
        Token::Catch,              // catch
        Token::Class,              // class
        Token::Const,              // const
        Token::Continue,           // continue
        Token::Debugger,           // debugger
        Token::Default,            // default
        Token::Delete,             // delete
        Token::Do,                 // do
        Token::Else,               // else
        Token::Enum,               // enum
        Token::Export,             // export
        Token::Extends,            // extends
        Token::Finally,            // finally
        Token::For,                // for
        Token::Function,           // function
        Token::If,                 // if
        Token::Implements,         // implements
        Token::Import,             // import
        Token::In,                 // in
        Token::Instanceof,         // instanceof
        Token::Interface,          // interface
        Token::Let,                // let
        Token::New,                // new
        Token::Package,            // package
        Token::Private,            // private
        Token::Protected,          // protected
        Token::Public,             // public
        Token::Return,             // return
        Token::Static,             // static
        Token::Super,              // super
        Token::Switch,             // switch
        Token::This,               // this
        Token::Throw,              // throw
        Token::Try,                // try
        Token::Typeof,             // typeof
        Token::Var,                // var
        Token::Void,               // void
        Token::While,              // while
        Token::With,               // with
        Token::Yield,              // yield
        Token::Dot,                // .
        Token::LeftParenth,        // (
        Token::LeftBrace,          // {
        Token::LeftBracket,        // [
        Token::RightParenth,       // )
        Token::RightBrace,         // }
        Token::RightBracket,       // ]
        Token::Plus,               // +
        Token::PlusPlus,           // ++
        Token::PlusAssign,         // +=
        Token::Minus,              // -
        Token::MinusMinus,         // --
        Token::MinusAssign,        // -=
        Token::Caret,              // ^
        Token::XorAssign,          // ^=
        Token::Exclamation,        // !
        Token::NotEqual,           // !=
        Token::NotIdentity,        // !==
        Token::Asterisk,           // *
        Token::MulAssign,          // *=
        Token::Slash,              // /
        Token::SlashAssign,        // /=
        Token::Percent,            // %
        Token::PercentAssign,      // %=
        Token::Less,               // <
        Token::LessEqual,          // <=
        Token::LShift,             // <<
        Token::LShiftAssign,       // <<=
        Token::Greater,            // >
        Token::GreaterEqual,       // >=
        Token::RShift,             // >>
        Token::RShiftAssign,       // >>=
        Token::RShiftZero,         // >>>
        Token::RShiftZeroAssign,   // >>>=
        Token::Assign,             // =
        Token::Equal,              // ==
        Token::Identity,           // ===
        Token::Ampersand,          // &
        Token::LogicalAnd,         // &&
        Token::BitAndAssign,       // &=
        Token::Vertical,           // |
        Token::LogicalOr,          // ||
        Token::BitOrAssign,        // |=
        Token::BitNot,             // ~
        Token::BitNotAssign,       // ~=
        Token::Question,           // ?
        Token::Colon,              // :
        Token::Comma,              // ,
        Token::Semicolon,          // ;
    };

    Lexer lexer(strOnlyTokens);
    int n = sizeof (tokens) / sizeof(Token);
    int i = 0;
    while (lexer.next() != Token::Eof) {
        assert(i < n);
        assert(lexer.currentToken() == tokens[i]);
        i++;
    }
}

void Lexer_Test::test_realNumbers()
{
    u32string strRealNumbers = U"1234.5 6.78 .89E-11 0.12e+5 0.0034 345E+9";
    const int n = 6;
    const u32string numberTokens[n] = {
        U"1234.5", U"6.78", U".89E-11", U"0.12e+5", U"0.0034", U"345E+9"
    };
    Lexer lexerReal(strRealNumbers);
    int i = 0;
    while (lexerReal.next() != Token::Eof) {
        assert(i < n);
        assert(lexerReal.currentToken() == Token::RealNumber);
        assert(lexerReal.tokenText() == numberTokens[i]);
        i++;
    }
}

void Lexer_Test::test_intNumbers()
{
    u32string strIntNumbers = U"12345 678 8911 125 34 3454";
    const int n = 6;
    const u32string numberTokens[n] = {
        U"12345", U"678", U"8911", U"125", U"34", U"3454"
    };
    Lexer lexerReal(strIntNumbers);
    int i = 0;
    while (lexerReal.next() != Token::Eof) {
        assert(i < n);
        assert(lexerReal.currentToken() == Token::IntegerNumber);
        assert(lexerReal.tokenText() == numberTokens[i]);
        i++;
    }
}

void Lexer_Test::test_identifiers()
{
    u32string strIdentifiers = U"for id theClass in myArray E";
    const int n = 6;
    const Token tokens[n] = {
        Token::For, Token::Identifier, Token::Identifier,
        Token::In, Token::Identifier, Token::Identifier
    };
    Lexer lexerIds(strIdentifiers);
    int i = 0;
    while (lexerIds.next() != Token::Eof) {
        assert(i < n);
        assert(lexerIds.currentToken() == tokens[i]);
        i++;
    }
}

void Lexer_Test::test_quotedString()
{
    const int n = 6;
    u32string strCode[6] = {
        U"\"hello\"",
        U"\'hello\'",
        U"`hello`",
        U"\"single \'quote\' inside \\\"double\\\" \nquoted `string`\"",
        U"\'double \"quoted\" \\\"string\\\" inside the \\'single\\\' quoted \\`string\\`\'",
        U"`back \\`quoted\\` string with \'single\\\' and \"double\\\" quote`"
    };

    u32string strCodePattern[6] = {
        U"hello",
        U"hello",
        U"hello",
        U"single \'quote\' inside \"double\" \nquoted `string`",
        U"double \"quoted\" \"string\" inside the \'single\' quoted `string`",
        U"back `quoted` string with \'single\' and \"double\" quote",
    };
    Token strTypes[6] = {
        Token::QuotedString,
        Token::QuotedString,
        Token::BackQuotedString,
        Token::QuotedString,
        Token::QuotedString,
        Token::BackQuotedString,
    };
    // это генератор нормальных строк, используйте его для вывода в терминал
    // и создания массива strCodePattern[], который используется для проверки
    // возвращаемой лексемы
    // но их нужно чуть исправить, чтобы экранировать в C++ коде
    /*for (auto &s : strCode) {
        wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
        cout << converter.to_bytes(s) << "," << endl;
    }*/
    int i = 0;
    for (; i < n; i++) {
        Lexer lexer(strCode[i]);
        Token t = lexer.next();
        assert(t == strTypes[i]);
        // это оставим для отладочного вывода
        // wstring_convert<codecvt_utf8<char32_t>, char32_t> converter;
        // cout << converter.to_bytes(lexer.tokenText()) << endl;
        assert(lexer.tokenText() == strCodePattern[i]);
    }
}

void Lexer_Test::test_comments()
{
    const u32string comment = U"for /*multiline\n\ncomment\n*/ var i; //\n\ntest";
    const int n = 5;
    Token tokens[n] = {
        Token::For, Token::Var, Token::Identifier,
        Token::Semicolon, Token::Identifier
    };
    Lexer lexer(comment);
    int i = 0;
    while (lexer.next() != Token::Eof) {
        assert(i < n);
        assert(lexer.currentToken() == tokens[i]);
        i++;
    }
}

