//
// Created by andiw on 20/12/2023.
//

#include "core/IntComputer.h"
#include <string>
#include <string_view>

#include <gtest/gtest.h>

TEST(IntComp, Parsing)
{

    IntComputer c{ "1,2,3" };
    std::vector<CodeType> expected = { { 1, 2, 3 } };

    ASSERT_EQ(c.opcodes(), expected);
}

TEST(IntComp, Default)
{

    IntComputer c{};
    std::vector<CodeType> expected = { { 99 } };

    ASSERT_EQ(c.opcodes(), expected);

    c.run();

    ASSERT_EQ(c.opcodes(), expected);
}

void check_output(const std::string &code, CodeType input, CodeType expected_output)
{
    IntComputer program(code);
    program.run();
    ASSERT_EQ(program.get_state(), State::INPUT);
    program.set_input(input);
    program.run();
    ASSERT_EQ(program.get_state(), State::OUTPUT);

    ASSERT_EQ(program.get_output(), expected_output);
}

TEST(IntComp, Day5Ex1)
{
    const std::string code = "3,9,8,9,10,9,4,9,99,-1,8";

    check_output(code, 8, 1);
    check_output(code, 9, 0);
    check_output(code, 7, 0);
    check_output(code, -7, 0);
}
