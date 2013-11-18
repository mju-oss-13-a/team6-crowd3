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

#include "crowd3.h"

/* Create the main class. */
IMPLEMENT_APP(MyApp);

/**
 * Application initialization code: start up components and create the 
 * <b>main</b> application window.
 * @return <code>true</code> if everything is OK; <code>false</code> otherwise.
 */
bool MyApp::OnInit() {
    // Initialize logging.
    initLogging();

    // Init wxWidgets image handlers.
    wxInitAllImageHandlers();
    
    // Initialize settings.
    Settings *s = new Settings();
    
    // Create Crowd3 folder if it does not exist.
    if ( ! wxFileName::Mkdir(Tools::crowd3Folder(), 0777, wxPATH_MKDIR_FULL )) {
        Tools::logFatal(_T("The Crowd3 folder could not be created."));
    }
    
    // Open image database. Close it in onExit().
    if ( ! ImageDB::open()) {
        Tools::logFatal(_T("The image database could not be opened."));
    }
    
    // Create and show the application window.
    AppFrame *app = new AppFrame(PROGRAM_NAME + _T(" ") + PROGRAM_VERSION);
    app->Show();
    return true;
}

/**
 * Initialize the logging subsystem and direct log messages to a popup window
 * and the application's log file.
 * Log errors with "wxLogError(wxString);" or "wxLogFatalError(wxString);"
 */
void MyApp::initLogging() {
    // Log everything.
    wxLog::SetLogLevel(wxLOG_Max);
    wxLog::SetVerbose();
    wxLog::SetTimestamp(_T("%c"));
    
    // Send log messages to a popup window...
    wxLog::SetActiveTarget(new wxLogGui());

    // ...and a log file.
    FILE *f = fopen(Tools::wx2str(
            Tools::userFolder() + SEPARATOR + PROJECT_LOG).c_str(), "a");
    if(f) {
        wxLog *logger_file = new wxLogStderr(f);
        wxLogChain *logChain = new wxLogChain(logger_file);
    }
}

/**
 * Application shutdown code.
 * @return not used.
 */
int MyApp::OnExit() {
    // Close image database.
    ImageDB::close();
}