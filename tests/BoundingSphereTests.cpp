#include "runtime/BoundingSphere.h"

#include <gtest/gtest.h>

#include <stdexcept>

TEST(BoundingSphereTests, DetectsOverlapAndSeparation) {
    const runtime::BoundingSphere first{{}, 1.0F};
    EXPECT_TRUE(first.intersects({{1.5F, 0.0F, 0.0F}, 1.0F}));
    EXPECT_FALSE(first.intersects({{2.1F, 0.0F, 0.0F}, 1.0F}));
}

TEST(BoundingSphereTests, TransformsCenterAndRadius) {
    runtime::Transform transform;
    transform.setLocalPosition({3.0F, 0.0F, 0.0F});
    transform.setLocalScale({2.0F, 3.0F, 4.0F});

    const runtime::BoundingSphere world = runtime::BoundingSphere{{1.0F, 0.0F, 0.0F}, 0.5F}
        .transformed(transform);
    EXPECT_FLOAT_EQ(world.center.x(), 5.0F);
    EXPECT_FLOAT_EQ(world.radius, 2.0F);
}

TEST(BoundingSphereTests, RejectsNegativeRadius) {
    EXPECT_THROW(runtime::BoundingSphere({}, -1.0F), std::invalid_argument);
}
