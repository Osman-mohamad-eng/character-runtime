#include "runtime/Component.h"

#include "runtime/Entity.h"

#include <stdexcept>

namespace runtime {

Entity& Component::owner() const {
    if (owner_ == nullptr) {
        throw std::logic_error("Component is not attached to an Entity");
    }
    return *owner_;
}

} // namespace runtime
