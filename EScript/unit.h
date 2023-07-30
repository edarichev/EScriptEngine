/**
 * @file unit.h
 * @brief Программный модуль
 */
#ifndef UNIT_H
#define UNIT_H

#include "EScript_global.h"
#include "block.h"

namespace escript {

/**
 * @brief Программный модуль.
 */
class ESCRIPT_EXPORT Unit
{
private:
    std::shared_ptr<Block> _block;
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
};

} // namespace escript

#endif // UNIT_H
