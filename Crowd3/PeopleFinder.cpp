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

#include "PeopleFinder.h"
using namespace cv;

/** The face detection cascade object. */
CascadeClassifier faceCascade;

// Colors.
const Scalar redColor = Scalar(0,0,255);
const Scalar greenColor = Scalar(0,255,0);
const Scalar blueColor = Scalar(255,0,0);
const Scalar yellowColor = Scalar(0,255,255);

// Line drawing constants.
const wxInt32 lineWidth = 2;
const wxInt32 lineType = 8;

/** Create and initialize the PeopleFinder. */
PeopleFinder::PeopleFinder() {
    initFaceDetection();
    initImageTypes();
}

PeopleFinder::PeopleFinder(const PeopleFinder& orig) {}
PeopleFinder::~PeopleFinder() {}

/** Initialize the PeopleFinder object by loading the face detection cascade. */
void PeopleFinder::initFaceDetection() {
    wxString cascadeFile = Tools::dataFolder() + SEPARATOR + FACECASCADENAME;
    if ( ! faceCascade.load(Tools::wx2str(cascadeFile))) {
        Tools::logFatal(cascadeFile + _T("\nThe cascade file could not be loaded"));
    }
}

/** Initialize the list of source image types to search. */
void PeopleFinder::initImageTypes() {
    // Search files with these extensions.
    myTypes = new wxArrayString();
    myTypes->Add(_T("*.jpg"));
#ifdef __UNIX__
    myTypes->Add(_T("*.JPG")); // Linux filenames are case sensitive.
#endif
}

/**
 * Search for people in a source image. Write the people images to disk. Write
 * an image tree/database record for the source image file stating which people
 * image files belong to it. (ImageTree is the interface to the on-screen display
 * of source image files and the on-disk database.)
 * @param imageFile the source image pathname.
 * @return the number of people images found.
 */
