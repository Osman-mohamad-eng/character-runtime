#include "runtime/Transform.h"

#include <gtest/gtest.h>

#include <cmath>
#include <stdexcept>

TEST(TransformTests, CombinesParentAndChildWorldTransforms) {
    runtime::Transform parent;
    runtime::Transform child;
    parent.setLocalPosition({10.0F, 0.0F, 0.0F});
    child.setLocalPosition({0.0F, 2.0F, 0.0F});
    child.setParent(&parent);

    const runtime::Vector3 position = child.worldPosition();
    EXPECT_FLOAT_EQ(position.x(), 10.0F);
    EXPECT_FLOAT_EQ(position.y(), 2.0F);
    EXPECT_FLOAT_EQ(position.z(), 0.0F);
}

TEST(TransformTests, ParentChangesInvalidateDescendantWorldMatrices) {
    runtime::Transform root;
    runtime::Transform child;
    child.setParent(&root);
    EXPECT_FLOAT_EQ(child.worldPosition().x(), 0.0F);

    root.setLocalPosition({4.0F, 0.0F, 0.0F});
    EXPECT_FLOAT_EQ(child.worldPosition().x(), 4.0F);
}

TEST(TransformTests, ConvertsCoordinatesBothWays) {
    runtime::Transform transform;
    transform.setLocalPosition({3.0F, -2.0F, 5.0F});
    transform.setLocalRotation({0.1F, 0.3F, -0.2F});
    transform.setLocalScale({2.0F, 3.0F, 0.5F});
    const runtime::Vector3 local{2.0F, -1.0F, 4.0F};

    const runtime::Vector3 restored = transform.worldToLocal(transform.localToWorld(local));
    EXPECT_NEAR(restored.x(), local.x(), 1.0e-5F);
    EXPECT_NEAR(restored.y(), local.y(), 1.0e-5F);
    EXPECT_NEAR(restored.z(), local.z(), 1.0e-5F);
}

TEST(TransformTests, RejectsParentCyclesAndSingularWorldToLocal) {
    runtime::Transform root;
    runtime::Transform child;
    child.setParent(&root);
    EXPECT_THROW(root.setParent(&child), std::invalid_argument);
    root.setLocalScale({1.0F, 0.0F, 1.0F});
    EXPECT_THROW(root.worldToLocal({}), std::domain_error);
}

TEST(TransformTests, DetachesChildrenWhenParentIsDestroyed) {
    runtime::Transform child;
    {
        runtime::Transform parent;
        child.setParent(&parent);
        parent.setLocalPosition({8.0F, 0.0F, 0.0F});
        EXPECT_FLOAT_EQ(child.worldPosition().x(), 8.0F);
    }
    EXPECT_EQ(child.parent(), nullptr);
    EXPECT_FLOAT_EQ(child.worldPosition().x(), 0.0F);
}
