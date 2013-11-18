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

#ifndef CROWDMAKER_H
#define	CROWDMAKER_H

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/progdlg.h>
#include "const.h"
#include "PeopleFinder.h"
#include "Tools.h"
#include "Settings.h"

/** Create a crowd image using people images extracted by the PeopleFinder.<p>
 * The CrowdMaker recognizes a set of user options.<p>
 * Usage:<p><code>
 * c = CrowdMaker();<p>
 * c.setBackground(); or c.setImageSize();<p>
 * c.setPeopleList();<p>
 * c.setPeopleCount();<p>
 * c.setPerspective();<p>
 * c.makeCrowdImage(); or c.shuffle();<p>
 * c.getCrowdImage());<p></code>
 */
class CrowdMaker {
public:
    CrowdMaker();
    CrowdMaker(const CrowdMaker& orig);
    virtual ~CrowdMaker();
    void setBackgroundPath(wxString anImagePath);
    wxString getBackgroundPath();
    void setImageSize(wxInt32 aWidth, wxInt32 aHeight);
    wxSize getImageSize();
    void setPeopleCount(wxInt32 aCount);
    wxInt32 getPeopleCount();
    void setPerspective(bool perSetting);
    bool getPerspective();
    void makeCrowdImage(wxFrame *p);
    void shuffle(wxFrame *p);
    wxImage getCrowdImage();
private:
    void loadAllSettings();
    void saveAllSettings();
    void assembleTheImage(wxFrame *p);
    void crowdMerge(wxImage *aPerson, wxInt32 mRow, wxInt32 mCol);
    
    WX_DEFINE_ARRAY_INT(wxInt32, ArrayOfInts);
    WX_DEFINE_ARRAY_DOUBLE(double, ArrayOfDoubles);
    
    /** Path to the background image of the crowd image. */
    wxString myBackgroundPath;

    /** Width of the crowd image. */
    wxInt32 myImageWidth;

    /** Height of the crowd image. */
    wxInt32 myImageHeight;

    /* Number of people to appear in the crowd image. */
    wxInt32 myPeopleCount;

    /** Is perspective requested in the crowd image? true==yes. */
    bool myUsingPer;
    
    /** Perspective factor for the crowd image. 1.0->no perspective. */
    double myPerFactor;
    
    /** The list of image files available for a crowd scene. */
    wxArrayString *myImageFiles;
    
    /** The list of images used in the current crowd scene. */
    wxArrayString *myCurrentCrowd;
    
    /** The Crowd scene object. */
    wxImage *myCrowd;
    
    /** A progress indicator while building a crowd image. */
    wxProgressDialog *myProgress;
};

#endif	/* CROWDMAKER_H */

