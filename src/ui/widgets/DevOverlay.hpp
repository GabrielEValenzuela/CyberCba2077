#ifndef CYBER_CBA_DEV_OVERLAY_HPP
#define CYBER_CBA_DEV_OVERLAY_HPP

/**
 * @file DevOverlay.hpp
 * @brief Superposición de telemetría exclusiva para desarrollo.
 * @details Muestrea FPS con raylib y recursos del host Linux sin intervenir en la lógica de juego.
 */
class DevOverlay
{
public:
    /** @brief Inicializa la telemetría sin tomar muestras todavía. */
    DevOverlay();
    /** @brief Actualiza las muestras del sistema. @param dt Delta time en segundos. */
    void update(float dt);
    /** @brief Dibuja FPS, CPU, GPU y memoria. @pre Debe llamarse entre BeginDrawing y EndDrawing. */
    void draw() const;

private:
    void sampleCpu();
    void sampleMemory();
    void sampleGpu();

    float m_elapsedSeconds;
    float m_cpuPercent;
    float m_memoryPercent;
    int m_gpuPercent;
    unsigned long long m_previousCpuIdle;
    unsigned long long m_previousCpuTotal;
    bool m_hasCpuSample;
};

#endif // CYBER_CBA_DEV_OVERLAY_HPP
