#include "cybercba/Campaign.hpp"
#include "cybercba/GameController.hpp"
#include "cybercba/SaveService.hpp"
#include "cybercba/WorldLayout.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

TEST(QueueChallenge, PreservesFifoAndUndo)
{
    cybercba::QueueChallenge q;
    EXPECT_EQ(q.expectedDequeue(), "Hospital");
    q.executeNext();
    EXPECT_EQ(q.expectedDequeue(), "Estacion Guemes");
    EXPECT_TRUE(q.undo());
    EXPECT_EQ(q.expectedDequeue(), "Hospital");
    q.restart();
    EXPECT_EQ(q.size(), 4);
}
TEST(QueueChallenge, FeedbackAndResultAreTracked)
{
    cybercba::QueueChallenge q;
    EXPECT_FALSE(q.predict("Terminal").correct);
    EXPECT_TRUE(q.predict("Hospital").correct);
    EXPECT_GT(q.result().attempts, 0);
    EXPECT_LT(q.result().accuracyPercent, 100);
}
TEST(GameController, RewardUpdatesExistingModel)
{
    cybercba::GameSession s;
    cybercba::GameController c(s);
    c.dispatch(cybercba::GameCommand::NewGame);
    s.recordQueueResult(85, 75);
    EXPECT_EQ(s.model().credits(), 75);
    EXPECT_TRUE(s.progress().queueMissionCompleted);
    EXPECT_EQ(s.progress().unlockedMission, 2);
}
TEST(SaveService, HandlesMissingCorruptAndRoundTrip)
{
    auto path = (std::filesystem::temp_directory_path() / "cybercba-save-test.txt").string();
    std::filesystem::remove(path);
    cybercba::SaveService service(path);
    cybercba::GameSession loaded;
    EXPECT_EQ(service.load(loaded), cybercba::SaveLoadStatus::Missing);
    cybercba::GameSession saved;
    saved.startNewGame();
    saved.recordQueueResult(90, 50);
    EXPECT_TRUE(service.save(saved));
    EXPECT_EQ(service.load(loaded), cybercba::SaveLoadStatus::Loaded);
    EXPECT_EQ(loaded.model().credits(), 50);
    EXPECT_TRUE(loaded.progress().queueMissionCompleted);
    {
        std::ofstream corrupt(path);
        corrupt << "not-a-save";
    }
    EXPECT_EQ(service.load(loaded), cybercba::SaveLoadStatus::Corrupt);
    std::filesystem::remove(path);
}
TEST(Campaign, CharactersHaveExclusiveAbilities)
{
    EXPECT_TRUE(cybercba::AbilitySystem::hasHacking(cybercba::CharacterId::Emma));
    EXPECT_FALSE(cybercba::AbilitySystem::hasCombat(cybercba::CharacterId::Emma));
    EXPECT_TRUE(cybercba::AbilitySystem::hasCombat(cybercba::CharacterId::Magga));
    EXPECT_FALSE(cybercba::AbilitySystem::hasHacking(cybercba::CharacterId::Magga));
}
TEST(Campaign, HackingCombatAndMissionAreTestableWithoutRaylib)
{
    cybercba::GameSession session;
    ASSERT_TRUE(session.startPrologue(cybercba::CharacterId::Emma));
    EXPECT_EQ(session.campaign().stage, cybercba::PrologueStage::Shelter);
    EXPECT_EQ(cybercba::AbilitySystem::hack(session.player(), false), cybercba::AbilityResult::InvalidTarget);
    EXPECT_EQ(cybercba::AbilitySystem::hack(session.player(), true), cybercba::AbilityResult::Success);
    EXPECT_EQ(cybercba::AbilitySystem::hack(session.player(), true), cybercba::AbilityResult::CoolingDown);
    EXPECT_TRUE(cybercba::MissionSystem::advance(
        session.campaign(), cybercba::PrologueStage::Shelter, cybercba::PrologueStage::Transmission, "transmission"));
    EXPECT_TRUE(session.campaign().checkpointReached);
    cybercba::PlayerState player;
    EXPECT_EQ(cybercba::AbilitySystem::strike(player, true, true), cybercba::AbilityResult::Success);
    EXPECT_TRUE(cybercba::AbilitySystem::applyDamage(player, 25));
    EXPECT_EQ(player.health, 75);
    EXPECT_FALSE(cybercba::AbilitySystem::applyDamage(player, 25));
}
TEST(Campaign, CompletionUnlocksNextRelease)
{
    cybercba::CampaignProgress progress;
    ASSERT_TRUE(cybercba::MissionSystem::startPrologue(progress, cybercba::CharacterId::Magga));
    EXPECT_TRUE(cybercba::MissionSystem::advance(
        progress, cybercba::PrologueStage::Shelter, cybercba::PrologueStage::Transmission));
    EXPECT_TRUE(cybercba::MissionSystem::advance(
        progress, cybercba::PrologueStage::Transmission, cybercba::PrologueStage::Route));
    EXPECT_TRUE(cybercba::MissionSystem::advance(
        progress, cybercba::PrologueStage::Route, cybercba::PrologueStage::Convergence));
    EXPECT_TRUE(cybercba::MissionSystem::advance(
        progress, cybercba::PrologueStage::Convergence, cybercba::PrologueStage::Epilogue));
    EXPECT_TRUE(cybercba::MissionSystem::complete(progress));
    EXPECT_TRUE(progress.neometroUnlocked);
}
TEST(GameSession, CheckpointRestoresMissionAndPlayerState)
{
    cybercba::GameSession session;
    ASSERT_TRUE(session.startPrologue(cybercba::CharacterId::Magga));
    ASSERT_TRUE(cybercba::MissionSystem::advance(
        session.campaign(), cybercba::PrologueStage::Shelter, cybercba::PrologueStage::Transmission, "transmission"));
    session.player().health = 5.0F;
    session.player().stamina = 10.0F;
    EXPECT_TRUE(session.resetToCheckpoint());
    EXPECT_EQ(session.campaign().stage, cybercba::PrologueStage::Transmission);
    EXPECT_EQ(session.player().health, 100.0F);
    EXPECT_EQ(session.player().stamina, 100.0F);
}
TEST(WorldLayout, DefinesUsableShelterAndExteriorProps)
{
    ASSERT_EQ(cybercba::shelterProps().size(), 8U);
    ASSERT_EQ(cybercba::exteriorProps().size(), 6U);
    const auto* transmitter = cybercba::worldProp("safehouse_transmitter");
    ASSERT_NE(transmitter, nullptr);
    EXPECT_STREQ(transmitter->assetId, "prop.shelter.luciernaga_transmitter");
    EXPECT_STREQ(transmitter->interactionId, "inspect_transmitter");
    EXPECT_NE(cybercba::worldProp("neometro_entrance"), nullptr);
}
TEST(WorldLayout, HasValidSpawnsAndFurnitureCollision)
{
    EXPECT_TRUE(cybercba::isValidSpawn(cybercba::WorldZone::Shelter, 300.0F, 470.0F));
    EXPECT_FALSE(cybercba::isValidSpawn(cybercba::WorldZone::Shelter, 200.0F, 255.0F));
    EXPECT_TRUE(cybercba::isValidSpawn(cybercba::WorldZone::Exterior, 350.0F, 540.0F));
    EXPECT_TRUE(cybercba::blocksMovement(cybercba::WorldZone::Exterior, 520.0F, 470.0F));
}
TEST(SaveService, PersistsAccessibilityAndExtendedAudioSettings)
{
    auto path = (std::filesystem::temp_directory_path() / "cybercba-settings-save-test.txt").string();
    std::filesystem::remove(path);
    cybercba::GameSession saved;
    saved.startNewGame();
    saved.accessibility().reducedMotion = true;
    saved.accessibility().subtitles = false;
    saved.accessibility().dialogueSpeed = 1.25F;
    saved.audio().ambienceVolume = 0.4F;
    saved.audio().dialogueVolume = 0.6F;
    cybercba::SaveService service(path);
    ASSERT_TRUE(service.save(saved));
    cybercba::GameSession loaded;
    ASSERT_EQ(service.load(loaded), cybercba::SaveLoadStatus::Loaded);
    EXPECT_TRUE(loaded.accessibility().reducedMotion);
    EXPECT_FALSE(loaded.accessibility().subtitles);
    EXPECT_FLOAT_EQ(loaded.accessibility().dialogueSpeed, 1.25F);
    EXPECT_FLOAT_EQ(loaded.audio().ambienceVolume, 0.4F);
    EXPECT_FLOAT_EQ(loaded.audio().dialogueVolume, 0.6F);
    std::filesystem::remove(path);
}
TEST(SaveService, PersistsExtendedNarrativeState)
{
    auto path = (std::filesystem::temp_directory_path() / "cybercba-narrative-save-test.txt").string();
    std::filesystem::remove(path);
    cybercba::GameSession saved;
    saved.startNewGame();
    saved.narrative().trust = 3;
    saved.narrative().caution = 2;
    saved.narrative().attachment = 4;
    cybercba::SaveService service(path);
    ASSERT_TRUE(service.save(saved));
    cybercba::GameSession loaded;
    ASSERT_EQ(service.load(loaded), cybercba::SaveLoadStatus::Loaded);
    EXPECT_EQ(loaded.narrative().trust, 3);
    EXPECT_EQ(loaded.narrative().caution, 2);
    EXPECT_EQ(loaded.narrative().attachment, 4);
    std::filesystem::remove(path);
}
TEST(SaveService, PersistsMissionProgressAndDiscoveredEvidence)
{
    auto path = (std::filesystem::temp_directory_path() / "cybercba-mission-save-test.txt").string();
    std::filesystem::remove(path);
    cybercba::GameSession saved;
    ASSERT_TRUE(saved.startPrologue(cybercba::CharacterId::Emma));
    ASSERT_TRUE(saved.missionGraph().advanceTo("workshop"));
    saved.missionGraph().setFlag("power_restored");
    ASSERT_TRUE(saved.evidenceJournal().discover("insignia"));
    cybercba::SaveService service(path);
    ASSERT_TRUE(service.save(saved));

    cybercba::GameSession loaded;
    ASSERT_EQ(service.load(loaded), cybercba::SaveLoadStatus::Loaded);
    EXPECT_EQ(loaded.missionGraph().current(), "workshop");
    EXPECT_TRUE(loaded.missionGraph().flag("power_restored"));
    EXPECT_TRUE(loaded.evidenceJournal().isDiscovered("insignia"));
    std::filesystem::remove(path);
}
TEST(SaveService, MigratesVersion4SavesWithoutMissionOrEvidenceState)
{
    // Version-4 saves predate mission_node/mission_flags/evidence; loading one
    // must default to the prologue's starting node with nothing discovered.
    auto path = (std::filesystem::temp_directory_path() / "cybercba-v4-migration-test.txt").string();
    std::ofstream file(path, std::ios::trunc);
    file << "version=4\nprofile=default\ncredits=10\nhas_save=1\ntutorial=0\nqueue_completed=0\nbest_accuracy=0\n"
            "unlocked=0\nreduced_motion=0\nscanlines=1\nsubtitles=1\nhigh_contrast=0\nreduce_flashes=0\n"
            "screen_shake=1\npersistent_prompts=0\nui_scale=1\ndialogue_speed=1\nmuted=0\nmaster=1\nmusic=1\n"
            "ambience=1\ndialogue=1\neffects=1\ncharacter=1\nprologue=1\nobjective=0\nstage=0\n"
            "prologue_started=1\nprologue_completed=0\ncheckpoint_reached=0\nneometro_unlocked=0\n"
            "checkpoint=shelter\nhealth=100\nstamina=100\ntrust=1\nhurt=1\ntruth=0\ncaution=0\nattachment=0\n";
    file.close();
    cybercba::SaveService service(path);
    cybercba::GameSession loaded;
    ASSERT_EQ(service.load(loaded), cybercba::SaveLoadStatus::Loaded);
    EXPECT_EQ(loaded.missionGraph().current(), "refuge");
    EXPECT_EQ(loaded.evidenceJournal().discoveredCount(), 0U);
    std::filesystem::remove(path);
}
TEST(SaveService, MigratesVersion3SavesWithDefaultNarrativeFields)
{
    // Version-3 saves predate `caution`/`attachment`; loading one must not fail
    // and must default the new fields to zero instead of requiring a re-save.
    auto path = (std::filesystem::temp_directory_path() / "cybercba-v3-migration-test.txt").string();
    std::ofstream file(path, std::ios::trunc);
    file << "version=3\nprofile=default\ncredits=10\nhas_save=1\ntutorial=0\nqueue_completed=0\nbest_accuracy=0\n"
            "unlocked=0\nreduced_motion=0\nscanlines=1\nsubtitles=1\nhigh_contrast=0\nreduce_flashes=0\n"
            "screen_shake=1\npersistent_prompts=0\nui_scale=1\ndialogue_speed=1\nmuted=0\nmaster=1\nmusic=1\n"
            "ambience=1\ndialogue=1\neffects=1\ncharacter=1\nprologue=1\nobjective=0\nstage=0\n"
            "prologue_started=1\nprologue_completed=0\ncheckpoint_reached=0\nneometro_unlocked=0\n"
            "checkpoint=shelter\nhealth=100\nstamina=100\ntrust=1\nhurt=1\ntruth=0\n";
    file.close();
    cybercba::SaveService service(path);
    cybercba::GameSession loaded;
    ASSERT_EQ(service.load(loaded), cybercba::SaveLoadStatus::Loaded);
    EXPECT_EQ(loaded.narrative().trust, 1);
    EXPECT_EQ(loaded.narrative().caution, 0);
    EXPECT_EQ(loaded.narrative().attachment, 0);
    std::filesystem::remove(path);
}
