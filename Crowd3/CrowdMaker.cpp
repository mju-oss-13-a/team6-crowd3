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

#include "CrowdMaker.h"
#include "ImageTree.h"

/** Create a CrowdMaker object with user's default settings. */
CrowdMaker::CrowdMaker() {
    // Get default values from user preferences. 
    // (Update user preferences when a Make is performed.)
    loadAllSettings();
    
    // Initialize random number generator
    srand(wxDateTime::UNow().GetMillisecond());
}

CrowdMaker::CrowdMaker(const CrowdMaker& orig) {}
CrowdMaker::~CrowdMaker() {}

/** Get all crowd settings from user preferences. */
void CrowdMaker::loadAllSettings() {
    myBackgroundPath = Settings::getBackgroundPath();
    wxSize is = Settings::getCrowdImageSize();
    myImageWidth = is.GetWidth();
    myImageHeight = is.GetHeight();
    myPeopleCount = Settings::getPeopleCount();
    setPerspective(Settings::getPerspective());
    
    // These are not saved as user preferences:
    myImageFiles = new wxArrayString();
    myCurrentCrowd = new wxArrayString();
    myCrowd = new wxImage(myImageWidth, myImageHeight);
}

/** Save all crowd settings to user preferences. */
void CrowdMaker::saveAllSettings() {
    Settings::setBackgroundPath(myBackgroundPath);
    Settings::setCrowdImageSize(wxSize(myImageWidth, myImageHeight));
    Settings::setPeopleCount(myPeopleCount);
    Settings::setPerspective(myUsingPer);
    // These are not saved as user preferences: myImageFiles, myCurrentCrowd, myCrowd.
}

/**
 * Set the background image for the crowd image. The background image determines
 * the size of the crowd image.
 * @param anImagePath path to an image file.
 */
void CrowdMaker::setBackgroundPath(wxString anImagePath) {
    myBackgroundPath = anImagePath;
}

/**
 * Get the background image for the crowd image. The background image determines
 * the size of the crowd image.
 */
wxString CrowdMaker::getBackgroundPath() {
    return myBackgroundPath;
}

/**
 * Set the size of the image if there is no background image.
 * @param aWidth Image width.
 * @param aHeight Image height.
 */
void CrowdMaker::setImageSize(wxInt32 aWidth, wxInt32 aHeight) {
    myImageWidth = aWidth;
    myImageHeight = aHeight;
    myCrowd = new wxImage(myImageWidth, myImageHeight);
}

/** Get the size of the image if there is no background image. */
wxSize CrowdMaker::getImageSize() {
    wxSize w = wxSize(myImageWidth, myImageHeight);
    return w;
}

/**
 * Set the number of people in the crowd image.
 * @param aCount The number of people.
 */
void CrowdMaker::setPeopleCount(wxInt32 aCount) {
    myPeopleCount = aCount;
}

/**
 * Get the number of people in the crowd image.
 * @return The people count.
 */
wxInt32 CrowdMaker::getPeopleCount() {
    return myPeopleCount;
}

/**
 * Set the perspective flag and factor for the image.
 * @param perSetting The perspective flag. true==use Perspective.
 */
void CrowdMaker::setPerspective(bool perSetting) {
    myUsingPer = perSetting;
    if (myUsingPer) {
        myPerFactor = 0.9; // perspective
    }
    else {
        myPerFactor = 1.0; // no perspective
    }
}

/** Get the Using Perspective flag for the image. */
bool CrowdMaker::getPerspective() {
    return myUsingPer;
}

/** Make a crowd image using a random selection of images. */
void CrowdMaker::makeCrowdImage(wxFrame *parent) {
    // Save all settings as user preferences.
    saveAllSettings();
    
    // Get the list of requested people image files.
    myImageFiles = ImageTree::getSelectedPeopleFiles();
    
    // Select myPeopleCount images randomly from myImageFiles. Set myCurrentCrowd.
    myCurrentCrowd->Empty();
    wxInt32 fileCount = myImageFiles->Count();
    if (fileCount == 0) {
        Tools::log(_T("There are no image files selected or available.\n")
                   _T("Run the Find command or make a different file selection"));
        return;
    }
    for (wxInt32 i = 0; i < myPeopleCount; i++) {
        wxInt32 aFileIndex = rand() % fileCount;
        myCurrentCrowd->Add(myImageFiles->Item(aFileIndex));
    }
    
    // Build a crowd image.
    assembleTheImage(parent);
}

