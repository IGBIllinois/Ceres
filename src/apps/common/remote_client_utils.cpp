
#include "remote_client_utils.hpp"
#include "packet_utils.hpp"
#include "net_buffer.hpp"

#include <string>


/*
 * Ceres <---> Ceres Remote Client Packets
 */

std::string to_experiment_type_1(const ExperimentType_1& pckt)
{
    return pckt.type();
}

int encode_experiment_type(const std::string& type, net_buffer& buffer)
{
    ExperimentType_1 pckt;

    pckt.set_type(type);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::EXPERIMENT_TYPE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::EXPERIMENT_INFO);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::EXPERIMENT_INFO);
    hdr.revision = 2;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

std::string to_experiment_title_1(const ExperimentTitle_1& pckt)
{
    return pckt.title();
}

int encode_experiment_title(const std::string& title, net_buffer& buffer)
{
    ExperimentTitle_1 pckt;

    pckt.set_title(title);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::EXPERIMENT_TITLE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

std::string to_measurement_title_1(const MeasurementTitle_1& pckt)
{
    return pckt.title();
}

int encode_measurement_title(const std::string& title, net_buffer& buffer)
{
    ExperimentTitle_1 pckt;

    pckt.set_title(title);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::MEASUREMENT_TITLE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

std::string to_experiment_document_1(const ExperimentDocument_1& pckt)
{
    return pckt.document();
}

int encode_experiment_document(const std::string& doc, net_buffer& buffer)
{
    ExperimentDocument_1 pckt;

    pckt.set_document(doc);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::EXPERIMENT_DOCUMENT);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

std::string to_species_1(const Species_1& pckt)
{
    return pckt.species();
}

int encode_species(const std::string& species, net_buffer& buffer)
{
    Species_1 pckt;

    pckt.set_species(species);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SPECIES);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

std::string to_cultivar_1(const Cultivar_1& pckt)
{
    return pckt.cultivar();
}

int encode_cultivar(const std::string& cultivar, net_buffer& buffer)
{
    Cultivar_1 pckt;

    pckt.set_cultivar(cultivar);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CULTIVAR);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::PRINCIPAL_INVESTIGATOR);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

std::string to_researcher_1(const Researcher_1& pckt)
{
    return pckt.researcher();
}

int encode_start_of_researcher_list(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::START_OF_RESEARCHER_LIST);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_end_of_researcher_list(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::END_OF_RESEARCHER_LIST);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_researcher(const std::string& researcher, net_buffer& buffer)
{
    Researcher_1 pckt;

    pckt.set_researcher(researcher);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::RESEARCHER);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CONSTRUCT_NAME);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

std::string to_event_number_1(const EventNumber_1& pckt)
{
    return pckt.event_number();
}

int encode_start_of_event_number_list(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::START_OF_EVENT_NUMBER_LIST);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_end_of_event_number_list(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::END_OF_EVENT_NUMBER_LIST);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_event_number(const std::string& number, net_buffer& buffer)
{
    EventNumber_1 pckt;

    pckt.set_event_number(number);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::EVENT_NUMBER);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::FIELD_DESIGN);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::PLANTING_DATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::HARVEST_DATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

std::string to_treatment_1(const ExperimentTreatment_1& pckt)
{
    return pckt.treatment();
}

int encode_start_of_treatment_list(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::START_OF_TREATMENT_LIST);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_end_of_treatment_list(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::END_OF_TREATMENT_LIST);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_treatment(const std::string& treatment, net_buffer& buffer)
{
    ExperimentTreatment_1 pckt;

    pckt.set_treatment(treatment);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::TREATMENT);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

std::string to_comment_1(const ExperimentComment_1& pckt)
{
    return pckt.comment();
}

int encode_start_of_comment_list(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::START_OF_COMMENT_LIST);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_end_of_comment_list(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::END_OF_COMMENT_LIST);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_comment(const std::string& comment, net_buffer& buffer)
{
    ExperimentComment_1 pckt;

    pckt.set_comment(comment);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::COMMENT);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

std::string to_permit_info_1(const ExperimentPermitInfo_1& pckt)
{
    return pckt.permit();
}

int encode_permit_info(const std::string& permit, net_buffer& buffer)
{
    ExperimentPermitInfo_1 pckt;

    pckt.set_permit(permit);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::PERMIT_INFO);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

