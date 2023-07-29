/**
 * @file lexer.h
 * @brief Лексический анализатор - объявление
 */
#ifndef LEXER_H
#define LEXER_H

#include "EScript_global.h"
#include "token.h"

namespace escript {

/**
 * @brief Лексический анализатор
 */
class ESCRIPT_EXPORT Lexer
{
private:
    // разбираемый текст
    const std::u32string &_text;
    // начало строки
    std::u32string::const_iterator _begin;
    // конец строки
    std::u32string::const_iterator _end;
    // доступ к строке через итератор
    std::u32string::const_iterator _p;
    // последний разобранный токен
    Token _token = Token::Eof;
    // строковое представление последнего разобранного токена
    std::u32string _tokenText;
    // текущая позиция в строке (отсчёт с 1, для вывода сообщений об ошибках)
    int _pos = 1;
    // номер строки (отсчёт с 1, для вывода сообщений об ошибках)
    int _line = 1;
    // длина предыдущей строки (для подсчёта позиции при возврате символа)
    int _prevLineLen = 0;
public:
    /**
     * @brief Создаёт новый экземпляр класса Lexer
     */
    Lexer(const std::u32string &strCode);
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Lexer();
    /**
     * @brief Возвращает следующий токен из входного потока
     * @return Token::Eof в случе достижения конца потока
     */
    Token next();
    /**
     * @brief Возвращает последний разобранный токен
     * @return
     */
    Token currentToken() const;
    /**
     * @brief Возвращает строковое представление последнего разобранного токена
     * @return
     */
    const std::u32string &tokenText() const;
private:
    /**
     * @brief Считывает следующий символ из входного потока
     * @return код символа или -1, если достигнут конец потока
     */
    int read();
    /**
     * @brief Возвращается на 1 символ назад во входном потоке
     */
    void back();
    /**
     * @brief Проверяет, является ли данный символ пробелом
     * @param charCode код символа
     * @return true, если символ является одним из диапазона Unicode "whitespace"
     */
    bool isSpace(int charCode) const;
    /**
     * @brief Проверяет, является ли данный символ буквой
     * @param charCode код символа
     * @return true, если символ является одним из диапазона Unicode "letters"
     */
    bool isLetter(int charCode) const;
    /**
     * @brief Проверяет, является ли данный символ буквой или цифрой.
     * @param charCode код символа
     * @return true, если символ является одним из диапазона Unicode "letters"
     *         или цифрой 0...9.
     */
    bool isLetterOrDigit(int charCode) const;
    /**
     * @brief Считывает лексему-идентификатор из входного потока
     * @details Считанный идентификатор сохраняется в _tokenText,
     *        в _token заносится Token::Identifier.
     * @param charCode только считанный код символа
     */
    void readIdentifier(int charCode);
    /**
     * @brief Считывает число и возвращает тип считанного числа.
     * Входной указатель - на первом прочитанном символе числа,
     * его символ передаётся в параметре, т.к. уже прочитан.
     * Выходной указатель - на последнем символе числового литерала.
     * @param firstChar прочитанный только что символ 0..9 или точка
     * @return Token::RealNumber или IntegerNumber
     */
    void readNumber(int firstChar);
    /**
     * @brief Пропускает текст до конца строки (после //)
     */
    void skipToEndOfLine();
    /**
     * @brief пропускает многострочный комментарий, включая '*' и '/'
     */
    void skipMultilineComment();
    /**
     * @brief Считывает строку в кавычках
     * @param firstQuoteChar первый символ - это код кавычки.
     */
    void readQuotedString(int firstQuoteChar);

    // обработка ошибок
private:
    /**
     * @brief Выбрасывает исключение
     * @param msg сообщение
     */
    void error(const std::string &msg);
    /**
     * @brief Выводит предупреждение
     * @param msg сообщение
     */
    void warning(const std::string &msg);
    /**
     * @brief Выбрасывает исключение при недопустимом символе
     * @param unexpectedChar неправильный символ
     */
    void unexpected(char unexpectedChar);
    /**
     * @brief Неожиданный конец потока
     */
    void unexpectedEnd();
    /**
     * @brief Переводит второй символ управляющей последовательности в код
     *        управляющего символа. Предполагается, сто ведущий слэш прочитан.
     * @param charCode код экранированного символа, например, в "\t" (TAB) слэш
     *        предварительно считан, а входящий сюда символ - это "t".
     * @return Если это экранированный управляющий символ, то возвращается его
     *        код, иначе возвращается сам символ, это не будет считаться ошибкой.
     */
    int escapeChar(int charCode);
};

} // namespace escript

#endif // LEXER_H
