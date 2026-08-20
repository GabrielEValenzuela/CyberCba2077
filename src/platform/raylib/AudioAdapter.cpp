#include "cybercba/platform/AudioAdapter.hpp"

#include <raylib.h>

namespace cybercba::platform
{

AudioAdapter::AudioAdapter()
{
    InitAudioDevice();
}

AudioAdapter::~AudioAdapter()
{
    CloseAudioDevice();
}

} // namespace cybercba::platform
