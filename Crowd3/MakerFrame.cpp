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

#include "MakerFrame.h"

/** Initialize the background image size choices. */
wxString sizeChoices[] = {
    _T("1280x800"),        // 16:10 - default
    _T("1400x1050"),       // 4:3
    _T("1920x1080"),       // 16:9
    _T("Use Background")}; // Use size of background image.

/**
 * Create and initialize the crowd maker window and its components.
 * @param title The window title.
 */
MakerFrame::MakerFrame(const wxString &title) : wxFrame((wxFrame*) NULL, wxID_ANY, title) {
    this->SetIcon(Icon::getIcon(Icon::iMake2));
    
    // Initialize the CrowdMaker object.
    cm = new CrowdMaker();
    
    // Initialize the window panels, buttons, and controls.
    initPanels();
    initControls();
    initHandlers();
}

/**
 * Create and initialize panels in the maker window: controls (top) panel and 
 * buttons and image (bottom) panels.
 */        
void MakerFrame::initPanels() {
    // The top panel is the controls panel.
    // The controls panel height is fixed in size.
    controlsPanel = new wxPanel(this);
    controlsPanel->SetMinSize(wxSize(-1, 100));
    controlsPanel->SetMaxSize(controlsPanel->GetMinSize());
    controlsPanel->SetBackgroundColour(WX_COLOR_BACKGROUND);
    
    // The bottom panel contains the button panel and the image panel.
    bottomPanel = new wxPanel(this);
    bottomPanel->SetMinSize(wxSize(600, 300));
    
    // The button panel width is fixed in size.
    buttonPanel = new wxPanel(bottomPanel);
    buttonPanel->SetMinSize(wxSize(100, -1));
    buttonPanel->SetMaxSize(buttonPanel->GetMinSize());
    buttonPanel->SetBackgroundColour(WX_COLOR_BACKGROUND);
    
    // The image panel may grow in size. Initialize with a blank image.
    imagePanel = new wxImagePanel(bottomPanel, wxImage(400, 300, true));
    imagePanel->SetMinSize(wxSize(400, 300));
    imagePanel->SetBackgroundColour(wxColour(255, 255, 255));
    
    // Add top and bottom panels to a vertical sizer panel with thin border.
    wxBoxSizer *windowSizer = new wxBoxSizer(wxVERTICAL);
    windowSizer->Add(controlsPanel, 0, wxEXPAND | wxALL, 1);
    windowSizer->Add(bottomPanel,   1, wxEXPAND | wxALL, 1);
    windowSizer->SetSizeHints(this); // Prevent shrinking below min size.
    
    // Add sizer panel to the maker window.
    // Set window position and size to last used or default.
    this->SetSizer(windowSizer);
    this->SetPosition(Settings::getMakerLocation());
    this->SetSize(Settings::getMakerSize());
}

