
#include "ExperimentMetaInfo.hpp"

namespace
{
    std::tuple<std::string, std::string, std::string> date_split(const std::string& date)
    {
        std::array<std::string, 3> results;
        int slot = 0;
        std::size_t startPos = 0;
        std::size_t lastPos = date.find('/');
        while ((lastPos != std::string::npos) && (slot < 2))
        {
            results[slot++] = date.substr(startPos, lastPos - startPos);
            startPos = lastPos + 1;
            lastPos = date.find('/', startPos);
        }

        results[slot++] = date.substr(startPos, date.size() - startPos);

        return { results[0], results[1], results[2] };
    }
}


cExperimentMetaInfo::~cExperimentMetaInfo()
{}

void cExperimentMetaInfo::clear()
{
	mDirty = false;

	mPrincipalInvestigator.clear();
	mResearchers.clear();
	mComments.clear();

	mSpecies.clear();
	mCultivar.clear();
	mEvents.clear();
	mConstructName.clear();
	mTreatments.clear();

	mFieldDesign.clear();

    mPlantingDay.clear();
    mPlantingMonth.clear();
    mPlantingYear.clear();

    mTargetHarvestDay.clear();
    mTargetHarvestMonth.clear();
    mTargetHarvestYear.clear();

	mAuthorization.clear();
    mPermit.clear();
}

bool cExperimentMetaInfo::isDirty() const
{
	return mDirty;
}

const std::string& cExperimentMetaInfo::getPrincipalInvestigator() const
{
	return mPrincipalInvestigator;
}

const std::vector<std::string>& cExperimentMetaInfo::getResearchers() const
{
	return mResearchers;
}

const std::vector<std::string>& cExperimentMetaInfo::getComments() const
{
	return mComments;
}

const std::string& cExperimentMetaInfo::getSpecies() const
{
	return mSpecies;
}

const std::string& cExperimentMetaInfo::getCultivar() const
{
	return mCultivar;
}

const std::vector<std::string>& cExperimentMetaInfo::getEvents() const
{
	return mEvents;
}

const std::string& cExperimentMetaInfo::getConstructName() const
{
	return mConstructName;
}

const std::vector<std::string>& cExperimentMetaInfo::getTreatments() const
{
	return mTreatments;
}

const std::string& cExperimentMetaInfo::getFieldDesign() const
{
	return mFieldDesign;
}

std::string cExperimentMetaInfo::getPlantingDate_ymd() const
{
    std::string date = mPlantingYear;
    date += "/" + mPlantingMonth;
    date += "/" + mPlantingDay;

    return date;
}

const std::string& cExperimentMetaInfo::getPlantingDay() const
{
	return mPlantingDay;
}

const std::string& cExperimentMetaInfo::getPlantingMonth() const
{
    return mPlantingMonth;
}

const std::string& cExperimentMetaInfo::getPlantingYear() const
{
    return mPlantingYear;
}

std::string cExperimentMetaInfo::getTargetHarvestDate_ymd() const
{
    std::string date = mTargetHarvestYear;
    date += "/" + mTargetHarvestMonth;
    date += "/" + mTargetHarvestDay;

    return date;
}

const std::string& cExperimentMetaInfo::getTargetHarvestDay() const
{
	return mTargetHarvestDay;
}

const std::string& cExperimentMetaInfo::getTargetHarvestMonth() const
{
    return mTargetHarvestMonth;
}

const std::string& cExperimentMetaInfo::getTargetHarvestYear() const
{
    return mTargetHarvestYear;
}

const std::string& cExperimentMetaInfo::getAuthorization() const
{
    return mAuthorization;
}

const std::string& cExperimentMetaInfo::getPermitInfo() const
{
	return mPermit;
}


void cExperimentMetaInfo::setPrincipalInvestigator(const std::string& pi)
{
    mDirty |= mPrincipalInvestigator != pi;
    mPrincipalInvestigator = pi;
}

void cExperimentMetaInfo::setResearchers(const std::vector<std::string>& researchers)
{
    mDirty |= mResearchers != researchers;
    mResearchers = researchers;
}

void cExperimentMetaInfo::setComments(const std::vector<std::string>& comments)
{
    mDirty |= mComments != comments;
    mComments = comments;
}

void cExperimentMetaInfo::setSpecies(const std::string& species)
{
    mDirty |= mSpecies != species;
    mSpecies = species;
}

void cExperimentMetaInfo::setCultivar(const std::string& cultivar)
{
    mDirty |= mCultivar != cultivar;
    mCultivar = cultivar;
}

void cExperimentMetaInfo::setEvents(const std::vector<std::string>& events)
{
    mDirty |= mEvents != events;
    mEvents = events;
}

void cExperimentMetaInfo::setConstructName(const std::string& construct)
{
    mDirty |= mConstructName != construct;
    mConstructName = construct;
}

void cExperimentMetaInfo::setTreatments(const std::vector<std::string>& treatments)
{
    mDirty |= mTreatments != treatments;
    mTreatments = treatments;
}

