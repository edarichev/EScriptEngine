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
    std::ostream *_outStream = nullptr;
    bool _showTCode = false;
    bool _showDisassembleListing = false;
    Machine _machine;
    std::shared_ptr<Unit> _unit;
    // сюда помещаются объекты через функцию detach
    // перед первым выполнением из этого контейнера объекты извлекаются
    // и добавляются в глобальную таблицу символов
    std::vector<std::pair<AutomationObject*, std::u32string> > _deferredObjects;
    std::vector<std::shared_ptr<Symbol>> _deferredSymbols;
    // уже запускали?
    bool _isAlreadyRunned = false;
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
    const Machine &machine() const;
    void setShowTCode(bool newShowTCode);
    void setShowDisassembleListing(bool newShowDisassembleListing);
    void setOutStream(std::ostream &newOutStream);
    void attachObject(AutomationObject *obj, const std::u32string &name);
    void detachObject(AutomationObject *obj);
};

} // namespace escript

#endif // ESCRIPT_H