sPermitInfo_t to_permit_info_2(const ExperimentPermitInfo_2& pckt)
{
    sPermitInfo_t result;

    result.authorization = pckt.authorization();
    result.permit = pckt.permit();

    return result;
}

int encode_permit_info(const std::string& authorization, const std::string& permit, net_buffer& buffer)
{
    ExperimentPermitInfo_2 pckt;

    pckt.set_authorization(authorization);
    pckt.set_permit(permit);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::PERMIT_INFO);
    hdr.revision = 2;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

int encode_experiment_info_reply(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::EXPERIMENT_INFO_REPLY);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_end_of_experiment_info(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::END_OF_EXPERIMENT_INFO);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}


int encode_start_experiment(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::START_EXPERIMENT);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_stop_experiment(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::STOP_EXPERIMENT);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::OPEN_DATA_FILE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

int encode_close_data_file(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::CLOSE_DATA_FILE);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::DATA_FILE_STATE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

int encode_start_data_recording(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::START_DATA_RECORDING);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

int encode_stop_data_recording(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::STOP_DATA_RECORDING);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t);

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::STATUS_MESSAGE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

sLogMessage_t to_log_message_1(const LogMessage_1& pckt)
{
    sLogMessage_t data;

    data.msg_type = pckt.msg_type();
    data.device = pckt.device();
    data.message = pckt.message();

    if (pckt.has_instance())
        data.instance = pckt.instance();

    return data;
}