void cExperimentMetaInfo::setFieldDesign(const std::string& design)
{
    mDirty |= mFieldDesign != design;
    mFieldDesign = design;
}

void cExperimentMetaInfo::setPlantingDay(const std::string& day)
{
    mDirty |= mPlantingDay != day;
    mPlantingDay = day;
}

void cExperimentMetaInfo::setPlantingMonth(const std::string& month)
{
    mDirty |= mPlantingMonth != month;
    mPlantingMonth = month;
}

void cExperimentMetaInfo::setPlantingYear(const std::string& year)
{
    mDirty |= mPlantingYear != year;
    mPlantingYear = year;
}

void cExperimentMetaInfo::setTargetHarvestDay(const std::string& day)
{
    mDirty |= mTargetHarvestDay != day;
    mTargetHarvestDay = day;
}

void cExperimentMetaInfo::setTargetHarvestMonth(const std::string& month)
{
    mDirty |= mTargetHarvestMonth != month;
    mTargetHarvestMonth = month;
}

void cExperimentMetaInfo::setTargetHarvestYear(const std::string& year)
{
    mDirty |= mTargetHarvestYear != year;
    mTargetHarvestYear = year;
}

void cExperimentMetaInfo::setPermitInfo(const std::string& permit)
{
    mDirty |= mPermit != permit;
    mPermit = permit;
}

void cExperimentMetaInfo::setPermitInfo(const std::string& authorization, const std::string& permit)
{
    mDirty |= mAuthorization != authorization;
    mDirty |= mPermit != permit;
    mAuthorization = authorization;
    mPermit = permit;
}

bool cExperimentMetaInfo::operator!=(const cExperimentMetaInfo& rhs) const
{
    return (mDirty != rhs.mDirty) ||
        (mPrincipalInvestigator != rhs.mPrincipalInvestigator) ||
        (mResearchers != rhs.mResearchers) ||
        (mComments != rhs.mComments) ||
        (mSpecies != rhs.mSpecies) ||
        (mCultivar != rhs.mCultivar) ||
        (mEvents != rhs.mEvents) ||
        (mConstructName != rhs.mConstructName) ||
        (mTreatments != rhs.mTreatments) ||
        (mFieldDesign != rhs.mFieldDesign) ||
        (mPlantingDay != rhs.mPlantingDay) ||
        (mPlantingMonth != rhs.mPlantingMonth) ||
        (mPlantingYear != rhs.mPlantingYear) ||
        (mTargetHarvestDay != rhs.mTargetHarvestDay) ||
        (mTargetHarvestMonth != rhs.mTargetHarvestMonth) ||
        (mTargetHarvestYear != rhs.mTargetHarvestYear) ||
        (mAuthorization != rhs.mAuthorization) ||
        (mPermit != rhs.mPermit);
}


void cExperimentMetaInfo::load(const nlohmann::json& jdoc)
{
    if (jdoc.contains("principal investigator"))
    {
        mPrincipalInvestigator = jdoc["principal investigator"];
    }

    if (jdoc.contains("researcher"))
    {
        mResearchers.push_back(jdoc["researcher"]);
    }

    if (jdoc.contains("researchers"))
    {
        auto researchers = jdoc["researchers"];
        for (auto it = researchers.begin(); it != researchers.end(); ++it)
            mResearchers.push_back(*it);
    }

    if (jdoc.contains("species"))
    {
        mSpecies = jdoc["species"];
    }

    if (jdoc.contains("cultivar"))
    {
        mCultivar = jdoc["cultivar"];
    }

    if (jdoc.contains("permit info"))
    {
        auto permit_info = jdoc["permit info"];
        if (permit_info.is_string())
        {
            mPermit = permit_info;
        }
        else if (permit_info.is_object())
        {
            mAuthorization = permit_info["authorization"];
            mPermit = permit_info["permit"];
        }
    }

    if (jdoc.contains("construct"))
    {
        mConstructName = jdoc["construct"];
    }

    if (jdoc.contains("event number"))
    {
        mEvents.push_back(jdoc["event number"]);
    }

    if (jdoc.contains("event numbers"))
    {
        auto event_numbers = jdoc["event numbers"];
        for (auto it = event_numbers.begin(); it != event_numbers.end(); ++it)
            mEvents.push_back(*it);
    }

    if (jdoc.contains("field design"))
    {
        mFieldDesign = jdoc["field design"];
    }

    if (jdoc.contains("treatment"))
    {
        mTreatments.push_back(jdoc["treatment"]);
    }

    if (jdoc.contains("treatments"))
    {
        auto treatments = jdoc["treatments"];
        if (treatments.is_string())
        {
            mTreatments.push_back(treatments);
        }
        else if (treatments.is_array())
        {
            for (auto it = treatments.begin(); it != treatments.end(); ++it)
                mTreatments.push_back(*it);
        }
    }

    if (jdoc.contains("comment"))
    {
        mComments.push_back(jdoc["comment"]);
    }

    if (jdoc.contains("comments"))
    {
        auto comments = jdoc["comments"];
        if (comments.is_string())
        {
            mComments.push_back(comments);
        }
        else if (comments.is_array())
        {
            for (auto it = comments.begin(); it != comments.end(); ++it)
                mComments.push_back(*it);
        }
    }

    if (jdoc.contains("planting date (m/d/y)"))
    {
        std::string date = jdoc["planting date (m/d/y)"];
        std::tie(mPlantingMonth, mPlantingDay, mPlantingYear) = date_split(date);
    }

    if (jdoc.contains("planting date (d/m/y)"))
    {
        std::string date = jdoc["planting date (d/m/y)"];
        std::tie(mPlantingDay, mPlantingMonth, mPlantingYear) = date_split(date);
    }

    if (jdoc.contains("planting date (y/m/d)"))
    {
        std::string date = jdoc["planting date (y/m/d)"];
        std::tie(mPlantingYear, mPlantingMonth, mPlantingDay) = date_split(date);
    }

    if (jdoc.contains("target harvest date (m/d/y)"))
    {
        std::string date = jdoc["target harvest date (m/d/y)"];
        std::tie(mTargetHarvestMonth, mTargetHarvestDay, mTargetHarvestYear) = date_split(date);
    }

    if (jdoc.contains("target harvest date (d/m/y)"))
    {
        std::string date = jdoc["target harvest date (d/m/y)"];
        std::tie(mTargetHarvestDay, mTargetHarvestMonth, mTargetHarvestYear) = date_split(date);
    }

    if (jdoc.contains("target harvest date (y/m/d)"))
    {
        std::string date = jdoc["target harvest date (y/m/d)"];
        std::tie(mTargetHarvestYear, mTargetHarvestMonth, mTargetHarvestDay) = date_split(date);
    }
}

