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

#include "Settings.h"
#include "Tools.h"

/** The configuration object. Stored as a static. */
wxConfig *myConfig;

/** Create a settings object using operating system services. */
Settings::Settings() {
    myConfig = new wxConfig(PROGRAM_NAME);
    //myConfig->DeleteAll(); // For testing: Erase current settings.
}

Settings::Settings(const Settings& orig) {}
Settings::~Settings() {}

// Default application window size.
const wxInt32 winW = 400;
const wxInt32 winH = 300;

/**
 * Save the application window's size to program settings.
 * @param s The application window size.
 */
void Settings::setWindowSize(wxSize s) {
    myConfig->Write(_T("winW"), s.GetWidth());
    myConfig->Write(_T("winH"), s.GetHeight());
    myConfig->Flush();
}

/**
 * Get the application window's size from program settings or a default size.
 * @return The application window size
 */
wxSize Settings::getWindowSize() {
    wxSize w;
    w.SetWidth(myConfig->Read(_T("winW"), winW));
    w.SetHeight(myConfig->Read(_T("winH"), winH));
    return w;
}

/**
 * Save the application window's location to program settings.
 * @param p The application window location.
 */
void Settings::setWindowLocation(wxPoint p) {
    myConfig->Write(_T("winX"), p.x);
    myConfig->Write(_T("winY"), p.y);
    myConfig->Flush();
}

/**
 * Get the application window's location from program settings or a default location.
 * The default location is the center of the screen.
 * @return The application window location.
 */
wxPoint Settings::getWindowLocation() {
    wxPoint p;
    p.x = myConfig->Read(_T("winX"), -1);
    p.y = myConfig->Read(_T("winY"), -1);
    if (p.x == -1) {
        // Return default location.
        wxDisplay *d = new wxDisplay();
        wxRect r = d->GetClientArea();
        p.x = (r.x + r.width) / 2 - (winW / 2);
        p.y = (r.y + r.height) /2 - (winH / 2);
    }
    return p;
}

// Default Maker Panel size.
const wxInt32 makW = 400;
const wxInt32 makH = 400;

/**
 * Save the maker panel's size to program settings.
 * @param s The maker panel size.
 */
void Settings::setMakerSize(wxSize s) {
    myConfig->Write(_T("makW"), s.GetWidth());
    myConfig->Write(_T("makH"), s.GetHeight());
    myConfig->Flush();
}

/**
 * Get the maker panel's size from program settings or a default size.
 * @return The maker panel size
 */
wxSize Settings::getMakerSize() {
    wxSize w;
    w.SetWidth(myConfig->Read(_T("makW"), makW));
    w.SetHeight(myConfig->Read(_T("makH"), makH));
    return w;
}

/**
 * Save the maker panel's location to program settings.
 * @param p The maker panel location.
 */
void Settings::setMakerLocation(wxPoint p) {
    myConfig->Write(_T("makX"), p.x);
    myConfig->Write(_T("makY"), p.y);
    myConfig->Flush();
}

/**
 * Get the maker panel's location from program settings or a default location.
 * The default location is the center of the screen.
 * @return The maker panel location.
 */
wxPoint Settings::getMakerLocation() {
    wxPoint p;
    p.x = myConfig->Read(_T("makX"), -1);
    p.y = myConfig->Read(_T("makY"), -1);
    if (p.x == -1) {
        // Return default location.
        wxDisplay *d = new wxDisplay();
        wxRect r = d->GetClientArea();
        p.x = (r.x + r.width) / 2 - (makW / 2);
        p.y = (r.y + r.height) /2 - (makH / 2);
    }
    return p;
}

/**
 * Save the most recent folder used to search for person images.
 * @param path The path to a person image folder.
 */
void Settings::setPersonPath(wxString path) {
    myConfig->Write(_T("pPath"), path);
    myConfig->Flush();
}
    
/**
 * Get the most recent folder used to search for person images or default.
 * @return The path to a person image folder.
 */
wxString Settings::getPersonPath() {
    return myConfig->Read(_T("pPath"), _T(""));
}

/**
 * Save the most recent folder used to save crowd images.
 * @param path The path to folder.
 */
void Settings::setCrowdPath(wxString path) {
    myConfig->Write(_T("cPath"), path);
    myConfig->Flush();
}
    
/**
 * Get the most recent folder used to save crowd images or default.
 * @return The path to a folder.
 */
wxString Settings::getCrowdPath() {
    return myConfig->Read(_T("cPath"), _T(""));
}

/**
 * Save the most recent crowd image background image path.
 * @param path The path to file.
 */
void Settings::setBackgroundPath(wxString path) {
    myConfig->Write(_T("bPath"), path);
    myConfig->Flush();
}
    
/**
 * Get the most recent crowd image background image path.
 * @return The path to a file.
 */
wxString Settings::getBackgroundPath() {
    return myConfig->Read(_T("bPath"), Tools::dataFolder() + SEPARATOR);
}

/**
 * Save the most recent crowd person count.
 * @param count The person count.
 */
void Settings::setPeopleCount(wxInt32 count) {
    myConfig->Write(_T("pCount"), count);
    myConfig->Flush();
}

/**
 * Get the most recent crowd person count or default.
 * @return the person count.
 */
wxInt32 Settings::getPeopleCount() {
    return myConfig->Read(_T("pCount"), 100l);
}

/**
 * Save the perspective setting.
 * @param value The perspective setting.
 */
void Settings::setPerspective(bool value) {
    myConfig->Write(_T("perspec"), value);
    myConfig->Flush();
}

/**
 * Get the most recent perspective setting.
 * @return The perspective setting.
 */
bool Settings::getPerspective() {
    bool val = false; // default return value.
    myConfig->Read(_T("perspec"), &val);
    return val;
}

/**
 * Save the crowd image size to program settings.
 * @param s The crowd image size.
 */
void Settings::setCrowdImageSize(wxSize s) {
    myConfig->Write(_T("crowdW"), s.GetWidth());
    myConfig->Write(_T("crowdH"), s.GetHeight());
    myConfig->Flush();
}

/**
 * Get the crowd image size from program settings or a default size.
 * @return The crowd image size
 */
wxSize Settings::getCrowdImageSize() {
    wxSize w;
    w.SetWidth(myConfig->Read(_T("crowdW"), 1280l));
    w.SetHeight(myConfig->Read(_T("crowdH"), 800l));
    // Error check for zero sizes.  Should never happen.
    if (w.GetWidth() == 0 || w.GetHeight() == 0) {
        w.SetWidth(1280l);
        w.SetHeight(800l);
    }
    return w;
}

/**
 * Save the unique image ID to program settings.
 * @param i The Unique ID.
 */
void Settings::setImageID(wxInt32 i) {
    myConfig->Write(_T("UID"), i);
    myConfig->Flush();
}

/**
 * Get the unique image ID from program settings or a default value.
 * @return The Unique ID.
 */
wxInt32 Settings::getImageID() {
    return myConfig->Read(_T("UID"), 0l);
}