wxInt32 PeopleFinder::findPeople(wxString imageFile) {
    // Skip source image files that are already in the database and whose
    // modification dates have not changed unless rescan is requested.
    const wxString dbDateFormat = _T("%d-%b-%Y %H:%M:%S");
    wxString dbModDate;
    wxInt32 dbFirstID = -1;
    wxInt32 dbLastID = -1;
    if (ImageTree::read(imageFile, dbModDate, dbFirstID, dbLastID)) {
        // imageFile is in the database. Check mod date. Honor rescan request.
        wxFileName imageFileNameObject(imageFile);
        wxDateTime md = imageFileNameObject.GetModificationTime();
        wxString osModDate = md.Format(dbDateFormat, wxDateTime::UTC);
        if ( (myRescan == false) && osModDate.IsSameAs(dbModDate)) {
            // Mod date has not changed. Skip this file.
            return 0;
        }
        else {
            // ImageFile is in the database but the mod date has changed or a
            // rescan is requested.
            // Delete imageFile's record. Delete the images associated with it.
            // Re-search the image file for faces.
            ImageTree::remove(imageFile);
            deleteOldImages(dbFirstID, dbLastID);
        }
    }

    // Read the source image file.
    string imageFilePath = Tools::wx2str(imageFile);
    Mat theImage = imread(imageFilePath, CV_LOAD_IMAGE_UNCHANGED);
    if ( ! theImage.data) {
        // Unsuccessful read. Quit.
        Tools::log(_T("An error occurred while trying to read ") +
                Tools::str2wx(imageFilePath));
        return 0;
    }

    // Get the next available image ID from settings. Image IDs are never reused.
    myNextImageID = Settings::getImageID();

    // Remember the first image id associated with this source image file.
    wxInt32 firstImageID = myNextImageID;
    wxInt32 lastImageID = -1;

    // Search for faces in the source image.
    vector<Rect> faces;
    faces = findFaces(theImage);

    // Convert grayscale images to color so that only 3-channel color images will
    // have to be dealt with from this point on.
    if (theImage.channels() == 1) {
        cvtColor(theImage, theImage, CV_GRAY2BGR);
    }
    // Convert non unsigned 8-bit images to 8-bit images. Not sure if this can occur.
    if (theImage.depth() != CV_8U) {
        theImage.convertTo(theImage, CV_8UC3);
    }

    // Enlarge each detected face and assume a body beneath it.
    for(wxInt32 i = 0; i < faces.size(); i++) {
        Rect aFaceRect = faces[i];

        // For debugging: Draw key rectangles on face. Disable call to maskHead when drawing.
        // rectangle(theImage, aFaceRect, blueColor, 3); // Outline the face.
        // Rect central = Rect(aFaceRect.x + aFaceRect.width/3, aFaceRect.y + aFaceRect.height/4, aFaceRect.width/3, aFaceRect.height/2);
        // rectangle(theImage, central, redColor, 3);
        // Rect topcentral = Rect(aFaceRect.x + aFaceRect.width/3, aFaceRect.y - aFaceRect.height/5, aFaceRect.width/3, aFaceRect.height/2);
        // rectangle(theImage, topcentral, yellowColor, 3);

        // Expand the face area to encompass the entire (mostly) head.
        Rect head = aFaceRect;
        head.x = head.x - ((HEADWIDTH - 1) * aFaceRect.width) / 2;
        head.width = HEADWIDTH * aFaceRect.width;
        head.y = head.y - ((HEADHEIGHT - 1) * aFaceRect.height);
        head.height = HEADHEIGHT * aFaceRect.height;
        // rectangle(theImage, head, greenColor, 2); // Outline the head.

        // Assume an area below head is part of the upper body.
        Rect body;
        body.x = head.x - ((BODYWIDTH - 1) * head.width) / 2;
        body.width = BODYWIDTH * head.width;
        body.y = head.y + head.height;
        body.height = BODYHEIGHT * head.height;

        // Bounds corrections: the enlarged rectangles surrounding the head
        // and body may extend beyond the image boundaries. Bring them back in.
        if (head.x < 0) {
            head.width = head.width + head.x; // Subtract head.x
            head.x = 0;
        }
        if (head.y < 0) {
            head.height = head.height + head.y; // Subtract head.y
            head.y = 0;
        }
        if (head.x + head.width > theImage.cols) {
            head.width = theImage.cols - head.x;
        }
        if (head.y + head.height > theImage.rows) {
            head.height = theImage.rows - head.y;
        }
        if (body.x < 0) {
            body.width = body.width + body.x; // Subtract body.x
            body.x = 0;
        }
        if (body.y < 0) {
            body.height = body.height + body.y; // Subtract body.y
            body.y = 0;
        }
        if (body.x + body.width > theImage.cols) {
            body.width = theImage.cols - body.x;
        }
        if (body.y + body.height > theImage.rows) {
            body.height = theImage.rows - body.y;
        }

        // Combine head & body then scale to a standard size.
        Mat p(theImage, Rect(body.x, head.y, body.width, head.height + body.height));
        Mat person;
        double scaleFactor = SCALEDFACEWIDTH/aFaceRect.width;
        resize(p, person, Size(), scaleFactor, scaleFactor);

        // Scale the face rectangle and make relative to head/body.
        aFaceRect.x = (aFaceRect.x - body.x) * scaleFactor;
        aFaceRect.y = (aFaceRect.y - head.y) * scaleFactor;
        aFaceRect.width = aFaceRect.width * scaleFactor;
        aFaceRect.height = aFaceRect.height * scaleFactor;

        // Make the head rectangle relative to the top-left of the scaled person.
        head.x = (head.x - body.x) * scaleFactor;
        head.y = 0;
        head.width = head.width * scaleFactor;
        head.height = head.height * scaleFactor;

        // Make regions around the head and shoulders invisible.
        maskHead(person, head, aFaceRect);

        // Write the person image to disk with uniqueImageID as filename.
        string prefix = Tools::wx2str(Tools::crowd3Folder() + SEPARATOR);
        string destPath = prefix + Tools::int2str(myNextImageID++) + ".png";
        imwrite(destPath, person);
    }

    // Write a record for this source image file.
    // Write a range of image IDs or write -1 if no image IDs.
    wxFileName imageFileNameObject(imageFile);
    wxDateTime md = imageFileNameObject.GetModificationTime();
    wxString osModDate = md.Format(dbDateFormat, wxDateTime::UTC);
    if (firstImageID == myNextImageID) {
        // There were no people found in this image. Write -1 for this file.
        firstImageID = -1;
        lastImageID = -1;
    }
    else {
        // Get the last image ID for this image file.
        lastImageID = myNextImageID - 1;
    }
    ImageTree::write(imageFile, osModDate, firstImageID, lastImageID);

    // Write the next available image ID to settings.
    Settings::setImageID(myNextImageID);

    return faces.size();
}

/**
 * Search recursively for faces in a user selected folder hierarchy.
 * @param parent The parent frame.
 * @param rescan True if the folder should be reanalyzed.
 */
