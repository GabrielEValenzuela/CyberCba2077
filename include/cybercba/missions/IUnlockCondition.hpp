#ifndef CYBERCBA2077_IUNLOCKCONDITION_HPP
#define CYBERCBA2077_IUNLOCKCONDITION_HPP
#include "PlayerProgress.hpp"

namespace cybercba::missions {

    class IUnlockCondition
    {
        public:
            virtual ~IUnlockCondition() = default;
            virtual bool estaSatisfecha (const PlayerProgress& progreso) const = 0;



        protected:
            IUnlockCondition() = default;
    };

    class RequisitoDeMisionPrevia final : public IUnlockCondition {
        public:
            bool estaSatisfecha(const PlayerProgress& progreso) const override;
    };



    class RequisitoDeFlag final : public IUnlockCondition {
        public:
            bool estaSatisfecha(const PlayerProgress& progreso) const override;
    };
}


#endif // CYBERCBA2077_IUNLOCKCONDITION_HPP
