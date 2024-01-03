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
    expected.resize(MEM_SIZE);

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

TEST(IntComp, Day5Ex2)
{
    const std::string code = "3,3,1105,-1,9,1101,0,0,12,4,12,99,1";

    check_output(code, 0, 0);
    check_output(code, 9, 1);
    check_output(code, 7, 1);
    check_output(code, -7, 1);
}

TEST(IntComp, Day5Ex3)
{
    const std::string code = "3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9";

    check_output(code, 0, 0);
    check_output(code, 9, 1);
    check_output(code, 7, 1);
    check_output(code, -7, 1);
}

TEST(IntComp, Day5Ex4)
{
    const std::string code =
        "3,21,1008,21,8,20,1005,20,22,107,8,21,20,1006,20,31,1106,0,36,98,0,0,1002,21,125,20,4,20,1105,1,46,104,999,"
        "1105,1,46,1101,1000,1,20,4,20,1105,1,46,98,99";

    check_output(code, 8, 1000);
    check_output(code, 9, 1001);
    check_output(code, 7, 999);
    check_output(code, -7, 999);
}

TEST(IntComp, Day9Ex1)
{
    const std::string code = "3,9,8,9,10,9,4,9,99,-1,8";

    check_output(code, 8, 1);
    check_output(code, 9, 0);
    check_output(code, 7, 0);
    check_output(code, -7, 0);
}

TEST(IntComp, Day9Ex2)
{
    const std::string code = "104,1125899906842624,99";

    IntComputer program{code};

    program.run();

    ASSERT_EQ(program.get_output(), 1125899906842624);
}
