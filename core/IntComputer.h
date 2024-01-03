//
// Created by andiw on 20/12/2023.
//

#ifndef INTCOMPUTER_H
#define INTCOMPUTER_H

#include <array>
#include <charconv>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>

#include "ColorPalette.h"


#include <qtypes.h>

using CodeType = int64_t;

enum class OpCode {
    ADD = 1,
    MUL,
    INP,
    OUT,
    JTR,
    JFA,
    LES,
    EQU,
    END = 99,
};

enum class State {
    IDLE,
    RUN,
    INPUT,
    OUTPUT,
    ATEND,
};

enum class Mode { POSITION = 0, IMMEDIATE, RELATIVE };


class IntComputer
{

  public:
    IntComputer() : m_opcodes({ static_cast<CodeType>(OpCode::END) }) {}

    IntComputer(std::string const &code)
    {
        auto pos = code.begin();

        while (true) {
            auto until = std::find(pos, code.end(), ',');

            std::string_view v(pos, until);
            int value;
            std::from_chars(v.data(), v.data() + v.length(), value);

            m_opcodes.push_back(value);

            if (until == code.end()) break;
            pos = until + 1;
        }
    }

    auto opcodes() const -> std::vector<CodeType> const & { return m_opcodes; }

    auto get_opcode()
    {
        CodeType c = m_opcodes[position];

        codes[0] = c % 100;
        c /= 100;
        for (int i = 1; i < 5; ++i) {
            codes[i] = c % 10;
            c /= 10;
        }
    }

    auto write_addr(uint8_t offset) const -> std::string { return std::format("@{}", m_opcodes[position + offset]); }

    auto write_value(uint8_t offset) const -> std::string
    {
        switch (static_cast<Mode>(codes[offset])) {
        case Mode::POSITION:// position
            return std::format("@{}", m_opcodes[position + offset]);
        case Mode::IMMEDIATE:// immediate
            return std::format("<font color=\"yellow\">{}</font>", m_opcodes[position + offset]);
        case Mode::RELATIVE:// immediate
            return std::format("<font color=\"orange\">${}</font>", m_opcodes[position + offset]);
        default:
            throw std::runtime_error("unknown mode");
        }
    }

    auto get_value(uint8_t offset) -> CodeType &
    {
        switch (static_cast<Mode>(codes[offset])) {
        case Mode::POSITION:
            return m_opcodes[m_opcodes[position + offset]];
        case Mode::IMMEDIATE:
            return m_opcodes[position + offset];
        case Mode::RELATIVE:
            return m_opcodes[relative_position + offset];
        default:
            throw std::runtime_error("unknown mode");
        }
    }

    auto get_address(uint8_t offset) -> CodeType &
    {
        switch (static_cast<Mode>(codes[offset])) {
        case Mode::POSITION:
            return m_opcodes[position + offset];
        case Mode::IMMEDIATE:
            return m_opcodes[position + offset];
        case Mode::RELATIVE:
            return m_opcodes[relative_position + offset];
        default:
            throw std::runtime_error("unknown mode");
        }
    }

    /// \brief runs the program until
    ///  - it hits an END opcode
    ///  - it requires input
    ///  - it prints output (?)
    ///
    void run()
    {
        do {
            step();
        } while (m_state == State::RUN);
    }

    auto print() -> std::string
    {
        auto const start = position;
        position = 0;
        auto const start_state = m_state;

        std::string formatted_string{};
        while (m_state != State::ATEND && position < m_opcodes.size()) {
            if (position == start) {
                formatted_string += std::format(
                    "<span style = \"background-color:{};display:block\">", m_color_palette.highlighted_background);
            } else {
                formatted_string += "<span>";
            }
            formatted_string += print_step();
            formatted_string += "<br/></span>";
        }
        while (position < m_opcodes.size()) {
            codes[0] = 1;
            // here is probably garbage, set to immediate to just print the value
            formatted_string += std::format("<font color=\"#808080\">{:03d} | </font>", position);
            formatted_string += std::format("{}<br/>", get_value(0));
            position += 1;
        }
        position = start;
        m_state = start_state;
        return formatted_string;
    }

