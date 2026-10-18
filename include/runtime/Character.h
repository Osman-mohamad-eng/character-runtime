#pragma once

#include "runtime/Entity.h"

namespace runtime {

struct CharacterParameters {
    float height = 1.8F;
    float width = 0.55F;
    float depth = 0.3F;
    Vector3 skinColor{0.72F, 0.55F, 0.42F};
};

class CharacterAppearanceComponent final : public Component {
public:
    void setParameters(const CharacterParameters& parameters);
    const CharacterParameters& parameters() const noexcept { return parameters_; }
    void update(float deltaSeconds) override;

private:
    static void validate(const CharacterParameters& parameters);
    CharacterParameters parameters_;
};

class Character final : public Entity {
public:
    explicit Character(std::string name);

    CharacterAppearanceComponent& appearance() noexcept { return *appearance_; }
    const CharacterAppearanceComponent& appearance() const noexcept { return *appearance_; }

private:
    CharacterAppearanceComponent* appearance_ = nullptr;
};

} // namespace runtime