void PeopleFinder::searchFolder(wxFrame *parent, bool rescan) {
    myRescan = rescan;

    // Ask user to select a folder to search for person images.
    wxString prompt = _T("Select a folder to search");
    if (myRescan) prompt = prompt + _T(" and REPLACE existing images");
    wxDirDialog *dd = new wxDirDialog(
            parent,
            prompt,
            Settings::getPersonPath(),
            wxDD_DIR_MUST_EXIST);
    if (dd->ShowModal() == wxID_CANCEL) {
        return;
    }
    wxString folder = dd->GetPath();
    Settings::setPersonPath(folder);
    wxDir dir(folder);

    // Check that the folder was successfully opened.
    if ( ! dir.IsOpened() ) {
        Tools::log(_T("An error occurred while trying to open ") + folder);
        return;
    }

    // Show a progress dialog displaying file searched, total files searched,
    // total faces found.
    myFaceCount = 0;
    myFileCount = 0; // Set to -1 to stop the search.
    myProgress = new wxProgressDialog(
            _T("Searching..."),
            _T("Searching..."),
            100,
            parent,
            wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_CAN_ABORT | wxPD_ELAPSED_TIME);
    myProgress->SetSize(myProgress->GetSize().GetWidth() * 2,
                        myProgress->GetSize().GetHeight());

    // Start the search.
    for (wxInt32 i = 0; i < myTypes->Count(); i++) {
        if (myFileCount >= 0) { // Search continuing...
            dir.Traverse(*this, myTypes->Item(i), wxDIR_DIRS | wxDIR_FILES);
        }
    }
    // Search complete. Sort the new source images into the image tree.
    ImageTree::sortImageTree();

    myProgress->Destroy();
}

/**
 * Called from searchFolder() with a discovered file when searching a folder hierarchy.
 * @param filename The discovered file path.
 * @return Continue flag.
 */
wxDirTraverseResult PeopleFinder::OnFile(const wxString& filename) {
    // Find the faces in the image.
    wxInt32 found = findPeople(filename);
    myFaceCount = myFaceCount + found;
    myFileCount++;

    if (myProgress->Pulse(filename +
            _T("\nFiles searched: ") + Tools::int2wx(myFileCount) +
            _T(", People found: ") + Tools::int2wx(myFaceCount))) {
        return wxDIR_CONTINUE;
    }
    else {
        myFileCount = -1; // Stop traversing.
        return wxDIR_STOP;
    }
}

/**
 * Called from searchFolder() with a discovered folder when searching a folder hierarchy.
 * @param dirname The discovered folder path.
 * @return Continue flag.
 */
wxDirTraverseResult PeopleFinder::OnDir(const wxString& dirname) {
    if (myProgress->Pulse(dirname +
            _T("\nFiles searched: ") + Tools::int2wx(myFileCount) +
            _T(", People found: ") + Tools::int2wx(myFaceCount))) {
        return wxDIR_CONTINUE;
    }
    else {
        myFileCount = -1; // Stop traversing.
        return wxDIR_STOP;
    }
}

/**
 * Called from searchFolder() when a discovered folder cannot be opened.
 * @param dirname The discovered folder path.
 * @return Ignore flag so that traversal will continue.
 */
wxDirTraverseResult PeopleFinder::OnOpenError(const wxString& dirname) {
    Tools::log(dirname + _T(" was skipped because it could not be opened."));
    return wxDIR_IGNORE;
}

/**
 * Search for faces in an image.
 * @param theImage a Mat structure containing an image.
 * @return a vector of discovered faces.
 */
std::vector<Rect> PeopleFinder::findFaces(Mat theImage) {
    // Convert color images to grayscale for the face search.
    Mat theImageGray;
    if (theImage.channels() != 1) {
        cvtColor(theImage, theImageGray, CV_RGB2GRAY);
        equalizeHist(theImageGray, theImageGray);
    }
    else {
        theImageGray = theImage;
    }

    // Search for faces that exceed a minimum size.
    wxInt32 faceMin = FACEPERCENT * min(theImageGray.rows, theImageGray.cols);
    std::vector<Rect> faces;
    faceCascade.detectMultiScale(theImageGray, faces, 1.1, 10,
            0 |CV_HAAR_SCALE_IMAGE, Size(faceMin, faceMin));

    return faces;
}

/**
 * Set pixels outside the head and shoulder areas to an "invisible" color that
 * can be detected and blended away when crowd images are created.
 * @param m A Mat structure containing a person image (head & body).
 * @param aHead The head rectangle in the person.
 * @param aFace The face rectangle discovered by face detection.
 */
