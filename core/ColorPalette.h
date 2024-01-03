#ifndef _COLOR_PALETTE_H
#define _COLOR_PALETTE_H

#include "fmttools.h"
#include <format>
#include <string>

struct ColorPalette
{
    std::string background = "#FFFFFF";
    std::string highlighted_background = "#FFFFFF";
    std::string text = "#000000";
    std::string comment = "#808080";
    std::string keyword = "#2020A0";
    std::string operator_ = "#A02020";
    std::string number = "#20A0A0";
    std::string string = "#20A020";
};

auto format_value(ColorPalette const &palette, auto fmt_it)
{
    auto it = std::format_to(fmt_it, "ColorPalette {{text = {}, ", palette.text);
    it = std::format_to(it, "background = {}, ", palette.background);
    it = std::format_to(it, "highlighted_bg = {}, ", palette.highlighted_background);

    return std::format_to(it, "}}");
}

#endif
