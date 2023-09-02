/**
 * @file escript.h
 * @brief Определение класса скриптового движка.
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
#ifndef ESCRIPT_H
#define ESCRIPT_H

#include "EScript_global.h"
#include "unit.h"
#include "machine.h"

namespace escript {

/**
 * @brief Класс движка. Используйте его в своей программе для запуска скриптов.
 */
class ESCRIPT_EXPORT EScript
{
private:
    std::ostream *_outStream = nullptr;
    bool _showTCode = false;
    bool _showDisassembleListing = false;
    Machine _machine;
    std::shared_ptr<Unit> _unit;
    // сюда помещаются объекты через функцию attachObject;
    // перед первым выполнением из этого контейнера объекты извлекаются
    // и добавляются в глобальную таблицу символов
    std::vector<std::pair<AutomationObject*, std::u32string> > _deferredObjects;
    std::vector<std::shared_ptr<Symbol>> _deferredSymbols;
    // уже добавили стандартные объекты?
    bool _standardObjectsAdded = false;
    std::map<std::u32string, AutomationObject*> _standardObjects;
    // классы, которые можно создавать в скрипте: производные от AutomationObject
    std::map<std::u32string, ConstructorFunction> _classes;
public:
    /**
     * @brief Создаёт новый экземпляр скриптового движка
     */
    EScript();
    /**
     * @brief Удаляет связанные ресурсы
     */
    virtual ~EScript();
    /**
     * @brief Выполняет указанную строку кода
     * @param strCode строка кода
     */
    void eval(const std::u32string &strCode);
    /**
     * @brief Компилирует, загружает в память, но не запускает
     * @param strCode
     */
    void compile(const std::u32string &strCode);
    /**
     * @brief Запускает предварительно скомпилированную программу
     *        с текущего адреса.
     */
    void run();
    /**
     * @brief Возвращает указатель на программный модуль.
     * @return
     */
    std::shared_ptr<Unit> unit();
    /**
     * @brief Сбрасывает движок в первоначальное состояние.
     */
    void clear();
    ObjectRecord *getObjectRecord(std::shared_ptr<Symbol> symbol);
    Machine *machine();
    void setShowTCode(bool newShowTCode);
    void setShowDisassembleListing(bool newShowDisassembleListing);
    void setOutStream(std::ostream &newOutStream);
    void attachObject(AutomationObject *obj, const std::u32string &name);
    void detachObject(AutomationObject *obj);
    void registerClass(const std::u32string &className, ConstructorFunction pFn);
private:
    /**
     * @brief Регистрирует в глобальной таблице символов стандартные объекты,
     *        например, "console".
     */
    void registerStandardObjects(std::shared_ptr<Block> &firstBlock);
    /**
     * @brief Создаёт и добавляет после первой трансляции стандартные объекты
     *        в секцию DATA первого блока.
     */
    void addStandardObjects();
    void registerDeferredObjects(std::shared_ptr<Block> &objectFileBlock);
    void addDeferredObjects();
};

} // namespace escript

#endif // ESCRIPT_H
