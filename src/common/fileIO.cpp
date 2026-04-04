#include "common/fileIO.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

namespace CyberpunkCba
{
    namespace FileIO
    {

        // =============================================================================
        // Helpers internos — no expuestos en el header
        // =============================================================================

        namespace
        {

            // Separador de campos en todos los archivos
            constexpr char SEPARATOR {'|'};

            // Abre un archivo para lectura. Lanza FileIOException si existe pero falla.
            // Retorna false (sin lanzar) si el archivo simplemente no existe.
            bool openForRead(const std::filesystem::path& path, std::ifstream& file)
            {
                if (!std::filesystem::exists(path))
                {
                    return false;
                }
                file.open(path);
                if (!file.is_open())
                {
                    throw FileIOException {path, "cannot open for reading"};
                }
                return true;
            }

            // Abre un archivo para escritura en modo append, creando si no existe.
            void openForAppend(const std::filesystem::path& path, std::ofstream& file)
            {
                // Crear directorios intermedios si no existen
                if (path.has_parent_path())
                {
                    std::filesystem::create_directories(path.parent_path());
                }
                file.open(path, std::ios::app);
                if (!file.is_open())
                {
                    throw FileIOException {path, "cannot open for appending"};
                }
            }

            // Abre un archivo para escritura, sobreescribiendo si existe.
            void openForWrite(const std::filesystem::path& path, std::ofstream& file)
            {
                if (path.has_parent_path())
                {
                    std::filesystem::create_directories(path.parent_path());
                }
                file.open(path, std::ios::trunc);
                if (!file.is_open())
                {
                    throw FileIOException {path, "cannot open for writing"};
                }
            }

            // Extrae el siguiente campo delimitado por SEPARATOR de un stringstream.
            // Retorna false si no hay más campos.
            bool nextField(std::istringstream& ss, std::string& out)
            {
                return static_cast<bool>(std::getline(ss, out, SEPARATOR));
            }

            // Convierte string a int. Retorna false si la conversión falla.
            bool toInt(const std::string& s, int& out)
            {
                try
                {
                    std::size_t pos {};
                    out = std::stoi(s, &pos);
                    return pos == s.size(); // todos los caracteres fueron consumidos
                }
                catch (...)
                {
                    return false;
                }
            }

            // Convierte string a bool. Acepta "true"/"false" case-insensitive.
            bool toBool(const std::string& s, bool& out)
            {
                auto lower {s};
                for (auto& c : lower)
                {
                    c = static_cast<char>(std::tolower(c));
                }
                if (lower == "true")
                {
                    out = true;
                    return true;
                }
                if (lower == "false")
                {
                    out = false;
                    return true;
                }
                return false;
            }

            // Convierte string a ItemType. Retorna false si no es válido.
            bool toItemType(const std::string& s, ItemType& out)
            {
                if (s == "Weapon")
                {
                    out = ItemType::Weapon;
                    return true;
                }
                if (s == "Consumable")
                {
                    out = ItemType::Consumable;
                    return true;
                }
                if (s == "Value")
                {
                    out = ItemType::Value;
                    return true;
                }
                if (s == "Tech")
                {
                    out = ItemType::Tech;
                    return true;
                }
                return false;
            }

            // Convierte string a MissionStatus. Retorna false si no es válido.
            bool toMissionStatus(const std::string& s, MissionStatus& out)
            {
                if (s == "Pending")
                {
                    out = MissionStatus::Pending;
                    return true;
                }
                if (s == "InProgress")
                {
                    out = MissionStatus::InProgress;
                    return true;
                }
                if (s == "Completed")
                {
                    out = MissionStatus::Completed;
                    return true;
                }
                if (s == "Failed")
                {
                    out = MissionStatus::Failed;
                    return true;
                }
                return false;
            }

            // Convierte string a EntityDisposition. Retorna false si no es válido.
            bool toDisposition(const std::string& s, EntityDisposition& out)
            {
                if (s == "Friendly")
                {
                    out = EntityDisposition::Friendly;
                    return true;
                }
                if (s == "Neutral")
                {
                    out = EntityDisposition::Neutral;
                    return true;
                }
                if (s == "Hostile")
                {
                    out = EntityDisposition::Hostile;
                    return true;
                }
                return false;
            }

            // Convierte string a Faction. Retorna false si no es válido.
            bool toFaction(const std::string& s, Faction& out)
            {
                if (s == "Corporations")
                {
                    out = Faction::Corporations;
                    return true;
                }
                if (s == "Streeters")
                {
                    out = Faction::Streeters;
                    return true;
                }
                if (s == "NetRunners")
                {
                    out = Faction::NetRunners;
                    return true;
                }
                return false;
            }

            // Imprime un aviso de línea inválida en stderr — no interrumpe la carga.
            void warnInvalidLine(const std::filesystem::path& path, int lineNumber, const std::string& line)
            {
                std::cerr << "[FileIO] " << path.filename().string() << " línea " << lineNumber
                          << ": formato inválido, ignorada → \"" << line << "\"\n";
            }

        } // namespace

