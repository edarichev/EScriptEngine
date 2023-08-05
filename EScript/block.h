/**
 * @file block.h
 * @brief Блок кода
 */
#ifndef ESCRIPT_BLOCK_H
#define ESCRIPT_BLOCK_H

#include "EScript_global.h"
#include "symboltable.h"
#include <vector>
#include <memory>

namespace escript {

/**
 * @brief Блок кода.
 */
class ESCRIPT_EXPORT Block : public std::enable_shared_from_this<Block>
{
private:
    std::shared_ptr<Unit> _unit;
    std::vector<std::shared_ptr<Block> > _blocks;
    std::shared_ptr<Block> _parentBlock;
    std::shared_ptr<SymbolTable> _symbolTable;
public:
    /**
     * @brief Создаёт новый экземпляр класса Block
     */
    Block(std::shared_ptr<Unit> unit, std::shared_ptr<Block> parent = nullptr);
    /**
     * @brief Освобождает связанные с этим экземпляром ресурсы
     */
    virtual ~Block();
    /**
     * @brief Добавить новый вложенный блок.
     * @return
     */
    std::shared_ptr<Block> addBlock();
    /**
     * @brief Возвращает таблицу символов этого блока
     * @return
     */
    std::shared_ptr<SymbolTable> symbolTable();
    /**
     * @brief Возвращает глобальный блок (самый верхний в иерархии)
     * @return
     */
    std::shared_ptr<Block> globalBlock();
    /**
     * @brief Добавляет смещение ко всем символам всех дочерних таблиц символов
     * @param offset смещение
     */
    void addOffset(uint64_t offset);
};

} // namespace escript

#endif // ESCRIPT_BLOCK_H
