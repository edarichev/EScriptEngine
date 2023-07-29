/**
 * @file keyword.cpp
 * @brief Реализация класса KeyWord
 */
#include "stdafx.h"
#include "keyword.h"

namespace escript {

/**
 * Ключевые слова
 */
static const constexpr char32_t *keyWords[] = {
    U"break",
    U"case",
    U"catch",
    U"class",
    U"const",
    U"continue",
    U"debugger",
    U"default",
    U"delete",
    U"do",
    U"else",
    U"enum",
    U"export",
    U"extends",
    U"finally",
    U"for",
    U"function",
    U"if",
    U"implements",
    U"import",
    U"in",
    U"instanceof",
    U"interface",
    U"let",
    U"new",
    U"package",
    U"private",
    U"protected",
    U"public",
    U"return",
    U"static",
    U"super",
    U"switch",
    U"this",
    U"throw",
    U"try",
    U"typeof",
    U"var",
    U"void",
    U"while",
    U"with",
    U"yield",
};

/**
 * Токены, соответствующие ключевым словам
 */
static const constexpr Token tokenKeywords[] {
    Token::Break,
    Token::Case,
    Token::Catch,
    Token::Class,
    Token::Const,
    Token::Continue,
    Token::Debugger,
    Token::Default,
    Token::Delete,
    Token::Do,
    Token::Else,
    Token::Enum,
    Token::Export,
    Token::Extends,
    Token::Finally,
    Token::For,
    Token::Function,
    Token::If,
    Token::Implements,
    Token::Import,
    Token::In,
    Token::Instanceof,
    Token::Interface,
    Token::Let,
    Token::New,
    Token::Package,
    Token::Private,
    Token::Protected,
    Token::Public,
    Token::Return,
    Token::Static,
    Token::Super,
    Token::Switch,
    Token::This,
    Token::Throw,
    Token::Try,
    Token::Typeof,
    Token::Var,
    Token::Void,
    Token::While,
    Token::With,
    Token::Yield,
};

Token KeyWord::checkKeyWord(const std::u32string &str)
{
    static std::map<std::u32string, Token> keyWordsHash;
    if (keyWordsHash.empty()) {
        int i = 0;
        for (auto &t : tokenKeywords) {
            // мы гарантируем, что размер обоих массивов одинаковый
            keyWordsHash[keyWords[i++]] = t;
        }
    }
    auto it = keyWordsHash.find(str);
    if (it == keyWordsHash.end())
        return Token::Eof;
    return it->second;
}

} // namespace escript
