/**
 * @file parser.h
 * @brief Синтаксический анализатор - объявление
 *
 * Eugen-Script
 * The ECMAScript-like engine for C++ programs.
 * Copyright (C) 2023, Evgeny Darichev
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
#ifndef PARSER_H
#define PARSER_H

#include "EScript_global.h"
#include "lexer.h"
#include "icodeemitter.h"
#include <stack>
#include <deque>
#include "tcode.h"
#include "pvalue.h"

namespace escript {

class Unit;
class SymbolTable;
class Symbol;
class Block;
class StringContainer;

/**
 * @brief Cинтаксический анализатор
 */
class ESCRIPT_EXPORT Parser
{
private:
    std::unique_ptr<Lexer> _lexer;
    std::shared_ptr<Block> _rootBlock;
    std::shared_ptr<Block> _currentBlock;
    std::unique_ptr<ICodeEmitter> _emitter;
    StringContainer &_strings;
    // типы лексем для возврата
    std::deque<Token> _tokensQueue;
    // тексты лексем для возврата
    std::deque<std::u32string> _tokenTextsQueue;
    // счётчик меток
    int _labelCounter = 0;
    // стеки меток для поддержки break/continue
    std::stack<int> _startLabels;
    std::stack<int> _exitLabels;
    // для подсчёта аргументов в ArgumentsList
    std::stack<int> _argumentsCountStack;
    // для return: здесь помещаются указатели на функции,
    // return применяется к ближайшей функции (верхней в стеке)
    std::stack<std::shared_ptr<Symbol> > _returnStack;
    const std::map<std::u32string, ConstructorFunction> &_classes;
    // стек значений
    std::stack<Operand> _values;
public:
    /**
     * @brief Создаёт новый экземпляр класса Parser
     * @todo Скорее всего, стартовые параметры нужно вынести в какой-нибудь
     *       Context-класс.
     */
    Parser(std::shared_ptr<Block> &block,
           StringContainer &strContainer,
           const std::map<std::u32string, ConstructorFunction> &classes,
           std::vector<TCode> &outBuffer);
    /**
     * @brief производит синтаксический разбор строки кода
     * @param strCode строка кода
     */
    void parse(const std::u32string &strCode);
    // правила: см. файл грамматики
    
private:
    void StatementList();
    void Statement();
    void CompoundStatement();
    void IfElseStatement();
    void WhileStatement();
    void DoWhileStattement();
    void ForStatement();
    void OptionalStatement();
    void BreakStatement();
    void ContinueStatement();
    void FunctionDeclStatement();
    void OptionalParameterDeclList();
    void OptionalExpressionList();
    void ParameterDeclList();
    void ExpressionList();
    void AssignExpression();
    void FunctionDeclExpression();
    void Variable();
    void Expression();
    void TernaryExpression();
    void SimpleExpression();
    void SimpleOrShiftExpression();
    void ShiftOrRelationExpression();
    void RelationOrEqualityExpression();
    void BitwiseAndOrEqualityExpression();
    void BitwiseXOROrAndExpression();
    void BitwiseOROrXORExpression();
    void LogicalAndOrBitORExpression();
    void LogicalOrOrAndExpression();
    void LogicalOrNCOExpression();
    void Term();
    void Factor();
    void FunctionCallExpression();
    void OptionalFunctorCall();
    void FunctionCallArgs();
    void ArgumentList();
    void AnyStatement();
    void ReturnStatement();
    void ArrayDeclExpression();
    void ArrayItemRefExpression();
    void OptionalArrayItemRefExpression();
    void DotOperation();
    void OptionalDotOperation();
    void PostfixOperation();
    void VariableDeclBlock();
    void SwitchStatement();
    void OptionalDotOrBracketExpression();
    void NewExpression();

