#pragma once

#include <cstddef>
#include <utility>
#include <vector>

#pragma region test

namespace arms::domain {

class MovementToConsole {
   public:
    static constexpr double K_SLOW_SPEED_UPPER_BOUND = 1.0;

    MovementToConsole() = default;

    explicit MovementToConsole(double speed, std::vector<double> distance_trace)
        : speed_(speed), distance_trace_(std::move(distance_trace)) {}

    double getSpeed() const { return speed_; }

    const std::vector<double> &getDistanceTrace() const { return distance_trace_; }

    bool isSlow() const { return speed_ <= K_SLOW_SPEED_UPPER_BOUND; }

    bool isSteadyApproach() const {
        if (distance_trace_.size() < 2) {
            return false;
        }

        bool has_progress = false;
        for (size_t idx = 1; idx < distance_trace_.size(); ++idx) {
            const double prev = distance_trace_[idx - 1];
            const double current = distance_trace_[idx];
            if (current > prev) {
                return false;
            }
            if (current < prev) {
                has_progress = true;
            }
        }
        return has_progress;
    }

   private:
    double speed_{0.0};
    std::vector<double> distance_trace_{};
};

}  // namespace arms::domain

#pragma endregion