/** Create the buttons and controls in the maker window. */
void MakerFrame::initControls() {
    // People count label and selector.
    wxStaticText *pLabel = new wxStaticText(controlsPanel, -1, _T("People Count:"));
    peopleCtrl = new wxSpinCtrl(
            controlsPanel,
            -1,
            Tools::int2wx(cm->getPeopleCount()),
            wxDefaultPosition,
            wxSize(60, -1),
            wxSP_ARROW_KEYS,
            1,
            1000,
            10,
            _T(""));
    peopleCtrl->SetToolTip(_T("Enter number of people in the crowd"));
    
    // Perspective label and selector.
    wxStaticText *perLabel = new wxStaticText(controlsPanel, -1, _T("Perspective:"));
    perCtrl = new wxCheckBox(
            controlsPanel,
            -1,
            _T(""),
            wxDefaultPosition,
            wxDefaultSize,
            wxCHK_2STATE);
    perCtrl->SetValue(cm->getPerspective());
    perCtrl->SetToolTip(_T("Use or don't use perspective in the crowd image"));
    
    // Image size selector.
    wxStaticText *sLabel = new wxStaticText(controlsPanel, -1, _T("Image Size:"));
    // The image size to be displayed is either the most recently used standard
    // size or the "use background image size" size choice.
    wxString sizeSelection = // most recently used standard size...
            Tools::int2wx(cm->getImageSize().GetWidth()) + _T("x") +
            Tools::int2wx(cm->getImageSize().GetHeight());
    imageCtrlFullPath = cm->getBackgroundPath();
    wxString backgroundFile = _T("");
    wxFileName::SplitPath(imageCtrlFullPath, NULL, &backgroundFile, NULL);
    if (backgroundFile.Length() > 0) { // use background image size
        sizeSelection = sizeChoices[WXSIZEOF(sizeChoices) - 1];
    }
    sizeCtrl = new wxComboBox(
            controlsPanel,
            -1,
            sizeSelection,
            wxDefaultPosition,
            wxSize(150, -1),
            WXSIZEOF(sizeChoices),
            sizeChoices,
            wxCB_DROPDOWN | wxCB_READONLY);
    sizeCtrl->SetToolTip(_T("Select size of the crowd image."));
    
    // Background image path displayer.
    imageCtrl = new wxTextCtrl(
            controlsPanel, 
            -1,
            backgroundFile, // Just the file part of the background path.
            wxDefaultPosition,
            wxSize(210, -1),
            wxTE_READONLY);
    imageCtrl->SetToolTip(_T("The background image file"));

    // Buttons.
    makeButton = new wxBitmapButton(buttonPanel, -1, Icon::getBitmap(Icon::iMake2));
    makeButton->SetMinSize(wxSize(40, 40));
    makeButton->SetMaxSize(wxSize(40, 40));
    makeButton->SetToolTip(_T("Make a crowd scene image"));
    
    shuffleButton = new wxBitmapButton(buttonPanel, -1, Icon::getBitmap(Icon::iShuffle));
    shuffleButton->SetMinSize(wxSize(40, 40));
    shuffleButton->SetMaxSize(wxSize(40, 40));
    shuffleButton->SetToolTip(_T("Shuffle the people in the current crowd image"));
    
    saveButton = new wxBitmapButton(buttonPanel, -1, Icon::getBitmap(Icon::iSave));
    saveButton->SetMinSize(wxSize(40, 40));
    saveButton->SetMaxSize(wxSize(40, 40));
    saveButton->SetToolTip(_T("Save the crowd image to a file"));
    
    cancelButton = new wxBitmapButton(buttonPanel, -1, Icon::getBitmap(Icon::iCancel));
    cancelButton->SetMinSize(wxSize(40, 40));
    cancelButton->SetMaxSize(wxSize(40, 40));
    cancelButton->SetToolTip(_T("Cancel"));
    
    // Create a horizontal sizer to contain the controls.
    wxBoxSizer *controlsSizer = new wxBoxSizer(wxHORIZONTAL);
    controlsSizer->Add(pLabel,     0, wxALIGN_CENTRE | wxTOP | wxBOTTOM | wxLEFT, 2);
    controlsSizer->Add(peopleCtrl, 0, wxALIGN_CENTRE | wxTOP | wxBOTTOM | wxRIGHT, 2);
    controlsSizer->Add(perLabel,   0, wxALIGN_CENTRE | wxTOP | wxBOTTOM | wxLEFT, 2);
    controlsSizer->Add(perCtrl,    0, wxALIGN_CENTRE | wxTOP | wxBOTTOM | wxRIGHT, 2);
    controlsSizer->Add(sLabel,     0, wxALIGN_CENTRE | wxTOP | wxBOTTOM | wxLEFT, 2);
    controlsSizer->Add(sizeCtrl,   0, wxALIGN_CENTRE | wxTOP | wxBOTTOM | wxRIGHT, 2);
    controlsSizer->Add(imageCtrl,  0, wxALIGN_CENTRE | wxALL, 2);
    controlsSizer->AddStretchSpacer();
    controlsSizer->SetSizeHints(controlsPanel); // Shrink to min size.
    
    // Create a vertical sizer to contain the buttons.
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer->Add(makeButton,    0, wxEXPAND | wxALL, 1);
    buttonSizer->Add(shuffleButton, 0, wxEXPAND | wxALL, 1);
    buttonSizer->Add(saveButton,    0, wxEXPAND | wxALL, 1);
    buttonSizer->Add(cancelButton,  0, wxEXPAND | wxALL, 1);
    buttonSizer->AddStretchSpacer();
    buttonSizer->SetSizeHints(buttonPanel); // Shrink to min size.
    
    // Add bottom panels to a horizontal sizer panel with thin border.
    wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomSizer->Add(buttonPanel, 0, wxEXPAND | wxALL, 1);
    bottomSizer->Add(imagePanel,  1, wxEXPAND | wxALL, 1);
    
    // Add the sizers to their main window panels.
    controlsPanel->SetSizer(controlsSizer);
    buttonPanel->SetSizer(buttonSizer);
    bottomPanel->SetSizer(bottomSizer);
}

/** Create handlers for the maker control buttons and the window close box. */
void MakerFrame::initHandlers() {
    
    // Init Control handlers.
    sizeCtrl->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED,
            wxEventHandler(MakerFrame::iSizer),       NULL, this);
    makeButton->Connect(wxEVT_LEFT_DOWN, 
            wxMouseEventHandler(MakerFrame::make),    NULL, this);
    shuffleButton->Connect(wxEVT_LEFT_DOWN, 
            wxMouseEventHandler(MakerFrame::shuffle), NULL, this);
    saveButton->Connect(wxEVT_LEFT_DOWN, 
            wxMouseEventHandler(MakerFrame::save),    NULL, this);
    cancelButton->Connect(wxEVT_LEFT_DOWN, 
            wxMouseEventHandler(MakerFrame::cancel),  NULL, this);
    this->Connect(wxEVT_CLOSE_WINDOW, 
            wxMouseEventHandler(MakerFrame::cancel),  NULL, this);
}

