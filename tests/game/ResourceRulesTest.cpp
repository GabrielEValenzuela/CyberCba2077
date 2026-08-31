#include "cybercba/game/ResourceRules/ReglaDeBonusDeCobertura.hpp"
#include "cybercba/game/ResourceRules/ReglaDeCargaEMP.hpp"
#include "cybercba/game/ResourceRules/ResourceRulesEngine.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

#include "cybercba/game/CampaignState.hpp"
#include "cybercba/structures/DynamicArray.hpp"

namespace cybercba::game
{
namespace
{

using structures::DynamicArray;

void agregarRegla(DynamicArray<const IResourceRule*>& reglas, const IResourceRule& regla)
{
    reglas.pushBack(&regla);
}

TEST(ResourceTest, EnrutaConsultaDeConsumoHaciaLaReglaCorrecta)
{
    ReglaDeCargaEMP reglaEmp;
    ReglaDeBonusDeCobertura reglaCover;

    DynamicArray<const IResourceRule*> reglas;
    agregarRegla(reglas, reglaEmp);
    agregarRegla(reglas, reglaCover);

    CampaignState state{};
    state.empCharges = 1;
    state.coverBonus = 0;

    const ResourceRulesEngine engine(&state, reglas);

    EXPECT_TRUE(engine.consultar(ResourceType::EmpCharge, 1));
    EXPECT_FALSE(engine.consultar(ResourceType::CoverBonus, 1));
}

TEST(ResourceTest, PermiteConsumirYOtorgarCantidadesValidas)
{
    ReglaDeCargaEMP reglaEmp;
    ReglaDeBonusDeCobertura reglaCover;

    DynamicArray<const IResourceRule*> reglas;
    agregarRegla(reglas, reglaEmp);
    agregarRegla(reglas, reglaCover);

    CampaignState state{};
    state.empCharges = 1;
    state.coverBonus = 0;

    const ResourceRulesEngine engine(&state, reglas);

    EXPECT_TRUE(engine.consultar(ResourceType::EmpCharge, 1));
    EXPECT_TRUE(engine.consultar(ResourceType::CoverBonus, -1));
}

TEST(ResourceTest, RechazaCantidadCero)
{
    ReglaDeCargaEMP reglaEmp;
    ReglaDeBonusDeCobertura reglaCover;

    DynamicArray<const IResourceRule*> reglas;
    agregarRegla(reglas, reglaEmp);
    agregarRegla(reglas, reglaCover);

    CampaignState state{};
    const ResourceRulesEngine engine(&state, reglas);

    EXPECT_FALSE(engine.consultar(ResourceType::EmpCharge, 0));
    EXPECT_FALSE(engine.consultar(ResourceType::CoverBonus, 0));
}

TEST(ResourceTest, RechazaSignoIncorrectoEnLasReglas)
{
    ReglaDeCargaEMP reglaEmp;
    ReglaDeBonusDeCobertura reglaCover;

    CampaignState state{};
    state.empCharges = 1;
    state.coverBonus = 1;

    EXPECT_FALSE(reglaEmp.puedeConsumir(state, -1));
    EXPECT_FALSE(reglaEmp.puedeOtorgar(state, 1));
    EXPECT_FALSE(reglaCover.puedeConsumir(state, -1));
    EXPECT_FALSE(reglaCover.puedeOtorgar(state, 1));
}

TEST(ResourceTest, RechazaConsumoSiLaCantidadEsInsuficiente)
{
    ReglaDeCargaEMP reglaEmp;
    ReglaDeBonusDeCobertura reglaCover;

    DynamicArray<const IResourceRule*> reglas;
    agregarRegla(reglas, reglaEmp);
    agregarRegla(reglas, reglaCover);

    CampaignState state{};
    state.empCharges = 1;
    state.coverBonus = 0;

    const ResourceRulesEngine engine(&state, reglas);

    EXPECT_FALSE(engine.consultar(ResourceType::EmpCharge, 2));
    EXPECT_FALSE(engine.consultar(ResourceType::CoverBonus, 1));
}

TEST(ResourceTest, RechazaConsultaSiEstadoEsNulo)
{
    ReglaDeCargaEMP reglaEmp;
    DynamicArray<const IResourceRule*> reglas;
    agregarRegla(reglas, reglaEmp);

    const ResourceRulesEngine engine(nullptr, reglas);

    EXPECT_FALSE(engine.consultar(ResourceType::EmpCharge, 1));
}

TEST(ResourceTest, RechazaConsultaSiRecursoNoTieneReglaRegistrada)
{
    ReglaDeCargaEMP reglaEmp;
    DynamicArray<const IResourceRule*> reglas;
    agregarRegla(reglas, reglaEmp);

    CampaignState state{};
    state.coverBonus = 2;

    const ResourceRulesEngine engine(&state, reglas);

    EXPECT_FALSE(engine.consultar(ResourceType::CoverBonus, 1));
}

TEST(ResourceTest, CadenaDeDependenciasSeEvaluaEnOrden)
{
    ReglaDeCargaEMP reglaEmp;
    ReglaDeBonusDeCobertura reglaCover(&reglaEmp);

    CampaignState state{};
    state.empCharges = 0;
    state.coverBonus = 1;

    EXPECT_FALSE(reglaCover.puedeConsumir(state, 1));

    state.empCharges = 1;
    EXPECT_TRUE(reglaCover.puedeConsumir(state, 1));
}

class ReglaDePrueba final : public IResourceRule
{
  public:
    explicit ReglaDePrueba(bool bAcepta = true) : m_bAcepta(bAcepta) {}

