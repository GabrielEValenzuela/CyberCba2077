#include "ui/widgets/DevOverlay.hpp"

#include <cstdio>
#include <fstream>
#include <string>

#include "raylib.h"
#include "ui/NlmColors.hpp"
#include "ui/widgets/NlmPanel.hpp"
#include "ui/widgets/NlmText.hpp"

namespace
{
    constexpr float SAMPLE_INTERVAL_SECONDS = 0.5F;
    constexpr int UNKNOWN_GPU_PERCENT = -1;
} // namespace

DevOverlay::DevOverlay()
    : m_elapsedSeconds(SAMPLE_INTERVAL_SECONDS)
    , m_cpuPercent(0.0F)
    , m_memoryPercent(0.0F)
    , m_gpuPercent(UNKNOWN_GPU_PERCENT)
    , m_previousCpuIdle(0)
    , m_previousCpuTotal(0)
    , m_hasCpuSample(false)
{
}

void DevOverlay::update(float dt)
{
    m_elapsedSeconds += dt;
    if (m_elapsedSeconds < SAMPLE_INTERVAL_SECONDS)
    {
        return;
    }
    m_elapsedSeconds = 0.0F;
    sampleCpu();
    sampleMemory();
    sampleGpu();
}

void DevOverlay::draw() const
{
    NlmPanel::draw(Rectangle {28.0F, 28.0F, 235.0F, 138.0F}, NlmColors::AMBER);
    char line[64] {};
    NlmText::draw("DEV TELEMETRY", 44, 42, 16, NlmColors::AMBER);
    std::snprintf(line, sizeof(line), "FPS: %d", GetFPS());
    NlmText::draw(line, 44, 66, 16, NlmColors::TEXT);
    std::snprintf(line, sizeof(line), "CPU: %.1f%%", m_cpuPercent);
    NlmText::draw(line, 44, 88, 16, NlmColors::TEXT);
    std::snprintf(line, sizeof(line), "MEM: %.1f%%", m_memoryPercent);
    NlmText::draw(line, 44, 110, 16, NlmColors::TEXT);
    if (m_gpuPercent == UNKNOWN_GPU_PERCENT)
    {
        NlmText::draw("GPU: unavailable", 44, 132, 16, NlmColors::TEXT_DIM);
        return;
    }
    std::snprintf(line, sizeof(line), "GPU: %d%%", m_gpuPercent);
    NlmText::draw(line, 44, 132, 16, NlmColors::TEXT);
}

void DevOverlay::sampleCpu()
{
    std::ifstream file("/proc/stat");
    std::string label;
    unsigned long long user = 0;
    unsigned long long nice = 0;
    unsigned long long system = 0;
    unsigned long long idle = 0;
    unsigned long long ioWait = 0;
    if (!(file >> label >> user >> nice >> system >> idle >> ioWait) || label != "cpu")
    {
        return;
    }
    const unsigned long long total = user + nice + system + idle + ioWait;
    const unsigned long long idleTotal = idle + ioWait;
    if (m_hasCpuSample && total > m_previousCpuTotal)
    {
        const unsigned long long totalDelta = total - m_previousCpuTotal;
        const unsigned long long idleDelta = idleTotal - m_previousCpuIdle;
        m_cpuPercent = 100.0F * static_cast<float>(totalDelta - idleDelta) / static_cast<float>(totalDelta);
    }
    m_previousCpuTotal = total;
    m_previousCpuIdle = idleTotal;
    m_hasCpuSample = true;
}

void DevOverlay::sampleMemory()
{
    std::ifstream file("/proc/meminfo");
    std::string label;
    unsigned long long value = 0;
    std::string unit;
    unsigned long long total = 0;
    unsigned long long available = 0;
    while (file >> label >> value >> unit)
    {
        if (label == "MemTotal:")
        {
            total = value;
        }
        else if (label == "MemAvailable:")
        {
            available = value;
        }
    }
    if (total > 0 && available <= total)
    {
        m_memoryPercent = 100.0F * static_cast<float>(total - available) / static_cast<float>(total);
    }
}

void DevOverlay::sampleGpu()
{
    std::ifstream file("/sys/class/drm/card0/device/gpu_busy_percent");
    int value = UNKNOWN_GPU_PERCENT;
    if (file >> value && value >= 0 && value <= 100)
    {
        m_gpuPercent = value;
    }
    else
    {
        m_gpuPercent = UNKNOWN_GPU_PERCENT;
    }
}
