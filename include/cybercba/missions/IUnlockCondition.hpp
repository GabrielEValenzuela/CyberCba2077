#ifndef CYBERCBA_MISSIONS_IUNLOCKCONDITION_H
#define CYBERCBA_MISSIONS_IUNLOCKCONDITION_H
#include "PlayerProgress.hpp"
#include <string>

namespace cybercba::missions
{
class IUnlockCondition
{
  public:
    IUnlockCondition();
    virtual ~IUnlockCondition()                                       = default;
    virtual bool estaSatisfecha(const PlayerProgress& progreso) const = 0;
};

class RequisitoDeMisionPrevia final : public IUnlockCondition
{
  public:
    RequisitoDeMisionPrevia(int idMision);
    bool estaSatisfecha(const PlayerProgress& progreso) const override;

  private:
    int m_idMision;
};

class RequisitoDeFlag final : public IUnlockCondition
{
  public:
    RequisitoDeFlag(const std::string& flag);
    bool estaSatisfecha(const PlayerProgress& progreso) const override;

  private:
    std::string m_flag;
};
} // namespace cybercba::missions

#endif // CYBERCBA_MISSIONS_IUNLOCKCONDITION_H
