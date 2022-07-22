
#pragma once

namespace experiment
{
    /**********************************************************
       WARNING: DO NOT CHANGE THE ORDER OF THESE ENUMS!!!!

       Only add new one to the end of the list.
    **********************************************************/
    enum class DataID : uint16_t
    {
        EXPERIMENT,
        START_TIME,
        END_TIME,
        START_RECORDING_TIMESTAMP,
        END_RECORDING_TIMESTAMP,
        RESEARCHER,
        CULTIVAR,
        EXPERIMENT_TITLE,
        BEGIN_HEADER,
        END_OF_HEADER,
        BEGIN_FOOTER,
        END_OF_FOOTER,
        BEGIN_SENSOR_LIST,
        END_OF_SENSOR_LIST,
        SENSOR_DATA_BLOCK_INFO,
        RECORDING_HEARTBEAT_TIMESTAMP,
    };
}


