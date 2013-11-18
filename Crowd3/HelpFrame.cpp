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

#include "HelpFrame.h"

/**
 * Create and initialize a Help window.
 * @param title The window title.
 */
HelpFrame::HelpFrame(const wxString &title) : wxFrame((wxFrame*) NULL, wxID_ANY, title) {
    this->SetIcon(Icon::getIcon(Icon::iHelp));
    
    // Initialize the window panels and event handlers.
    initPanels();
    initHandlers();
}

HelpFrame::HelpFrame(const HelpFrame& orig) {}
HelpFrame::~HelpFrame() {}

/**
 * Initialize the Help window with a tabbed HelpPanel in the center of the
 * screen.  Populate each tab.
 */
void HelpFrame::initPanels() {
    // Create a Help panel.
    myHelpPanel = new HelpPanel(this);
    wxBoxSizer *helpSizer = new wxBoxSizer(wxVERTICAL);
    helpSizer->Add(myHelpPanel, 1, wxEXPAND | wxALL, 0);
    this->SetSizer(helpSizer);
    
    // Set size and position to center of screen.
    wxDisplay *d = new wxDisplay();
    wxRect r = d->GetClientArea();
    wxSize w = myHelpPanel->GetMinSize();
    wxPoint p;
    p.x = (r.x + r.width) / 2 - (w.x / 2);
    p.y = (r.y + r.height) /2 - (w.y / 2);
    this->SetPosition(p);
    this->SetSize(w);
    this->SetMinSize(w);
    
    // Populate the tabbed panels.
    setAboutContent();
    setDocContent();
    setCreditContent();
}

/** Assign handlers for window close and URL clicked events. */
void HelpFrame::initHandlers() {
    // Handle URL events.
    myHelpPanel->aboutHtml->Connect(wxEVT_COMMAND_HTML_LINK_CLICKED,
            wxHtmlLinkEventHandler(HelpFrame::followLink), NULL, this );
    
    myHelpPanel->docHtml->Connect(wxEVT_COMMAND_HTML_LINK_CLICKED,
            wxHtmlLinkEventHandler(HelpFrame::followLink), NULL, this );
    
    myHelpPanel->creditsHtml->Connect(wxEVT_COMMAND_HTML_LINK_CLICKED,
            wxHtmlLinkEventHandler(HelpFrame::followLink), NULL, this );
    
    // Handle window close event.
    this->Connect(wxEVT_CLOSE_WINDOW, 
            wxMouseEventHandler(HelpFrame::cancel),  NULL, this);
}

/** Populate the About tab of the Help panel. */
void HelpFrame::setAboutContent() {
    myHelpPanel->programImageA->SetBitmap(Icon::getBitmap(Icon::iCrowd3));
    string message = "<html>"
        "<center><h1>" + Tools::wx2str(PROGRAM_NAME) + "</h1></center><br>"
        "<center><b>" "Version " + Tools::wx2str(PROGRAM_VERSION) + "</b></center><br><br>"
        "Copyright (c) Dennis Damico, dennis508@yahoo.com<br>"
        + getLink(Tools::wx2str(PROJECT_URL)) + "<br><br>"
        "This is open source software under the BSD License:<br>"
        + getLink("http://www.opensource.org/licenses/bsd-license") + "<br>"
        "</html>";
    if ( ! myHelpPanel->aboutHtml->SetPage(Tools::str2wx(message))) {
        Tools::log(_T("An error occurred while trying to write the About panel."));
    }
}

/** Populate the Documentation tab of the Help panel. */
void HelpFrame::setDocContent() {
    myHelpPanel->programImageD->SetBitmap(Icon::getBitmap(Icon::iCrowd3));
    string message = "<html>"
        "<center><h1><b>" + Tools::wx2str(PROGRAM_NAME) + "</b></h1></center><br>"
        "<center><b>Documentation</b></center><br><br>"
        "o " + getLink("User Guide", "http://crowd3.sourceforge.net/crowd3.html") + "<br>"
        "o " + getLink("Installation, Program Notes, Licenses", "http://crowd3.sourceforge.net/about.html") + "<br>"
        "o " + getLink("Revision history", "http://crowd3.sourceforge.net/revisions.html") + "<br>"
        "<br><br>"
        "Comments, suggestions, and defect reports are always welcome. "
        "Email to dennis508@yahoo.com."
        "</html>";
    if ( ! myHelpPanel->docHtml->SetPage(Tools::str2wx(message))) {
        Tools::log(_T("An error occurred while trying to write the Doc panel."));
    }
}

