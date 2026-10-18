#include "runtime/Matrix4.h"

#include <gtest/gtest.h>

#include <cmath>
#include <stdexcept>

TEST(Matrix4Tests, ComposesTranslationRotationAndScale) {
    const runtime::Matrix4 transform = runtime::Matrix4::translation({5.0F, 0.0F, 0.0F})
        * runtime::Matrix4::rotationZ(3.14159265358979323846F / 2.0F)
        * runtime::Matrix4::scale({2.0F, 2.0F, 2.0F});
    const runtime::Vector3 result = transform.transformPoint({1.0F, 0.0F, 0.0F});

    EXPECT_NEAR(result.x(), 5.0F, 1.0e-5F);
    EXPECT_NEAR(result.y(), 2.0F, 1.0e-5F);
    EXPECT_NEAR(result.z(), 0.0F, 1.0e-5F);
}

TEST(Matrix4Tests, InvertsAnAffineTransform) {
    const runtime::Matrix4 matrix = runtime::Matrix4::translation({2.0F, -3.0F, 4.0F})
        * runtime::Matrix4::rotationY(0.4F)
        * runtime::Matrix4::scale({2.0F, 3.0F, 4.0F});
    const runtime::Vector3 point{3.0F, 5.0F, -2.0F};
    const runtime::Vector3 restored = matrix.inverse().transformPoint(matrix.transformPoint(point));

    EXPECT_NEAR(restored.x(), point.x(), 1.0e-5F);
    EXPECT_NEAR(restored.y(), point.y(), 1.0e-5F);
    EXPECT_NEAR(restored.z(), point.z(), 1.0e-5F);
}

TEST(Matrix4Tests, RejectsSingularMatrix) {
    EXPECT_THROW(runtime::Matrix4::scale({1.0F, 0.0F, 1.0F}).inverse(), std::domain_error);
}
