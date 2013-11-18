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

#ifndef _APP_FRAME_H
#define	_APP_FRAME_H

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/mstream.h>
#include <wx/notebook.h>
#include <iostream>
#include "Icon.h"
#include "const.h"
#include "Settings.h"
#include "ImageTree.h"
#include "PeopleFinder.h"
#include "MakerFrame.h"
#include "HelpFrame.h"

/**
 * This is the <b>main</b> application window. It contains command buttons to 
 * drive various program operations and displays information about the program.
 */
class AppFrame : public wxFrame {
    
public:

    AppFrame(const wxString &title);
    
private:
    
    /** The people finder object. */
    PeopleFinder *myPF;
    
    /** The crowd maker object. */
    MakerFrame *myMaker;
    
    /** The Help/About/Credits panel. */
    HelpFrame *myHelp;
    
    /** The button (left) panel on the main window. */
    wxPanel *buttonPanel;
    
    /** The data (right) panel on the main window. */
    wxPanel *dataPanel;
    
    /** The Help/About button on the main window. */
    wxBitmapButton *helpButton;
    
    /** The Find People in images control button on the main window. */
    wxBitmapButton *findButton;
    
    /** The Make Crowd Photo button on the main window. */
    wxBitmapButton *makeButton;
    
    /** The Exit Application button on the main window. */
    wxBitmapButton *exitButton;
    
    /** The File Tree control. */
    ImageTree *fileCtrl;
            
    void initPanels();
    void initControls();
    void initHandlers();
    void help(wxMouseEvent &event);
    void findPeople(wxMouseEvent &event);
    void makeCrowdPhoto(wxMouseEvent &event);
    void exitApplication(wxMouseEvent &event);
};

#endif	/* _APP_FRAME_H */

