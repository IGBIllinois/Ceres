
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


/********************************************************************
 * Marker the Start Position of a Reference Measurement
 *******************************************************************/
cExperimentState_MarkStartReference::cExperimentState_MarkStartReference(iMarkerStartReference* pMarkerStartReference)
	: mpMarkerStartReference(pMarkerStartReference)
{
}

QString cExperimentState_MarkStartReference::getStatusStr()
{
	QString msg = "Marking the start position of a reference measurement...";
	return msg;
}

void cExperimentState_MarkStartReference::run()
{
	if (mpMarkerStartReference)
		mpMarkerStartReference->recordStartReference();
}


/********************************************************************
 * Marker the End Position of a Reference Measurement
 *******************************************************************/
cExperimentState_MarkEndReference::cExperimentState_MarkEndReference(iMarkerEndReference* pMarkerEndReference)
	: mpMarkerEndReference(pMarkerEndReference)
{
}

QString cExperimentState_MarkEndReference::getStatusStr()
{
	QString msg = "Marking the end position of a reference measurement...";
	return msg;
}

void cExperimentState_MarkEndReference::run()
{
	if (mpMarkerEndReference)
		mpMarkerEndReference->recordEndReference();
}


