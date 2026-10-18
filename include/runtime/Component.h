#pragma once

namespace runtime {

class Entity;

class Component {
public:
    virtual ~Component() = default;
    virtual void update(float deltaSeconds) = 0;

protected:
    Entity& owner() const;

private:
    friend class Entity;
    Entity* owner_ = nullptr;
};

} // namespace runtime
