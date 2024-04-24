#pragma once

#include "../common.h"
#include "frequencies.h"

struct Sound
{
    Note *notes;
    uint32_t length;
};

class Soundplayer
{
public:
    Soundplayer() = default;

    void play(Sound sound);

private:
    void enable_speaker();
    void disable_speaker();
public:
    void play_note(uint32_t frequency);
};