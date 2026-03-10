#pragma once

#include "camera_shot.hpp"
#include "head.hpp"
#include "inference.hpp"
#include "movement_to_console.hpp"
#include "perception_event.hpp"

namespace arms::domain {

class Arthur {
   public:
    Arthur() = default;

    explicit Arthur(Head head) : head_(head) {}

    const Head &getHead() const { return head_; }

    const MovementToConsole &getMovement() const { return movement_; }

    const PerceptionEvent &getLastPerception() const { return last_perception_; }

    const Inference &getLastInference() const { return last_inference_; }

    void beginMovingToConsole(const MovementToConsole &movement) { movement_ = movement; }

    void processFilmingScene(const CameraShot &shot) {
        if (shot.isDollyIn() && movement_.isSlow() && movement_.isSteadyApproach()) {
            head_.mutableHair().setState(HairState::MOVING);
            last_perception_ = PerceptionEvent::illusoryHairStirring();
            last_inference_ =
                Inference{CauseHypothesis::CAMERA_EFFECT,
                          {CognitiveState::SURPRISED, CognitiveState::REASONING, CognitiveState::CONCLUDED}};
            return;
        }

        head_.mutableHair().setState(HairState::STILL);
        last_perception_ = PerceptionEvent::none();
        last_inference_ = Inference{};
    }

    bool isNarrativeConsistent() const {
        return !(head_.getMateriality() == Materiality::IMMATERIAL &&
                 last_inference_.getCause() == CauseHypothesis::PHYSICAL_CAUSE);
    }

   private:
    Head head_{Hair{}, Materiality::IMMATERIAL};
    MovementToConsole movement_{};
    PerceptionEvent last_perception_{};
    Inference last_inference_{};
};

}  // namespace arms::domain
