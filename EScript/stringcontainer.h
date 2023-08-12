#ifndef ESCRIPT_STRINGCONTAINER_H
#define ESCRIPT_STRINGCONTAINER_H

#include "EScript_global.h"
#include <vector>

namespace escript {

/**
 * @brief Хранилище строк
 */
class ESCRIPT_EXPORT StringContainer
{
    std::vector<std::u32string> _strings;
public:
    /**
     * @brief Создаёт новый экземпляр класса StringContainer
     */
    StringContainer();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~StringContainer();
    const std::u32string &get(int index) const;
    /**
     * @brief Добавляет строку и возвращает её индекс
     * @param s
     * @return
     */
    size_t add(std::u32string s);
};

} // namespace escript

#endif // ESCRIPT_STRINGCONTAINER_H
