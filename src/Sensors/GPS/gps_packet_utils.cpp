
#include "gps_packet_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>

hyspex_eQuery gps::to_hyspex_query_enum_1(std::uint16_t length, const net_buffer_view& buffer)
{
    hyspex_QueryMessage_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);
    return pckt.query();
}

int gps::encode_hyspex_query(hyspex_eQuery query, net_buffer& buffer)
{
    hyspex_QueryMessage_1 pckt;
    pckt.set_query(query);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::HYSPEX_QUERY);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

hyspex_eCommand gps::to_hyspex_command_enum_1(std::uint16_t length, const net_buffer_view& buffer)
{
    hyspex_CommandMessage_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);
    return pckt.command();
}

int gps::encode_hyspex_command(hyspex_eCommand command, net_buffer& buffer)
{
    hyspex_CommandMessage_1 pckt;
    pckt.set_command(command);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::HYSPEX_COMMAND);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

hyspex::sAcquisitionParameters_t gps::to_acquisition_parameters_1(std::uint16_t length, const net_buffer_view& buffer)
{
    hyspex_SetAcquisitionParameters_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);

    hyspex::sAcquisitionParameters_t data;
    data.average_frames = pckt.average_frame();
    data.frame_period_us = pckt.frame_period_us();
    data.integration_time_us = pckt.integration_time_us();
    return data;
}

int gps::encode_acquisition_parameters(std::uint16_t average_frame,
    std::uint32_t frame_period_us, std::uint32_t integration_time_us, net_buffer& buffer)
{
    hyspex_SetAcquisitionParameters_1 pckt;
    pckt.set_average_frame(average_frame);
    pckt.set_frame_period_us(frame_period_us);
    pckt.set_integration_time_us(integration_time_us);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_ACQUISITION_PARAMETERS);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::string gps::to_lens_name_1(std::uint16_t length, const net_buffer_view& buffer)
{
    hyspex_SetLens_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);

    return pckt.lens_name();
}

int gps::encode_lens_name(const std::string& lens_name, net_buffer& buffer)
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

int gps::to_num_backgrounds_1(std::uint16_t length, const net_buffer_view& buffer)
{
    hyspex_SetNumOfBackgrounds_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);
    return pckt.num_backgrounds();
}

int gps::encode_num_backgrounds(int num_backgrounds, net_buffer& buffer)
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

int gps::encode_calc_background(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CALC_BACKGROUND);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}

hyspex::sCurrentState_t gps::to_current_state_1(std::uint16_t length, const net_buffer_view& buffer)
{
    hyspex::sCurrentState_t data;

    hyspex_CurrentState_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);

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

int gps::encode_current_state(const sCurrentState_t& state, net_buffer& buffer)
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


std::vector<std::string> gps::to_lens_names_1(std::uint16_t length, const net_buffer_view& buffer)
{
    std::vector<std::string> names;
    std::string name;
 
    hyspex_LensNames_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);

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

int gps::encode_lens_names(const std::vector<std::string>& names, net_buffer& buffer)
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
        hdr.id = static_cast<uint16_t>(gps::ePacketType::LENS_NAMES);
        hdr.revision = 1;
        hdr.length = str.length();
        set_timestamp(&hdr.timestamp);

        buffer << hdr;
        buffer.write(str);
        total_length += sizeof(sPacketHeader_t) + hdr.length;
    }

    return total_length;
}


/*** send/receive the command reply message ***/
hyspex_eCommand gps::to_command_reply_1(std::uint16_t length, const net_buffer_view& buffer)
{
    hyspex_CommandReply_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);
    return pckt.reply();
}

int gps::encode_command_reply(hyspex_eCommand reply, net_buffer& buffer)
{
    hyspex_CommandReply_1 pckt;
    pckt.set_reply(reply);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::COMMAND_REPLY);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

hyspex_eBackgroundReply gps::to_background_reply_1(std::uint16_t length, const net_buffer_view& buffer)
{
    hyspex_BackgroundReply_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);
    return pckt.reply();
}

int gps::encode_background_reply(hyspex_eBackgroundReply reply, net_buffer& buffer)
{
    hyspex_BackgroundReply_1 pckt;
    pckt.set_reply(reply);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::BACKGROUND_REPLY);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


/*** send/receive the shutter state reply message ***/
hyspex_eShutterState gps::to_set_shutter_state_1(std::uint16_t length, const net_buffer_view& buffer)
{
    hyspex_SetShutterState_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);
    return pckt.state();
}

int gps::encode_set_shutter_state(hyspex_eShutterState state, net_buffer& buffer)
{
    hyspex_SetShutterState_1 pckt;
    pckt.set_state(state);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SET_SHUTTER_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


hyspex_eShutterState gps::to_shutter_state_reply_1(std::uint16_t length, const net_buffer_view& buffer)
{
    hyspex_ShutterStateReply_1 pckt;
    pckt.ParseFromArray(buffer.data(), length);
    return pckt.state();
}

int gps::encode_shutter_state_reply(hyspex_eShutterState state, net_buffer& buffer)
{
    hyspex_ShutterStateReply_1 pckt;
    pckt.set_state(state);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SHUTTER_STATE_REPLY);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


