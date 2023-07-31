/**
 * @file unit.h
 * @brief Программный модуль
 */
#ifndef UNIT_H
#define UNIT_H

#include "EScript_global.h"
#include "block.h"
#include <forward_list>
#include <memory>

namespace escript {

/**
 * @brief Программный модуль.
 */
class ESCRIPT_EXPORT Unit : public std::enable_shared_from_this<Unit>
{
private:
    std::shared_ptr<Block> _block;
    // статический блок для хранения целых чисел
    // т.к. ссылки должны всегда сохраняться, используем список
    std::forward_list<long> _staticIntegerValues;
public:
    /**
     * @brief Создаёт новый экземпляр класса Unit
     */
    Unit();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Unit();
    /**
     * @brief Очищает все данные в этом модуле.
     */
    void clear();
    /**
     * @brief Возвращает корневой блок этого модуля (глобальная область видимости).
     * @return
     */
    std::shared_ptr<Block> block();
    /**
     * @brief Добавляет целочисленное значение в блок целых чисел
     * @return указатель на целое вставленное число.
     */
    long *addStaticIntValue();
    const std::forward_list<long> &staticIntegerValues() const;
};

} // namespace escript

#endif // UNIT_H