    // некоторые подстановки для уменьшения длины методов
private:
    bool resolveMathConstant();
    // перемещение по потоку
private:
    /**
     * @brief Проверяет на совпадение текущий токен с указанным токеном
     *        и в случае ошибки выбрасывает исключение, в случае
     *        успеха считывает следующий токен.
     * @param t токен, с которым должен совпадать текущий символ
     */
    void match(Token t);
    /**
     * @brief Читает следующий токен
     */
    void next();
    /**
     * @brief Возвращает текущий токен лексического анализатора.
     */
    Token lookahead();
    /**
     * @brief Возвращает текущий токен
     */
    const std::u32string &tokenText() const;
    // emitter
private:
    /**
     * @brief Возвращает id следующей метки.
     * @return
     */
    int nextLabel();
    void pushJumpLabels(int startLabelId, int exitLabelId);
    void popJumpLabels();
    /**
     * @brief Выводит инструкцию для if с меткой.
     *        Применяется к переменной, находящейся наверху стека.
     * @param exitOrFalseLabelId метка для ветки false или выхода,
     *        в зависимости то того, есть ли далее блок "else"
     */
    void emitIfFalseHeader(int exitOrFalseLabelId);
    void emitGoto(int labelId);
    void emitLabel(int labelId);
    void emitBinaryOp(OperationType opType);
    void emitBreak();
    void emitContinue();
    /**
     * @brief Унарная операция. Применяется к переменной, находящейся наверху
     *        стека.
     * @param opType тип операции
     */
    void emitUnaryOp(OperationType opType);
    void emitAssign(Symbol *lvalue);
    void emitAssign(std::shared_ptr<Symbol> &lvalue);
    /**
     * @brief Записывает начало функции.
     * Текст текущего токена - имя функции в сигнатуре.
     */
    void emitFnStart(std::shared_ptr<Symbol> &func);
    /**
     * @brief Текст текущего токена - имя параметра функции в сигнатуре.
     */
    void emitFnArg();
    /**
     * @brief При генерации кода будет использован адрес записи в таблице
     *        симоволов для связи объект Function и стартового адреса.
     * @param func
     */
    void emitFnCode(std::shared_ptr<Symbol> &func);
    void emitLoadFnArgs();
    void emitPush(); // для return
    void emitRet(); // для return
    void emitEmptyReturn();
    void emitReturn();
    void emitCall(std::shared_ptr<Symbol> &func, int nArgs, std::shared_ptr<Symbol> &resultVariable);
    void emitCall(Symbol *func, int nArgs, std::shared_ptr<Symbol> &resultVariable);
    void emitFnEnd();
    /**
     * @brief Генерация вызова метода объекта автоматизации
     * @param leftVariable переменная "слева", у которой вызываем метод
     * @param propName имя свойства/метода
     * @param resultVariable в какую переменную поместить результат вызова
     * @param nArgs число аргументов, помещённых в стек
     */
    void emitCallAOMethod(Symbol *leftVariable,
                          const std::u32string &propName,
                          std::shared_ptr<Symbol> &resultVariable,
                          int nArgs);
    void emitAllocArray(std::shared_ptr<Symbol> &arrVariable);
    void emitIncrement();
    void emitDecrement();
    void emitPop();
    void emitNewObject(const std::u32string &className, ConstructorFunction pFn, std::shared_ptr<Symbol> &resultVariable, int nArgs);
    // работа с символами
private:
    /**
     * @brief Возвращает текущую таблицу символов
     */
    std::shared_ptr<SymbolTable> currentSymbolTable();
    void pushInt(IntType value);
    void pushReal(RealType value);
    void pushBoolean(bool value);
    void pushVariable(std::shared_ptr<Symbol> &variable);
    void pushVariable(Symbol *variable);
    void pushFunction(std::shared_ptr<Symbol> &func);
    void pushString(const std::u32string &s);
    void pushArray(std::shared_ptr<Symbol> &arrVariable);
    void pushNull();
    Operand popStackValue();
    Operand stackValue();
    void pushBack(Token t, const std::u32string &str);
    void pushBack(Token t, std::u32string &&str);
    IntType popInt();
    bool popBoolean();
    RealType popReal();

    std::shared_ptr<Block> currentBlock() const;
    /**
     * @brief Добавляет дочерний блок и переходит в него
     * @return
     */
    void addAndEntrySubBlock();
    /**
     * @brief Выходит из блока в родительский блок
     */
    void exitToUpLevelBlock();
    // обработка ошибок
private:
    void error(const std::string &msg);
    void expected(Token expectedToken) noexcept(false);
    void unexpected(Token unexpectedToken);
    void undeclaredIdentifier();
    void undeclaredIdentifier(const std::u32string &s);
    void duplicateIdentifier(const std::u32string &id);
};

} // namespace escript

#endif // PARSER_H
