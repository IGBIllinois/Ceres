
#include "DummyCtrlView.hpp"

#include <QLayout>
#include <QLineEdit>
#include <QLabel>


cDummyControlView::cDummyControlView()
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setMinimumSize(500, 500);
	setMaximumSize(16777215, 16777215);
}

cDummyControlView::~cDummyControlView()
{
}

void cDummyControlView::configure(const nlohmann::json& jsonCfg)
{
}

void cDummyControlView::updateRecordingState(bool recording)
{
}
