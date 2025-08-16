
#pragma once



/**
 * Interfaces for signaling the beginning and ending of a measurement scan
 */
class iMarkerStartPosition
{
public:
	virtual void recordStartPosition() = 0;

protected:
	iMarkerStartPosition() = default;
	virtual ~iMarkerStartPosition() = default;
};


class iMarkerEndPosition
{
public:
	virtual void recordEndPosition() = 0;

protected:
	iMarkerEndPosition() = default;
	virtual ~iMarkerEndPosition() = default;
};


/**
 * Interfaces for signaling the beginning and ending of a reference scan
 */
class iMarkerStartReference
{
public:
	virtual void recordStartReference() = 0;

protected:
	iMarkerStartReference() = default;
	virtual ~iMarkerStartReference() = default;
};


class iMarkerEndReference
{
public:
	virtual void recordEndReference() = 0;

protected:
	iMarkerEndReference() = default;
	virtual ~iMarkerEndReference() = default;
};

