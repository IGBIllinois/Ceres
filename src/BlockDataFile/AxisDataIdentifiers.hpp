
#pragma once

namespace axis
{
    /**********************************************************
       WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

       Only add new one to the end of the list.
    **********************************************************/
    enum class DataID : uint16_t
    {
        TIMESTAMP,
        CAMERA_ID,
        RESOLUTION,
        FRAMES_PER_SECOND,
        BITMAP,
        JPEG,
        MPEG_FRAME,
    };
}