/** Make a new crowd image after shuffling the order of people in the current 
 * crowd image. */
void CrowdMaker::shuffle(wxFrame *parent) {
    // Randomize the order of the images in myCurrentCrowd by swapping random
    // elements a bunch of times.
    wxInt32 crowdCount = myCurrentCrowd->Count();
    if (crowdCount == 0) {
        // No crowd yet. Do a Make before a Shuffle.
        return;
    }
    for (wxInt32 i = 0; i < crowdCount * 2; i++) {
        wxInt32 r1 = rand() % crowdCount;
        wxInt32 r2 = rand() % crowdCount;
        wxString temp = myCurrentCrowd->Item(r1);
        myCurrentCrowd->Item(r1) = myCurrentCrowd->Item(r2);
        myCurrentCrowd->Item(r2) = temp;
    }
    
    // Build a crowd image.
    assembleTheImage(parent);
}

/** Construct the crowd image in myCrowd using all the settings. */
void CrowdMaker::assembleTheImage(wxFrame *parent) {
    // Reinitialize the crowd image and reload the background image.
    if (myBackgroundPath.length() > 0) {
        if ( ! myCrowd->LoadFile(myBackgroundPath, wxBITMAP_TYPE_JPEG)) {
            Tools::log(_T("An error occurred while trying to read ") + myBackgroundPath);
            return;
        }
        
        // Slightly blur the background image to suggest depth.
        *myCrowd = myCrowd->Blur(5);

        myImageWidth = myCrowd->GetWidth();
        myImageHeight = myCrowd->GetHeight();
    }
    else {    
        myCrowd = new wxImage(myImageWidth, myImageHeight);
    }
    
    // Show a progress dialog displaying count of images added to the crowd.
    myProgress = new wxProgressDialog(
            _T("Creating a crowd image"),
            _T("Starting..."),
            myPeopleCount,
            parent,
            wxPD_APP_MODAL | wxPD_SMOOTH | wxPD_CAN_ABORT | 
            wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME);
    myProgress->SetSize(myProgress->GetSize().GetWidth() * 2,
                        myProgress->GetSize().GetHeight());
    
    // Estimate rows and columns of people proportional to size of the image.
    // To estimate, assume a rectangular grid of people:
    // columnsOfPeople * rowsOfPeople = myPeopleCount
    // columnsOfPeople = m * aspectRatio * rowsofPeople
    // m * aspectRatio * rowsOfPeople * rowsOfPeople = myPeopleCount
    // rowsOfPeople = SQRT(myPeopleCount / (m * aspectRatio))
    // row0people = columnsOfPeople = (number of people in row 0)
    double m = 0.0; // Empirical. Lower->more rows, fewer columns.
    if (myUsingPer) {
        m = 0.25; // Perspective: More rows, fewer columns, more face overlap.
    }
    else {
        m = 0.75; // No perspective: Fewer rows, more columns, less face overlap.
    }
    double aspectRatio = ((double) myImageWidth) / ((double) myImageHeight);
    wxInt32 rowsOfPeople = sqrt(((double) myPeopleCount) / (m * aspectRatio));
    wxInt32 row0people = ceil(m * aspectRatio * rowsOfPeople); // round up.
    
    // Assign actual numbers of people to each row. Alternate the leftmost
    // position of the first person in each row so that heads can be between
    // the heads of the prior row. Compute a scale factor for each row using
    // the perspective factor.
    ArrayOfInts rowPopulation;
    rowPopulation.Empty();
    ArrayOfDoubles rowScale;
    rowScale.Empty();
    
    // Based on the number of people in row 0, determine how to scale the people
    // images so that they will fit. (Scale to shrink, never to enlarge.)
    wxInt32 targetWidth = min(myImageWidth / (double) row0people, (double) PERSONWIDTH);
    double row0ScaleFactor = min(targetWidth / (double) PERSONWIDTH, 1.0);
    
    // Loop through the rows assigning as many people as will fit. Compute a
    // scale factor for each row.
    wxInt32 peopleRemaining = myPeopleCount;
    wxInt32 row = 0;
    while (peopleRemaining > 0) {
        double rowScaleFactor = row0ScaleFactor * pow(myPerFactor, row);
        wxInt32 rowPeople = myImageWidth / (PERSONWIDTH * rowScaleFactor);
        if (row % 2) { // odd numbered rows.
            // One fewer person in odd rows because they are indented.
            rowPeople--;
        }
        // Increase the people in rows by an empirical amount to force images
        // all the way to the right edge. Because some images are narrow, the
        // right edge of the crowd image may otherwise be unpopulated.
        // (We're not reading the image files at this point so we don't know
        // actual widths.)
        rowPeople = rowPeople + 2 + row/8;
        
        // Collect row data.
        rowScale.Add(rowScaleFactor);
        rowPopulation.Add(min(peopleRemaining, rowPeople));
        peopleRemaining = peopleRemaining - rowPopulation.Item(row);
        row++;
    }
        
    // Determine the vertical space required for the heads in all rows.
    // idealHeadRoom is the vertical space for the row0 heads. Alternatives:
    //wxInt32 idealHeadRoom = row0ScaleFactor * HEADHEIGHT * SCALEDFACEWIDTH / 2; // half head
    wxInt32 idealHeadRoom = row0ScaleFactor * HEADHEIGHT * SCALEDFACEWIDTH; // full head
    
    // Is there enough vertical space for all rows using the ideal value?
    wxInt32 headSpaceRequired = 0;
    for (int r=0; r<rowPopulation.GetCount(); r++) {
        headSpaceRequired = headSpaceRequired + idealHeadRoom * pow(myPerFactor, r);
    }
    wxInt32 actualHeadRoom = 0;
    // Is the image large enough for this head space plus row0 bodies?
    if (headSpaceRequired + row0ScaleFactor * PERSONHEIGHT < myImageHeight) {
        // There is enough vertical space for the ideal value.
        actualHeadRoom = idealHeadRoom;
    }
    else {
        // Insufficient space for the ideal value. Scale it down.
        actualHeadRoom = idealHeadRoom * 
                ((double) myImageHeight - row0ScaleFactor * PERSONHEIGHT) / 
                ((double) headSpaceRequired);
    }

    // Determine the vertical position of the top of each row. Start with the 
    // vertical position of row 0.
    ArrayOfInts rowPosition;
    wxInt32 row0Position = (double) myImageHeight - row0ScaleFactor * PERSONHEIGHT;
    rowPosition.Add(row0Position);
    for (wxInt32 r = 1; r < rowPopulation.GetCount(); r++) {
        // Set headroom for each row. Alternatives:
          // Decrease headroom by perspective factor.
          rowPosition.Add(rowPosition.Item(r - 1) - actualHeadRoom * pow(myPerFactor, r));
            // Decrease headroom faster than perspective factor.
            //rowPosition.Add(rowPosition.Item(r - 1) - actualHeadRoom * pow(myPerFactor, r) * myPerFactor);
    }
    
    // Add people images to the crowd image from back row to front row so that
    // front people will partially obscure back people.
    wxInt32 crowdMember = 0;
    wxInt32 mCol = 0;
    wxInt32 mRow = 0;
    for (wxInt32 r = rowPopulation.GetCount() - 1; r >= 0; r--) { // for each row...
        // Get scale for the row.
        double rScale = rowScale.Item(r);
        
        // Get target width for the row.
        wxInt32 rowTargetWidth = targetWidth * pow(myPerFactor, max(0,r-1));
        
        // Compute the horizontal position of the first person image in the row.
        if (r % 2) { // odd numbered rows.
            // leftmost image is right indented quarter image.
            mCol = rowTargetWidth / 4;
        }
        else { // even numbered rows.
            // leftmost image is left indented quarter image.
            mCol = -rowTargetWidth / 4;
        }
        
        for (wxInt32 p = 0; p < rowPopulation.Item(r); p++) {// for each person in row...
            // Read a person image file. 
            wxString aFilePath = Tools::crowd3Folder() + 
                    SEPARATOR + myCurrentCrowd->Item(crowdMember);
            wxImage *aPerson = new wxImage();
            if ( ! aPerson->LoadFile(aFilePath, wxBITMAP_TYPE_PNG)) {
                Tools::log(_T("An error occurred while trying to read ") + aFilePath);
                continue;
            }
          
            // Mark the invisible pixels using the color mask and the alpha channel.
            aPerson->SetMaskColour(
                WX_COLOR_TRANSPARENT[0], 
                WX_COLOR_TRANSPARENT[1], 
                WX_COLOR_TRANSPARENT[2]);
            aPerson->InitAlpha();
            
            // Scale the person image to desired width. Apply perspective.
            aPerson->Rescale(aPerson->GetWidth() * rScale, 
                             aPerson->GetHeight() * rScale, wxIMAGE_QUALITY_HIGH);
          
            // Vertical position (adjust for short images)
            mRow = rowPosition.Item(r);
            if (aPerson->GetHeight() < FULLPERSONHEIGHT * rScale) {
                mRow = mRow + FULLPERSONHEIGHT * rScale - aPerson->GetHeight();
            }
            
            // Merge the person image into the crowd image.
            crowdMerge(aPerson, mRow, mCol);

            // Next person image.
            crowdMember++;
            
            // Column position of next person image. Alternatives:
            //mCol = mCol + aPerson.cols; // exact width of person
            //mCol = mCol + targetWidth * pow(myPerFactor, 0); // target width in row_0
            //mCol = mCol + targetWidth * pow(myPerFactor, r); // target width in row_r
            //mCol = mCol + targetWidth * pow(myPerFactor, max(0, r-1)); // target width in row_r-1
            // When image width in a row starts to shrink and reveal background...
            if (pow(myPerFactor, r) < 0.75 * pow(myPerFactor, 0)) {
                // Pack back row people closely together.
                mCol = mCol + aPerson->GetWidth(); // exact width of person
            }
            else {
                // Line up front row people.
                mCol = mCol + targetWidth * pow(myPerFactor, 0); // target width in row_0
            }
            
            // Update progress.
            if ( ! myProgress->Update(crowdMember, 
                    _T("Images added: ") + Tools::int2wx(crowdMember))) {
                // Cancelled.  Clear the image.
                myCrowd = new wxImage(myImageWidth, myImageHeight);
                myProgress->Destroy();
                return;
            }
        }
    }
    
    // If a perspective image blur the crowd gradually from front rows to back.
    if (myUsingPer) {
        int divs = 3; // Divide crowd image into this number of horizontal strips.
        int rows = myCrowd->GetHeight()/divs; // rows in each strip.
        for (int r = 0; r < divs-1; r++) {
            // Blur strips of the image increasing blur from front to back.
            // Don't blur bottom strip.
            wxImage aStrip = myCrowd->GetSubImage(wxRect(
                    0, r*rows, myCrowd->GetWidth(), rows));
            aStrip = aStrip.Blur(divs-1-r);
            myCrowd->Paste(aStrip, 0, r*rows);
        }
    }
    
    myProgress->Destroy();
}

