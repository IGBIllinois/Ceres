/*
 * The HySpexSWIR_384_PropertyPage to control the HySpex SWIR 384.
 */

#pragma once

#include "HySpexCamera_PropertyPage.hpp"

 // Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QListWidget;
class QTextEdit;
class QLineEdit;
class QComboBox;
class QGroupBox;
QT_END_NAMESPACE


class cHySpexSWIR_384_PropertyPage : public cHySpexCamera_PropertyPage
{
public:
    cHySpexSWIR_384_PropertyPage(QWidget* parent = nullptr);
    ~cHySpexSWIR_384_PropertyPage() = default;

    void doLayout() override;

public:
    cExperimentState* createState(const std::string& type) override;
};


