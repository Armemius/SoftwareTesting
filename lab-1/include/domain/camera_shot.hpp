#pragma once

namespace arms::domain {

enum class CameraShotType { NONE, DOLLY_IN };

class CameraShot {
   public:
    CameraShot() = default;

    explicit CameraShot(CameraShotType type, bool active) : type_(type), active_(active) {}

    CameraShotType getType() const { return type_; }

    bool isActive() const { return active_; }

    bool isDollyIn() const { return active_ && type_ == CameraShotType::DOLLY_IN; }

    void setType(CameraShotType type) { type_ = type; }

    void setActive(bool active) { active_ = active; }

   private:
    CameraShotType type_{CameraShotType::NONE};
    bool active_{false};
};

}  // namespace arms::domain
