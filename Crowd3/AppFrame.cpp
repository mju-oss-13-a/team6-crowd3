/*
 * Copyright (c) 2012, Dennis Damico
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *    * Neither the name of the copyright holder nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "AppFrame.h"

/**
 * Create and initialize the main program window and its components.
 * @param title the window title.
 */
AppFrame::AppFrame(const wxString &title) : wxFrame((wxFrame*) NULL, wxID_ANY, title) {
    this->SetIcon(Icon::getIcon(Icon::iMake));
    
    // Initialize class data: the crowd maker and people finder objects.
    myMaker = NULL;
    myHelp = NULL;
    myPF = new PeopleFinder();
    
    // Initialize the window panels, buttons, and controls.
    initPanels();
    initControls();
    initHandlers();
}

/**
 * Create and initialize two panels in the main window: button (left) panel and 
 * data (right) panel.
 */        
void AppFrame::initPanels() {
    // The button panel width is fixed in size.
    buttonPanel = new wxPanel(this, -1);
    buttonPanel->SetMinSize(wxSize(200, -1));
    buttonPanel->SetMaxSize(buttonPanel->GetMinSize());
    buttonPanel->SetBackgroundColour(WX_COLOR_BACKGROUND);
    
    // The data panel may be resized.
    dataPanel = new wxPanel(this, -1);
    dataPanel->SetMinSize(wxSize(200, 200));
    dataPanel->SetBackgroundColour(wxColour(255, 255, 255));
    
    // Add panels to a horizontal sizer panel with thin border.
    wxBoxSizer *windowSizer = new wxBoxSizer(wxHORIZONTAL);
    windowSizer->Add(buttonPanel, false, wxEXPAND | wxALL, 1);
    windowSizer->Add(dataPanel, true, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 1);
    windowSizer->SetSizeHints(this);
    
    // Add sizer to the main window.
    // Set window position and size to last used or default.
    this->SetSizer(windowSizer);
    this->SetPosition(Settings::getWindowLocation());
    this->SetSize(Settings::getWindowSize());
}

/** Create the controls in the main application window. */
void AppFrame::initControls() {
    helpButton = new wxBitmapButton(buttonPanel, -1, Icon::getBitmap(Icon::iHelp));
    helpButton->SetMinSize(wxSize(40, 40));
    helpButton->SetMaxSize(wxSize(40, 40));
    helpButton->SetToolTip(_T("Help"));
    
    findButton = new wxBitmapButton(buttonPanel, -1, Icon::getBitmap(Icon::iFind));
    findButton->SetMinSize(wxSize(40, 40));
    findButton->SetMaxSize(wxSize(40, 40));
    findButton->SetToolTip(_T("Find people images in a folder"));
    
    makeButton = new wxBitmapButton(buttonPanel, -1, Icon::getBitmap(Icon::iMake));
    makeButton->SetMinSize(wxSize(40, 40));
    makeButton->SetMaxSize(wxSize(40, 40));
    makeButton->SetToolTip(_T("Open the crowd scene maker"));
    
    exitButton = new wxBitmapButton(buttonPanel, -1, Icon::getBitmap(Icon::iExit));
    exitButton->SetMinSize(wxSize(40, 40));
    exitButton->SetMaxSize(wxSize(40, 40));
    exitButton->SetToolTip(_T("Exit the program"));
    
    fileCtrl = ImageTree::create(dataPanel, -1, wxDefaultPosition, wxDefaultSize, 
                wxTR_MULTIPLE | wxTR_EXTENDED | wxTR_HAS_BUTTONS);
    fileCtrl->buildImageTree();
    
    // Create a vertical sizer to stack the buttons.
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(helpButton, false, wxEXPAND | wxALL, 1);
    buttonSizer->Add(findButton, false, wxEXPAND | wxALL, 1);
    buttonSizer->Add(makeButton, false, wxEXPAND | wxALL, 1);
    buttonSizer->Add(exitButton, false, wxEXPAND | wxALL, 1);
    buttonSizer->AddStretchSpacer();
    buttonSizer->SetSizeHints(buttonPanel);

    // Create a data sizer.
    wxBoxSizer *dataSizer = new wxBoxSizer(wxVERTICAL);
    dataSizer->Add(fileCtrl, true, wxEXPAND | wxALL, 1);
    dataSizer->SetSizeHints(dataPanel);
    
    // Add the sizers to their main window panels.
    buttonPanel->SetSizer(buttonSizer);
    dataPanel->SetSizer(dataSizer);
}

/** Create handlers for the control buttons and the window close box. */
void AppFrame::initHandlers() {
   helpButton->Connect(wxEVT_LEFT_DOWN, 
           wxMouseEventHandler(AppFrame::help),            NULL, this);
   findButton->Connect(wxEVT_LEFT_DOWN, 
           wxMouseEventHandler(AppFrame::findPeople),      NULL, this);
   makeButton->Connect(wxEVT_LEFT_DOWN, 
           wxMouseEventHandler(AppFrame::makeCrowdPhoto),  NULL, this);
   exitButton->Connect(wxEVT_LEFT_DOWN, 
           wxMouseEventHandler(AppFrame::exitApplication), NULL, this);
   this->Connect(wxEVT_CLOSE_WINDOW, 
           wxMouseEventHandler(AppFrame::exitApplication), NULL, this);
}

/** Help command button. */
void AppFrame::help(wxMouseEvent &event) {
    // Create and show the Help window.
    if (myHelp == NULL) {
        myHelp = new HelpFrame(PROGRAM_NAME + _T(" Help"));
    }
    myHelp->Show();
}

/** Find People command button. CMD down indicates a rescan request. */
void AppFrame::findPeople(wxMouseEvent &event) {
    myPF->searchFolder(this, event.CmdDown());
}

/** Make crowd photo command button.*/
void AppFrame::makeCrowdPhoto(wxMouseEvent &event) {
    // Create and show the Maker window.
    if (myMaker == NULL) {
        myMaker = new MakerFrame(_T("Make a crowd image"));
    }
    myMaker->Show();
}

/** Exit application command button. */
void AppFrame::exitApplication(wxMouseEvent &event) {
    // Save the application windows sizes and positions. Destroy the windows.
    if (myMaker != NULL) {
        Settings::setMakerLocation(myMaker->GetPosition());
        Settings::setMakerSize(myMaker->GetSize());
        myMaker->Destroy();
    }
    if (myHelp != NULL) {
        myHelp->Destroy();
    }
    Settings::setWindowLocation(this->GetPosition());
    Settings::setWindowSize(this->GetSize());
    Destroy(); 
}
