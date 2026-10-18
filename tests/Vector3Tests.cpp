#include "runtime/Vector3.h"

#include <gtest/gtest.h>

#include <stdexcept>

TEST(Vector3Tests, SupportsArithmetic) {
    const runtime::Vector3 first{1.0F, 2.0F, 3.0F};
    const runtime::Vector3 second{4.0F, -1.0F, 2.0F};

    const runtime::Vector3 sum = first + second;
    EXPECT_FLOAT_EQ(sum.x(), 5.0F);
    EXPECT_FLOAT_EQ(sum.y(), 1.0F);
    EXPECT_FLOAT_EQ(sum.z(), 5.0F);
    EXPECT_FLOAT_EQ((first - second).y(), 3.0F);
    EXPECT_FLOAT_EQ((first * 2.0F).z(), 6.0F);
    EXPECT_FLOAT_EQ((first / 2.0F).x(), 0.5F);
}

TEST(Vector3Tests, ComputesDotCrossAndMagnitude) {
    const runtime::Vector3 xAxis{1.0F, 0.0F, 0.0F};
    const runtime::Vector3 yAxis{0.0F, 1.0F, 0.0F};

    EXPECT_FLOAT_EQ(xAxis.dot(yAxis), 0.0F);
    const runtime::Vector3 cross = xAxis.cross(yAxis);
    EXPECT_FLOAT_EQ(cross.x(), 0.0F);
    EXPECT_FLOAT_EQ(cross.y(), 0.0F);
    EXPECT_FLOAT_EQ(cross.z(), 1.0F);
    EXPECT_FLOAT_EQ((runtime::Vector3{3.0F, 4.0F, 0.0F}.magnitude()), 5.0F);
}

TEST(Vector3Tests, NormalizesAndHandlesZeroVector) {
    const runtime::Vector3 normalized = runtime::Vector3{0.0F, 3.0F, 4.0F}.normalized();
    EXPECT_FLOAT_EQ(normalized.magnitude(), 1.0F);
    EXPECT_FLOAT_EQ(runtime::Vector3{}.normalized().magnitude(), 0.0F);
}

TEST(Vector3Tests, RejectsDivisionByZero) {
    EXPECT_THROW((runtime::Vector3{1.0F, 2.0F, 3.0F} / 0.0F), std::invalid_argument);
}
