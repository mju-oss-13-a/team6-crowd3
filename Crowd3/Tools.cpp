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

#include "Tools.h"

Tools::Tools() {}
Tools::Tools(const Tools& orig) {}
Tools::~Tools() {}

/**
 * Convert a wxString to a std string.
 * @param w the wxString.
 * @return the std string.
 */
string Tools::wx2str(wxString w) {
    string s(w.mb_str());
    return s;
}

/**
 * Convert a std string to a wxString.
 * @param s the std string.
 * @return the wxString.
 */
wxString Tools::str2wx(string s) {
    wxString w(s.c_str(), *wxConvCurrent);
    return w;
}

/**
 * Convert an integer to a std string.
 * @param anInt an integer
 * @return the string.
 */        
string Tools::int2str(wxInt32 anInt) {
    std::stringstream ss;
    ss << anInt;
    string result = ss.str();
    return result;
}

/**
 * Convert a char* to a wxString
 * @param chars A pointer to chars.
 * @return The wxString.
 */
wxString Tools::cstar2wx(char* chars) {
    wxString aString(chars, *wxConvCurrent);
    return aString;
}

/**
 * Convert a char* to an integer.
 * @param chars A pointer to chars.
 * @return The integer.
 */
wxInt32 Tools::cstar2int(char* chars) {
    wxString number(chars, *wxConvCurrent);
    long value;
    if( ! number.ToLong(&value)) {
        log(number + _T("\nError converting to integer"));
    }
    return value;
}

/**
 * Convert an integer to a wxString.
 * @param anInt an integer
 * @return the string.
 */        
wxString Tools::int2wx(wxInt32 anInt) {
    return str2wx(int2str(anInt));
}

/**
 * Get the user's document folder.
 * @return the folder.
 */
wxString Tools::userFolder() {
    return wxStandardPathsBase::Get().GetDocumentsDir();
}

/**
 * Get the full path of the Crowd3 folder. 
 * @return the path of the Crowd3 folder.
 */
wxString Tools::crowd3Folder() {
    return Tools::userFolder() + SEPARATOR + CROWD3FILE;
}

/**
 * Get the path of the program data folder.
 * @return The path of the program data folder.
 */
wxString Tools::dataFolder() {
    return wxStandardPathsBase::Get().GetDataDir();
}

/**
 * Convert Mat to wxImage.
 * Based on http://forums.wxwidgets.org/viewtopic.php?f=1&t=28986
 * @param cvImg The Mat image.
 * @param wxImg The returned wxImage.
 */
void Tools::Mat2WxImage(Mat *cvImg, wxImage& wxImg) {
  // Width and height of the image to be converted.
  wxInt32 w = cvImg->cols;
  wxInt32 h = cvImg->rows;

  // Allocate a wxImage with given width and height to be returned.
  // Get a pointer to its data.
  wxImg = wxImage(w, h);
  uchar* wxData = wxImg.GetData();

  // Create a Mat using the data of the wxImage. 
  // wxImg and cvwxImg share data. Mat will never deallocate wxData.
  Mat *cvwxImg = new Mat(h, w, CV_8UC3, wxData);

  // Convert Mat's BGR to wxImage's RGB.
  switch (cvImg->channels()) {
    case 1: 
        // 1-channel case: expand and copy.
        // Convert type if source is not an integer matrix.
        if (cvImg->depth() != CV_8U) {
            cvtColor(convertType(*cvImg, CV_8U, 1.0, 0.0), *cvwxImg, CV_GRAY2RGB);
        }
        else {
            // Convert grayscale to color.
            cvtColor(*cvImg, *cvwxImg, CV_GRAY2RGB);
        }
        break;
    
    case 3: {
        // 3-channel case: Copy input image (cvImg) to output image (cvwxImg aka
        // wxImg) while swapping R&B channels. BGR to RGB.
        wxInt32 mapping[] = {0,2,1,1,2,0}; // Copy channel 0 to 2, 1 to 1, 2 to 0.
        mixChannels(cvImg, 1, cvwxImg, 1, mapping, 3);
        break;
    }
    
    default: 
        log(int2wx(cvImg->channels()) + 
                _T("\nCv2WxImage : input image should be either 1- or 3-channel"));
  }
}

/**
 * Convert wxImage to Mat.
 * Based on http://forums.wxwidgets.org/viewtopic.php?f=1&t=28986
 * @param wxImg The wxImage.
 * @param cvImg The returned Mat.
 */
void Tools::Wx2MatImage(const wxImage& wxImg, Mat& cvImg) {
    // Get width and height of the image to be converted.
    wxInt32 w = wxImg.GetWidth();
    wxInt32 h = wxImg.GetHeight();
    
    // Allocate a Mat to be returned.
    cvImg = Mat(h, w, CV_8UC3);
    
    // Create a Mat using the data of the wxImage. Mat will never deallocate the data.
    Mat cvwxImg(h, w, CV_8UC3, wxImg.GetData());
    
    // Copy input image (wxImg aka cvwxImg) to output image (cvImg) while swapping 
    // R&B channels. BGR to RGB.
    wxInt32 mapping[] = {0,2,1,1,2,0}; // Copy channel 0 to 2, 1 to 1, 2 to 0.
    mixChannels(&cvwxImg, 1, &cvImg, 1, mapping, 3);
}

/** Convert Mat type. */
Mat Tools::convertType(const Mat& srcImg, wxInt32 toType, double alpha, double beta) {
    Mat dstImg;
    srcImg.convertTo(dstImg, toType, alpha, beta);
    return dstImg;
}

/** Convert Mat to grayscale. */
Mat Tools::rgb2gray(const Mat& rgbImg) {
    Mat grayImg;
    cvtColor(rgbImg, grayImg, CV_RGB2GRAY);
    return grayImg;
}

/**
 * Show an error message in a popup window and write it to the application log.
 * @param msg the error message.
 */
void Tools::log(wxString msg) {
    wxLogError(msg + _T(".\nThis error was written to the program log file.\n"));
}

/**
 * Show an error message in a popup window and write it to the application log. 
 * Terminate the program.
 * @param msg the error message.
 */
void Tools::logFatal(wxString msg) {
    log(msg);
    // This dialog gives the log dialog time to display before program termination.
    wxString fatalMsg = _T("Crowd3 is terminating. Please read the program log file.\n");
    wxMessageDialog* fatal = new wxMessageDialog(NULL, fatalMsg, _T("Fatal Program Error"), 
            wxOK | wxICON_ERROR | wxSTAY_ON_TOP, wxDefaultPosition);
    fatal->ShowModal();
    wxLogFatalError(fatalMsg);
}