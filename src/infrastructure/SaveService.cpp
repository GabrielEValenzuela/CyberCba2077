#include "cybercba/SaveService.hpp"
#include <filesystem>
#include <fstream>
#include <map>

namespace cybercba
{
    namespace
    {
        constexpr int FORMAT_VERSION = 3;
        bool boolValue(const std::map<std::string, std::string>& v, const char* k, bool& output)
        {
            auto it = v.find(k);
            if (it == v.end() || (it->second != "0" && it->second != "1"))
                return false;
            output = it->second == "1";
            return true;
        }
        bool intValue(const std::map<std::string, std::string>& v, const char* k, int& out)
        {
            try
            {
                auto it = v.find(k);
                if (it == v.end())
                    return false;
                out = std::stoi(it->second);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }
        bool floatValue(const std::map<std::string, std::string>& v, const char* k, float& out)
        {
            try
            {
                auto it = v.find(k);
                if (it == v.end())
                    return false;
                out = std::stof(it->second);
                return true;
            }
            catch (...)
            {
                return false;
            }
        }
    } // namespace
    SaveService::SaveService(std::string path)
        : m_path(std::move(path))
    {
    }
    bool SaveService::exists() const
    {
        return std::filesystem::exists(m_path);
    }
    const std::string& SaveService::path() const
    {
        return m_path;
    }
    bool SaveService::save(const GameSession& session, std::string* error) const
    {
        const std::string temporary = m_path + ".tmp";
        std::ofstream file(temporary, std::ios::trunc);
        if (!file)
        {
            if (error)
                *error = "No se pudo crear el archivo temporal.";
            return false;
        }
        const auto& p = session.progress();
        const auto& a = session.accessibility();
        const auto& audio = session.audio();
        const auto& c = session.campaign();
        const auto& player = session.player();
        const auto& n = session.narrative();
        file << "version=" << FORMAT_VERSION << "\nprofile=" << p.profileId << "\ncredits=" << session.model().credits()
             << "\nhas_save=" << p.hasSave << "\ntutorial=" << p.tutorialCompleted
             << "\nqueue_completed=" << p.queueMissionCompleted << "\nbest_accuracy=" << p.bestQueueAccuracy
             << "\nunlocked=" << p.unlockedMission << "\nreduced_motion=" << a.reducedMotion
             << "\nscanlines=" << a.scanlines << "\nsubtitles=" << a.subtitles << "\nhigh_contrast=" << a.highContrast
             << "\nreduce_flashes=" << a.reduceFlashes << "\nscreen_shake=" << a.screenShake
             << "\npersistent_prompts=" << a.persistentPrompts << "\nui_scale=" << a.uiScale
             << "\ndialogue_speed=" << a.dialogueSpeed << "\nmuted=" << audio.muted << "\nmaster=" << audio.masterVolume
             << "\nmusic=" << audio.musicVolume << "\nambience=" << audio.ambienceVolume
             << "\ndialogue=" << audio.dialogueVolume << "\neffects=" << audio.effectsVolume
             << "\ncharacter=" << static_cast<int>(c.selectedCharacter) << "\nprologue=" << static_cast<int>(c.prologue)
             << "\nobjective=" << static_cast<int>(c.objective) << "\nstage=" << static_cast<int>(c.stage)
             << "\nprologue_started=" << c.prologueStarted << "\nprologue_completed=" << c.prologueCompleted
             << "\ncheckpoint_reached=" << c.checkpointReached << "\nneometro_unlocked=" << c.neometroUnlocked
             << "\ncheckpoint=" << c.checkpoint << "\nhealth=" << player.health << "\nstamina=" << player.stamina
             << "\ntrust=" << n.trust << "\nhurt=" << n.unresolvedHurt << "\ntruth=" << n.recoveredTruth << "\n";
        file.close();
        if (!file)
        {
            if (error)
                *error = "No se pudo escribir la partida.";
            return false;
        }
        std::error_code ec;
        std::filesystem::rename(temporary, m_path, ec);
        if (ec)
        {
            std::filesystem::remove(m_path, ec);
            ec.clear();
            std::filesystem::rename(temporary, m_path, ec);
        }
        if (ec)
        {
            if (error)
                *error = "No se pudo reemplazar la partida.";
            return false;
        }
        return true;
    }
    SaveLoadStatus SaveService::load(GameSession& session, std::string* error) const
    {
        std::ifstream file(m_path);
        if (!file)
            return SaveLoadStatus::Missing;
        std::map<std::string, std::string> values;
        std::string line;
        while (std::getline(file, line))
        {
            auto pos = line.find('=');
            if (pos == std::string::npos)
                continue;
            values[line.substr(0, pos)] = line.substr(pos + 1);
        }
        int version = 0, credits = 0, best = 0, unlocked = 0, character = 0, prologue = 0, objective = 0, stage = 0,
            trust = 0, hurt = 0, truth = 0;
        float health = 0, stamina = 0;
        bool has = false, tutorial = false, queue = false, reduced = false, scan = true, subtitles = true, contrast = false,
             flashes = false, shake = true, prompts = false, muted = false, started = false, completed = false, reached = false,
             neometro = false;
        if (!intValue(values, "version", version) || (version != 2 && version != FORMAT_VERSION))
            return version ? SaveLoadStatus::UnsupportedVersion : SaveLoadStatus::Corrupt;
        if (!intValue(values, "credits", credits) || !intValue(values, "best_accuracy", best) ||
            !intValue(values, "unlocked", unlocked) || !boolValue(values, "has_save", has) ||
            !boolValue(values, "tutorial", tutorial) || !boolValue(values, "queue_completed", queue) ||
            !boolValue(values, "reduced_motion", reduced) || !boolValue(values, "scanlines", scan) ||
            !boolValue(values, "muted", muted) || !intValue(values, "character", character) ||
            !intValue(values, "prologue", prologue) || !intValue(values, "objective", objective) ||
            !intValue(values, "stage", stage) || !boolValue(values, "prologue_started", started) ||
            !boolValue(values, "prologue_completed", completed) || !boolValue(values, "checkpoint_reached", reached) ||
            !boolValue(values, "neometro_unlocked", neometro) || !floatValue(values, "health", health) ||
            !floatValue(values, "stamina", stamina) || !intValue(values, "trust", trust) ||
            !intValue(values, "hurt", hurt) || !intValue(values, "truth", truth))
        {
            if (error)
                *error = "Campos de guardado invalidos.";
            return SaveLoadStatus::Corrupt;
        }
        session.startNewGame();
        session.model().addCredits(credits);
        auto& p = session.progress();
        p.hasSave = has;
        p.tutorialCompleted = tutorial;
        p.queueMissionCompleted = queue;
        p.bestQueueAccuracy = best;
        p.unlockedMission = unlocked;
        auto profile = values.find("profile");
        if (profile != values.end())
            p.profileId = profile->second;
        session.accessibility().reducedMotion = reduced;
        session.accessibility().scanlines = scan;
        session.audio().muted = muted;
        try
        {
            session.accessibility().uiScale = std::stof(values.at("ui_scale"));
            session.audio().musicVolume = std::stof(values.at("music"));
            session.audio().effectsVolume = std::stof(values.at("effects"));
            if (version >= 3)
            {
                if (!boolValue(values, "subtitles", subtitles) || !boolValue(values, "high_contrast", contrast) ||
                    !boolValue(values, "reduce_flashes", flashes) || !boolValue(values, "screen_shake", shake) ||
                    !boolValue(values, "persistent_prompts", prompts))
                    return SaveLoadStatus::Corrupt;
                session.accessibility().subtitles = subtitles;
                session.accessibility().highContrast = contrast;
                session.accessibility().reduceFlashes = flashes;
                session.accessibility().screenShake = shake;
                session.accessibility().persistentPrompts = prompts;
                session.accessibility().dialogueSpeed = std::stof(values.at("dialogue_speed"));
                session.audio().masterVolume = std::stof(values.at("master"));
                session.audio().ambienceVolume = std::stof(values.at("ambience"));
                session.audio().dialogueVolume = std::stof(values.at("dialogue"));
            }
        }
        catch (...)
        {
            return SaveLoadStatus::Corrupt;
        }
        if (character < 0 || character > 2 || prologue < 0 || prologue > 4 || objective < 0 || objective > 3 ||
            stage < 0 || stage > 5)
            return SaveLoadStatus::Corrupt;
        auto& c = session.campaign();
        c.selectedCharacter = static_cast<CharacterId>(character);
        c.prologue = static_cast<MissionState>(prologue);
        c.objective = static_cast<ObjectiveState>(objective);
        c.stage = static_cast<PrologueStage>(stage);
        c.prologueStarted = started;
        c.prologueCompleted = completed;
        c.checkpointReached = reached;
        c.neometroUnlocked = neometro;
        c.checkpoint = values.count("checkpoint") ? values.at("checkpoint") : "shelter";
        session.player().health = health;
        session.player().stamina = stamina;
        session.narrative() = {trust, hurt, truth};
        return SaveLoadStatus::Loaded;
    }
} // namespace cybercba
