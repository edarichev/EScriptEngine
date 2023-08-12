/**
 * @file escript.h
 * @brief Определение класса скриптового движка.
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
    Machine _machine;
    std::shared_ptr<Unit> _unit;
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
     * @brief Возвращает указатель на программный модуль.
     * @return
     */
    Unit *unit();
    /**
     * @brief Сбрасывает движок в первоначальное состояние.
     */
    void clear();
    ObjectRecord *getObjectRecord(std::shared_ptr<Symbol> symbol);
    std::u32string getUString(int index);
};

} // namespace escript

#endif // ESCRIPT_H
