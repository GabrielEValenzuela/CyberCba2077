# ADR 0001: SceneStack

Se adopta `SceneStack` con contratos explicitos para actualizacion y dibujo de escenas inferiores. Los overlays pueden dibujar o actualizar por debajo de acuerdo con su contrato. Se evita concentrar dominio, input y recursos en SceneManager.