int encode_log_message(uint8_t msg_type, const std::string& device, const std::string& message, net_buffer& buffer)
{
    LogMessage_1 pckt;

    pckt.set_msg_type(msg_type);
    pckt.set_device(device);
    pckt.set_message(message);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::LOG_MESSAGE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

int encode_log_message(uint8_t msg_type, const std::string& device, const std::string& instance, const std::string& message, net_buffer& buffer)
{
    LogMessage_1 pckt;

    pckt.set_msg_type(msg_type);
    pckt.set_device(device);
    pckt.set_instance(instance);
    pckt.set_message(message);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::LOG_MESSAGE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

sSensorStatus_t to_sensor_status_1(const SensorStatus_1& pckt)
{
    sSensorStatus_t data;

    data.name = pckt.device();
    data.status = pckt.message();

    if (pckt.has_instance())
        data.instance = pckt.instance();

    return data;
}

int encode_sensor_status(const std::string& device, const std::string& message, net_buffer& buffer)
{
    SensorStatus_1 pckt;

    pckt.set_device(device);
    pckt.set_message(message);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SENSOR_STATUS);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

int encode_sensor_status(const std::string& device, const std::string& instance, const std::string& message, net_buffer& buffer)
{
    SensorStatus_1 pckt;

    pckt.set_device(device);
    pckt.set_message(message);
    pckt.set_instance(instance);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SENSOR_STATUS);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

sSensorNameChange_t to_sensor_name_change_1(const SensorNameChange_1& pckt)
{
    sSensorNameChange_t data;

    data.old_name = pckt.old_name();
    data.new_name = pckt.new_name();

    if (pckt.has_instance())
        data.instance = pckt.instance();

    return data;
}

int encode_sensor_name_change(const std::string& old_name, const std::string& new_name, net_buffer& buffer)
{
    SensorNameChange_1 pckt;

    pckt.set_old_name(old_name);
    pckt.set_new_name(new_name);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SENSOR_NAME_CHANGE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

int encode_sensor_name_change(const std::string& old_name, const std::string& new_name, const std::string& instance, net_buffer& buffer)
{
    SensorNameChange_1 pckt;

    pckt.set_old_name(old_name);
    pckt.set_new_name(new_name);
    pckt.set_instance(instance);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SENSOR_NAME_CHANGE);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
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

    if (pckt.has_instance())
    {
        data.instance = pckt.instance();
    }

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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SENSOR_PROPERTY_CONNECT_INFO);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

int encode_sensor_property_connect_info(const std::string& sensor, const std::string& model, uint32_t version,
    const std::string& name, const std::string& instance, const std::string& ip_address, uint16_t port, net_buffer& buffer)
{
    SensorPropertyConnectInfo_1 pckt;

    pckt.set_device(sensor);
    pckt.set_model(model);
    pckt.set_version(version);
    pckt.set_name(name);
    pckt.set_ip_address(ip_address);
    pckt.set_port(port);
    pckt.set_instance(instance);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SENSOR_PROPERTY_CONNECT_INFO);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}


eRemoteThread_STATUS to_remote_thread_status_1(const RemoteThreadStatus_1& pckt)
{
    return static_cast<eRemoteThread_STATUS>(pckt.status());
}

int encode_remote_thread_status(const eRemoteThread_STATUS status, net_buffer& buffer)
{
    RemoteThreadStatus_1 pckt;

    pckt.set_status(static_cast<remoteThread_eSTATUS>(status));

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::REMOTE_THREAD_STATUS);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

/*
 * Spidercam Packets
 */
spidercam::sPosition_1_t to_spidercam_position_1(std::uint16_t length, const net_buffer_view& buffer)
{
    Spidercam_Position_1 pckt;

    if (pckt.ParseFromArray(buffer.data(), length))
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

    return spidercam::sPosition_1_t();
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
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SPIDER_CAM_DATA);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

spidercam::sPosition_1_t to_spidercam_start_position_1(std::uint16_t length, const net_buffer_view& buffer)
{
    Spidercam_Start_Position_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
    {
        spidercam::sPosition_1_t data;

        data.X_mm = pckt.x_mm();
        data.Y_mm = pckt.y_mm();
        data.Z_mm = pckt.z_mm();
        data.pan_deg = pckt.pan_deg();
        data.pitch_deg = pckt.pitch_deg();
        data.roll_gimbal_deg = pckt.roll_deg();
        data.timestamp = pckt.timestamp();

        data.speed_mmps = 0;
        data.focus = 0;
        data.height_mm = data.Z_mm;
        data.iris = 0;
        data.pan_speed_dps = 0.0;
        data.roll_deg = data.roll_gimbal_deg;
        data.tilt_deg = 0;
        data.tilt_speed_dps = 0;

        return data;
    }

    return spidercam::sPosition_1_t();
}

int encode_spidercam_start_pos(const spidercam::sPosition_1_t& pos, net_buffer& buffer)
{
    Spidercam_Start_Position_1 pckt;

    pckt.set_datavalid(true);
    pckt.set_x_mm(pos.X_mm);
    pckt.set_y_mm(pos.Y_mm);
    pckt.set_z_mm(pos.Z_mm);
    pckt.set_pan_deg(pos.pan_deg);
    pckt.set_pitch_deg(pos.pitch_deg);
    pckt.set_roll_deg(pos.roll_gimbal_deg);
    pckt.set_timestamp(pos.timestamp);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SPIDER_CAM_START_POSITION);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

spidercam::sPosition_1_t to_spidercam_end_position_1(std::uint16_t length, const net_buffer_view& buffer)
{
    Spidercam_End_Position_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
    {
        spidercam::sPosition_1_t data;

        data.X_mm = pckt.x_mm();
        data.Y_mm = pckt.y_mm();
        data.Z_mm = pckt.z_mm();
        data.pan_deg = pckt.pan_deg();
        data.pitch_deg = pckt.pitch_deg();
        data.roll_gimbal_deg = pckt.roll_deg();
        data.timestamp = pckt.timestamp();

        data.speed_mmps = 0;
        data.focus = 0;
        data.height_mm = data.Z_mm;
        data.iris = 0;
        data.pan_speed_dps = 0.0;
        data.roll_deg = data.roll_gimbal_deg;
        data.tilt_deg = 0;
        data.tilt_speed_dps = 0;

        return data;
    }

    return spidercam::sPosition_1_t();
}

int encode_spidercam_end_pos(const spidercam::sPosition_1_t& pos, net_buffer& buffer)
{
    Spidercam_End_Position_1 pckt;

    pckt.set_datavalid(true);
    pckt.set_x_mm(pos.X_mm);
    pckt.set_y_mm(pos.Y_mm);
    pckt.set_z_mm(pos.Z_mm);
    pckt.set_pan_deg(pos.pan_deg);
    pckt.set_pitch_deg(pos.pitch_deg);
    pckt.set_roll_deg(pos.roll_gimbal_deg);
    pckt.set_timestamp(pos.timestamp);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::SPIDER_CAM_END_POSITION);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}