/**
 * Image size pulldown value selected. Either accept a new background image 
 * file or clear the background path.
 * @param event
 */
void MakerFrame::iSizer(wxEvent &event) {
    // Either ask for a new background image file or clear the background path.
    if (sizeCtrl->GetValue().IsSameAs(sizeChoices[WXSIZEOF(sizeChoices) - 1])) {
        // Ask user for a new background image file.
        // Initialize with starting dir from background path setting.
        wxString backgroundPath = Settings::getBackgroundPath();
        wxString backgroundDir = _T("");
        wxFileName::SplitPath(backgroundPath, &backgroundDir, NULL, NULL);
        wxFileDialog *bd = new wxFileDialog(
            this,
            _T("Select a background image file"),
            backgroundDir,
            _T(""),
            _T("JPEG files (*.jpg, *.JPG)|*.jpg;*.JPG"),
            wxFD_OPEN | wxFD_FILE_MUST_EXIST);
        if (bd->ShowModal() == wxID_CANCEL) {
            // Cancelled.  Restore default size value and clear background path.
            sizeCtrl->SetValue(sizeChoices[0]);
            imageCtrlFullPath = _T("");
            imageCtrl->SetValue(_T(""));
            return;
        }
        // Save the new background image path. Display the filename.
        imageCtrlFullPath = bd->GetPath();
        wxString theFile = _T("");
        wxFileName::SplitPath(imageCtrlFullPath, NULL, &theFile, NULL);
        imageCtrl->SetValue(theFile);
    }
    else {
        // A standard size selected. Clear the background path.
        imageCtrlFullPath = _T("");
        imageCtrl->SetValue(_T(""));
    }
    
#ifdef __UNIX__
    // Deselect the current pulldown choice so that repeat invocations of
    // "Use Background" will be seen. Needed in Linux only.
    sizeCtrl->SetSelection(-1);
#endif
}

/** Make command button pressed. */
void MakerFrame::make(wxMouseEvent &event) {
    try {
        // Transfer crowd settings to the crowd maker.
        cm->setPeopleCount(peopleCtrl->GetValue());
        cm->setPerspective(perCtrl->GetValue());
        
        wxString theSize = sizeCtrl->GetValue();
        if (theSize.IsSameAs(sizeChoices[WXSIZEOF(sizeChoices) - 1])) {
            // Use background image.
            cm->setImageSize(1280, 800); // default size if no background path.
            cm->setBackgroundPath(imageCtrlFullPath);
        }
        else {
            // Use a standard size. Extract width and height from size choice.
            long w = 0;
            long h = 0;
            theSize.Before('x').ToLong(&w);
            theSize.After('x').ToLong(&h);
            cm->setImageSize(w, h);
            cm->setBackgroundPath(_T(""));
        }
        
        // Create and display the crowd image.
        cm->makeCrowdImage(this);
        imagePanel->setImage(cm->getCrowdImage());
    }
    catch (Exception e) {
        Tools::log(Tools::str2wx(e.msg) + _T("\nError on Make operation"));
    }
}

/** Shuffle People command button pressed. */
void MakerFrame::shuffle(wxMouseEvent &event) {
    try {
        cm->shuffle(this);
        imagePanel->setImage(cm->getCrowdImage());
    }
    catch (Exception e) {
        Tools::log(Tools::str2wx(e.msg) + _T("\nError on Shuffle operation"));
    }
}

/** Save Crowd Photo command button pressed.*/
void MakerFrame::save(wxMouseEvent &event) {
    wxFileDialog *sd = new wxFileDialog(
            this,
            _T("Save the crowd image"),
            Settings::getCrowdPath(),
            _T(""),
            _T("JPEG files (*.jpg, *.JPG)|*.jpg;*.JPG"),
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (sd->ShowModal() == wxID_CANCEL) {
        return;
    }
    // Save the selected folder in settings.  Save the crowd image.
    Settings::setCrowdPath(sd->GetDirectory());
    wxString targetFile = sd->GetFilename();
    if ( ! targetFile.EndsWith(_T(".jpg"))) {
        targetFile.Append(_T(".jpg"));
    }
    targetFile = sd->GetDirectory() + SEPARATOR + targetFile;
    wxImage *theCrowd = new wxImage(cm->getCrowdImage());
    theCrowd->SaveFile(targetFile, wxBITMAP_TYPE_JPEG);
}

/** Cancel operation command button pressed. */
void MakerFrame::cancel(wxMouseEvent &event) {
    Hide();
}
