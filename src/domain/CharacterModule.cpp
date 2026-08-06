#include "cybercba/CharacterModule.hpp"

namespace cybercba
{

    void CharacterRoster::add(CharacterModulePtr module) { m_modules.push_back(std::move(module)); }

    const std::vector<CharacterModulePtr>& CharacterRoster::modules() const { return m_modules; }

    const ICharacterModule* CharacterRoster::find(const std::string& id) const
    {
        for (const auto& module : m_modules)
            if (module->id() == id)
                return module.get();
        return nullptr;
    }

} // namespace cybercba
