/**
 * @file machine.h
 * @brief Исполняющая машина
 */
#ifndef ESCRIPT_MACHINE_H
#define ESCRIPT_MACHINE_H

#include "EScript_global.h"

namespace escript {

/**
 * @brief Исполняющая машина
 */
class ESCRIPT_EXPORT Machine
{
public:
    /**
     * @brief Создаёт новый экземпляр класса Machine
     */
    Machine();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Machine();
};

} // namespace escript

#endif // ESCRIPT_MACHINE_H
