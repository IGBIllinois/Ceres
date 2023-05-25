
#include "remote_client_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>


/*
 * Ceres <---> Ceres Remote Client Packets
 */
sExperimentInfo_t to_experiment_info_1(const ExperimentInfo_1& pckt)
{
    sExperimentInfo_t data;

    data.title      = pckt.title();
    data.researcher = pckt.researcher();
    data.cultivar   = pckt.cultivar();
    data.doc        = pckt.file();

    data.species.clear();

    return data;
}

sExperimentInfo_t to_experiment_info_2(const ExperimentInfo_2& pckt)
{
    sExperimentInfo_t data;

    data.title = pckt.title();
    data.researcher = pckt.researcher();
    data.species = pckt.species();
    data.cultivar = pckt.cultivar();
    data.doc = pckt.file();

    return data;
}

int encode_exp_info_data(const std::string& title, const std::string& researcher,
    const std::string& cultivar, const std::string& doc, net_buffer& buffer)
{
    ExperimentInfo_1 pckt;

    pckt.set_title(title);

    if (!researcher.empty())
        pckt.set_researcher(researcher);

    if (!cultivar.empty())
        pckt.set_cultivar(cultivar);

    pckt.set_file(doc);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::EXPERIMENT_INFO);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int encode_exp_info_data(const std::string& title, const std::string& researcher,
    const std::string& species, const std::string& cultivar, const std::string& doc, net_buffer& buffer)
{
    ExperimentInfo_2 pckt;

    pckt.set_title(title);

    if (!researcher.empty())
        pckt.set_researcher(researcher);

    if (!species.empty())
        pckt.set_species(species);

    if (!cultivar.empty())
        pckt.set_cultivar(cultivar);

    pckt.set_file(doc);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::EXPERIMENT_INFO);
    hdr.revision = 2;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::string to_principal_investigator_1(const PrincipalInvestigator_1& pckt)
{
    return pckt.pi();
}

int encode_principal_investigator(const std::string& pi, net_buffer& buffer)
{
    PrincipalInvestigator_1 pckt;

    pckt.set_pi(pi);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::PRINCIPAL_INVESTIGATOR);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::string to_construct_name_1(const ConstructName_1& pckt)
{
    return pckt.construct_name();
}

int encode_construct_name(const std::string& name, net_buffer& buffer)
{
    ConstructName_1 pckt;

    pckt.set_construct_name(name);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CONSTRUCT_NAME);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::string to_event_number_1(const EventNumber_1& pckt)
{
    return pckt.event_number();
}

int encode_event_number(const std::string& number, net_buffer& buffer)
{
    EventNumber_1 pckt;

    pckt.set_event_number(number);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::EVENT_NUMBER);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::string to_field_design_1(const FieldDesign_1& pckt)
{
    return pckt.field_design();
}

int encode_field_design(const std::string& design, net_buffer& buffer)
{
    FieldDesign_1 pckt;

    pckt.set_field_design(design);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::FIELD_DESIGN);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::time_t to_planting_date_1(const PlantingDate_1& pckt)
{
    return static_cast<std::time_t>(pckt.planting_date());
}

int encode_planting_date(std::time_t date, net_buffer& buffer)
{
    PlantingDate_1 pckt;

    pckt.set_planting_date(static_cast<uint64_t>(date));

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::PLANTING_DATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::time_t to_harvest_date_1(const HarvestDate_1& pckt)
{
    return static_cast<std::time_t>(pckt.harvest_date());
}

int encode_harvest_date(std::time_t date, net_buffer& buffer)
{
    HarvestDate_1 pckt;

    pckt.set_harvest_date(static_cast<uint64_t>(date));

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::HARVEST_DATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::string to_treatment_1(const ExperimentTreatment_1& pckt)
{
    return pckt.treatment();
}

int encode_treatment(const std::string& treatment, net_buffer& buffer)
{
    ExperimentTreatment_1 pckt;

    pckt.set_treatment(treatment);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::TREATMENT);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

std::string to_comment_1(const ExperimentComment_1& pckt)
{
    return pckt.comment();
}

int encode_comment(const std::string& comment, net_buffer& buffer)
{
    ExperimentComment_1 pckt;

    pckt.set_comment(comment);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::COMMENT);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int encode_start_experiment(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::START_EXPERIMENT);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}

int encode_stop_experiment(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::STOP_EXPERIMENT);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}

std::string to_filename_1(const OpenDataFile_1& pckt)
{
    return pckt.filename();
}

int encode_open_data_file(const std::string& filename, net_buffer& buffer)
{
    OpenDataFile_1 pckt;

    pckt.set_filename(filename);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::OPEN_DATA_FILE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int encode_close_data_file(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CLOSE_DATA_FILE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}

bool to_file_open_state_1(const FileOpenState_1& pckt)
{
    return pckt.isopen();
}

int encode_file_open_state(bool open, net_buffer& buffer)
{
    FileOpenState_1 pckt;

    pckt.set_isopen(open);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::DATA_FILE_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

int encode_start_data_recording(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::START_DATA_RECORDING);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}

int encode_stop_data_recording(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::STOP_DATA_RECORDING);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    buffer << hdr;

    return sizeof(sPacketHeader_t);
}


