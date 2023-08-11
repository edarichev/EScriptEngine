/**
 * @file translator.cpp
 * @brief Транслятор трёхадресного кода в целевой
 */
#include "stdafx.h"
#include "translator.h"
#include "pvalue.h"

using std::cout;
using std::endl;

namespace escript {

// для конвертации операций
std::map<OperationType, ArithmeticOperation> Translator::optypes;

Translator::Translator()
{
    if (optypes.empty()) {
        optypes = {
            {OperationType::Add, ArithmeticOperation::Add },
            {OperationType::Minus, ArithmeticOperation::Sub },
            {OperationType::Multiply, ArithmeticOperation::Mul },
            {OperationType::Div, ArithmeticOperation::Div },
            {OperationType::Less, ArithmeticOperation::BoolLess },
            {OperationType::LessOrEqual, ArithmeticOperation::BoolLessOrEqual },
            {OperationType::Greater, ArithmeticOperation::BoolGreater },
            {OperationType::GreaterOrEqual, ArithmeticOperation::BoolGreaterOrEqual },
            {OperationType::Equal, ArithmeticOperation::BoolEqual },
        };
    }
}

void Translator::translate(std::shared_ptr<Block> block,
                           const std::vector<TCode> &inputBuffer,
                           std::vector<uint8_t> &outBuffer)
{
    _block = block;
    _asm = std::make_unique<Assembler>(outBuffer);
    Assembler &a = as();
    a.jmp_m(0); // сюда запишем смещение в конце
    writeVariableSection(block, outBuffer);
    const char codeHeader[] = {'C', 'O', 'D', 'E'};
    outBuffer.insert(outBuffer.end(),
                     codeHeader, codeHeader + sizeof (codeHeader));
    uint32_t codeSegmentLengthPosition = outBuffer.size();
    uint32_t codeSegmentLength = 0;
    outBuffer.insert(outBuffer.end(),
                     (uint8_t*)&codeSegmentLength,
                     (uint8_t*)&codeSegmentLength + sizeof (codeSegmentLength));
    uint64_t startPosition = outBuffer.size();

    for (auto it = inputBuffer.begin(); it != inputBuffer.end();) {
        translateSelectedOperation(it, inputBuffer, outBuffer);
    }
    as().halt();
    // записать длину сегмента кода
    codeSegmentLength = outBuffer.size() - codeSegmentLengthPosition - sizeof (codeSegmentLength);
    std::copy((uint8_t*)&codeSegmentLength,
              (uint8_t*)&codeSegmentLength + sizeof (codeSegmentLength),
              outBuffer.begin() + codeSegmentLengthPosition);
    // записать адрес перехода в начале объектного файла
    std::copy((uint8_t*)&startPosition,
              (uint8_t*)&startPosition + sizeof (startPosition),
              outBuffer.begin() + sizeof(OpCodeType));
    // замена всех меток
    replaceLabelsToAddresses(outBuffer, startPosition);
    _asm.reset();   // больше не нужен
    _block.reset(); // тоже не нужен
}

void Translator::writeVariableSection(std::shared_ptr<Block> block,
                                      std::vector<uint8_t> &outBuffer)
{
    // создаём область данных для переменных в таблице символов блока
    // пока только одна таблица
    auto table = block->symbolTable();
    const char dataHeader[] = {'D', 'A', 'T', 'A'};
    outBuffer.insert(outBuffer.end(),
                     dataHeader, dataHeader + sizeof (dataHeader));
     // запомнить, куда запишем настоящее значение dataHeaderLength
    uint64_t startPosition = outBuffer.size();
    // пока пишем 0
    uint32_t dataHeaderLength = 0;
    outBuffer.insert(outBuffer.end(),
                     (uint8_t*)&dataHeaderLength,
                     (uint8_t*)&dataHeaderLength + sizeof (dataHeaderLength));

    uint32_t totalRecords = 0;
    writeAllSymbols(block.get(), outBuffer, totalRecords);
    dataHeaderLength = sizeof(PtrIntType) * totalRecords;
    std::copy((uint8_t*)&dataHeaderLength,
              (uint8_t*)&dataHeaderLength + sizeof (dataHeaderLength),
              outBuffer.begin() + startPosition);
}

void Translator::writeAllSymbols(Block *block,
                                 std::vector<uint8_t> &outBuffer,
                                 uint32_t &totalRecords)
{
    PtrIntType empty {};
    // начинаем с таблицы символов родительского блока
    auto table = block->symbolTable();
    for (auto &symbol : *table) {
        // все записи в этой области представляют собой указатели
        // на объекты в списке объектов
        // забиваем здесь место под размеры этих указателей
        // записать смещение
        symbol->setLocation(outBuffer.size());
        outBuffer.insert(outBuffer.end(),
                         (uint8_t*)&empty,
                         (uint8_t*)&empty + sizeof (empty));
        totalRecords++;
    }
    // теперь таблицы символов дочерних {блоков}
    for (auto &b : block->blocks()) {
        // только обычные блоки, в функциях свои переменные
        if (b->type() == BlockType::Regular)
            writeAllSymbols(b.get(), outBuffer, totalRecords);
    }
}

void Translator::translateOperation(const TCode &c)
{
    switch (c.operation) {
    case OperationType::Add:
    case OperationType::Multiply:
    case OperationType::Div:
    case OperationType::Minus:
    case OperationType::Less:
    case OperationType::LessOrEqual:
    case OperationType::Greater:
    case OperationType::GreaterOrEqual:
    case OperationType::Equal:
        binaryOp(c);
        break;
    case OperationType::Assign:
        opAssign(c);
        break;
    case OperationType::UMinus:
        opUMinus(c);
        break;
    case OperationType::Goto:
        opGoto(c);
        break;
    case OperationType::Label:
        opLabel(c);
        break;
    case OperationType::IfFalse:
        opIfFalse(c);
        break;
    case OperationType::FunctionStart:  // FNSTART - начало блока функции, op1==Symbol*
        opFunctionStart(c);
        break; // просто как метка
    case OperationType::FunctionArgument: // аргумент функции+Symbol*
        opFunctionArgument(c);
        break;
    case OperationType::FunctionCode:   // начало блока кода (стартовая точка запуска) функции
        break;
    case OperationType::LoadArguments:  // загрузка аргументов из стека на входе в функцию
        opLdArgs(c);
        break;
    case OperationType::Push:           // push op1
        opPush(c);
        break;
    case OperationType::Ret:            // возврат из функции
        opRet(c);
        break;
    case OperationType::Call:           // вызов функции, addr==op1.intValue
        opCall(c);
        break;
    case OperationType::FunctionEnd:    // конец функции
        break;
    case OperationType::BlockStart:
        _tcodeBlock = c.operand1.block;
        break;
    case OperationType::BlockEnd:
        _tcodeBlock = c.operand1.block->parentBlock() ?
                    c.operand1.block->parentBlock().get() :
                    _block.get();
        break;
    default:
        throw std::domain_error("Can not translate operation: " + c.toString());
    }
    cout << c.toString() << endl;
}

void Translator::translateFunctionBlock(std::vector<TCode>::const_iterator &it,
                                        const std::vector<TCode> &inputBuffer,
                                        std::vector<uint8_t> &outBuffer)
{
    //fnstart
    assert((*it).operation == OperationType::FunctionStart);
    translateOperation(*it);
    // заголовок 4 == FUNC, 4 - длина секции переменных, 4 - общая длина
    const char marker[] = {'F', 'U', 'N', 'C'};
    uint32_t dataLength = 0;
    uint32_t totalLength = 0;
    Symbol *func = (*it).operand1.function;
    ++it;
    std::vector<Symbol*> parameters;
    // теперь должны идти аргументы, сохраним их по порядку:
    // всегда один элемент - это arguments, он первый
    while (it != inputBuffer.end()) {
        if ((*it).operation != OperationType::FunctionArgument)
            break;
        parameters.push_back((*it).operand1.variable);
        ++it;
    }
    size_t startOfFunction = outBuffer.size();
    // записываем заголовок функции:
    outBuffer.insert(outBuffer.end(), marker, marker + sizeof (marker)); // FUNC
    // забить место:
    size_t dataLenOffset = outBuffer.size();
    outBuffer.insert(outBuffer.end(), (uint8_t*)&dataLength,
                     ((uint8_t*)&dataLength) + sizeof (dataLength));
    size_t totalLengthOffset = outBuffer.size();
    outBuffer.insert(outBuffer.end(), (uint8_t*)&totalLength,
                     ((uint8_t*)&totalLength) + sizeof (totalLength));
    size_t startPointOfData = outBuffer.size();
    // здесь таблица символов
    // сохраняем только символы из блоков кода. Если есть вложенные функции,
    // то эти таблицы символов должны быть исключены
    // это проверяется в writeAllSymbols
    uint32_t totalRecords = 0;
    writeAllSymbols(_tcodeBlock, outBuffer, totalRecords);
    // число параметров
    int64_t numOfParams = parameters.size();
    outBuffer.insert(outBuffer.end(), (uint8_t*)&numOfParams,
                     ((uint8_t*)&numOfParams) + sizeof (numOfParams));
    // всего переменных
    int64_t numOfVars = totalRecords;
    outBuffer.insert(outBuffer.end(), (uint8_t*)&numOfVars,
                     ((uint8_t*)&numOfVars) + sizeof (numOfVars));
    dataLength = outBuffer.size() - startPointOfData;
    // стартовая точка функции
    func->setLocation(outBuffer.size());

    // остальная часть
    while (it < inputBuffer.end()) {
        if ((*it).operation == OperationType::FunctionEnd) {
            translateOperation(*it);
            ++it;
            break;
        }
        translateSelectedOperation(it, inputBuffer, outBuffer);
    }
    // вернуться в начало функции и записать размеры
    totalLength = outBuffer.size() - startOfFunction;
    std::copy((uint8_t*)&dataLength,
              ((uint8_t*)&dataLength) + sizeof (dataLength),
              outBuffer.begin() + dataLenOffset);
    std::copy((uint8_t*)&totalLength,
              ((uint8_t*)&totalLength) + sizeof (totalLength),
              outBuffer.begin() + totalLengthOffset);
}

void Translator::translateSelectedOperation(std::vector<TCode>::const_iterator &it,
                                            const std::vector<TCode> &inputBuffer,
                                            std::vector<uint8_t> &outBuffer)
{
    switch ((*it).operation) {
    case OperationType::FunctionStart:
        translateFunctionBlock(it, inputBuffer, outBuffer);
        break;
    default:
        translateOperation(*it);
        ++it;
        break;
    }
}

PtrIntType Translator::location(Symbol *symbol)
{
    return (PtrIntType)symbol;
}

void Translator::replaceLabelsToAddresses(std::vector<uint8_t> &outBuffer,
                                          uint64_t startPosition)
{
    // это версия с проходом по всему тексту
    // если быстрее будет через multimap - попробовать её
    uint64_t c = startPosition;
    uint64_t addr = 0;
    uint64_t labelId = 0;
    const char *fnMarker = "FUNC";
    while (c < outBuffer.size()) {
        uint8_t *p = outBuffer.data() + c;
        OpCode opCode = (OpCode) *((OpCodeType*)p);
        auto shift = 0;
        try {
            shift = Assembler::instructionSize(opCode);
        } catch (const std::out_of_range &e) {
            if (*p == 'F') {
                if (strncmp((char*)p, fnMarker, 4) == 0) {
                    p += 4;
                    c += 4;
                    int32_t dataLen = *(int32_t*)p;
                    p += dataLen;
                    c += dataLen;
                    continue;
                }
            }
            throw;
        }
        switch (opCode) {
        case OpCode::IFFALSE_M:
        case OpCode::JMP_M:
            c += sizeof (OpCodeType); // размер кода команды
            p += sizeof (OpCodeType);
            labelId = *(uint64_t*)p;
            addr = _labels[labelId];
            std::copy((uint8_t*)&addr,
                      (uint8_t*)&addr + sizeof (addr),
                      outBuffer.begin() + c);
            c += sizeof (addr);
            break;
        default:
            c += shift;
            break;
        }
    }
}

void Translator::binaryOp(const TCode &c)
{
    if (tryCalcBinaryOp(c))
        return;
    Assembler &a = as();
    emit_ldc(c.operand1Type, c.operand1);
    emit_ldc(c.operand2Type, c.operand2);

    // действие над элементами
    switch (c.operation) {
    case OperationType::Add:
        a.addst();
        break;
    case OperationType::Multiply:
        a.mulst();
        break;
    case OperationType::Minus:
        a.subst();
        break;
    case OperationType::Div:
        a.divst();
        break;
    case OperationType::Less:
        a.stless();
        break;
    case OperationType::LessOrEqual:
        a.stless_eq();
        break;
    case OperationType::Greater:
        a.stgt();
        break;
    case OperationType::GreaterOrEqual:
        a.stgt_eq();
        break;
    case OperationType::Equal:
        a.steq();
        break;
    default:
        throw std::domain_error("Unsupported binary operation");
    }
    // сохранить в значение переменную
    a.stloc_m(location(c.lvalue));
}

void Translator::opUMinus(const TCode &c)
{
    Assembler &a = as();
    switch (c.operand1Type) {
    case SymbolType::Integer:
        a.ldc_int64_data64(c.operand1.intValue);
        break;
    case SymbolType::Variable:
        a.ldloc_m(location(c.operand1.variable));
        break;
    default:
        throw std::domain_error("Unsupported operand type for unary operation");
    }
    a.neg();
    a.stloc_m(location(c.lvalue));
}

void Translator::opAssign(const TCode &c)
{
    Assembler &a = as();
    switch (c.operand1Type) {
    case SymbolType::Integer:
        a.ldc_int64_data64(c.operand1.intValue);
        break;
    case SymbolType::Real:
        a.ldc_double_data64(c.operand1.realValue);
        break;
    case SymbolType::Variable:
        a.ldloc_m(location(c.operand1.variable));
        break;
    case SymbolType::Boolean:
        a.ldc_bool_data8(c.operand1.boolValue);
        break;
    default:
        throw std::domain_error("Unsupported operand type for assign operation");
    }
    a.stloc_m(location(c.lvalue));
}

void Translator::opGoto(const TCode &c)
{
    Assembler &a = as();
    // все переходы изначально идут на метки, во втором проходе мы заменяем их
    // на реальные адреса
    a.jmp_m(c.operand1.intValue);// тут номер метки
    _labelReferences.insert({c.operand1.intValue, a.currentPos()});
}

void Translator::opLabel(const TCode &c)
{
    Assembler &a = as();
    // ничего не выводит
    int labelId = c.operand1.intValue;
    _labels[labelId] = a.currentPos();
}

void Translator::opIfFalse(const TCode &c)
{
    Assembler &a = as();
    // пусть ругается - остальные типы оптимизировать
    assert(c.operand1Type == SymbolType::Variable);
    emit_ldc(c.operand1Type, c.operand1);
    a.iffalse_m(c.operand2.intValue); // номер метки
}

void Translator::opPush(const TCode &c)
{
    emit_ldc(c.operand1Type, c.operand1);
}

void Translator::opFunctionStart([[maybe_unused]]const TCode &c)
{

}

void Translator::opFunctionArgument([[maybe_unused]]const TCode &c)
{

}

void Translator::opLdArgs([[maybe_unused]]const TCode &c)
{
    Assembler &a = as();
    a.ldargs();
}

void Translator::opRet([[maybe_unused]]const TCode &c)
{
    Assembler &a = as();
    a.ret();
}

void Translator::opCall(const TCode &c)
{
    Assembler &a = as();
    a.call(c.operand1.function->location());
}

void Translator::emit_ldc(SymbolType type, const OperandRecord &operand)
{
    Assembler &a = as();
    switch (type) {
    case SymbolType::Integer:
        a.ldc_int64_data64(operand.intValue);
        break;
    case SymbolType::Real:
        a.ldc_double_data64(operand.realValue);
        break;
    case SymbolType::Boolean:
        a.ldc_bool_data8(operand.boolValue ? 1 : 0);
        break;
    case SymbolType::Variable:
        a.ldloc_m(location(operand.variable));
        break;
    default:
        throw std::domain_error("Unsupported type");
    }
}

bool Translator::tryCalcBinaryOp(const TCode &c)
{
    Assembler &a = as();
    // во второй части чаще переменные, поэтому сначала проверим второй операнд
    PValue value1, value2;
    switch (c.operand2Type) {
    case SymbolType::Integer:
        value2 = c.operand2.intValue;
        break;
    case SymbolType::Real:
        value2 = c.operand2.realValue;
        break;
    case SymbolType::Boolean:
        value2 = c.operand2.boolValue;
        break;
    default:
        return false;
    }
    switch (c.operand1Type) {
    case SymbolType::Integer:
        value1 = c.operand1.intValue;
        break;
    case SymbolType::Real:
        value1 = c.operand1.realValue;
        break;
    case SymbolType::Boolean:
        value1 = c.operand1.boolValue;
        break;
    default:
        return false;
    }
    // это должно быть гарантировано, если это не так, пусть вылетит
    ArithmeticOperation op = optypes.find(c.operation)->second;
    // эта часть почти совпадает с void Processor::binaryStackOp(OpCode opCode)
    PValue result = PValue::binaryOpValues(value1, value2, op);
    switch (result.type) {
    case SymbolType::Integer:
        a.ldc_int64_data64(result.intValue);
        break;
    case SymbolType::Real:
        a.ldc_double_data64(result.realValue);
        break;
    case SymbolType::Boolean:
        a.ldc_bool_data8(result.boolValue);
        break;
    default:
        break; // уже проверено
    }
    a.stloc_m(location(c.lvalue));
    return true;
}

} // namespace escript