void PeopleFinder::maskHead(Mat& m, Rect aHead, Rect aFace) {

    // This function has two parts:
    // 1. Use the person image features to mask out pixels around the head.
    // 2. Use a fixed-size small "keyhole" mask around the head and shoulders
    //    to define a minimum size image to be returned.

    // 1. Try to find the contour of the person's head by identifying the
    // face and hair, and then drawing a convex hull around it.

    // Top part of the image down to the bottom of the head.
    Mat mTop(m, Rect(0, 0, m.cols, max(aHead.height, aFace.y + aFace.height)));
    // Collection of discovered face/hair points.
    vector<Point> facePoints;
    // Contains points in a hull drawn around the discovered head.
    vector<vector<Point> >hullFacePoints(1);
    // The ellipse drawn around the hull around the discovered head.
    RotatedRect rotEll;
    // Contains an ellipse drawn around the discovered head.
    Mat hullMat = Mat::zeros(m.size(), m.type());
    // The new face and head rectangles resulting from face/hair searches.
    Rect newFace = Rect(0, 0, 0, 0);
    Rect newHead = Rect(0, 0, 0, 0);
    // For debugging: record the face and hair search sensitivity.
    int faceSens = 0;
    int hairSens = 0;

    // Search for face.
    // Find connected color regions for a set of points on the face.
    // The objective is to identify the entire set of face points. Sometimes
    // hair is also discovered.
    // Search with declining sensitivity trying to find the largest acceptable face.
    // Use only the top part of the image (mTop).
    vector<Point> facePointsCopy;
    Mat hullMatCopy;
    for (wxInt32 sens = 20; sens > 0; sens = sens - 5) {
        // Save facePoints and hullMat. Restore later.
        facePointsCopy = facePoints;
        hullMatCopy = hullMat.clone();

        faceSearch(mTop, aFace, facePointsCopy, sens);

        // Find a new face rectangle based on discovered face points.
        Rect faceCopy = Rect(0,0,0,0);
        if (facePointsCopy.size() > 2) {
            faceCopy = boundingRect(facePointsCopy);

            // First test for acceptable face.  If face has grown too much
            // then reject and try search zerowith lower sensitivity.
            if ((faceCopy.width / (double) aFace.width) > 1.5) {
                continue;
            }

            // Construct a hull around all the discovered face and hair points.
            convexHull(facePointsCopy, hullFacePoints[0], false);
        }

        // Smooth the hull by drawing an ellipse around it.
        if (hullFacePoints[0].size() >= 5) {
            rotEll = fitEllipse(hullFacePoints[0]);

            // Draw the ellipse (filled) on hullMat.
            ellipse(hullMatCopy, rotEll, greenColor, -lineWidth, lineType); // Filled
        }

        // Second test for an acceptable face.
        if (headOK(mTop, hullMatCopy, faceCopy)) {
            // Acceptable. Restore facePoints and hullMat.
            hullMat = hullMatCopy;
            facePoints = facePointsCopy;
            newFace = faceCopy;
            faceSens = sens;
            break;
        }
    }

    /* For debugging: clear the face points so we can examine only hair points.
        facePoints.clear();
    */

    // Search for hair if a face was found. Same method as face search.
    if (newFace.width > 0) {
        for (wxInt32 sens = 20; sens > 0; sens = sens - 5) {
            // Save facePoints and hullMat. Restore later.
            facePointsCopy = facePoints;
            hullMatCopy = hullMat.clone();

            hairSearch(mTop, aFace, newFace, facePointsCopy, sens);

            // Find a new head rectangle based on discovered face and hair points.
            Rect headCopy = Rect(0,0,0,0);
            if (facePointsCopy.size() > 2) {
                headCopy = boundingRect(facePointsCopy);

                // First test for acceptable head.  If head has grown too much
                // then reject and try search with lower sensitivity.
                if ((headCopy.width / (double) aHead.width) > 1.7) {
                    continue;
                }

                // Construct a hull around all the discovered face and hair points.
                convexHull(facePointsCopy, hullFacePoints[0], false);
            }

            // Smooth the hull by drawing an ellipse around it.
            if (hullFacePoints[0].size() >= 5) {
                rotEll = fitEllipse(hullFacePoints[0]);

                // Draw the ellipse (filled) on hullMat.
                ellipse(hullMatCopy, rotEll, greenColor, -lineWidth, lineType); // Filled
            }

            // Second test for an acceptable head.
            if (headOK(mTop, hullMatCopy, headCopy)) {
                // Acceptable. Restore facePoints and hullMat.
                hullMat = hullMatCopy;
                facePoints = facePointsCopy;
                newHead = headCopy;
                hairSens = sens;
                break;
            }
        }
    }

    // Note that rotEll and hullFacePoints[0] are invalid if no acceptable head found.

    /* For debugging: set discovered face/hair points to red.
    for (wxInt32 i = 0; i<facePoints.size(); i++) {
        Point p = facePoints.at(i);
        m.at<Vec3b>(p) = Vec<unsigned char,3>(0,0,255);
    }
    */

    /* For debugging: Output image data.
    cout << myNextImageID  << " " <<
            faceSens       << " " << hairSens     << " " <<
            newHead.width  << " " << aHead.width  << " " <<
            newHead.height << " " << aHead.height << " " <<
            newFace.width  << " " << aFace.width  << " " <<
            newFace.height << " " << aFace.height << " " << endl;
    */

    /* For debugging: show face/hair discoveries and contours. Skip keyhole.
        rectangle(m, aFace, yellowColor, lineWidth); // Outline the old face.
        rectangle(m, newFace, blueColor, lineWidth); // Outline the new face.
        drawContours(m, hullFacePoints, 0, redColor, lineWidth, lineType, vector<Vec4i>(), 0, Point()); // Draw the hull.
        ellipse(m, rotEll, greenColor, lineWidth, lineType); // Draw the smoothed hull.
        return; // Skip keyhole.
    */

    // 2. Define a small keyhole outline around the head and body.
    // The small keyhole defines the minimum size of the person image to be
    // returned.  The image may be larger if the hull extends outside the keyhole.

    wxInt32 hx, hy, hw, hh, hcx, hcy, h2, sx, sy, s2;
    // Use either the old (original) head or the new head as center of the keyhole.
    // If a good new head wasn't found use the old head.
    // Compute head and shoulder characteristics using the choice.

    // Test for a too narrow head.  If so, default to old head and throw away
    // face/hair discoveries.
    if ((newHead.width / (double) aHead.height) < 0.5) {
        hullMat = Mat::zeros(m.size(), m.type());
        newHead.width = 0;
    }

    // Test for a narrow new face width vs. old face width.
    if (newHead.width > 0 && (newFace.width / (double) aFace.width) >= 0.7) {
        // It's likely that a good head was discovered.  Use newHead.

        // Get y, height, y-center of new head.
        hy = newHead.y;
        hh = newHead.height;
        hcy = hy + hh/2;

        // Find the actual head x and width half way down the head.
        for (wxInt32 c = 0; c < m.cols; c++) {
            if (hullMat.at<Vec3b>(hcy, c) != Vec<unsigned char,3>(0,0,0)) {
                hx = c;
                break;
            }
        }
        for (wxInt32 c = m.cols - 1; c > 0; c--) {
            if (hullMat.at<Vec3b>(hcy, c) != Vec<unsigned char,3>(0,0,0)) {
                hw = c - hx;
                break;
            }
        }

        // Get x-center of new head.
        hcx = hx + hw/2;

        // Suppress top (round) part of keyhole since there is a good head.
        // Set square of head radius (h2) to zero to suppress top part of keyhole.
        h2=0;
    }
    else {
        // Use old head as default.
        hx = aHead.x;
        hy = aHead.y;
        hw = aHead.width;
        hh = aHead.height;

        // Compute head center (hx, hy) and square of head radius (h2).
        hcx = hx + hw/2;
        hcy = aFace.y + (aFace.height / 5); // (hy is always = 0, can't be used.)
        h2 = hw/2 * hw/2;
    }

    // Compute shoulder center (sx, sy) and square of shoulder radius (s2).
    // s2 is distance from (sx, sy) to leftmost point where head and body meet.
    sx = hcx;
    wxInt32 radius = max(hcx, m.cols - hcx);
    sy = hy + hh + radius;
    s2 = (hx - sx) * (hx - sx) + (hy + hh - sy) * (hy + hh - sy);

    // Construct the keyhole:
    // Upper half of head: black out points outside the hull but not inside
    // the head circle.
    for (wxInt32 c = 0; c < m.cols; c++) {
        for (wxInt32 r=0; r<hcy; r++) {
            for (wxInt32 r=0; r<hcy; r++) {
                if (hullMat.at<Vec3b>(r, c) != Vec<unsigned char,3>(0,0,0)) break;
                if ((hcx-c)*(hcx-c) + (hcy-r)*(hcy-r) > h2) {
                    m.at<Vec3b>(r, c) = CV_COLOR_TRANSPARENT;
                }
            }
        }
    }

    // Lower half of head: black out points outside the hull but not inside
    // the head rectangle.
    for(wxInt32 r = 0; r < hy + hh; r++) {
        // Black out left of head.
        for(wxInt32 c = 0; c < hx; c++) {
            if (hullMat.at<Vec3b>(r, c) != Vec<unsigned char,3>(0,0,0)) break;
            m.at<Vec3b>(r, c) = CV_COLOR_TRANSPARENT;
        }
        // Black out right of head.
        for(wxInt32 c = m.cols - 1; c > hx + hw; c--) {
            if (hullMat.at<Vec3b>(r, c) != Vec<unsigned char,3>(0,0,0)) break;
            m.at<Vec3b>(r, c) = CV_COLOR_TRANSPARENT;
        }
    }

    // Upper half of body: black out points outside of shoulder circle but not
    // inside the hull.
    for (wxInt32 c = 0; c < m.cols; c++) {
        for (wxInt32 r=hy + hh; r<min(sy, m.rows); r++) {
            if (hullMat.at<Vec3b>(r, c) == Vec<unsigned char,3>(0,0,0)) {
                if ((sx-c)*(sx-c) + (sy-r)*(sy-r) > s2) {
                    m.at<Vec3b>(r, c) = CV_COLOR_TRANSPARENT;
                }
            }
        }
    }

    // Finally, remove top rows of the image that are marked as invisible.
   for (wxInt32 r = 0; r < m.rows; r++) {
       for (wxInt32 c = 0; c < m.cols; c++) {
            if (m.at<Vec3b>(r, c) != CV_COLOR_TRANSPARENT) {
                // Remove rows above r from mat m.
                m = m.rowRange(r, m.rows);
                return;
            }
        }
    }
}

