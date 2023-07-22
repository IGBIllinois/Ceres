/*
 * The HySpexVNIR_3000N_PropertyPage to control the HySpex VNIR 3000N.
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


class cHySpexVNIR_3000N_PropertyPage : public cHySpexCamera_PropertyPage
{
public:
    cHySpexVNIR_3000N_PropertyPage(QWidget* parent = nullptr);
    ~cHySpexVNIR_3000N_PropertyPage() = default;

    void doLayout() override;
};


