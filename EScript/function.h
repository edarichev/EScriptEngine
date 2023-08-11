#ifndef ESCRIPT_FUNCTION_H
#define ESCRIPT_FUNCTION_H

#include "EScript_global.h"

namespace escript {

/**
 * @brief Представляет объект Function
 */
class ESCRIPT_EXPORT Function
{
    uint64_t _callAddress {};
    std::u32string _name;
public:
    /**
     * @brief Создаёт новый экземпляр класса Function
     */
    Function();

    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Function();
    uint64_t callAddress() const;
    void setCallAddress(uint64_t newCallAddress);
    const std::u32string &name() const;
    void setName(const std::u32string &newName);
};

} // namespace escript

#endif // ESCRIPT_FUNCTION_H
