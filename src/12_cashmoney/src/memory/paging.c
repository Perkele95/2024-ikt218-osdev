#include "paging.h"

extern uint32_t placement_address;

typedef struct
{
    uint32_t index;
    uint32_t offset;
}frame_info_t;

uint32_t *pFrames = 0;
uint32_t frameCount = 0;

#define INDEX_FROM_BIT(frame) (frame/32)
#define OFFSET_FROM_BIT(frame) (frame%32)

static frame_info_t get_frame_info(uint32_t frameAddress)
{
    const uint32_t frame = frameAddress / 0x1000;
    frame_info_t info = {
        .index = INDEX_FROM_BIT(frame),
        .offset = OFFSET_FROM_BIT(frame)
    };
    return info;
}

static void set_frame(uint32_t frameAddress)
{
    const frame_info_t fi = get_frame_info(frameAddress);
    pFrames[fi.index] |= 1 << fi.offset; // Set
}

static void clear_frame(uint32_t frameAddress)
{
    const frame_info_t fi = get_frame_info(frameAddress);
    pFrames[fi.index] &= ~(1 << fi.offset); // Clear
}

static uint32_t check_frame(uint32_t frameAddress)
{
    const frame_info_t fi = get_frame_info(frameAddress);
    return pFrames[fi.index] & ~(1 << fi.offset); // Check
}

static uint32_t get_first_frame()
{
    // i is the index to the frame table
    // j is the nth bit in each frame value
    for (size_t i = 0; i < INDEX_FROM_BIT(frameCount); i++)
    {
        if(pFrames[i] != 0xFFFFFFFF)
        {
            for (size_t j = 0; j < 32; j++)
            {
                const uint32_t frameToTest = 1 << j;
                if(!(pFrames[i] & frameToTest))
                    return i * 32 + j;
            }
        }
    }
}

void init_paging()
{
    //
}