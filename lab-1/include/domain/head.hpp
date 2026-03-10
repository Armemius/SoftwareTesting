#pragma once

#include "hair.hpp"

namespace arms::domain {

enum class Materiality { MATERIAL, IMMATERIAL };

class Head {
   public:
    Head() = default;

    explicit Head(Hair hair, Materiality materiality) : hair_(hair), materiality_(materiality) {}

    const Hair &getHair() const { return hair_; }

    Hair &mutableHair() { return hair_; }

    Materiality getMateriality() const { return materiality_; }

    void setMateriality(Materiality materiality) { materiality_ = materiality; }

   private:
    Hair hair_;
    Materiality materiality_{Materiality::IMMATERIAL};
};

}  // namespace arms::domain
