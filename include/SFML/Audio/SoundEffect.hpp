#ifndef SFML_SOUNDEFFECT_HPP
#define SFML_SOUNDEFFECT_HPP

#include <SFML/Audio/Export.hpp>
#include <SFML/Audio/AlResource.hpp>

#include <set>

namespace sf
{
    class SoundSource;

    class SFML_AUDIO_API SoundEffect : AlResource
    {
    public:

        //returns true if effects are available on this platform
        static bool isAvailable();

        enum Type
        {
            Null,
            Reverb,
            Chorus
        };

        Type getType() const;

        //range 0 - 1
        void setVolume(float volume);

        float getVolume() const;

    protected:
        //used by concrete types to define what type of effect this
        //will be. Those types are responsible for exposing effect
        //specific interfaces for updating properties. Returns the effect handle
        std::uint32_t setType(Type type);

        //reapplies the effect on this slot after the parameters were changed
        void applyEffect();

        SoundEffect();
        SoundEffect(const SoundEffect& copy);

        virtual ~SoundEffect();

    private:
        friend class SoundSource;

        //allows tracking of sound sources using this effect
        void attachSoundSource(SoundSource* source) const;
        void detachSoundSource(SoundSource* source) const;

        std::uint32_t m_effectSlot;
        std::uint32_t m_effect;

        Type m_type;
        float m_volume;

        mutable std::set<SoundSource*> m_soundlist;
        void ensureEffect(Type);
    };
}

#endif // SFML_SOUNDEFFECT_HPP
