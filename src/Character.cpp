#include "runtime/Character.h"

#include <cmath>
#include <stdexcept>

namespace runtime {

void CharacterAppearanceComponent::validate(const CharacterParameters& parameters) {
    if (!std::isfinite(parameters.height) || parameters.height <= 0.0F
        || !std::isfinite(parameters.width) || parameters.width <= 0.0F
        || !std::isfinite(parameters.depth) || parameters.depth <= 0.0F) {
        throw std::invalid_argument("Character dimensions must be finite and positive");
    }
    const auto validColor = [](float channel) {
        return std::isfinite(channel) && channel >= 0.0F && channel <= 1.0F;
    };
    if (!validColor(parameters.skinColor.x()) || !validColor(parameters.skinColor.y())
        || !validColor(parameters.skinColor.z())) {
        throw std::invalid_argument("Character skin color channels must be between zero and one");
    }
}

void CharacterAppearanceComponent::setParameters(const CharacterParameters& parameters) {
    validate(parameters);
    parameters_ = parameters;
    update(0.0F);
}

void CharacterAppearanceComponent::update(float) {
    owner().transform().setLocalScale(
        {parameters_.width, parameters_.height, parameters_.depth});
}

Character::Character(std::string name) : Entity(std::move(name)) {
    appearance_ = &addComponent<CharacterAppearanceComponent>();
    appearance_->update(0.0F);
}

} // namespace runtime
