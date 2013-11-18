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

#ifndef CONST_H
#define	CONST_H

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <opencv2/imgproc/imgproc.hpp>

// Program constants:

    /** The program name. */
    const wxString PROGRAM_NAME = _T("Crowd3");
    
    /** The program revision. */
    const wxString PROGRAM_VERSION = _T("v1.0");
    
    /** The program Internet home. */
    const wxString PROJECT_URL = _T("http://crowd3.sourceforge.net");
    
    /** The program error log file. */
    const wxString PROJECT_LOG = _T("crowd3_log.txt");

    /** Make debugging versions of some settings unique. Set to "" for production.*/
    const wxString DEBUG = _T(""); // set to "deb" or ""
        
// Color constants: 

    /** Background color used on control panels. */
    const wxColour WX_COLOR_BACKGROUND = wxColour(200, 227, 255);

    /** A (hopefully) rare (almost black) color used to mark transparent areas 
     * on an image. For use with OpenCV. */
    const cv::Vec<unsigned char, 3> CV_COLOR_TRANSPARENT = cv::Vec<unsigned char, 3>(1, 1, 1);
    
    /** Another definition of COLOR_TRANSPARENT. For use with wxWidgets. */
    const wxInt32 WX_COLOR_TRANSPARENT[] = {1, 1, 1};
    
// Operating system constants:

    /** Operating system specific file path separator. */
    const wxString SEPARATOR = wxFileName::GetPathSeparator();

    /** The Crowd3 folder name. */
    const wxString CROWD3FILE = _T(".crowd3Images");
    
    /** The Crowd3 database name. */
    const wxString DATABASE = _T("crowd3.sqlite");

    /** The filename of the face detection cascade. */
    const wxString FACECASCADENAME = _T("crowd3.xml"); 
    // A copy of "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
            
#endif	/* CONST_H */

