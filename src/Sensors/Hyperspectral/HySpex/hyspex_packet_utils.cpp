
#include "hyspex_packet_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>


int hyspex::encode_query_state(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::QUERY_STATE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}

int hyspex::encode_query_lens_names(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::QUERY_LENS_NAMES);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}


std::uint32_t hyspex::to_average_frames_1(const hyspex_SetAverageFrames_1& pckt)
{
    return pckt.average_frame();
}

int hyspex::encode_average_frames(std::uint32_t average_frame, net_buffer& buffer)
{
    hyspex_SetAverageFrames_1 pckt;
    pckt.set_average_frame(average_frame);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_AVERAGE_FRAMES);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::uint32_t hyspex::to_frame_period_1(const hyspex_SetFramePeriod_1& pckt)
{
    return pckt.frame_period_us();
}

int hyspex::encode_frame_period(std::uint32_t frame_period_us, net_buffer& buffer)
{
    hyspex_SetFramePeriod_1 pckt;
    pckt.set_frame_period_us(frame_period_us);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_FRAME_PERIOD_US);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::uint32_t hyspex::to_integration_time_1(const hyspex_SetIntegrationTime_1& pckt)
{
    return pckt.integration_time_us();
}

int hyspex::encode_integration_time(std::uint32_t integration_time_us, net_buffer& buffer)
{
    hyspex_SetIntegrationTime_1 pckt;
    pckt.set_integration_time_us(integration_time_us);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_INTEGRATION_TIME_US);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::string hyspex::to_lens_name_1(const hyspex_SetLens_1& pckt)
{
    return pckt.lens_name();
}

int hyspex::encode_lens_name(const std::string& lens_name, net_buffer& buffer)
{
    hyspex_SetLens_1 pckt;
    pckt.set_lens_name(lens_name);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_LENS_NAME);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int hyspex::to_num_backgrounds_1(const hyspex_SetNumOfBackgrounds_1& pckt)
{
    return pckt.num_backgrounds();
}

int hyspex::encode_num_backgrounds(int num_backgrounds, net_buffer& buffer)
{
    hyspex_SetNumOfBackgrounds_1 pckt;
    pckt.set_num_backgrounds(num_backgrounds);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_NUM_BACKGROUNDS);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int hyspex::encode_calc_background(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CALC_BACKGROUND);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}

hyspex::sCurrentState_t hyspex::to_current_state_1(const hyspex_CurrentState_1& pckt)
{
    hyspex::sCurrentState_t data;

    data.valid = pckt.valid();

    if (data.valid)
    {
        data.average_frames = pckt.average_frame();
        data.frame_period_us = pckt.frame_period_us();
        data.min_frame_period_us = pckt.min_frame_period_us();
        data.integration_time_us = pckt.integration_time_us();
        data.max_integration_time_us = pckt.max_integration_time_us();
        data.num_backgrounds = pckt.num_backgrounds();
        data.lens_name = pckt.lens_name();
    }
    else
    {
        data.average_frames = 0;
        data.frame_period_us = 0;
        data.min_frame_period_us = 0;
        data.integration_time_us = 0;
        data.max_integration_time_us = 0;
        data.num_backgrounds = 0;
        data.lens_name.clear();
    }

    return data;
}

int hyspex::encode_current_state(const sCurrentState_t& state, net_buffer& buffer)
{
    hyspex_CurrentState_1 pckt;

    pckt.set_valid(state.valid);
    pckt.set_average_frame(state.average_frames);
    pckt.set_frame_period_us(state.frame_period_us);
    pckt.set_min_frame_period_us(state.min_frame_period_us);
    pckt.set_integration_time_us(state.integration_time_us);
    pckt.set_max_integration_time_us(state.max_integration_time_us);
    pckt.set_num_backgrounds(state.num_backgrounds);
    pckt.set_lens_name(state.lens_name);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CURRENT_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


std::vector<std::string> hyspex::to_lens_names_1(const hyspex_LensNames_1& pckt)
{
    std::vector<std::string> names;
    std::string name;
    
    name = pckt.lens_name_0();
    if (name.empty()) return names;
    names.push_back(name);

    name = pckt.lens_name_1();
    if (name.empty()) return names;
    names.push_back(name);

    name = pckt.lens_name_2();
    if (name.empty()) return names;
    names.push_back(name);

    name = pckt.lens_name_3();
    if (name.empty()) return names;
    names.push_back(name);

    name = pckt.lens_name_4();
    if (name.empty()) return names;
    names.push_back(name);

    name = pckt.lens_name_5();
    if (name.empty()) return names;
    names.push_back(name);

    name = pckt.lens_name_6();
    if (name.empty()) return names;
    names.push_back(name);

    name = pckt.lens_name_7();
    if (name.empty()) return names;
    names.push_back(name);

    name = pckt.lens_name_8();
    if (name.empty()) return names;
    names.push_back(name);

    name = pckt.lens_name_9();
    if (name.empty()) return names;
    names.push_back(name);

    return names;
}

int hyspex::encode_lens_names(const std::vector<std::string>& names, net_buffer& buffer)
{
    int total_length = 0;
    int count = 0;

    hyspex_LensNames_1 pckt;

    for (std::size_t i = 0; i < names.size(); ++i)
    {
        switch (count)
        {
        case 0:
            pckt.set_lens_name_0(names[i]);
            break;
        case 1:
            pckt.set_lens_name_1(names[i]);
            break;
        case 2:
            pckt.set_lens_name_2(names[i]);
            break;
        case 3:
            pckt.set_lens_name_3(names[i]);
            break;
        case 4:
            pckt.set_lens_name_4(names[i]);
            break;
        case 5:
            pckt.set_lens_name_5(names[i]);
            break;
        case 6:
            pckt.set_lens_name_6(names[i]);
            break;
        case 7:
            pckt.set_lens_name_7(names[i]);
            break;
        case 8:
            pckt.set_lens_name_8(names[i]);
            break;
        case 9:
            pckt.set_lens_name_9(names[i]);
            break;
        default:
        {
            std::string str;
            pckt.SerializeToString(&str);

            sPacketHeader_t hdr;
            hdr.id = static_cast<uint16_t>(ePacketType::LENS_NAMES);
            hdr.revision = 1;
            hdr.length = str.length();
            set_timestamp(&hdr.timestamp);

            buffer << hdr;
            buffer.write(str);
            total_length += sizeof(sPacketHeader_t) + hdr.length;

            pckt.Clear();
            count = -1;
        }
        }

        ++count;
    }

    if (pckt.ByteSize() > 0)
    {
        std::string str;
        pckt.SerializeToString(&str);

        sPacketHeader_t hdr;
        hdr.id = static_cast<uint16_t>(hyspex::ePacketType::LENS_NAMES);
        hdr.revision = 1;
        hdr.length = str.length();
        set_timestamp(&hdr.timestamp);

        buffer << hdr;
        buffer.write(str);
        total_length += sizeof(sPacketHeader_t) + hdr.length;
    }

    return total_length;
}

