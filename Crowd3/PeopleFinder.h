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

#ifndef PEOPLEFINDER_H
#define	PEOPLEFINDER_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/dirdlg.h>
#include <wx/filefn.h>
#include <wx/progdlg.h>
#include "const.h"
#include "Tools.h"
#include "Settings.h"
#include "ImageTree.h"
#include <string>
#include <iostream>
using namespace cv;
using namespace std;

// Face constants (empirical):
/** A face must be at least this fraction of the smaller photo dimension.
 *   This helps filter out detection errors. */
const double FACEPERCENT = 0.08;

/** Enlarge the detected face width by this factor to encompass the entire head. */
const double HEADWIDTH = 1.02;

/** Enlarge the detected face height by this factor to encompass the entire head. */
const double HEADHEIGHT = 1.9;

/** Enlarge the enlarged face by this factor to estimate the upper body width. */
const double BODYWIDTH = 2.1 / HEADWIDTH;

/** Enlarge the enlarged face by this factor to estimate the upper body height. */
const double BODYHEIGHT = 2.1 / HEADHEIGHT;

/** Scale detected head images to this size. */
const double SCALEDFACEWIDTH = 200.0;

/** Person images will be this many pixels wide. */
const double PERSONWIDTH = HEADWIDTH * BODYWIDTH * SCALEDFACEWIDTH;

/** Person images will be this many pixels high. */
const double FULLPERSONHEIGHT =
            SCALEDFACEWIDTH * (HEADHEIGHT + (HEADHEIGHT * BODYHEIGHT));

/** Persons displayed in a crowd will have reduced person height. */
const double PERSONHEIGHT = 0.75 * FULLPERSONHEIGHT;

/**
 * Search 'source image files' for people.  Extract the people into individual
 * 'people image files' that can be used to build crowd images.<p>
 * The PeopleFinder uses opencv face detection to find people images (actually
 * just their faces). Then the face portion is enlarged (by skin and hair
 * color searches) to try to include the face's entire head and upper body.<P>
 * Each person image is written to an individual file named with a unique ID. A
 * record is created for the source image file that associates it with
 * the people image files extracted from it.<p>
 * Usage: <p><code>
 * p = PeopleFinder();<p>
 * p.searchFolder(aFolder);<p></code>
 */
class PeopleFinder : public wxDirTraverser {
    public:
        PeopleFinder();
        PeopleFinder(const PeopleFinder& orig);
        virtual ~PeopleFinder();
        void searchFolder(wxFrame *parent, bool rescan);

    private:
        void initFaceDetection();
        void initImageTypes();
        wxInt32 findPeople(wxString imageFile);
        vector<Rect> findFaces(Mat theImage);
        void maskHead(Mat& m, Rect aHead, Rect aFace);
        void deleteOldImages(wxInt32 firstID, wxInt32 lastID);
        virtual wxDirTraverseResult OnFile(const wxString& filename);
        virtual wxDirTraverseResult OnDir(const wxString& dirname);
        virtual wxDirTraverseResult OnOpenError(const wxString& dirname);
        void faceSearch(Mat& m, Rect aFace, vector<Point>& facePoints, wxInt32 sensitivity);
        void hairSearch(Mat& m, Rect oldFace, Rect newFace, vector<Point>& facePoints, wxInt32 sensitivity);
        bool headOK(Mat headMat, Mat hullMat, Rect headRect);
        void findColorRegion(Mat& m, Mat& mask, Point seed, wxInt32 dl);
        void findColorRegion(Mat& m, Mat& mask, Point seed, wxInt32 dl1,wxInt32 dl2,wxInt32 dl3);

        /** The ID for a person image, also the filename of the person image file. */
        wxInt32 myNextImageID;

        /** A list of source image file types (*.extension) that are searched. */
        wxArrayString *myTypes;

        /** A progress indicator while searching source image files. */
        wxProgressDialog *myProgress;

        /** The number of faces detected while searching image files. */
        wxInt32 myFaceCount;

        /** The number of source image files searched. */
        wxInt32 myFileCount;

        /** True if a reanalysis of folders is requested. */
        bool myRescan;
};

#endif	/* PEOPLEFINDER_H */