void cExperimentMetaInfo::save(nlohmann::json& jdoc)
{
    if (!mPrincipalInvestigator.empty())
    {
        jdoc["principal investigator"] = mPrincipalInvestigator;
    }

    if (mResearchers.size() == 1)
    {
        jdoc["researcher"] = mResearchers.front();
    }
    else if (mResearchers.size() > 1)
    {
        nlohmann::json researchers;
        for (auto it = mResearchers.begin(); it != mResearchers.end(); ++it)
            researchers.push_back(*it);
        jdoc["researchers"] = researchers;
    }

    if (!mSpecies.empty())
    {
        jdoc["species"] = mSpecies;
    }

    if (!mCultivar.empty())
    {
        jdoc["cultivar"] = mCultivar;
    }

    if (!mAuthorization.empty() && !mPermit.empty())
    {
        nlohmann::json permit_info;
        permit_info["authorization"] = mAuthorization;
        permit_info["permit"] = mPermit;
        jdoc["permit info"] = permit_info;
    }
    else if (!mPermit.empty())
    {
        jdoc["permit info"] = mPermit;
    }

    if (!mConstructName.empty())
    {
        jdoc["construct"] = mConstructName;
    }

    if (mEvents.size() == 1)
    {
        jdoc["event number"] = mEvents.front();
    }

    if (mEvents.size() > 1)
    {
        nlohmann::json event_numbers;

        for (auto it = mEvents.begin(); it != mEvents.end(); ++it)
            event_numbers.push_back(*it);

        jdoc["event numbers"] = event_numbers;
    }

    if (!mFieldDesign.empty())
    {
        jdoc["field design"] = mFieldDesign;
    }

    if (mTreatments.size() == 1)
    {
        jdoc["treatment"] = mTreatments.front();
    }

    if (mTreatments.size() > 1)
    {
        nlohmann::json treatments;

        for (auto it = mTreatments.begin(); it != mTreatments.end(); ++it)
            treatments.push_back(*it);

        jdoc["treatments"] = treatments;
    }

    if (mComments.size() == 1)
    {
        jdoc["comment"] = mComments.front();
    }

    if (mComments.size() > 1)
    {
        nlohmann::json comments;

        for (auto it = mComments.begin(); it != mComments.end(); ++it)
            comments.push_back(*it);

        jdoc["comments"] = comments;
    }

    if ( !(mPlantingYear.empty() || mPlantingMonth.empty() || mPlantingDay.empty()))
    {
        std::string date = mPlantingYear;
        date += "/" + mPlantingMonth;
        date += "/" + mPlantingDay;

        jdoc["planting date (y/m/d)"] = date;
    }

    if (!(mTargetHarvestYear.empty() || mTargetHarvestMonth.empty() || mTargetHarvestDay.empty()))
    {
        std::string date = mTargetHarvestYear;
        date += "/" + mTargetHarvestMonth;
        date += "/" + mTargetHarvestDay;

        jdoc["target harvest date (y/m/d)"] = date;
    }

    mDirty = false;
}

