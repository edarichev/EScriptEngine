#ifndef ESCRIPT_STRINGCONTAINER_H
#define ESCRIPT_STRINGCONTAINER_H

#include "EScript_global.h"
#include <list>

namespace escript {

class StringObject;

/**
 * @brief Хранилище строк
 */
class ESCRIPT_EXPORT StringContainer
{
    std::list<StringObject*> _strings;
public:
    /**
     * @brief Создаёт новый экземпляр класса StringContainer
     */
    StringContainer();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~StringContainer();
    /**
     * @brief Удаляет все строки
     */
    void clear();
    /**
     * @brief Добавляет строку и возвращает её индекс
     * @param s
     * @return
     */
    StringObject *add(const std::u32string &s);
    StringObject *add(StringObject *s);
    void checkDuplicates() const;
private:
    StringObject *findString(const std::u32string &s) const;
};

} // namespace escript

#endif // ESCRIPT_STRINGCONTAINER_H
