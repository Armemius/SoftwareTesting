#include <gtest/gtest.h>

#include <vector>

#include <domain/arthur.hpp>
#include <domain/camera_shot.hpp>
#include <domain/head.hpp>
#include <domain/inference.hpp>
#include <domain/movement_to_console.hpp>
#include <domain/perception_event.hpp>

namespace dmn = arms::domain;

TEST(DomainModelTest, BaseScenarioResolvesToCameraEffect) {
    dmn::Arthur arthur{dmn::Head{dmn::Hair{}, dmn::Materiality::IMMATERIAL}};
    arthur.beginMovingToConsole(dmn::MovementToConsole{0.4, {10.0, 8.0, 6.0, 4.0}});
    arthur.processFilmingScene(dmn::CameraShot{dmn::CameraShotType::DOLLY_IN, true});

    const dmn::PerceptionEvent &event = arthur.getLastPerception();
    const dmn::Inference &inference = arthur.getLastInference();

    EXPECT_TRUE(event.hasOccurred());
    EXPECT_TRUE(event.isIllusory());
    EXPECT_EQ(event.getType(), dmn::PerceptionType::HAIR_STIRRING);
    EXPECT_EQ(inference.getCause(), dmn::CauseHypothesis::CAMERA_EFFECT);
}

TEST(DomainModelTest, NoCameraDollyNoIllusoryPerception) {
    dmn::Arthur arthur{dmn::Head{dmn::Hair{}, dmn::Materiality::IMMATERIAL}};
    arthur.beginMovingToConsole(dmn::MovementToConsole{0.4, {10.0, 8.0, 6.0, 4.0}});
    arthur.processFilmingScene(dmn::CameraShot{dmn::CameraShotType::NONE, false});

    const dmn::PerceptionEvent &event = arthur.getLastPerception();
    const dmn::Inference &inference = arthur.getLastInference();

    EXPECT_FALSE(event.hasOccurred());
    EXPECT_FALSE(event.isIllusory());
    EXPECT_EQ(event.getType(), dmn::PerceptionType::NONE);
    EXPECT_EQ(inference.getCause(), dmn::CauseHypothesis::UNDETERMINED);
}

TEST(DomainModelTest, InferenceCauseIsCameraEffectWhenEventOccurs) {
    dmn::Arthur arthur{dmn::Head{dmn::Hair{}, dmn::Materiality::IMMATERIAL}};
    arthur.beginMovingToConsole(dmn::MovementToConsole{0.7, {12.0, 9.0, 7.0, 5.0}});
    arthur.processFilmingScene(dmn::CameraShot{dmn::CameraShotType::DOLLY_IN, true});

    ASSERT_TRUE(arthur.getLastPerception().hasOccurred());
    EXPECT_EQ(arthur.getLastInference().getCause(), dmn::CauseHypothesis::CAMERA_EFFECT);
}

TEST(DomainModelTest, CognitiveStatesFollowExpectedOrder) {
    dmn::Arthur arthur{dmn::Head{dmn::Hair{}, dmn::Materiality::IMMATERIAL}};
    arthur.beginMovingToConsole(dmn::MovementToConsole{0.6, {9.0, 7.0, 5.0, 3.0}});
    arthur.processFilmingScene(dmn::CameraShot{dmn::CameraShotType::DOLLY_IN, true});

    EXPECT_EQ(arthur.getLastInference().getTimeline(),
              (std::vector<dmn::CognitiveState>{dmn::CognitiveState::SURPRISED, dmn::CognitiveState::REASONING,
                                                dmn::CognitiveState::CONCLUDED}));
}

TEST(DomainModelTest, MovementSteadyApproachIsDetected) {
    const dmn::MovementToConsole movement{0.5, {6.0, 5.0, 4.0, 4.0, 2.0}};

    EXPECT_TRUE(movement.isSteadyApproach());
}

TEST(DomainModelTest, SlowBoundaryIsInclusive) {
    const dmn::MovementToConsole movement{dmn::MovementToConsole::K_SLOW_SPEED_UPPER_BOUND, {5.0, 4.0, 3.0}};

    EXPECT_TRUE(movement.isSlow());
}

TEST(DomainModelTest, NonMonotonicApproachIsRejected) {
    const dmn::MovementToConsole movement{0.5, {6.0, 4.0, 5.0, 3.0}};

    EXPECT_FALSE(movement.isSteadyApproach());
}

TEST(DomainModelTest, ImmaterialHeadScenarioRemainsNarrativelyConsistent) {
    dmn::Arthur arthur{dmn::Head{dmn::Hair{}, dmn::Materiality::IMMATERIAL}};
    arthur.beginMovingToConsole(dmn::MovementToConsole{0.4, {8.0, 6.0, 4.0, 2.0}});
    arthur.processFilmingScene(dmn::CameraShot{dmn::CameraShotType::DOLLY_IN, true});

    EXPECT_TRUE(arthur.getLastPerception().hasOccurred());
    EXPECT_EQ(arthur.getLastInference().getCause(), dmn::CauseHypothesis::CAMERA_EFFECT);
    EXPECT_TRUE(arthur.isNarrativeConsistent());
}