std::string to_status_message_1(const StatusMessage_1& pckt)
{
    return pckt.message();
}

int encode_status_message(const std::string& message, net_buffer& buffer)
{
    StatusMessage_1 pckt;

    pckt.set_message(message);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::STATUS_MESSAGE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

sLogMessage_t to_log_message_1(const LogMessage_1& pckt)
{
    sLogMessage_t data;

    data.msg_type = pckt.msg_type();
    data.device = pckt.device();
    data.message = pckt.message();

    return data;
}

int encode_log_message(uint8_t msg_type, const std::string& device, const std::string& message, net_buffer& buffer)
{
    LogMessage_1 pckt;

    pckt.set_msg_type(msg_type);
    pckt.set_device(device);
    pckt.set_message(message);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::LOG_MESSAGE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

sSensorStatus_t to_sensor_status_1(const SensorStatus_1& pckt)
{
    sSensorStatus_t data;

    data.name = pckt.device();
    data.status = pckt.message();

    return data;
}

int encode_sensor_status(const std::string& device, const std::string& message, net_buffer& buffer)
{
    SensorStatus_1 pckt;

    pckt.set_device(device);
    pckt.set_message(message);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SENSOR_STATUS);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

sSensorNameChange_t to_sensor_name_change_1(const SensorNameChange_1& pckt)
{
    sSensorNameChange_t data;

    data.old_name = pckt.old_name();
    data.new_name = pckt.new_name();

    return data;
}

int encode_sensor_name_change(const std::string& old_name, const std::string& new_name, net_buffer& buffer)
{
    SensorNameChange_1 pckt;

    pckt.set_old_name(old_name);
    pckt.set_new_name(new_name);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SENSOR_NAME_CHANGE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

sSensorPropertyConnectInfo_t to_sensor_property_connect_info_1(const SensorPropertyConnectInfo_1& pckt)
{
    sSensorPropertyConnectInfo_t data;

    data.sensor     = pckt.device();
    data.model      = pckt.model();
    data.version    = pckt.version();
    data.name       = pckt.name();
    data.ip_address = pckt.ip_address();
    data.port       = pckt.port();

    return data;
}

int encode_sensor_property_connect_info(const std::string& sensor, const std::string& model, uint32_t version,
    const std::string& name, const std::string& ip_address, uint16_t port, net_buffer& buffer)
{
    SensorPropertyConnectInfo_1 pckt;

    pckt.set_device(sensor);
    pckt.set_model(model);
    pckt.set_version(version);
    pckt.set_name(name);
    pckt.set_ip_address(ip_address);
    pckt.set_port(port);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SENSOR_PROPERTY_CONNECT_INFO);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}

/*
 * Spidercam Packets
 */
spidercam::sPosition_1_t to_spidercam_position_1(const Spidercam_Position_1& pckt)
{
    spidercam::sPosition_1_t data;

    data.X_mm = pckt.x_mm();
    data.Y_mm = pckt.y_mm();
    data.Z_mm = pckt.z_mm();
    data.speed_mmps = pckt.speed_mmps();
    data.pan_deg = pckt.pan_deg();
    data.pitch_deg = pckt.pitch_deg();
    data.roll_gimbal_deg = pckt.roll_deg();
    data.timestamp = pckt.timestamp();

    data.focus = 0;
    data.height_mm = data.Z_mm;
    data.iris = 0;
    data.pan_speed_dps = 0.0;
    data.roll_deg = data.roll_gimbal_deg;
    data.tilt_deg = 0;
    data.tilt_speed_dps = 0;

    return data;
}

int encode_spidercam_pos(const spidercam::sPosition_1_t& pos, net_buffer& buffer)
{
    Spidercam_Position_1 pckt;

    pckt.set_datavalid(true);
    pckt.set_x_mm(pos.X_mm);
    pckt.set_y_mm(pos.Y_mm);
    pckt.set_z_mm(pos.Z_mm);
    pckt.set_speed_mmps(pos.speed_mmps);
    pckt.set_pan_deg(pos.pan_deg);
    pckt.set_pitch_deg(pos.pitch_deg);
    pckt.set_roll_deg(pos.roll_gimbal_deg);
    pckt.set_timestamp(pos.timestamp);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SPIDER_CAM_DATA);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}


/*
 * Weather Packets
 */
sWeatherData_t to_weather_data_1(const WeatherData_1& pckt)
{
    sWeatherData_t data;

    data.dataValid = pckt.datavalid();
    data.wind_speed_mps = pckt.wind_speed_mps();
    data.wind_direction_deg = pckt.wind_direction_deg();

    return data;
}

int encode_weather_data(bool valid, double wind_speed_mps, double wind_direction_deg, net_buffer& buffer)
{
    WeatherData_1 pckt;

    pckt.set_datavalid(valid);
    pckt.set_wind_speed_mps(wind_speed_mps);
    pckt.set_wind_direction_deg(wind_direction_deg);

    std::string str;
    pckt.SerializeToString(&str);

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::WEATHER_DATA);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    buffer << hdr;
    buffer.write(str);

    return sizeof(sPacketHeader_t) + hdr.length;
}