/**
 * Delete a set of obsolete person image files. Image IDs are sequential.
 * @param firstID Image ID of first image or -1 if no images.
 * @param lastID Image ID of last image.
 */
void PeopleFinder::deleteOldImages(wxInt32 firstID, wxInt32 lastID) {
    if (firstID != -1) {
        wxString prefix = Tools::crowd3Folder() + SEPARATOR;
        for (wxInt32 id = firstID; id <= lastID; id++) {
            wxString imagePath = prefix + Tools::int2wx(id) + _T(".png");
            // Ignore errors since the files may have already been deleted.
            wxRemoveFile(imagePath);
        }
    }
}

/**
 * Isolate the skin points on a discovered face by searching for colors similar
 * to those of the skin points on a cross through a central rectangle on the
 * discovered face.
 * @param m An image
 * @param aFace A discovered face on m.
 * @param facePoints The skin colored face points discovered.
 * @param sensitivity The sensitivity of the search.
 */
void PeopleFinder::faceSearch(Mat& m, Rect aFace,
        vector<Point>& facePoints, wxInt32 sensitivity) {
    Point seed;

    // Create a mask slightly larger than the image for floodFill.
    Mat imgMask = Mat::zeros(m.rows+2, m.cols+2, CV_8UC1);

    // Vertical part of the cross:
    for (wxInt32 y = aFace.y + aFace.height/4; y < aFace.y + aFace.height*3/4; y=y+2) {
        seed = Point(aFace.x + aFace.width/2, y);
        findColorRegion(m, imgMask, seed, sensitivity);
    }
    // Horizontal part of the cross:
    for (wxInt32 x = aFace.x + aFace.width/3; x < aFace.x + aFace.width*2/3; x=x+2) {
        seed = Point(x, aFace.y + aFace.height/2);
        findColorRegion(m, imgMask, seed, sensitivity);
    }

    // Reduce the mask to the image size.
    Mat imgSMask(imgMask, Rect(1, 1, m.cols, m.rows));

    // Test the leftmost and rightmost discovered point half way down the face
    // to try to extend the sides of the head.
    wxInt32 r = aFace.y + aFace.height/2;
    for (wxInt32 c = 0; c < m.cols; c++) {
        if (imgSMask.at<unsigned char>(r, c) == 1) {
            seed = Point(c, r);
            findColorRegion(m, imgMask, seed, sensitivity);
            break;
        }
    }
    for (wxInt32 c = m.cols-1; c >= 0; c--) {
        if (imgSMask.at<unsigned char>(r, c) == 1) {
            seed = Point(c, r);
            findColorRegion(m, imgMask, seed, sensitivity);
            break;
        }
    }

    // Get the points from the mask to facePoints.
    for (wxInt32 c = 0; c < m.cols; c++) {
        for (wxInt32 r = 0; r < m.rows; r++) {
            if (imgSMask.at<unsigned char>(r, c) == 1) {
                facePoints.push_back(Point(c, r));
            }
        }
    }
}

