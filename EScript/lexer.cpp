/**
 * @file lexer.cpp
 * @brief Лексический анализатор - реализация
 */
#include "stdafx.h"
#include "lexer.h"
#include "keyword.h"

namespace escript {

Lexer::Lexer(const std::u32string &strCode)
    : _text(strCode),
      _begin(strCode.begin()),
      _end(strCode.end()),
      _p(strCode.begin())
{

}

Lexer::~Lexer()
{

}

// поместить символ в буфер токена
#define PUSH(c) _tokenText.push_back((c))
// выражение возврата
#define TOKEN(t) _token = (t)

Token Lexer::next()
{
    _tokenText.clear();
    int c = 0;
    while ((c = read()) >= 0) {
        if (isSpace(c))
            continue;
        if (isLetter(c) || c == '_') {
            readIdentifier(c);
            return currentToken();
        }
        if (c == '.' || isdigit(c)) {
            readNumber(c);
            switch (currentToken()) {
            case Token::RealNumber:
            case Token::IntegerNumber:
                return currentToken();
            default:
                // что-то тут не так, может быть это точка
                _tokenText.clear();
                break;
            }

        }
        switch (c) {
        case '\'':
        case '\"':
        case '`':
            readQuotedString(c);
            return currentToken();
        }
        PUSH(c);
        switch (c) {
        case ';':
            return TOKEN(Token::Semicolon);
        case '.':
            return TOKEN(Token::Dot);
        case ',':
            return TOKEN(Token::Comma);
        case ':':
            return TOKEN(Token::Colon);
        case '?':
            return TOKEN(Token::Question);
        case '(':
            return TOKEN(Token::LeftParenth);
        case '{':
            return TOKEN(Token::LeftBrace);
        case '[':
            return TOKEN(Token::LeftBracket);
        case ')':
            return TOKEN(Token::RightParenth);
        case '}':
            return TOKEN(Token::RightBrace);
        case ']':
            return TOKEN(Token::RightBracket);
        case '=':
            if ((c = read()) < 0)
                return TOKEN(Token::Assign);
            if (c == '=') { // ==
                PUSH(c);
                if ((c = read()) < 0)
                    return TOKEN(Token::Equal);
                if (c == '=') { // ===
                    PUSH(c);
                    return TOKEN(Token::Identity);
                }
                back(); // ==  <---
                return TOKEN(Token::Equal);
            }
            back(); // = <---
            return TOKEN(Token::Assign);
        case '!':
            if ((c = read()) < 0)
                return TOKEN(Token::Exclamation);
            if (c == '=') { // !=
                PUSH(c);
                if ((c = read()) < 0)
                    return TOKEN(Token::NotEqual);
                if (c == '=') { // !==
                    PUSH(c);
                    return TOKEN(Token::NotIdentity);
                }
                back(); // !=  <---
                return TOKEN(Token::NotEqual);
            }
            back(); // ! <---
            return TOKEN(Token::Exclamation);
        case '+':
            if ((c = read()) < 0)
                return TOKEN(Token::Plus);
            if (c == '+') { // ++
                PUSH(c);
                return TOKEN(Token::PlusPlus);
            } else if (c == '=') { // +=
                PUSH(c);
                return TOKEN(Token::PlusAssign);
            }
            back(); // вернуть 1 символ
            return TOKEN(Token::Plus);
        case '-':
            if ((c = read()) < 0)
                return TOKEN(Token::Minus);
            if (c == '-') { // --
                PUSH(c);
                return TOKEN(Token::MinusMinus);
            } else if (c == '=') { // -=
                PUSH(c);
                return TOKEN(Token::MinusAssign);
            }
            back(); // - <---
            return TOKEN(Token::Minus);
        case '^':
            if ((c = read()) < 0)
                return TOKEN(Token::Caret);
            if (c == '=') { // ^=
                PUSH(c);
                return TOKEN(Token::XorAssign);
            }
            back();
            return _token = Token::Caret;
        case '~':
            if ((c = read()) < 0)
                return TOKEN(Token::BitNot);
            if (c == '=') { // ~=
                PUSH(c);
                return TOKEN(Token::BitNotAssign);
            }
            back();
            return _token = Token::BitNot;
        case '*':
            if ((c = read()) < 0)
                return TOKEN(Token::Asterisk);
            if (c == '=') { // *=
                PUSH(c);
                return TOKEN(Token::MulAssign);
            }
            back(); // * <---
            return _token = Token::Asterisk;
        case '/':
            if ((c = read()) < 0)
                return TOKEN(Token::Slash);
            if (c == '/') { // однострочный комментарий
                skipToEndOfLine();
                continue;
            } else if (c == '*') { // многострочный комментарий
                skipMultilineComment();
                continue;
            }
            else if (c == '=') { // /=
                PUSH(c);
                return TOKEN(Token::SlashAssign);
            }
            back(); // / <---
            return _token = Token::Slash;
        case '%':
            if ((c = read()) < 0)
                return TOKEN(Token::Percent);
            if (c == '=') { // %=
                PUSH(c);
                return TOKEN(Token::PercentAssign);
            }
            back(); // % <---
            return TOKEN(Token::Percent);
        case '<':
            if ((c = read()) < 0)
                return TOKEN(Token::Less);
            if (c == '=') { // <=
                PUSH(c);
                return TOKEN(Token::LessEqual);
            } else if (c == '<') { // <<
                PUSH(c);
                if ((c = read()) < 0)
                    return TOKEN(Token::LShift);
                if (c == '=') { // <<=
                    PUSH(c);
                    return TOKEN(Token::LShiftAssign);
                }
                back(); // << <---
                return TOKEN(Token::LShift);
            }
            back(); // '<' <---
            return TOKEN(Token::Less);
        case '>':
            if ((c = read()) < 0)
                return TOKEN(Token::Greater);
            if (c == '=') { // >=
                PUSH(c);
                return TOKEN(Token::GreaterEqual);
            } else if (c == '>') { // >>
                PUSH(c);
                if ((c = read()) < 0)
                    return TOKEN(Token::RShift);
                if (c == '>') { // >>>
                    PUSH(c);
                    if ((c = read()) < 0)
                        return TOKEN(Token::RShiftZero);
                    if (c == '=') { // >>>=
                        PUSH(c);
                        return TOKEN(Token::RShiftZeroAssign);
                    }
                    back(); // >>>   <---
                    return TOKEN(Token::RShiftZero);
                } else if (c == '=') { // >>=
                    PUSH(c);
                    return TOKEN(Token::RShiftAssign);
                }
                back(); // >>   <---
                return TOKEN(Token::RShift);
            }
            back(); // >   <---
            return TOKEN(Token::Greater);
        case '&':
            if ((c = read()) < 0)
                return TOKEN(Token::Ampersand);
            if (c == '&') { // &&
                PUSH(c);
                return TOKEN(Token::LogicalAnd);
            } else if (c == '=') { // &=
                PUSH(c);
                return TOKEN(Token::BitAndAssign);
            }
            back(); // &   <---
            return TOKEN(Token::Ampersand);
        case '|':
            if ((c = read()) < 0)
                return TOKEN(Token::Vertical);
            if (c == '|') { // ||
                PUSH(c);
                return TOKEN(Token::LogicalOr);
            } else if (c == '=') { // |=
                PUSH(c);
                return TOKEN(Token::BitOrAssign);
            }
            back(); // &   <---
            return TOKEN(Token::Vertical);
        default:
            unexpected(c);
        }
    }
    return Token::Eof;
}

Token Lexer::currentToken() const
{
    return _token;
}

const std::u32string &Lexer::tokenText() const
{
    return _tokenText;
}

int Lexer::read()
{
    if (_p == _end)
        return -1;
    int c = *_p;
    ++_p;

    if (c == '\n') {
        _prevLineLen = _pos;
        _line++;
        _pos = 1;
    } else {
        if (c != '\r') // не считаем сдвиг на '\r', иначе визуально это будет на
            _pos++;    // 1 символ в строке больше, чем отображается в редакторе
    }
    return c;
}

void Lexer::back()
{
    if (_p == _text.begin())
        throw std::out_of_range("jump over the beginning of the buffer");
    --_p;
    auto c = *_p;
    if (c == '\n') {
        _pos = _prevLineLen;
        _line--;
    } else {
        if (_pos > 0)
            _pos--;
    }
}

bool Lexer::isSpace(int charCode) const
{
    switch (charCode) {
    case 0x0020: // пробел, наиболее вероятно
    case 0x0009: // TAB
    case '\r':
    case '\n':
    case 0x00A0:
    case 0x1680:
    case 0x180E:
    case 0x2000:
    case 0x2001:
    case 0x2002:
    case 0x2003:
    case 0x2004:
    case 0x2005:
    case 0x2006:
    case 0x2007:
    case 0x2008:
    case 0x2009:
    case 0x200A:
    case 0x200B:
    case 0x202F:
    case 0x205F:
    case 0x3000:
    case 0xFEFF:
        return true;
    default:
        break;
    }
    return false;
}

bool Lexer::isLetter(int charCode) const
{
    return std::isalpha(charCode);
}

bool Lexer::isLetterOrDigit(int charCode) const
{
    return isLetter(charCode) || std::isdigit(charCode);
}

void Lexer::readIdentifier(int charCode)
{
    _tokenText.clear();
    while (charCode > 0) {
        _tokenText.push_back(charCode);
        charCode = read();
        if (isLetterOrDigit(charCode) || charCode == '_')
            continue;
        if (charCode > 0)
            back(); // лишний
        break;
    }
    Token kw = KeyWord::checkKeyWord(_tokenText);
    if (kw == Token::Eof) { // ключевое слово не найдено, это идентификатор
        _token = Token::Identifier;
    } else { // это ключевое слово
        _token = kw;
    }
}

void Lexer::readNumber(int firstChar)
{
    _tokenText.clear();
    bool isReal = firstChar == '.'; // ставили ли уже точку?
    bool hasExpSign = false;        // есть + или -, иначе ошибка
    bool hasExpE = false;           // знак E
    bool hasExpNum = false;         // есть ли число после E
    _tokenText.push_back(firstChar);
    int c;
    while ((c = read()) > 0) {
        if (std::isdigit(c)) {
            if (hasExpE)
                hasExpNum = true;
            PUSH(c);
            continue;
        } else if (c == '.') {
            if (_tokenText.length()) {
                if (_tokenText.find_first_of('.') == _tokenText.length() - 1) {
                    // в числе есть последняя точка, но идут две точки подряд -
                    // надо вернуть число, а две точки запихнуть обратно
                    back();
                    back();
                    _tokenText.erase(_tokenText.length() - 1, 1);
                    isReal = hasExpE; // иначе это целое, т.к. точки нет
                    break;
                } else {
                    // эта точка - после разобранного числа
                    if (hasExpE && hasExpNum) { // число закончилось
                        // это какая-то другая точка,
                        // разберём на следующем проходе
                        back();
                        break;
                    } // ниже вылетим в unexpected
                }
            }
            if (isReal) // точка уже есть, неправильное число
                unexpected(c);
            isReal = true;
        } else if (c == '+' || c == '-') {
            if (hasExpSign)
                unexpected(c);
            hasExpSign = true;
        } else if (c == 'E' || c == 'e') {
            if (hasExpE)
                unexpected(c);
            hasExpE = true;
            isReal = true;
        } else {
            back(); // символ не допускается в число
            break;
        }
        PUSH(c);
    }
    if (_tokenText == U".") {
        _token = Token::Dot;
        return;
    }
    if (isReal)
        _token = Token::RealNumber;
    else
        _token = Token::IntegerNumber;
}

void Lexer::error(const std::string &msg)
{
    std::cout << "line: " << _line << ", pos:" << _pos << " " << msg << "\n";
    exit(1);
}

void Lexer::warning(const std::string &msg)
{
    std::cout << "WARNING: line: " << _line << ", pos:" << _pos
              << " " << msg << "\n";
}

void Lexer::unexpected(char unexpectedChar)
{
    error(std::string("Unexpected character: [") + unexpectedChar + "]");
}

void Lexer::unexpectedEnd()
{
    error("Unexpected end of code stream");
}

int Lexer::escapeChar(int charCode)
{
    switch (charCode) {
    case 'r': return '\r';
    case 'n': return '\n';
    case 't': return '\t';
    case 'v': return '\v';
    case 'f': return '\f';
    case 'b': return '\b';
    case '\'': return '\'';
    case '\"': return '\"';
    case '\\': return '\\';
    case '`': return '`';
    default:
        warning("Unknown escape sequence");
        break;
    }

    return charCode;
}

void Lexer::skipToEndOfLine()
{
    _tokenText.clear();
    int c;
    while ((c = read()) > 0) {
        // не возвращаем его в поток,
        // анализ продолжится со следующего первого символа строки
        if (c == '\n')
            break;
    }
}

void Lexer::skipMultilineComment()
{
    _tokenText.clear();
    int c;
    while ((c = read()) >= 0) {
        if (c == '*') {
            c = read();
            if (c == '/' || c < 0)
                break;
        }
    }
}

void Lexer::readQuotedString(int firstQuoteChar)
{
    _tokenText.clear();
    assert(firstQuoteChar == '\'' ||
           firstQuoteChar == '\"' ||
           firstQuoteChar == '`');
    switch (firstQuoteChar) {
    case '\"':
    case '\'':
        _token = Token::QuotedString;
        break;
    case '`':
        _token = Token::BackQuotedString;
        break;
    }
    int c = 0;
    while ((c = read()) > 0) {
        if (c == '\\') {
            if ((c = read()) < 0)
                unexpectedEnd();
            PUSH(escapeChar(c));
            continue;
        }
        if (c == firstQuoteChar) {
            break; // строка закрыта
        }
        PUSH(c);
    }
}

} // namespace escript
