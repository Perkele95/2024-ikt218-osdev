#include "Soundplayer.h"

extern "C"
{
#include "../pit/pit.h"
}

void Soundplayer::play(Sound sound)
{
    enable_speaker();

    for (size_t i = 0; i < sound.length; i++)
    {
        Note *note = sound.notes + i;
        play_note(note->frequency);
        sleep_interrupt(note->duration);
    }
    disable_speaker();
}

void Soundplayer::enable_speaker()
{
    // Read pc speaker state
    const uint8_t state = inbyte(PC_SPEAKER_PORT);
    const uint8_t desired_bits = PC_SPEAKER_GATE_BIT | PC_SPEAKER_DATA_BIT;

    // Set desired btis if not set
    if((state & desired_bits) != desired_bits)
        outbyte(PC_SPEAKER_PORT, state | desired_bits);
}

void Soundplayer::disable_speaker()
{
    // Clear the gate and data bits
    const uint8_t state = inbyte(PC_SPEAKER_PORT);
    outbyte(PC_SPEAKER_PORT, state & 0xFC);
}

void Soundplayer::play_note(uint32_t frequency)
{
    if(frequency)
    {
        const uint16_t divisor = PIT_FREQUENCY / frequency;

        // send command byte to channel 2
        outbyte(PIT_COMMAND, 0xB6); // Value 0xB6 is from OsDev Wiki
        outbyte(PIT_CH2, (uint8_t)(divisor & 0xFF));
        outbyte(PIT_CH2, (uint8_t)((divisor >> 8) & 0xFF));
    }
}