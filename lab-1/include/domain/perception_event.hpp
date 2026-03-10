#pragma once

namespace arms::domain {

enum class PerceptionType { NONE, HAIR_STIRRING };

class PerceptionEvent {
   public:
    PerceptionEvent() = default;

    explicit PerceptionEvent(PerceptionType type, bool occurred, bool illusory)
        : type_(type), occurred_(occurred), illusory_(illusory) {}

    static PerceptionEvent none() { return PerceptionEvent{}; }

    static PerceptionEvent illusoryHairStirring() {
        return PerceptionEvent{PerceptionType::HAIR_STIRRING, true, true};
    }

    PerceptionType getType() const { return type_; }

    bool hasOccurred() const { return occurred_; }

    bool isIllusory() const { return illusory_; }

   private:
    PerceptionType type_{PerceptionType::NONE};
    bool occurred_{false};
    bool illusory_{false};
};

}  // namespace arms::domain
