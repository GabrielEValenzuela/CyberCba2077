#ifndef CYBERCBA_MISSIONS_IUNLOCKCONDITION_H
#define CYBERCBA_MISSIONS_IUNLOCKCONDITION_H
#include "PlayerProgress.hpp"

namespace cybercba::missions
{
class IUnlockCondition
{
  public:
    IUnlockCondition();
    virtual ~IUnlockCondition() = default;
    virtual bool estaSatisfecha (const PlayerProgress& progreso) const = 0;

  protected:
    IUnlockCondition() = default;
};

class RequisitoDeMisionPrevia final : public IUnlockCondition
{
  public:
    bool estaSatisfecha(const PlayerProgress& progreso) const override;
};



class RequisitoDeFlag final : public IUnlockCondition
{
  public:
    bool estaSatisfecha(const PlayerProgress& progreso) const override;
};
}

#endif // CYBERCBA_MISSIONS_IUNLOCKCONDITION_H