    auto print_step() -> std::string
    {
        std::string formatted_code;

        get_opcode();

        formatted_code += std::format("<font color=\"#808080\">{:03} | </font>", position);

        switch (static_cast<OpCode>(codes[0])) {
        case OpCode::ADD:
            formatted_code += "<font color=\"#00a000\">ADD</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " " + write_value(2);
            formatted_code += " -> " + write_addr(3);
            position += 4;
            break;
        case OpCode::MUL:
            formatted_code += "<font color=\"#00a000\">MUL</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " " + write_value(2);
            formatted_code += " -> " + write_addr(3);
            position += 4;
            break;
        case OpCode::INP:
            formatted_code += "<font color=\"#00a000\">INP</font>";
            formatted_code += " -> " + write_addr(1);
            position += 2;
            m_state = State::INPUT;
            break;
        case OpCode::OUT:
            formatted_code += "<font color=\"#00a000\">OUT</font>";
            formatted_code += " -> " + write_addr(1);
            position += 2;
            m_state = State::OUTPUT;
            break;
        case OpCode::JTR:
            formatted_code += "<font color=\"#00a000\">JTR</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " -> " + write_value(2);
            position += 3;
            break;
        case OpCode::JFA:
            formatted_code += "<font color=\"#00a000\">JFA</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " -> " + write_value(2);
            position += 3;
            break;
        case OpCode::LES:
            formatted_code += "<font color=\"#00a000\">LES</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " < " + write_value(2);
            formatted_code += " -> " + write_addr(3);
            // position = get_value(1) == 0 ? get_value(2) : position + 3;
            // m_opcodes[position + 3] = get_value(1) < get_value(2) ? 1 : 0;
            position += 4;
            break;
        case OpCode::EQU:
            formatted_code += "<font color=\"#00a000\">EQU</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " == " + write_value(2);
            formatted_code += " -> " + write_addr(3);
            // m_opcodes[position + 3] = get_value(1) == get_value(2) ? 1 : 0;
            position += 4;
            break;
        case OpCode::END:
            formatted_code += "<font color=\"#a00000\">END</font>";
            position += 1;
            m_state = State::ATEND;
            break;
        default:
            codes[0] = 1;
            // here is probably garbage, set to immediate to just print the value
            formatted_code += std::format("{}", get_value(0));
            position += 1;
            break;
        }
        // formatted_code += "<br/>";

        return formatted_code;
    }

    auto step() -> bool
    {

        if (m_state == State::INPUT) {
            m_opcodes[get_address(1)] = m_input;
            position += 2;
            m_state = State::RUN;
            return true;
        }

        get_opcode();

        switch (static_cast<OpCode>(codes[0])) {
        case OpCode::ADD:
            m_opcodes[position + 3] = get_value(1) + get_value(2);
            position += 4;
            break;
        case OpCode::MUL:
            m_opcodes[position + 3] = get_value(1) * get_value(2);
            position += 4;
            break;
        case OpCode::INP:
            m_state = State::INPUT;
            return false;
        case OpCode::OUT:
            m_output = get_value(1);
            m_state = State::OUTPUT;
            position += 2;
            return false;
        case OpCode::JTR:
            position = get_value(1) != 0 ? get_value(2) : position + 3;
            break;
        case OpCode::JFA:
            position = get_value(1) == 0 ? get_value(2) : position + 3;
            break;
        case OpCode::LES: {
            const auto pos = get_address(3);
            m_opcodes[pos] = get_value(1) < get_value(2) ? 1 : 0;
            position += 4;
            break;
        }
        case OpCode::EQU: {
            const auto pos = get_address(3);
            m_opcodes[pos] = get_value(1) == get_value(2) ? 1 : 0;
            position += 4;
            break;
        }
        case OpCode::END:
            m_state = State::ATEND;
            return false;
        }
        return true;
    }

    void set_color_palette(ColorPalette const &color_palette) { m_color_palette = color_palette; }

    void set_input(CodeType input) { m_input = input; }

    CodeType get_output() const { return m_output; }

    State get_state() const { return m_state; }

  private:
    std::vector<CodeType> m_opcodes = {};
    CodeType m_input = -1;
    CodeType m_output = -1;
    size_t position = 0;
    size_t relative_position = 0;
    std::array<uint8_t, 5> codes = { 0 };
    State m_state = State::RUN;
    ColorPalette m_color_palette = {};
};

#endif// INTCOMPUTER_H
