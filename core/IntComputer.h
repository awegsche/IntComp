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

using CodeType = int64_t;

class IntComputer
{

  public:
    enum OpCode {
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

    enum State {
        IDLE,
        RUN,
        INPUT,
        OUTPUT,
        ATEND,
    };

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
        switch (codes[offset]) {
        case 0:// position
            return std::format("@{}", m_opcodes[position + offset]);
        case 1:// immediate
            return std::format("<font color=\"yellow\">{}</font>", m_opcodes[position + offset]);
        default:
            throw std::runtime_error("unknown mode");
        }
    }

    auto get_value(uint8_t offset) -> CodeType &
    {
        switch (codes[offset]) {
        case 0:// position
            return m_opcodes[m_opcodes[position + offset]];
        case 1:// immediate
            return m_opcodes[position + offset];
        default:
            throw std::runtime_error("unknown mode");
        }
    }

    auto run()
    {
        m_state = RUN;

        do {
            step();
        } while (m_state == RUN);
    }

    auto print() -> std::string
    {
        auto const start = position;
        auto const start_state = m_state;

        std::string formatted_string{};
        while (m_state != ATEND && position < m_opcodes.size()) {
            if (position == start) {
                formatted_string += "<span style = \"background-color:#202040\">";
            } else {
                formatted_string += "<span>";
            }
            formatted_string += print_step();
            formatted_string += "</span><br/>";
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

        switch (codes[0]) {
        case ADD:
            formatted_code += "<font color=\"#00a000\">ADD</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " " + write_value(2);
            formatted_code += " -> " + write_addr(3);
            position += 4;
            break;
        case MUL:
            formatted_code += "<font color=\"#00a000\">MUL</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " " + write_value(2);
            formatted_code += " -> " + write_addr(3);
            position += 4;
            break;
        case INP:
            formatted_code += "<font color=\"#00a000\">INP</font>";
            formatted_code += " -> " + write_addr(1);
            formatted_code += " -> " + write_addr(1);
            position += 2;
            m_state = INPUT;
            break;
        case OUT:
            formatted_code += "<font color=\"#00a000\">OUT</font>";
            formatted_code += " -> " + write_addr(1);
            position += 2;
            m_state = OUTPUT;
            break;
        case JTR:
            formatted_code += "<font color=\"#00a000\">JTR</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " -> " + write_value(2);
            position += 3;
            break;
        case JFA:
            formatted_code += "<font color=\"#00a000\">JFA</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " -> " + write_value(2);
            position += 3;
            break;
        case LES:
            formatted_code += "<font color=\"#00a000\">LES</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " < " + write_value(2);
            formatted_code += " -> " + write_addr(3);
            // position = get_value(1) == 0 ? get_value(2) : position + 3;
            // m_opcodes[position + 3] = get_value(1) < get_value(2) ? 1 : 0;
            position += 4;
            break;
        case EQU:
            formatted_code += "<font color=\"#00a000\">EQU</font>";
            formatted_code += " " + write_value(1);
            formatted_code += " == " + write_value(2);
            formatted_code += " -> " + write_addr(3);
            // m_opcodes[position + 3] = get_value(1) == get_value(2) ? 1 : 0;
            position += 4;
            break;
        case END:
            formatted_code += "<font color=\"#a00000\">END</font>";
            position += 1;
            m_state = ATEND;
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

        if (m_state == INPUT) {
            m_opcodes[position + 1] = m_input;
            position += 2;
            return true;
        }

        get_opcode();

        switch (codes[0]) {
        case ADD:
            m_opcodes[position + 3] = get_value(1) + get_value(2);
            position += 4;
            break;
        case MUL:
            m_opcodes[position + 3] = get_value(1) * get_value(2);
            position += 4;
            break;
        case INP:
            m_state = INPUT;
            return false;
        case OUT:
            m_output = get_value(1);
            m_state = OUTPUT;
            return false;
        case JTR:
            position = get_value(1) != 0 ? get_value(2) : position + 3;
            break;
        case JFA:
            position = get_value(1) == 0 ? get_value(2) : position + 3;
            break;
        case LES:
            m_opcodes[position + 3] = get_value(1) < get_value(2) ? 1 : 0;
            position += 4;
            break;
        case EQU:
            m_opcodes[position + 3] = get_value(1) == get_value(2) ? 1 : 0;
            position += 4;
            break;
        case END:
            m_state = ATEND;
            return false;
        }
        return true;
    }

    void set_color_palette(ColorPalette const &color_palette) { m_color_palette = color_palette; }

  private:
    std::vector<CodeType> m_opcodes = {};
    CodeType m_input = -1;
    CodeType m_output = -1;
    size_t position = 0;
    std::array<uint8_t, 5> codes = { 0 };
    State m_state = IDLE;
    ColorPalette m_color_palette = {};
};

#endif// INTCOMPUTER_H