/** Populate the Credits tab of the Help panel. */
void HelpFrame::setCreditContent() {
    myHelpPanel->programImageC->SetBitmap(Icon::getBitmap(Icon::iCrowd3));
    string message = "<html>"
        "<center><h1><b>" + Tools::wx2str(PROGRAM_NAME) + "</b></h1></center><br>"
        "<center><b>was built with these open source components: </b></center><br><br>"

        "<b>wxWidgets, Version 2.8.11</b>" "<br>"
        + getLink("http://www.wxwidgets.org/") + "<br>"
        "wxWindows license" "<br>"
        + getLink("http://www.opensource.org/licenses/wxwindows.php") + "<br><br>"

        "<b>OpenCV, Version 2.3.1</b>" "<br>"
        + getLink("http://opencv.willowgarage.com/wiki/") + "<br>"
        "BSD license" "<br>"
        + getLink("http://opensource.org/licenses/bsd-license.php") + "<br><br>"

        "<b>SQLite, Version 3.7.7</b>" "<br>"
        + getLink("http://www.sqlite.org/") + "<br>"
        "Public domain" "<br><br>"

        "<b>An image panel</b>" "<br>"
        + getLink("http://wiki.wxwidgets.org/An_image_panel") + "<br>"
        "wxWindows license" "<br>"
        + getLink("http://www.opensource.org/licenses/wxwindows.php") + "<br><br>"

        "<b>Embedding PNG Images</b>" "<br>"
        + getLink("http://wiki.wxwidgets.org/Embedding_PNG_Images") + "<br>"
        "wxWindows license" "<br>"
        + getLink("http://www.opensource.org/licenses/wxwindows.php") + "<br><br>"

        "<b>wxFormBuilder, Version 3.1.59</b>" "<br>"
        + getLink("http://wxformbuilder.org/") + "<br>"
        "GNU Public License" "<br>"
        + getLink("http://www.opensource.org/licenses/gpl-license.php") + "<br><br>"

        "<b>Inno Setup, Version 5.4.3</b>" "<br>"
        + getLink("http://www.jrsoftware.org/") + "<br>"
        "BSD License" "<br>"
        + getLink("http://opensource.org/licenses/bsd-license.php") + "<br><br>"

        "<b>InnoIDE, Version 1.0.0.0078</b>" "<br>"
        + getLink("http://www.innoide.org/") + "<br>"
        "Mozilla Public License" "<br>"
        + getLink("http://www.mozilla.org/MPL") + "<br><br>"
            
        "<b>Use wxWidgets to manipulate images</b>" "<br>"
        + getLink("http://forums.wxwidgets.org/viewtopic.php?f=1&t=28986") + "<br>"
        "wxWindows license" "<br>"
        + getLink("http://www.opensource.org/licenses/wxwindows.php") + "<br><br>"  
            
        "<b>wxWidgets for Swing Developers</b>" "<br>"
        + getLink("http://www.daltonfilho.com/articles/swingwx") + "<br>"
        "Copyright Dalton Filho" "<br><br>"

        "<b>Tango Icon Library, Version 0.8.90</b><br>"
        + getLink("http://tango.freedesktop.org/Tango_Icon_Library") + "<br>"
        "Public Domain<br><br>"

        "</html>";
    if ( ! myHelpPanel->creditsHtml->SetPage(Tools::str2wx(message))) {
        Tools::log(_T("An error occurred while trying to write the Credits panel."));
    }
}

/** Window close event: hide the window. */
void HelpFrame::cancel(wxMouseEvent &event) {
    Hide();
}

/**
 * Get the HTML code for a URL.
 * @param theURL the URL.
 * @return HTML code for the URL.
 */
string HelpFrame::getLink(string theURL) {
    return "<a href='" + theURL + "'>" + theURL + "</a>";
}

/**
 * Get the HTML code for a URL.
 * @param theText the text to be displayed.
 * @param theURL the URL.
 * @return HTML code for the URL.
 */
string HelpFrame::getLink(string theText, string theURL) {
    return "<a href='" + theURL + "'>" + theText + "</a>";
}

/**
 * Follow a clicked URL using the default web browser.
 * @param event The click event.
 */
void HelpFrame::followLink(wxHtmlLinkEvent& event) {
    wxString theURL = event.GetLinkInfo().GetHref();
    if ( ! wxLaunchDefaultBrowser(theURL)) {
        Tools::log(_T("An error occurred while trying to start the default web browser."));
    }
}
