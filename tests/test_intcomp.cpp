//
// Created by andiw on 20/12/2023.
//

#include <gtest/gtest.h>
#include "core/IntComputer.h"

TEST(IntComp, Parsing) {

    IntComputer c{"1,2,3"};
    std::vector<CodeType> expected = {{1,2,3}};

    ASSERT_EQ(c.opcodes(), expected);
}