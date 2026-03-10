#pragma once

#include <utility>
#include <vector>

namespace arms::domain {

enum class CauseHypothesis { UNDETERMINED, PHYSICAL_CAUSE, CAMERA_EFFECT };

enum class CognitiveState { CALM, SURPRISED, REASONING, CONCLUDED };

class Inference {
   public:
    Inference() = default;

    explicit Inference(CauseHypothesis cause, std::vector<CognitiveState> timeline)
        : cause_(cause), timeline_(std::move(timeline)) {
        if (timeline_.empty()) {
            timeline_.push_back(CognitiveState::CALM);
        }
    }

    CauseHypothesis getCause() const { return cause_; }

    const std::vector<CognitiveState> &getTimeline() const { return timeline_; }

   private:
    CauseHypothesis cause_{CauseHypothesis::UNDETERMINED};
    std::vector<CognitiveState> timeline_{CognitiveState::CALM};
};

}  // namespace arms::domain
