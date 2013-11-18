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

#ifndef _MAKERFRAME_H
#define	_MAKERFRAME_H

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include <wx/mstream.h>
#include "wxImagePanel.h"
#include <iostream>
#include "Icon.h"
#include "const.h"
#include "Tools.h"
#include "Settings.h"
#include "CrowdMaker.h"

/** Provide the user interface for customizing and displaying crowd images.
 * Use a CrowdMaker object to save/load preferences and create the crowd image. */
class MakerFrame : public wxFrame {
    
public:
    MakerFrame(const wxString &title);
    
private:
    /** The crowd maker object. */
    CrowdMaker *cm;
    
    /** The top panel on the maker window.  Contains controls. */
    wxPanel *controlsPanel;
    
    /** The bottom panel on the maker window. Contains buttons and image. */
    wxPanel *bottomPanel;
    
    /** The button (bottom-left) panel on the maker window. */
    wxPanel *buttonPanel;
    
    /** The image (bottom-right) panel on the maker window. */
    wxImagePanel *imagePanel;
    
    /** The People Count selector. */
    wxSpinCtrl *peopleCtrl;
    
    /** The Perspective selector. */
    wxCheckBox * perCtrl;
    
    /* The Image size selector. */
    wxComboBox *sizeCtrl;
    
    /** The full path to the background image file.  Kept in sync with imageCtrl. */
    wxString imageCtrlFullPath;
    
    /** The Background image file displayer. */
    wxTextCtrl *imageCtrl;
    
    /** The Make Crowd Image button. */
    wxBitmapButton *makeButton;
    
    /** The Shuffle People button. */
    wxBitmapButton *shuffleButton;
    
    /** The Save button. */
    wxBitmapButton *saveButton;
    
    /** The Cancel button. */
    wxBitmapButton *cancelButton;
            
    void initPanels();
    void initControls();
    void initHandlers();
    void setPeopleCount(wxEvent &event);
    void perBox(wxEvent &event);
    void iSizer(wxEvent &event);
    void make(wxMouseEvent &event);
    void shuffle(wxMouseEvent &event);
    void save(wxMouseEvent &event);
    void cancel(wxMouseEvent &event);
};

#endif	/* _MAKERFRAME_H */

