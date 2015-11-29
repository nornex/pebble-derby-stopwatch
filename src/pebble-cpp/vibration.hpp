//
// Created by matt on 28/11/15.
//

#ifndef PEBBLE_CPP_VIBRATION_HANDLER_HPP
#define PEBBLE_CPP_VIBRATION_HANDLER_HPP

#include "pebble-sdk.hpp"
#include "util/duration.hpp"

namespace pebble
{
    class Vibration
    {
    public:
        ~Vibration()
        {
            ::vibes_cancel();
        }

        void ShortPulse()
        {
            ::vibes_short_pulse();
        }

        void LongPulse()
        {
            ::vibes_long_pulse();
        }

        void DoublePulse()
        {
            ::vibes_double_pulse();
        }
    };

    namespace vibration
    {
        struct Pulse
        {
            Pulse(util::Duration duration) : duration(duration) {}
            const util::Duration duration;
        };

        struct Wait
        {
            Wait(util::Duration duration) : duration(duration) {}
            const util::Duration duration;
        };
    }

    template <uint32_t TNum>
    class CustomVibration : public Vibration
    {
    public:
        typedef vibration::Pulse Pulse;
        typedef vibration::Wait Wait;

        template <class... TPulses>
        void StartSequence(TPulses... pulses)
        {
            static_assert(
                sizeof...(pulses) <= TNum,
                "CustomVibration not large enough for the supplied number of pulses"
            );

            SetValuesPulse(0, pulses...);

            VibePattern pattern = (VibePattern){
                .durations = pulses_,
                .num_segments = static_cast<uint32_t>(
                    // Drop the final Wait if there is one, its pointless
                    sizeof...(pulses) - (sizeof...(pulses) % 2 == 0 ? 1 : 0)
                ),
            };
            ::vibes_enqueue_custom_pattern(pattern);
        }

        template <size_t TCount, class... TPulses>
        void StartRepeatingSequence(TPulses... pulses)
        {
            static_assert(
                sizeof...(pulses) * TCount <= TNum,
                "CustomVibration not large enough for the supplied number of pulses and repeat"
            );

            static_assert(
                sizeof...(pulses) % 2 == 0,
                "Pulse sequence must end in a Wait"
            );

            for (size_t i = 0; i < TCount; i++)
            {
                SetValuesPulse(i * sizeof...(pulses), pulses...);
            }

            VibePattern pattern = (VibePattern){
                .durations = pulses_,
                .num_segments = static_cast<uint32_t>(
                    // Drop the final Wait
                    (sizeof...(pulses) * TCount) - (sizeof...(pulses) % 2 == 0 ? 1 : 0)
                ),
            };
            ::vibes_enqueue_custom_pattern(pattern);
        }

    private:

        // SetValuesPulse

        template <class... TPulses> inline
        void SetValuesPulse(size_t i, Pulse pulse, TPulses... pulses)
        {
            SetValuesPulse(i, pulse);
            SetValuesWait(i + 1, pulses...);
        }

        template <class... TPulses> inline
        void SetValuesPulse(size_t i, Pulse pulse)
        {
            pulses_[i] = static_cast<uint32_t>(pulse.duration.total_milliseconds());
        }

        // SetValuesWait

        template <class... TPulses> inline
        void SetValuesWait(size_t i, Wait wait, TPulses... pulses)
        {
            SetValuesWait(i, wait);
            SetValuesPulse(i + 1, pulses...);
        }

        template <class... TPulses> inline
        void SetValuesWait(size_t i, Wait wait)
        {
            pulses_[i] = static_cast<uint32_t>(wait.duration.total_milliseconds());
        }

        uint32_t pulses_[TNum];
    };
}

#endif //PEBBLE_CPP_VIBRATION_HANDLER_HPP
