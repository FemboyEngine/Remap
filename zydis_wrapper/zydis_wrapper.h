#pragma once

#include <Zydis/Zydis.h>
#include <Zycore/Format.h>
#include <format>
#include <string>

class Zydis {
public:
    Zydis();

    auto Disassemble(uint64_t address, const unsigned char* data, size_t size) -> bool;

    [[nodiscard]] auto GetInstruction() const->std::string;

    [[nodiscard]] auto GetInstructionBytes(const unsigned char* data) const->std::string;

    [[nodiscard]] auto GetInstructionAddress() const->uint64_t;

    [[nodiscard]] auto GetVisibleOperandCount() const -> int;

    [[nodiscard]] auto GetDecodedInstruction() const->ZydisDecodedInstruction;

    [[nodiscard]] auto GetDecodedOperands() const -> const ZydisDecodedOperand*;

private:
    ZydisDecoder decoder_;
    ZydisFormatter formatter_;
    uint64_t address_;
    bool success_;
    int visible_operand_count_;
    char instruction_text_[256];
    struct Instruction {
        ZydisDecodedInstruction info;
        ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
    } instruction_;
};