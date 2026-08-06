#pragma once
#include <string>
#include "raylib.h"
class AudioService { public: AudioService(); ~AudioService(); AudioService(const AudioService&)=delete; AudioService& operator=(const AudioService&)=delete; void setMuted(bool muted); void setVolumes(float music,float effects); void playUiConfirm(); void playUiCancel(); void playSuccess(); bool available()const; private: Sound m_confirm{}; Sound m_cancel{}; Sound m_success{}; bool m_ready{false}; bool m_muted{false}; float m_effects{0.8F}; };
