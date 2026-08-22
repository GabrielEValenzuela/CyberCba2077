#ifndef CYBERCBA_PLATFORM_AUDIOADAPTER_H
#define CYBERCBA_PLATFORM_AUDIOADAPTER_H

namespace cybercba::platform
{

/// Owns the raylib audio device lifecycle. No SoundId is wired
// up yet (VS-001 §13 does not list audio assets) — this exists so the
// device init/shutdown pairing lives in exactly one place when sounds are
// added, instead of being bolted onto AssetStore later.
class AudioAdapter final
{
  public:
    AudioAdapter();
    ~AudioAdapter();

    AudioAdapter(const AudioAdapter&)            = delete;
    AudioAdapter& operator=(const AudioAdapter&) = delete;
    AudioAdapter(AudioAdapter&&)                 = delete;
    AudioAdapter& operator=(AudioAdapter&&)      = delete;
};

} // namespace cybercba::platform

#endif // CYBERCBA_PLATFORM_AUDIOADAPTER_H
