#pragma once

namespace arms::domain {

enum class HairState { STILL, MOVING };

class Hair {
   public:
    Hair() = default;

    explicit Hair(HairState state) : state_(state) {}

    HairState getState() const { return state_; }

    bool isMoving() const { return state_ == HairState::MOVING; }

    void setState(HairState state) { state_ = state; }

   private:
    HairState state_{HairState::STILL};
};

}  // namespace arms::domain
