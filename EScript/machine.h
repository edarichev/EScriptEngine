/**
 * @file machine.h
 * @brief Исполняющая машина
 */
#ifndef ESCRIPT_MACHINE_H
#define ESCRIPT_MACHINE_H

#include "EScript_global.h"
#include <vector>
#include "storage.h"
#include "processor.h"

namespace escript {

class Block;

/**
 * @brief Исполняющая машина
 */
class ESCRIPT_EXPORT Machine
{
    // память машины
    std::vector<uint8_t> _memory;
    // хранилище объектов
    Storage _storage;
    // ЦП
    Processor _cpu;
public:
    /**
     * @brief Создаёт новый экземпляр класса Machine
     */
    Machine();
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Machine();
    const Storage &storage() const;
    /**
     * @brief Загружает в память объектный файл
     * @param block информация о блоке (таблицы символов и т.п.)
     * @param objectFile объектный файл
     */
    void load(std::shared_ptr<Block> block, const std::vector<uint8_t> &objectFile);
    /**
     * @brief Запустить исполнение с текущей позиции PC
     */
    void run();
private:
    size_t startOffsetOf(const std::vector<uint8_t> &objectFile);
};

} // namespace escript

#endif // ESCRIPT_MACHINE_H
