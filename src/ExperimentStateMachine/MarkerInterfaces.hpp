
#pragma once



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