/**
 * Search for hair points.
 * @param m An Image.
 * @param oldFace A discovered face on m.
 * @param newFace A face expanded by a faceSearch.
 * @param facePoints The skin and hair points discovered.
 * @param sensitivity The sensitivity of the search.
 */
void PeopleFinder::hairSearch(Mat& m, Rect oldFace, Rect newFace,
        vector<Point>& facePoints, wxInt32 sensitivity) {
    Point seed;

    // Create a mask slightly larger than the image for floodFill.
    Mat imgMask = Mat::zeros(m.rows+2, m.cols+2, CV_8UC1);

    // Search differently for hair depending on the face growth due to the face search.
    if ((newFace.height / (double) oldFace.height) > 1.25) {
        // If the new face grew significantly:
        // Search upwards from the old face toward but not reaching the new face.
        wxInt32 x = newFace.x + newFace.width/2;
        for (wxInt32 y = oldFace.y;
                y > std::max(0.0, newFace.y + newFace.height - 1.20 * oldFace.height); y=y-2) {
            seed = Point(x, y);
            findColorRegion(m, imgMask, seed, sensitivity);
        }
    }
    else {
        // Search the center of the newFace top line.
        for (wxInt32 x = newFace.x + newFace.width*4/10;
                x < newFace.x + newFace.width*6/10; x = x + 4) {
            seed = Point(x, newFace.y);
            findColorRegion(m, imgMask, seed, sensitivity);
        }

        // Search slightly upwards from the newFace top line.
        for (wxInt32 y = newFace.y; y > max(0, newFace.y - newFace.height/20); y = y - 2) {
            seed = Point(newFace.x + newFace.width/2, y);
            findColorRegion(m, imgMask, seed, sensitivity);
        }

        // Near left corner of oldFace top line.
        // If top-left corner of old face is inside top-left corner of new face
        // then test a point on old face top line in from corner.
        if (oldFace.x > newFace.x && oldFace.y > newFace.y) {
            seed = Point(oldFace.x + oldFace.width/10, oldFace.y);
            findColorRegion(m, imgMask, seed, sensitivity);
        }

        // Near right corner of oldFace top line.
        // If top-right corner of old face is inside top-right corner of new face
        // then test a point on old face top line in from corner.
        if (oldFace.x + oldFace.width < newFace.x + newFace.width) {
            seed = Point(oldFace.x + oldFace.width*9/10, oldFace.y);
            findColorRegion(m, imgMask, seed, sensitivity);
        }

        // Near left and right corners of oldFace top line.
        // If newFace sides are inside oldFace sides and newFace top is
        // above oldFace top then test points in from corners on oldFace top.
        if (oldFace.y > newFace.y && newFace.x > oldFace.x &&
                newFace.x + newFace.width < oldFace.x + oldFace.width) {
            seed = Point(newFace.x + newFace.width/10, oldFace.y);
            findColorRegion(m, imgMask, seed, sensitivity);
            seed = Point(newFace.x + newFace.width*9/10, oldFace.y);
            findColorRegion(m, imgMask, seed, sensitivity);
        }

        // If newFace top corners are inside oldFace top corners then test
        // near corners of both faces.
        if (newFace.x > oldFace.x &&
                newFace.x + newFace.width < oldFace.x + oldFace.width &&
                newFace.y > oldFace.y) {
            // Test 20% from corners on old line, at corners on new line.
            seed = Point(oldFace.x + oldFace.width*2/10, oldFace.y);
            findColorRegion(m, imgMask, seed, sensitivity);
            seed = Point(oldFace.x + oldFace.width*8/10, oldFace.y);
            findColorRegion(m, imgMask, seed, sensitivity);
            seed = Point(newFace.x, newFace.y);
            findColorRegion(m, imgMask, seed, sensitivity);
            seed = Point(newFace.x + newFace.width, newFace.y);
            findColorRegion(m, imgMask, seed, sensitivity);
        }
    }

    // Reduce the mask to the image size.
    Mat imgSMask(imgMask, Rect(1, 1, m.cols, m.rows));

    // Test the topmost discovered point in the center of the new face
    // to try to extend the top of the head..
    wxInt32 c = newFace.x + newFace.width/2;
    for (wxInt32 r = 0; r < m.rows; r++) {
        if (imgSMask.at<unsigned char>(r, c) == 1) {
            seed = Point(c, r);
            findColorRegion(m, imgMask, seed, sensitivity);
            break;
        }
    }
    // test the leftmost point on the new face top line
    wxInt32 r = newFace.y;
    for (wxInt32 c = 0; c < m.cols; c++) {
        if (imgSMask.at<unsigned char>(r, c) == 1) {
            seed = Point(c, r);
            findColorRegion(m, imgMask, seed, sensitivity);
            break;
        }
    }
    // Test the rightmost point on the new face top line
    for (wxInt32 c = m.cols-1; c >=0; c--) {
        if (imgSMask.at<unsigned char>(r, c) == 1) {
            seed = Point(c, r);
            findColorRegion(m, imgMask, seed, sensitivity);
            break;
        }
    }

    // test the leftmost point on the old face top line
    r = oldFace.y;
    for (wxInt32 c = 0; c < m.cols; c++) {
        if (imgSMask.at<unsigned char>(r, c) == 1) {
            seed = Point(c, r);
            findColorRegion(m, imgMask, seed, sensitivity);
            break;
        }
    }
    // Test the rightmost point on the old face top line
    for (wxInt32 c = m.cols-1; c >=0; c--) {
        if (imgSMask.at<unsigned char>(r, c) == 1) {
            seed = Point(c, r);
            findColorRegion(m, imgMask, seed, sensitivity);
            break;
        }
    }

    // Get the points from the mask to facePoints.
    for (wxInt32 c = 0; c < m.cols; c++) {
        for (wxInt32 r = 0; r < m.rows; r++) {
            if (imgSMask.at<unsigned char>(r, c) == 1) {
                facePoints.push_back(Point(c, r));
            }
        }
    }
}