    ResourceType tipo() const override
    {
        return ResourceType::EmpCharge;
    }

    bool puedeConsumir(const CampaignState& /*state*/, int /*cantidad*/) const override
    {
        return m_bAcepta;
    }

    bool puedeOtorgar(const CampaignState& /*state*/, int /*cantidad*/) const override
    {
        return m_bAcepta;
    }

    const IResourceRule* reglaDeLaQueDepende() const override
    {
        return m_pDependencia;
    }

    void setDependencia(const IResourceRule* pDependencia)
    {
        m_pDependencia = pDependencia;
    }

  private:
    bool m_bAcepta;
    const IResourceRule* m_pDependencia = nullptr;
};

TEST(ResourceTest, ConsultaExigeQueTodasLasReglasDelRecursoSeCumplan)
{
    ReglaDeCargaEMP reglaEmp;
    ReglaDePrueba reglaExtra(false);

    DynamicArray<const IResourceRule*> reglas;
    agregarRegla(reglas, reglaEmp);
    agregarRegla(reglas, reglaExtra);

    CampaignState state{};
    state.empCharges = 1;

    const ResourceRulesEngine engine(&state, reglas);

    EXPECT_FALSE(engine.consultar(ResourceType::EmpCharge, 1));

    DynamicArray<const IResourceRule*> reglasSoloEmp;
    agregarRegla(reglasSoloEmp, reglaEmp);
    const ResourceRulesEngine engineSoloEmp(&state, reglasSoloEmp);
    EXPECT_TRUE(engineSoloEmp.consultar(ResourceType::EmpCharge, 1));
}

TEST(ResourceTest, CadenaProfundaPropagaFallaEnOrden)
{
    ReglaDePrueba reglaA(true);
    ReglaDePrueba reglaB(true);
    ReglaDePrueba reglaC(false);

    reglaA.setDependencia(&reglaB);
    reglaB.setDependencia(&reglaC);

    DynamicArray<const IResourceRule*> reglas;
    agregarRegla(reglas, reglaA);

    CampaignState state{};
    const ResourceRulesEngine engine(&state, reglas);

    EXPECT_FALSE(engine.consultar(ResourceType::EmpCharge, 1));
}

TEST(ResourceTest, DependenciaCircularSeCortaConSeguridad)
{
    ReglaDePrueba reglaA;
    ReglaDePrueba reglaB;
    reglaA.setDependencia(&reglaB);
    reglaB.setDependencia(&reglaA);

    DynamicArray<const IResourceRule*> reglasCiclo;
    agregarRegla(reglasCiclo, reglaA);
    agregarRegla(reglasCiclo, reglaB);

    CampaignState state{};

    EXPECT_THROW({ ResourceRulesEngine engine(&state, reglasCiclo); }, std::invalid_argument);

    ReglaDePrueba reglaSelf;
    reglaSelf.setDependencia(&reglaSelf);

    DynamicArray<const IResourceRule*> reglasSelf;
    agregarRegla(reglasSelf, reglaSelf);
    EXPECT_THROW({ ResourceRulesEngine engine(&state, reglasSelf); }, std::invalid_argument);
}

TEST(ResourceTest, ReglasIndependientesNoSeAfectanEntreSi)
{
    ReglaDeCargaEMP reglaEmp;
    ReglaDeBonusDeCobertura reglaCover;

    CampaignState state{};
    state.empCharges = 0;
    state.coverBonus = 1;

    EXPECT_FALSE(reglaEmp.puedeConsumir(state, 1));
    EXPECT_TRUE(reglaCover.puedeConsumir(state, 1));

    state.empCharges = 1;
    state.coverBonus = 0;
    EXPECT_TRUE(reglaEmp.puedeConsumir(state, 1));
    EXPECT_FALSE(reglaCover.puedeConsumir(state, 1));
}

} // namespace
} // namespace cybercba::game
