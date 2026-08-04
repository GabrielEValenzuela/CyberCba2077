#include <gtest/gtest.h>

#include <algorithm>

#include "cybercba/MissionGraph.hpp"

namespace
{
cybercba::MissionGraph buildSample()
{
    cybercba::MissionGraph graph;
    graph.addNode({"shelter", cybercba::MissionNodeType::Scene, false,
                   {{"workshop", ""}, {"exit", "power_restored"}}});
    graph.addNode({"workshop", cybercba::MissionNodeType::Investigation, true, {{"shelter", ""}}});
    graph.addNode({"exit", cybercba::MissionNodeType::Transition, false, {}});
    graph.setStart("shelter");
    return graph;
}
} // namespace

TEST(MissionGraph, UnknownNodeIsRejected)
{
    cybercba::MissionGraph graph;
    graph.addNode({"a", cybercba::MissionNodeType::Scene, false, {}});
    EXPECT_FALSE(graph.setStart("missing"));
    EXPECT_TRUE(graph.setStart("a"));
}

TEST(MissionGraph, UnconditionalTransitionIsAvailableImmediately)
{
    auto graph = buildSample();
    const auto transitions = graph.availableTransitions();
    EXPECT_NE(std::find(transitions.begin(), transitions.end(), "workshop"), transitions.end());
    EXPECT_EQ(std::find(transitions.begin(), transitions.end(), "exit"), transitions.end());
}

TEST(MissionGraph, LockedTransitionRequiresFlag)
{
    auto graph = buildSample();
    EXPECT_FALSE(graph.advanceTo("exit"));
    EXPECT_EQ(graph.current(), "shelter");

    graph.setFlag("power_restored");
    EXPECT_TRUE(graph.advanceTo("exit"));
    EXPECT_EQ(graph.current(), "exit");
}

TEST(MissionGraph, AdvanceMarksPreviousNodeCompleted)
{
    auto graph = buildSample();
    EXPECT_FALSE(graph.isCompleted("shelter"));
    ASSERT_TRUE(graph.advanceTo("workshop"));
    EXPECT_TRUE(graph.isCompleted("shelter"));
    EXPECT_EQ(graph.current(), "workshop");
}

TEST(MissionGraph, OptionalNodeCanConvergeBackToMainPath)
{
    auto graph = buildSample();
    ASSERT_TRUE(graph.advanceTo("workshop"));
    ASSERT_TRUE(graph.advanceTo("shelter"));
    EXPECT_EQ(graph.current(), "shelter");
}

TEST(MissionGraph, NodesExposesTheFullRegisteredGraphForMapRendering)
{
    auto graph = buildSample();
    ASSERT_EQ(graph.nodes().size(), 3U);
    EXPECT_EQ(graph.nodes()[0].id, "shelter");
    EXPECT_EQ(graph.nodes()[1].id, "workshop");
    EXPECT_TRUE(graph.nodes()[1].optional);
    EXPECT_FALSE(graph.nodes()[0].optional);
}