        // =============================================================================
        // Funciones de carga
        // =============================================================================

        std::vector<LogEntry> loadLog(const std::filesystem::path& path)
        {
            std::ifstream file;
            if (!openForRead(path, file))
            {
                return {};
            }

            std::vector<LogEntry> result;
            std::string line;
            int lineNumber {};

            while (std::getline(file, line))
            {
                ++lineNumber;

                // Ignorar comentarios y líneas vacías
                if (line.empty() || line.front() == '#')
                {
                    continue;
                }

                std::istringstream ss {line};
                std::string hourStr, minuteStr, message;

                if (!nextField(ss, hourStr) || !nextField(ss, minuteStr) || !nextField(ss, message))
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                int hour {}, minute {};
                if (!toInt(hourStr, hour) || !toInt(minuteStr, minute) || hour < 0 || hour > 23 || minute < 0 ||
                    minute > 59)
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                result.push_back({hour, minute, message});
            }
            return result;
        }

        std::vector<Item> loadInventory(const std::filesystem::path& path)
        {
            std::ifstream file;
            if (!openForRead(path, file))
            {
                return {};
            }

            std::vector<Item> result;
            std::string line;
            int lineNumber {};

            while (std::getline(file, line))
            {
                ++lineNumber;
                if (line.empty() || line.front() == '#')
                {
                    continue;
                }

                std::istringstream ss {line};
                std::string name, typeStr, priceStr, quantityStr;

                if (!nextField(ss, name) || !nextField(ss, typeStr) || !nextField(ss, priceStr) ||
                    !nextField(ss, quantityStr))
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                ItemType type {};
                int price {}, quantity {};

                if (!toItemType(typeStr, type) || !toInt(priceStr, price) || !toInt(quantityStr, quantity) ||
                    price < 0 || quantity < 1)
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                result.push_back({name, type, price, quantity});
            }
            return result;
        }

        std::vector<ZoneEntry> loadMap(const std::filesystem::path& path)
        {
            std::ifstream file;
            if (!openForRead(path, file))
            {
                return {};
            }

            std::vector<ZoneEntry> result;
            std::string line;
            int lineNumber {};

            while (std::getline(file, line))
            {
                ++lineNumber;
                if (line.empty() || line.front() == '#')
                {
                    continue;
                }

                std::istringstream ss {line};
                std::string name, visitedStr, unlockedStr;

                if (!nextField(ss, name) || !nextField(ss, visitedStr) || !nextField(ss, unlockedStr))
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                bool visited {}, unlocked {};
                if (!toBool(visitedStr, visited) || !toBool(unlockedStr, unlocked))
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                result.push_back({name, visited, unlocked});
            }
            return result;
        }

        std::vector<MissionEntry> loadMissions(const std::filesystem::path& path)
        {
            std::ifstream file;
            if (!openForRead(path, file))
            {
                return {};
            }

            std::vector<MissionEntry> result;
            std::string line;
            int lineNumber {};

            while (std::getline(file, line))
            {
                ++lineNumber;
                if (line.empty() || line.front() == '#')
                {
                    continue;
                }

                std::istringstream ss {line};
                std::string name, statusStr, rewardStr, progressStr;

                if (!nextField(ss, name) || !nextField(ss, statusStr) || !nextField(ss, rewardStr) ||
                    !nextField(ss, progressStr))
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                MissionStatus status {};
                int reward {}, progress {};

                if (!toMissionStatus(statusStr, status) || !toInt(rewardStr, reward) || !toInt(progressStr, progress) ||
                    reward < 0 || progress < 0 || progress > 100)
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                result.push_back({name, status, reward, progress});
            }
            return result;
        }

        std::vector<EntityEntry> loadEntities(const std::filesystem::path& path)
        {
            std::ifstream file;
            if (!openForRead(path, file))
            {
                return {};
            }

            std::vector<EntityEntry> result;
            std::string line;
            int lineNumber {};

            while (std::getline(file, line))
            {
                ++lineNumber;
                if (line.empty() || line.front() == '#')
                {
                    continue;
                }

                std::istringstream ss {line};
                std::string name, dispositionStr, distanceStr;

                if (!nextField(ss, name) || !nextField(ss, dispositionStr) || !nextField(ss, distanceStr))
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                EntityDisposition disposition {};
                int distance {};

                if (!toDisposition(dispositionStr, disposition) || !toInt(distanceStr, distance) || distance < 0)
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                result.push_back({name, disposition, distance});
            }
            return result;
        }

        std::vector<RepEntry> loadReputation(const std::filesystem::path& path)
        {
            std::ifstream file;
            if (!openForRead(path, file))
            {
                return {};
            }

            std::vector<RepEntry> result;
            std::string line;
            int lineNumber {};

            while (std::getline(file, line))
            {
                ++lineNumber;
                if (line.empty() || line.front() == '#')
                {
                    continue;
                }

                std::istringstream ss {line};
                std::string factionStr, valueStr, date;

                if (!nextField(ss, factionStr) || !nextField(ss, valueStr) || !nextField(ss, date))
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                Faction faction {};
                int value {};

                if (!toFaction(factionStr, faction) || !toInt(valueStr, value) || value < 0 || value > 100)
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                result.push_back({faction, value, date});
            }
            return result;
        }

