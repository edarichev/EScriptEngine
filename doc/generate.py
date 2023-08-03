#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Aug  2 14:25:12 2023

@author: de

Генерирует классы Assembler, OpCode, Assembler_Test
"""

from datetime import datetime

dtNow = datetime.now()
dtNow = dtNow.strftime("%d.%m.%Y %H:%M:%S")

# файл с кодами операций
descFileName = "opcodes.csv"

def wordWrap(src):
    n = 60 # разбить по 60 примерно
    s = ""
    i = 0
    for c in src:
        i += 1
        s += c
        if (i >= n and c == ' '):
            s += "\n"
            i = 0
    s = s.rstrip("\n")
    return s

def commentOut(strComment, shift=0):
    s = ""
    prefix = " "*shift
    sComment = prefix + "/**\n"
    for sline in wordWrap(strComment).splitlines():
        sComment += prefix + " * " + sline + "\n"
    sComment += prefix + " */\n"
    s += sComment;
    return s

def shiftText(t, shift):
    lines = t.splitlines()
    s = ""
    prefix = " "*shift
    for sline in lines:
        s += prefix + sline + "\n"
    return s

def makeEnumId(sMnemonic):
    return sMnemonic.replace(",", "_").replace(" ", "_").replace("__", "_");

def generateEnumItem(parts, shift, orderNum):
    s = ""
    s = commentOut(parts[7], shift)
    s += " "*shift + makeEnumId(parts[0]) + " = " + str(orderNum) + ",\n"
    return s

def makeFuncName(stext):
    return makeEnumId(stext).lower()
    
def makeOpCodeWriteMethod(parts, shift):
    s = ""
    # функция, записывающая команду
    sTemplate1 = '''$(Comment)void $(FuncName)()
{
    op1(OpCode::$(EnumItem));
}
'''
    sTemplate2 = '''$(Comment)void $(FuncName)($(DataType) data)
{
    op2(OpCode::$(EnumItem), data);
}
'''
    nOperands = int(parts[1])
    if nOperands == 0:
        s = sTemplate1
    elif nOperands == 1: # есть аргументы
        s = sTemplate2
    else:
        raise Exception("Много аргументов у команды")
    sDataType = parts[3]
    sComment = commentOut("@brief " + parts[7])
    s = s.replace("$(Comment)", sComment)
    s = s.replace("$(FuncName)", makeFuncName(parts[0]))
    s = s.replace("$(EnumItem)", makeEnumId(parts[0]))
    s = s.replace("$(DataType)", sDataType)
    s = shiftText(s, shift)
    return s
    

def makeTestCode(parts):
    s = ""
    sTemplateOpCode1 = '''
    a.$(Func)();
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::$(OpCode));
    assert(*((Assembler::OpCodeType *)p) == (Assembler::OpCodeType)OpCode::$(OpCode));
    assert(obj.size() == n);
    '''
    
    sTemplateOpCode2 = '''
    value64 = rand();
    a.$(Func)($(Variable));
    p = obj.data() + n;
    n += Assembler::instructionSize(OpCode::$(OpCode));
    assert(*p == (Assembler::OpCodeType)OpCode::$(OpCode));
    p += Assembler::INSTRUCTION_CODE_SIZE;
    assert(*(($(DataType) *)p) == $(Variable));
    assert(obj.size() == n);
    '''
    
    sVariable = "value64" # заменить на другой
    sType = parts[3]
    if sType == "uint64_t":
        sVariable = "value64"
    elif sType == "uint32_t":
        sVariable = "value32"
    elif sType == "uint16_t":
        sVariable = "value16"
    elif sType == "uint8_t":
        sVariable = "value8"
    nOperands = int(parts[1])
    if nOperands == 0:
        s = sTemplateOpCode1
    elif nOperands == 1:
        s = sTemplateOpCode2
    else:
        Exception("Много аргументов у команды")
    
    s = s.replace("$(DataType)", sType)
    s = s.replace("$(Variable)", sVariable)
    s = s.replace("$(OpCode)", makeEnumId(parts[0]))
    s = s.replace("$(Func)", makeFuncName(parts[0]))
    return s

##############################################################################

# перечисление кодов 

i = 0
iEnumValue = 0
sEnumBody = ""
sCommandList = "";
shift = 4

listItemsInRow = 4
sOpCodesList = ""     # $(OpCodesList)
sOpCodeNamesList = "" # $(OpCodeNamesList)
sOpCodesSizeList = "" # $(OpCodesSizeList)

sTest = "" # $(Test)
with open(descFileName) as f:
    for line in f:
        i = i + 1
        if i == 1: # заголовок
            continue
        parts = line.split("\t")
        if len(parts) == 0:
            continue
        sEnumBody += generateEnumItem(parts, shift, iEnumValue)
        sCommandList += makeOpCodeWriteMethod(parts, shift)
        sOpCodesList += "OpCode::" + makeEnumId(parts[0]) + ", "
        sOpCodesSizeList += str(int(parts[6])) + ", "
        sOpCodeNamesList += "\"" + parts[0].strip() + "\", "
        sTest += makeTestCode(parts)
        iEnumValue+=1
        if (iEnumValue % listItemsInRow == 0):
            sOpCodesList += "\n    "
            sOpCodesSizeList += "\n    "
            sOpCodeNamesList += "\n    "
    pass

# тип данных
enumDataType = "uint8_t"
if iEnumValue > 255:
    enumDataType = "uint16_t"

######################### Загрузка шаблонов ###################################
strOpcodeH = ''
opcodeHTemplateFile = "template.opcode.h"
with open(opcodeHTemplateFile, 'r') as f:
    strOpcodeH = f.read()

# класс Assembler
strAssemblerClass = ''
assemblerHTemplateFile = "template.assembler.h"
with open(assemblerHTemplateFile, 'r') as f:
    strAssemblerClass = f.read()

strAssemblerCPPClass = ''
assemblerCPPTemplateFile = "template.assembler.cpp"
with open(assemblerCPPTemplateFile, 'r') as f:
    strAssemblerCPPClass = f.read()

testTemplateFile = "template.assembler_test.cpp"
strTestContent = ""
with open(testTemplateFile, 'r') as f:
    strTestContent = f.read()

######################### Замены в шаблонах ###################################

strOpCodeHContent = strOpcodeH.replace("$(Enum)", sEnumBody).replace("$(OpCodeEnumDataType)", enumDataType)
strOpCodeHContent = strOpCodeHContent.replace("$(Date)", dtNow)

strAssemblerHContent = strAssemblerClass
strAssemblerHContent = strAssemblerHContent.replace("$(CommandList)", sCommandList)
strAssemblerHContent = strAssemblerHContent.replace("$(OpCodeEnumDataType)", enumDataType)
strAssemblerHContent = strAssemblerHContent.replace("$(Date)", dtNow)

strAssemblerCPPContent = strAssemblerCPPClass.replace("$(Date)", dtNow)
strAssemblerCPPContent = strAssemblerCPPContent.replace("$(OpCodesList)", sOpCodesList)
strAssemblerCPPContent = strAssemblerCPPContent.replace("$(OpCodeNamesList)", sOpCodeNamesList)
strAssemblerCPPContent = strAssemblerCPPContent.replace("$(OpCodesSizeList)", sOpCodesSizeList)

strTestContent = strTestContent.replace("$(Date)", dtNow)
strTestContent = strTestContent.replace("$(Test)", sTest)

############################## Запись ########################################

import codecs

fileOpCodeH = "opcode.h"
with codecs.open(fileOpCodeH, "w", "utf-8") as temp:
    temp.write(strOpCodeHContent)

fileAssemblerH = "assembler.h"
with codecs.open(fileAssemblerH, "w", "utf-8") as temp:
    temp.write(strAssemblerHContent)

fileAssemblerCPP = "assembler.cpp"
with codecs.open(fileAssemblerCPP, "w", "utf-8") as temp:
    temp.write(strAssemblerCPPContent)

testCppFile = "assembler_test.cpp"
with codecs.open(testCppFile, "w", "utf-8") as temp:
    temp.write(strTestContent)









