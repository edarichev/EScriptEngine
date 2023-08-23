#ifndef ESCRIPT_FUNCTION_H
#define ESCRIPT_FUNCTION_H

#include "EScript_global.h"
#include "automationobject.h"

namespace escript {

class Symbol;

/**
 * @brief Представляет объект Function
 */
class ESCRIPT_EXPORT Function : public AutomationObject
{
    uint64_t _callAddress {};
    std::u32string _name;
    Symbol *_symbol = nullptr;
public:
    /**
     * @brief Создаёт новый экземпляр класса Function
     */
    Function(Symbol *sym);

    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Function();
    uint64_t callAddress() const;
    void setCallAddress(uint64_t newCallAddress);
    const std::u32string &name() const;
    void setName(const std::u32string &newName);
    void addOffset(int64_t offset);
};

} // namespace escript

#endif // ESCRIPT_FUNCTION_H
