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

#ifndef TOOLS_H
#define	TOOLS_H

#include <wx/wx.h>
#include <wx/dir.h>
#include "const.h"
using namespace cv;
using namespace std;

/** Various conversion and other helpful functions. All functions are static. */
class Tools {
public:
    Tools();
    Tools(const Tools& orig);
    virtual ~Tools();
    
    // Conversions
    static string wx2str(wxString);
    static wxString str2wx(string);
    static string int2str(wxInt32 anInt);
    static wxString cstar2wx(char* chars);
    static wxInt32 cstar2int(char* chars);
    static wxString int2wx(wxInt32 anInt);
    static void Mat2WxImage(Mat*, wxImage&);
    static void Wx2MatImage(const wxImage& wx, Mat& cv);
    static Mat convertType(const Mat& src, wxInt32 type, double alpha, double beta);
    static Mat rgb2gray(const Mat& rgb);
    
    // Logging
    static void log(wxString msg);
    static void logFatal(wxString msg);
    
    // File System
    static wxString userFolder();
    static wxString dataFolder();
    static wxString crowd3Folder();
    
private:

};

#endif	/* TOOLS_H */
