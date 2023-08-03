

#include "ceres_net_encoder.hpp"
#include "remote_client_utils.hpp"


cCeresNetEncoder::cCeresNetEncoder(std::size_t capacity)
    : cNetworkEncoder(capacity)
{}


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


