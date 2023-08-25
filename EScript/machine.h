/**
 * @file machine.h
 * @brief Исполняющая машина
 */
#ifndef ESCRIPT_MACHINE_H
#define ESCRIPT_MACHINE_H

#include "EScript_global.h"
#include <vector>
#include "storage.h"
#include "stringcontainer.h"
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
    // хранилище строк
    StringContainer _strings;
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
    /**
     * @brief Удаляет память, строки, объекты.
     */
    void clear();
    Storage &storage();
    StringContainer &strings();
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
    Processor &cpu();
    /**
     * @brief Возвращает значение, расположенное по адресу, на который
     * указывает свойство location() данного символа.
     * @details Значение извлекается из абсолютного адреса.
     * Например, в секции DATA находится 8 байт, соответствующие
     * символу "symbol", эти 8 байт возвращаются.
     * @param symbol
     * @return
     */
    uint64_t addressValueOf(std::shared_ptr<Symbol> &symbol) const;
    /**
     * @brief Заменяет значение указателя в секции DATA для данного символа так,
     * чтобы он указывал на запись rec.
     * @param symbol
     * @param rec
     */
    void replaceValuePtr(std::shared_ptr<Symbol> &symbol, ObjectRecord *rec);
    void replaceValuePtr(std::shared_ptr<Symbol> &symbol, StringObject *s);

private:
    size_t offsetOfStartPoint(const std::vector<uint8_t> &objectFile);
    /**
     * @brief Добавляет смещения к операндам всех команд перехода.
     * @param startPosition стартовая позиция кода в сегменте CODE.
     * @param offset смещение
     */
    void replaceJMPAddresses(uint64_t startPosition, uint64_t offset);
    void addCallAddressOffsetToFunctions(std::shared_ptr<Block> &block, uint64_t offset);
};

} // namespace escript

#endif // ESCRIPT_MACHINE_H
