/**
 * @file instruction.h
 * @brief Машинная инструкция
 */
#ifndef ESCRIPT_INSTRUCTION_H
#define ESCRIPT_INSTRUCTION_H

#include "EScript_global.h"

namespace escript {

class ESCRIPT_EXPORT Instruction
{
public:
    /**
     * @brief Создаёт новый экземпляр класса Instruction
     */
    Instruction();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Instruction();
    // команды. код и операнды записываются в буфер памяти
public:
    void st_m_a(uint64_t addr)
    { // примерная запись кода и данных
        uint8_t buf[100]; // буфер (условно)
        uint8_t ST_M_A = 47; // код команды
        uint8_t *p = buf + 23;
        *p = ST_M_A; // записать код
        p++;
        *(uint64_t *)p = addr; // операнд 8 байт
        p += sizeof(uint64_t);
    }
};

} // namespace escript

#endif // ESCRIPT_INSTRUCTION_H
