#pragma once

#include <SFML/Audio/SoundEffect.hpp>

namespace sf
{
    class SFML_AUDIO_API ReverbEffect final : public SoundEffect
    {
    public:
        ReverbEffect();
        ReverbEffect(const ReverbEffect& copy);

        //0-1
        void setDensity(float density);

        //0-1
        void setDiffusion(float);

        //0-1
        void setGain(float);

        //0.1 - 20 (seconds)
        void setDecayTime(float);

        //0-1
        void setReflectionGain(float);

        //0-0.3
        void setReflectionDelay(float);

        //0-1
        void setLateReverbGain(float);

        //0-0.1
        void setLateReverbDelay(float);

        //0-10
        void setRoomRolloff(float);


        float getDensity() const;
        float getDiffusion() const;
        float getGain() const;
        float getDecayTime() const;
        float getReflectionGain() const;
        float getReflectionDelay() const;
        float getLateReverbGain() const;
        float getLateReverbDelay() const;
        float getRoomRolloff() const;

    private:
        float m_density;
        float m_diffusion;
        float m_gain;
        float m_decayTime;
        float m_reflectionGain;
        float m_reflectionDelay;
        float m_lateReverbGain;
        float m_lateReverbDelay;
        float m_roomRolloff;

        //OpenAL handle if setType() is successful
        std::uint32_t m_effect; 

        void applyParameterNames();
    };
}
