/**
 * @file token.h
 * @brief Определение токенов
 */
#ifndef TOKEN_H
#define TOKEN_H

namespace escript {

/**
 * @brief Токен/лексема
 */
enum class Token
{
    Eof = 0,            // Конец потока
    Break,              // break
    Case,               // case
    Catch,              // catch
    Class,              // class
    Const,              // const
    Continue,           // continue
    Debugger,           // debugger
    Default,            // default
    Delete,             // delete
    Do,                 // do
    Else,               // else
    Enum,               // enum
    Export,             // export
    Extends,            // extends
    Finally,            // finally
    For,                // for
    Function,           // function
    If,                 // if
    Implements,         // implements
    Import,             // import
    In,                 // in
    Instanceof,         // instanceof
    Interface,          // interface
    Let,                // let
    New,                // new
    Package,            // package
    Private,            // private
    Protected,          // protected
    Public,             // public
    Return,             // return
    Static,             // static
    Super,              // super
    Switch,             // switch
    This,               // this
    Throw,              // throw
    Try,                // try
    Typeof,             // typeof
    Var,                // var
    Void,               // void
    While,              // while
    With,               // with
    Yield,              // yield
    Dot,                // .
    LeftParenth,        // (
    LeftBrace,          // {
    LeftBracket,        // [
    RightParenth,       // )
    RightBrace,         // }
    RightBracket,       // ]
    Plus,               // +
    PlusPlus,           // ++
    PlusAssign,         // +=
    Minus,              // -
    MinusMinus,         // --
    MinusAssign,        // -=
    Caret,              // ^
    XorAssign,          // ^=
    Exclamation,        // !
    NotEqual,           // !=
    NotIdentity,        // !==
    Asterisk,           // *
    MulAssign,          // *=
    Slash,              // /
    SlashAssign,        // /=
    Percent,            // %
    PercentAssign,      // %=
    Less,               // <
    LessEqual,          // <=
    LShift,             // <<
    LShiftAssign,       // <<=
    Greater,            // >
    GreaterEqual,       // >=
    RShift,             // >>
    RShiftAssign,       // >>=
    RShiftZero,         // >>>
    RShiftZeroAssign,   // >>>=
    Assign,             // =
    Equal,              // ==
    Identity,           // ===
    Ampersand,          // &
    LogicalAnd,         // &&
    BitAndAssign,       // &=
    Vertical,           // |
    LogicalOr,          // ||
    BitOrAssign,        // |=
    BitNot,             // ~
    BitNotAssign,       // ~=
    Question,           // ?
    Colon,              // :
    Comma,              // ,
    Semicolon,          // ;
    RealNumber,         // действительное число
    IntegerNumber,      // целое число
    QuotedString,       // строка в кавычках
    BackQuotedString,   // строка в кавычках
    Identifier,         // идентификатор
};

/**
 * @brief Предназначен для преобразования токена в его строковое представление.
 */
class TokenName
{
private:
    static const char *_names[];
    static const Token _values[];
public:
    /**
     * @brief Преобразует токен в его строковое (читабельное) представление
     * @param token токен для преобразования
     * @return U"UNKNOWN", если не удалось найти
     */
    static std::string toString(Token token);
};

} // namespace escript

#endif // TOKEN_H