/**
 * Find a contiguous region in an image with similar colors.
 * @param img The image.
 * @param imgMask Returned cumulative mask indicating points matching color of interest.
 * @param seed A starting point for the region with the color of interest.
 * @param dl The delta to define similar colors.
 */
void PeopleFinder::findColorRegion(Mat& img, Mat& imgMask, Point seed, wxInt32 dl) {
    findColorRegion(img, imgMask, seed, dl, dl, dl);
}

/**
 * Find a contiguous region in an image with similar colors.
 * @param img The image.
 * @param imgMask Returned cumulative points matching color of interest.
 * @param seed A starting point for the region with the color of interest.
 * @param dl1 The delta to define similar colors, subscript 1.
 * @param dl2 The delta to define similar colors, subscript 2.
 * @param dl3 The delta to define similar colors, subscript 3.
 */
void PeopleFinder::findColorRegion(Mat& img, Mat& imgMask, Point seed,
                                   wxInt32 dl1, wxInt32 dl2, wxInt32 dl3) {
    Scalar loDiff = Scalar(dl1, dl2, dl3);
    Scalar upDiff = Scalar(dl1, dl2, dl3);

    // Find the region. Set the mask to 1 wherever there is a similar color.
    Rect region;
    Mat newMask = Mat::zeros(imgMask.rows, imgMask.cols, CV_8UC1);
    floodFill(img, newMask, seed, Scalar(1), &region, loDiff, upDiff,
            FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY + 8);

    // Copy discoveries from newMask to imgMask.
    for (wxInt32 c = 0; c < imgMask.cols; c++) {
        for (wxInt32 r = 0; r < imgMask.rows; r++) {
            if (newMask.at<unsigned char>(r, c) == 1) {
                imgMask.at<unsigned char>(r, c) = 1;
            }
        }
    }
}

