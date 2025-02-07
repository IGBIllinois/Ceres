

#include "ceres_net_encoder.hpp"
#include "remote_client_utils.hpp"


cCeresNetEncoder::cCeresNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}

void cCeresNetEncoder::encodeExperimentType(const std::string& type)
{
    if (encode_experiment_type(type, mBuffer) < 0)
    {
        sendData();
        encode_experiment_type(type, mBuffer);
    }
}

void cCeresNetEncoder::encodeExperimentInfo(const std::string& title, const std::string& researcher,
    const std::string& cultivar, const std::string& doc)
{
    if (encode_exp_info_data(title, researcher, cultivar, doc, mBuffer) < 0)
    {
        sendData();
        encode_exp_info_data(title, researcher, cultivar, doc, mBuffer);
    }
}

void cCeresNetEncoder::encodeExperimentInfo(const std::string& title, const std::string& researcher,
    const std::string& species, const std::string& cultivar, const std::string& doc)
{
    if (encode_exp_info_data(title, researcher, species, cultivar, doc, mBuffer) < 0)
    {
        sendData();
        encode_exp_info_data(title, researcher, species, cultivar, doc, mBuffer);
    }
}

void cCeresNetEncoder::encodeTitle(const std::string& title)
{
    if (encode_experiment_title(title, mBuffer) < 0)
    {
        sendData();
        encode_experiment_title(title, mBuffer);
    }
}

void cCeresNetEncoder::encodeDocument(const std::string& doc)
{
    if (encode_experiment_document(doc, mBuffer) < 0)
    {
        sendData();
        encode_experiment_document(doc, mBuffer);
    }
}

void cCeresNetEncoder::encodeSpecies(const std::string& species)
{
    if (encode_species(species, mBuffer) < 0)
    {
        sendData();
        encode_species(species, mBuffer);
    }
}

void cCeresNetEncoder::encodeCultivar(const std::string& cultivar)
{
    if (encode_cultivar(cultivar, mBuffer) < 0)
    {
        sendData();
        encode_cultivar(cultivar, mBuffer);
    }
}

void cCeresNetEncoder::encodePrincipalInvestigator(const std::string& pi)
{
    if (encode_principal_investigator(pi, mBuffer) < 0)
    {
        sendData();
        encode_principal_investigator(pi, mBuffer);
    }
}

void cCeresNetEncoder::encodeResearcher(const std::string& researcher)
{
    if (encode_researcher(researcher, mBuffer) < 0)
    {
        sendData();
        encode_researcher(researcher, mBuffer);
    }
}

void cCeresNetEncoder::encodeResearchers(const std::vector<std::string>& researchers)
{
    if (encode_start_of_researcher_list(mBuffer) < 0)
    {
        sendData();
        encode_start_of_event_number_list(mBuffer);
    }

    for (auto researcher : researchers)
    {
        if (encode_researcher(researcher, mBuffer) < 0)
        {
            sendData();
            encode_researcher(researcher, mBuffer);
        }
    }

    if (encode_end_of_researcher_list(mBuffer) < 0)
    {
        sendData();
        encode_end_of_event_number_list(mBuffer);
    }
}

void cCeresNetEncoder::encodeConstructName(const std::string& name)
{
    if (encode_construct_name(name, mBuffer) < 0)
    {
        sendData();
        encode_construct_name(name, mBuffer);
    }
}

void cCeresNetEncoder::encodeEventNumbers(const std::vector<std::string>& event_numbers)
{
    if (encode_start_of_event_number_list(mBuffer) < 0)
    {
        sendData();
        encode_start_of_event_number_list(mBuffer);
    }

    for (auto event_number : event_numbers)
    {
        if (encode_event_number(event_number, mBuffer) < 0)
        {
            sendData();
            encode_event_number(event_number, mBuffer);
        }
    }

    if (encode_end_of_event_number_list(mBuffer) < 0)
    {
        sendData();
        encode_end_of_event_number_list(mBuffer);
    }
}

void cCeresNetEncoder::encodeFieldDesign(const std::string& design)
{
    if (encode_field_design(design, mBuffer) < 0)
    {
        sendData();
        encode_field_design(design, mBuffer);
    }
}

void cCeresNetEncoder::encodePlantingDate(std::time_t date)
{
    if (encode_planting_date(date, mBuffer) < 0)
    {
        sendData();
        encode_planting_date(date, mBuffer);
    }
}

void cCeresNetEncoder::encodeHarvestDate(std::time_t date)
{
    if (encode_harvest_date(date, mBuffer) < 0)
    {
        sendData();
        encode_harvest_date(date, mBuffer);
    }
}

void cCeresNetEncoder::encodeTreatments(const std::vector<std::string>& treatments)
{
    if (encode_start_of_treatment_list(mBuffer) < 0)
    {
        sendData();
        encode_start_of_treatment_list(mBuffer);
    }

    for (auto treatment : treatments)
    {
        if (encode_treatment(treatment, mBuffer) < 0)
        {
            sendData();
            encode_treatment(treatment, mBuffer);
        }
    }

    if (encode_end_of_treatment_list(mBuffer) < 0)
    {
        sendData();
        encode_end_of_treatment_list(mBuffer);
    }
}