        std::vector<Transaction> loadWallet(const std::filesystem::path& path)
        {
            std::ifstream file;
            if (!openForRead(path, file))
            {
                return {};
            }

            std::vector<Transaction> result;
            std::string line;
            int lineNumber {};

            while (std::getline(file, line))
            {
                ++lineNumber;
                if (line.empty() || line.front() == '#')
                {
                    continue;
                }

                std::istringstream ss {line};
                std::string type, amountStr, concept;

                if (!nextField(ss, type) || !nextField(ss, amountStr) || !nextField(ss, concept))
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                int amount {};
                if ((type != "ingreso" && type != "gasto") || !toInt(amountStr, amount) || amount < 0)
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                result.push_back({type, amount, concept});
            }
            return result;
        }

        std::vector<SessionEntry> loadSessions(const std::filesystem::path& path)
        {
            std::ifstream file;
            if (!openForRead(path, file))
            {
                return {};
            }

            std::vector<SessionEntry> result;
            std::string line;
            int lineNumber {};

            while (std::getline(file, line))
            {
                ++lineNumber;
                if (line.empty() || line.front() == '#')
                {
                    continue;
                }

                std::istringstream ss {line};
                std::string date, durationStr, commandsStr;

                if (!nextField(ss, date) || !nextField(ss, durationStr) || !nextField(ss, commandsStr))
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                int duration {}, commands {};
                if (!toInt(durationStr, duration) || !toInt(commandsStr, commands) || duration < 0 || commands < 0)
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                result.push_back({date, duration, commands});
            }
            return result;
        }

        std::vector<PurchaseEntry> loadShop(const std::filesystem::path& path)
        {
            std::ifstream file;
            if (!openForRead(path, file))
            {
                return {};
            }

            std::vector<PurchaseEntry> result;
            std::string line;
            int lineNumber {};

            while (std::getline(file, line))
            {
                ++lineNumber;
                if (line.empty() || line.front() == '#')
                {
                    continue;
                }

                std::istringstream ss {line};
                std::string name, priceStr, quantityStr, date;

                if (!nextField(ss, name) || !nextField(ss, priceStr) || !nextField(ss, quantityStr) ||
                    !nextField(ss, date))
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                int price {}, quantity {};
                if (!toInt(priceStr, price) || !toInt(quantityStr, quantity) || price < 0 || quantity < 1)
                {
                    warnInvalidLine(path, lineNumber, line);
                    continue;
                }

                result.push_back({name, price, quantity, date});
            }
            return result;
        }

        // =============================================================================
        // Funciones de escritura
        // =============================================================================

        void appendLog(const LogEntry& entry, const std::filesystem::path& path)
        {
            assert(!entry.message.empty());
            std::ofstream file;
            openForAppend(path, file);
            file << entry.hour << SEPARATOR << entry.minute << SEPARATOR << entry.message << "\n";
        }

        void saveInventory(const std::vector<Item>& inventory, const std::filesystem::path& path)
        {
            std::ofstream file;
            openForWrite(path, file);
            file << "# nombre|tipo|precio|cantidad\n";
            for (const auto& item : inventory)
            {
                file << item.name << SEPARATOR << itemTypeToString(item.type) << SEPARATOR << item.price << SEPARATOR
                     << item.quantity << "\n";
            }
        }

        void appendSession(const SessionEntry& entry, const std::filesystem::path& path)
        {
            assert(entry.durationSeconds >= 0);
            assert(entry.commandCount >= 0);
            std::ofstream file;
            openForAppend(path, file);
            file << entry.date << SEPARATOR << entry.durationSeconds << SEPARATOR << entry.commandCount << "\n";
        }

        void appendEntity(const EntityEntry& entry, const std::filesystem::path& path)
        {
            assert(!entry.name.empty());
            std::ofstream file;
            openForAppend(path, file);
            file << entry.name << SEPARATOR << entityDispositionToString(entry.disposition) << SEPARATOR
                 << entry.distanceMeters << "\n";
        }

        void appendTransaction(const Transaction& entry, const std::filesystem::path& path)
        {
            assert(entry.amount >= 0);
            assert(!entry.concept.empty());
            std::ofstream file;
            openForAppend(path, file);
            file << entry.type << SEPARATOR << entry.amount << SEPARATOR << entry.concept << "\n";
        }

        void appendPurchase(const PurchaseEntry& entry, const std::filesystem::path& path)
        {
            assert(entry.price >= 0);
            assert(entry.quantity >= 1);
            std::ofstream file;
            openForAppend(path, file);
            file << entry.name << SEPARATOR << entry.price << SEPARATOR << entry.quantity << SEPARATOR << entry.date
                 << "\n";
        }

    } // namespace FileIO
} // namespace CyberpunkCba