/**
 * Sanity check the discovered head. If it consumes the entire width or
 * length of the image of if its ellipse extends more than a little over the
 * image top, or it extends over both left and right image bounds, or it
 * covers a significant fraction of the left or right edge, assume the
 * face/hair search failed.
 * Typically failures occur because background colors are too similar to face
 * or hair colors or because other people are in the image.
 * @param headMat The portion of the image where the head was discovered.
 * @param hullMat A scratch pad where an ellipse surrounding the head was drawn.
 * @param headRect The rectangle around the discovered head.
 * @return true if the head is a reasonable size, else false.
 */
bool PeopleFinder::headOK(Mat headMat, Mat hullMat, Rect headRect) {
    // Check the head dimensions against the image dimensions.  If the head
    // is as large as the image width or height then assume a failed search.
    if (headRect.width == headMat.cols || headRect.height == headMat.rows) {
        return false;
    }

    // Sanity check the ellipse around the head.
    bool overTopEdge = false;
    bool overLeftEdge = false;
    bool overRightEdge = false;
    wxInt32 tCount = 0; // ellipse pixels on top edge.
    wxInt32 lCount = 0; // ellipse pixels on left edge.
    wxInt32 rCount = 0; // ellipse pixels on right edge.
    wxInt32 tMax = 0.10 * headMat.cols; // max pixels allowed on top edge.
    wxInt32 lrMax = 0.5 * headMat.rows; // max pixels allowed on left/right edge.

    // Check top edge. A little (tMax) over the top edge is OK.
    for (wxInt32 c = 0; c < headMat.cols; c++) {
        if (hullMat.at<Vec3b>(0, c) != Vec<unsigned char,3>(0,0,0)) {
            if (tCount++ > tMax) {
                overTopEdge = true;
                break;
            }
        }
    }

    if ( ! overTopEdge) {
        // Check left and right edges.
        for (wxInt32 r = 0; r < headMat.rows; r++) {
            if (hullMat.at<Vec3b>(r, 0) != Vec<unsigned char,3>(0,0,0)) {
                overLeftEdge = true;
                if (lCount++ > lrMax) break;
            }
        }
        for (wxInt32 r = 0; r < headMat.rows; r++) {
            if (hullMat.at<Vec3b>(r, headMat.cols-1) != Vec<unsigned char,3>(0,0,0)) {
                overRightEdge = true;
                if (rCount++ > lrMax) break;
            }
        }
    }

    if (overTopEdge || (overLeftEdge && overRightEdge) ||
            lCount > lrMax || rCount > lrMax) {
        // A good head has not been found.
        return false;
    }
    return true;
}
