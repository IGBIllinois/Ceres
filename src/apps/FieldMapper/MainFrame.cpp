
#include "MainFrame.hpp"
#include "MainWindow.hpp"

#include "RappFieldBoundary.hpp"
#include "GpsFileReader.hpp"

#include <wx/aui/framemanager.h>
#include <wx/aui/auibook.h>
#include <wx/aboutdlg.h>
#include <wx/thread.h>

#include <cbdf/BlockDataFile.hpp>

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	// Custom submenu items
	ID_IMPORT_MENU = wxID_HIGHEST,
	ID_IMPORT_GPS_CSV,
	ID_IMPORT_POINTCLOUD,
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(cMainFrame, wxFrame)
	EVT_MENU(wxID_OPEN, cMainFrame::OnFileOpen)
	EVT_MENU(ID_IMPORT_GPS_CSV, cMainFrame::OnFileImportGpsPoints)
	EVT_MENU(ID_IMPORT_POINTCLOUD, cMainFrame::OnFileImportPointcloud)
	EVT_MENU(wxID_SAVE, cMainFrame::OnFileExport)
	EVT_MENU(wxID_EXIT, cMainFrame::OnQuit)
	EVT_MENU(wxID_ABOUT, cMainFrame::OnAbout)
	EVT_CLOSE(cMainFrame::OnClose)
wxEND_EVENT_TABLE()



//-----------------------------------------------------------------------------
cMainFrame::cMainFrame()
	: wxFrame(NULL, wxID_ANY, "RAPP Field Mapper")
{
	mpMainWindow = new cMainWindow(this);

	cRappFieldBoundary field;

	// set the frame icon
	SetIcon(wxICON(ceresinfo));

#if wxUSE_MENUBAR
	// create a menu bar
	wxMenu* fileMenu = new wxMenu;

	// build the menus
// ----------------

	wxMenuItem* open_file = fileMenu->Append(wxID_OPEN);
	fileMenu->AppendSeparator();

	wxMenu* import_menu = new wxMenu;
	import_menu->Append(ID_IMPORT_GPS_CSV, "Import GPS CSV File...");
	import_menu->Append(ID_IMPORT_POINTCLOUD, "Import Pointcloud...");

	auto import_file = fileMenu->Append(wxID_ANY, "&Import\tCtrl+I", import_menu);

	fileMenu->AppendSeparator();
	wxMenuItem* export_file = fileMenu->Append(wxID_SAVE, "&Export\tCtrl+E", "Export data to text file");
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");

	// the "About" item should be in the help menu
	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT, "&About...", "Show about dialog");

	// now append the freshly created menu to the menu bar...
	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");

	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);

#else // !wxUSE_MENUBAR
	// If menus are not available add a button to access the about box
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* aboutBtn = new wxButton(this, wxID_ANY, "About...");
	aboutBtn->Bind(wxEVT_BUTTON, &MainFrame::OnAbout, this);
	sizer->Add(aboutBtn, wxSizerFlags().Center());
	SetSizer(sizer);
#endif // wxUSE_MENUBAR/!wxUSE_MENUBAR

	// create a status bar just for fun (by default with 1 pane only)
	CreateStatusBar();

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(mpMainWindow, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 0));

	sizer->Layout();
	SetSizerAndFit(sizer);

//	SetBackgroundColour(GetMenuBar()->GetBackgroundColour());
//	SetBackgroundStyle(wxBG_STYLE_SYSTEM);

	auto size = GetBestSize();
	size.SetWidth(1250);
	size.SetHeight(650);
	SetMinSize(size);

	Centre();

	mpMainWindow->Initialize();
}

cMainFrame::~cMainFrame()
{
	if (mpMainWindow) mpMainWindow->Delete();
}


// event handlers

void cMainFrame::OnFileOpen(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog
		openFileDialog(this, _("Open Ceres file"), "", "",
			"Ceres files (*.ceres)|*.ceres", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed their mind...

	mFilename = openFileDialog.GetPath().ToStdString();
}

void cMainFrame::OnFileImportGpsPoints(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog
		openFileDialog(this, _("Open CSV file"), "", "",
			"CSV files (*.csv)|*.csv", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed their mind...

	auto filename = openFileDialog.GetPath().ToStdString();
	cGpsFileReader reader;
	reader.loadFromFile(filename);

	if (!reader.empty())
		mDataModel.append(reader.gpsPoints());
}

void cMainFrame::OnFileImportPointcloud(wxCommandEvent& WXUNUSED(event))
{
	mpMainWindow->Render();
}

void cMainFrame::OnFileExport(wxCommandEvent& WXUNUSED(event))
{
}

void cMainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}

void cMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxAboutDialogInfo info;
	info.SetName(wxTheApp->GetAppDisplayName());
	info.SetVersion("1.0", "0.1.0");
	info.SetDescription(_("   The RAPP Field Mapper is used to generate the field ground map from GPS and pointcloud data.   \n"));
	info.SetCopyright(wxT("Copyright (c) 2022, Carl R. Woese Institute for Genomic Biology,\n"
		"University of Illinois.\n"
		"All rights reserved.\n"));
	info.SetIcon(wxICON(ceresinfo));
	info.AddDeveloper("Brett Feddersen");
	info.SetLicense("BSD 3 - Clause License\n"
		"\n"
		"This license applies to all files in the RappFieldMapper repository and source\n"
		"distribution.  This includes RappFieldMapper’s source code, the examples, and\n"
		"tests, as well as the documentation.\n"
		"\n"
		"Copyright(c) 2022, Carl R.Woese Institute for Genomic Biology\n"
		"All rights reserved.\n"
		"\n"
		"Redistribution and use in source and binary forms, with or without\n"
		"modification, are permitted provided that the following conditions are met :\n"
		"\n"
		"1. Redistributions of source code must retain the above copyright notice, this\n"
		"   list of conditions and the following disclaimer.\n"

		"2. Redistributions in binary form must reproduce the above copyright notice,\n"
		"   this list of conditions and the following disclaimer in the documentation\n"
		"   and /or other materials provided with the distribution.\n"
		"\n"
		"3. Neither the name of the copyright holder nor the names of its\n"
		"   contributors may be used to endorse or promote products derived from\n"
		"   this software without specific prior written permission.\n"
		"\n"
		"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n"
		"AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n"
		"IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
		"DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE\n"
		"FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n"
		"DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n"
		"SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER\n"
		"CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,\n"
		"OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n"
		"OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.");

	wxAboutBox(info);
}

void cMainFrame::OnClose(wxCloseEvent&)
{
	Destroy();
}



