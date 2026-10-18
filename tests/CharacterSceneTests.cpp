#include "runtime/RuntimeLoop.h"
#include "runtime/TransformProfiler.h"

#include <gtest/gtest.h>

#include <stdexcept>

TEST(CharacterSceneTests, AppliesConfigurableCharacterDimensions) {
    runtime::Scene scene;
    runtime::Character& character = scene.createCharacter("custom-player");
    runtime::CharacterParameters parameters;
    parameters.height = 2.0F;
    parameters.width = 0.8F;
    parameters.depth = 0.4F;
    parameters.skinColor = {0.2F, 0.4F, 0.6F};
    character.appearance().setParameters(parameters);

    const runtime::Vector3 scale = character.transform().localScale();
    EXPECT_FLOAT_EQ(scale.x(), 0.8F);
    EXPECT_FLOAT_EQ(scale.y(), 2.0F);
    EXPECT_FLOAT_EQ(scale.z(), 0.4F);
    EXPECT_FLOAT_EQ(character.appearance().parameters().skinColor.y(), 0.4F);
}

TEST(CharacterSceneTests, RejectsInvalidCharacterParameters) {
    runtime::Character character("player");
    runtime::CharacterParameters parameters;
    parameters.height = 0.0F;
    EXPECT_THROW(character.appearance().setParameters(parameters), std::invalid_argument);

    parameters.height = 1.8F;
    parameters.skinColor = {1.1F, 0.0F, 0.0F};
    EXPECT_THROW(character.appearance().setParameters(parameters), std::invalid_argument);
}

TEST(CharacterSceneTests, RuntimeLoopUpdatesSceneAndReportsCollisions) {
    runtime::Scene scene;
    runtime::Entity& first = scene.createEntity("first");
    runtime::Entity& second = scene.createEntity("second");
    first.setBoundingSphere({{}, 1.0F});
    second.setBoundingSphere({{}, 1.0F});
    second.transform().setLocalPosition({1.0F, 0.0F, 0.0F});

    const runtime::RuntimeLoop loop;
    const std::vector<runtime::CollisionPair> collisions = loop.tick(scene, 1.0F / 60.0F);
    ASSERT_EQ(collisions.size(), 1U);
    EXPECT_EQ(collisions.front().first, &first);
    EXPECT_EQ(collisions.front().second, &second);
}

TEST(CharacterSceneTests, ValidatesDeltaTimeAndProfilerIterations) {
    runtime::Scene scene;
    const runtime::RuntimeLoop loop;
    EXPECT_THROW(loop.tick(scene, -0.1F), std::invalid_argument);
    EXPECT_THROW(runtime::TransformProfiler{}.measure(scene, 0), std::invalid_argument);
}

TEST(CharacterSceneTests, ProfilesRepeatedWorldTransformUpdates) {
    runtime::Scene scene;
    scene.createEntity("profiled-entity").transform().setLocalPosition({1.0F, 2.0F, 3.0F});

    const runtime::ProfileResult result = runtime::TransformProfiler{}.measure(scene, 3);
    EXPECT_EQ(result.iterations, 3U);
    EXPECT_GE(result.totalMilliseconds, 0.0);
    EXPECT_GE(result.averageMilliseconds, 0.0);
    EXPECT_GE(result.maximumMilliseconds, result.averageMilliseconds);
}
