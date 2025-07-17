
#include "MarkerExperimentStates.hpp"


/********************************************************************
 * Marker the Start Position of a Measurement
 *******************************************************************/
cExperimentState_MarkStartPosition::cExperimentState_MarkStartPosition(iMarkerStartPosition* pMarkerStartPosition)
	: mpMarkerStartPosition(pMarkerStartPosition)
{}

QString cExperimentState_MarkStartPosition::getStatusStr()
{
	QString msg = "Marking the start position...";
	return msg;
}

void cExperimentState_MarkStartPosition::run()
{
	if (mpMarkerStartPosition)
		mpMarkerStartPosition->recordStartPosition();
}


/********************************************************************
 * Marker the End Position of a Measurement
 *******************************************************************/
cExperimentState_MarkEndPosition::cExperimentState_MarkEndPosition(iMarkerEndPosition* pMarkerEndPosition)
	: mpMarkerEndPosition(pMarkerEndPosition)
{
}

QString cExperimentState_MarkEndPosition::getStatusStr()
{
	QString msg = "Marking the end position...";
	return msg;
}

void cExperimentState_MarkEndPosition::run()
{
	if (mpMarkerEndPosition)
		mpMarkerEndPosition->recordEndPosition();
}


