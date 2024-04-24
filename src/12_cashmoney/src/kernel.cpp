extern "C" int kernel_main(void);

extern "C"
{
#include <libc/memory.h>
}

#include "sound/Soundplayer.h"
#include "sound/songs.h"

void *operator new(size_t size) {
    return malloc(size);
}

void *operator new[](size_t size) {
    return malloc(size);
}

void operator delete(void *ptr) noexcept {
    free(ptr);
}

void operator delete[](void *ptr) noexcept {
    free(ptr);
}

void operator delete(void *ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}

void operator delete[](void *ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}

#define GET_MUSIC_STRUCT(song)\
    {\
        .notes = song,\
        .length = array_size(song)\
    }

int kernel_main()
{
    auto soundplayer = new Soundplayer();

    const Sound music = GET_MUSIC_STRUCT(music_4);

    soundplayer->play(music);

    while(true){}
    return 0;
}