/*
 * Weather Packets
 */
sWindData_t to_wind_data_1(std::uint16_t length, const net_buffer_view& buffer)
{
    WindData_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
    {
        sWindData_t data;

        data.dataValid = pckt.datavalid();
        data.wind_speed_mps = pckt.wind_speed_mps();
        data.wind_direction_deg = pckt.wind_direction_deg();

        return data;
    }

    return sWindData_t();
}

int encode_wind_data(bool valid, double wind_speed_mps, double wind_direction_deg, net_buffer& buffer)
{
    WindData_1 pckt;

    pckt.set_datavalid(valid);
    pckt.set_wind_speed_mps(wind_speed_mps);
    pckt.set_wind_direction_deg(wind_direction_deg);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::WIND_DATA);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}


float to_temperature_data_1(std::uint16_t length, const net_buffer_view& buffer)
{
    TemperatureData_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
        return pckt.temperature_c();
    return -9999;
}

int encode_temperature_data(double temp_C, net_buffer& buffer)
{
    TemperatureData_1 pckt;

    pckt.set_temperature_c(temp_C);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::TEMPERATURE_DATA);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

float to_relative_humidity_data_1(std::uint16_t length, const net_buffer_view& buffer)
{
    RelativeHumidityData_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
        return pckt.rh_pct();
    return -9999;
}

int encode_relative_humidity_data(double rh_pct, net_buffer& buffer)
{
    RelativeHumidityData_1 pckt;

    pckt.set_rh_pct(rh_pct);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::RELATIVE_HUMIDITY_DATA);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

float to_par_data_1(std::uint16_t length, const net_buffer_view& buffer)
{
    ParData_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
        return pckt.par_umole();
    return -9999;
}

int encode_par_data(double par_umole, net_buffer& buffer)
{
    ParData_1 pckt;

    pckt.set_par_umole(par_umole);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::PAR_DATA);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

sWeatherData_t to_weather_data_1(std::uint16_t length, const net_buffer_view& buffer)
{
    WeatherData_1 pckt;
    if (pckt.ParseFromArray(buffer.data(), length))
    {
        sWeatherData_t data;

        data.wind_data_valid = pckt.wind_data_valid();
        data.wind_speed_mps = pckt.wind_speed_mps();
        data.wind_direction_deg = pckt.wind_direction_deg();
        data.temp_C = pckt.temperature_c();
        data.rh_pct = pckt.rh_pct();
        data.par_umole = pckt.par_umole();

        return data;
    }

    return sWeatherData_t();
}

int encode_weather_data(bool wind_data_valid, double wind_speed_mps, double wind_direction_deg,
    double temp_C, double rh_pct, double par_umole, net_buffer& buffer)
{
    WeatherData_1 pckt;

    pckt.set_wind_data_valid(wind_data_valid);
    pckt.set_wind_speed_mps(wind_speed_mps);
    pckt.set_wind_direction_deg(wind_direction_deg);
    pckt.set_temperature_c(temp_C);
    pckt.set_rh_pct(rh_pct);
    pckt.set_par_umole(par_umole);

    std::string str;
    if (!pckt.SerializeToString(&str))
        return -1;

    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::WEATHER_DATA);
    hdr.revision = 1;
    hdr.length = str.length();
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;
    buffer.write(str);

    return pckt_size;
}

bool to_marker_reference_start_1(std::uint16_t length, const net_buffer_view& buffer)
{
    return length == 0;
}

int encode_marker_reference_start(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::MARKER_REFERENCE_START);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}

bool to_marker_reference_end_1(std::uint16_t length, const net_buffer_view& buffer)
{
    return length == 0;
}

int encode_marker_reference_end(net_buffer& buffer)
{
    sPacketHeader_t hdr;
    hdr.id = static_cast<uint16_t>(ePacketType::MARKER_REFERENCE_END);
    hdr.revision = 1;
    hdr.length = 0;
    set_timestamp(&hdr.timestamp);

    int pckt_size = sizeof(sPacketHeader_t) + hdr.length;

    if (buffer.write_size() < pckt_size)
        return -pckt_size;

    buffer << hdr;

    return pckt_size;
}