void cCeresNetEncoder::encodeComments(const std::vector<std::string>& comments)
{
    if (encode_start_of_comment_list(mBuffer) < 0)
    {
        sendData();
        encode_start_of_comment_list(mBuffer);
    }

    for (auto comment : comments)
    {
        if (encode_comment(comment, mBuffer) < 0)
        {
            sendData();
            encode_comment(comment, mBuffer);
        }
    }

    if (encode_end_of_comment_list(mBuffer) < 0)
    {
        sendData();
        encode_end_of_comment_list(mBuffer);
    }
}

void cCeresNetEncoder::encodePermitInfo(const std::string& permit)
{
    if (encode_permit_info(permit, mBuffer) < 0)
    {
        sendData();
        encode_permit_info(permit, mBuffer);
    }
}

void cCeresNetEncoder::encodeSpidercamPosition(const spidercam::sPosition_1_t& pos)
{
    if (encode_spidercam_pos(pos, mBuffer) < 0)
    {
        sendData();
        encode_spidercam_pos(pos, mBuffer);
    }
}

void cCeresNetEncoder::encodeWindData(bool valid, double wind_speed_mps, double wind_direction_deg)
{
    if (encode_wind_data(valid, wind_speed_mps, wind_direction_deg, mBuffer) < 0)
    {
        sendData();
        encode_wind_data(valid, wind_speed_mps, wind_direction_deg, mBuffer);
    }
}

void cCeresNetEncoder::encodeTemperatureData(double temp_C)
{
    if (encode_temperature_data(temp_C, mBuffer) < 0)
    {
        sendData();
        encode_temperature_data(temp_C, mBuffer);
    }
}

void cCeresNetEncoder::encodeRelativeHumidityData(double rh_pct)
{
    if (encode_relative_humidity_data(rh_pct, mBuffer) < 0)
    {
        sendData();
        encode_relative_humidity_data(rh_pct, mBuffer);
    }
}

void cCeresNetEncoder::encodeParData(double par_umole)
{
    if (encode_par_data(par_umole, mBuffer) < 0)
    {
        sendData();
        encode_par_data(par_umole, mBuffer);
    }
}

void cCeresNetEncoder::encodeEndOfExperimentInfo()
{
    if (encode_end_of_experiment_info(mBuffer) < 0)
    {
        sendData();
        encode_end_of_experiment_info(mBuffer);
    }
}

void cCeresNetEncoder::sendExperimentType(const std::string& type)
{
    encode_experiment_type(type, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendExperimentInfo(const std::string& title, const std::string& researcher,
    const std::string& cultivar, const std::string& doc)
{
    encode_exp_info_data(title, researcher, cultivar, doc, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendExperimentInfo(const std::string& title, const std::string& researcher,
    const std::string& species, const std::string& cultivar, const std::string& doc)
{
    encode_exp_info_data(title, researcher, species, cultivar, doc, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendPrincipalInvestigator(const std::string& pi)
{
    encode_principal_investigator(pi, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendResearcher(const std::string& researcher)
{
    encode_researcher(researcher, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendConstructName(const std::string& name)
{
    encode_construct_name(name, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendEventNumber(const std::string& event_num)
{
    encode_event_number(event_num, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendFieldDesign(const std::string& design)
{
    encode_field_design(design, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendPlantingDate(std::time_t date)
{
    encode_planting_date(date, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendHarvestDate(std::time_t date)
{
    encode_harvest_date(date, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendTreatment(const std::string& treatment)
{
    encode_treatment(treatment, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendComment(const std::string& comment)
{
    encode_comment(comment, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendPermitInfo(const std::string& permit)
{
    encode_permit_info(permit, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendEndOfExperimentInfo()
{
    encode_end_of_experiment_info(mBuffer);
    sendData();
}

void cCeresNetEncoder::sendStartExperiment()
{
    encode_start_experiment(mBuffer);
    sendData();
}

void cCeresNetEncoder::sendStopExperiment()
{
    encode_stop_experiment(mBuffer);
    sendData();
}

void cCeresNetEncoder::sendOpenDataFile(const std::string& fileName)
{
    encode_open_data_file(fileName, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendCloseDataFile()
{
    encode_close_data_file(mBuffer);
    sendData();
}

void cCeresNetEncoder::sendStartDataRecording()
{
    encode_start_data_recording(mBuffer);
    sendData();
}

void cCeresNetEncoder::sendStopDataRecording()
{
    encode_stop_data_recording(mBuffer);
    sendData();
}


void cCeresNetEncoder::sendSpidercamPosition(const spidercam::sPosition_1_t& pos)
{
    encode_spidercam_pos(pos, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendWindData(bool valid, double wind_speed_mps, double wind_direction_deg)
{
    encode_wind_data(valid, wind_speed_mps, wind_direction_deg, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendTemperatureData(double temp_C)
{
    encode_temperature_data(temp_C, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendRelativeHumidityData(double rh_pct)
{
    encode_relative_humidity_data(rh_pct, mBuffer);
    sendData();
}

void cCeresNetEncoder::sendParData(double par_umole)
{
    encode_par_data(par_umole, mBuffer);
    sendData();
}