/**
 * Add an image to the crowd image at the given row and column. Recognize the
 * transparent parts of the new image.
 * @param aPerson The new person image to be added to the crowd image.
 * @param mRow The row in the crowd image where aPerson starts.
 * @param mCol The column in the crowd image where aPerson starts.
 */
void CrowdMaker::crowdMerge(wxImage *aPerson, wxInt32 mRow, wxInt32 mCol) {
    // cy, cx == myCrowd row, column. py, px == aPerson row, column.
    // Negative mRow and mCol are tolerated.
    for (wxInt32 cx = mCol, px = 0;
                cx < min(myCrowd->GetWidth(), mCol + aPerson->GetWidth());
                cx++, px++) {
        for (wxInt32 cy = mRow, py = 0;
                    cy < min(myCrowd->GetHeight(), mRow + aPerson->GetHeight());
                    cy++, py++) {
            
            // Move visible pixels from aPerson to myCrowd.
            if ( ( ! aPerson->IsTransparent(px, py)) && cx >= 0 && cy >=0) {
                // Copy aPerson pixel at px,py to myCrowd at cx,cy.
                myCrowd->SetRGB(cx, cy, 
                        aPerson->GetRed(px, py),
                        aPerson->GetGreen(px, py),
                        aPerson->GetBlue(px, py));
            }
        }
    }
}

/**
 * Return the assembled crowd image.
 * @return The crowd image.
 */
wxImage CrowdMaker::getCrowdImage() {
    return *myCrowd;
}