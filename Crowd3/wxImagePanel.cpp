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

/* This is a modified version of http://wiki.wxwidgets.org/An_image_panel */

#include "wxImagePanel.h"
#include "Tools.h"

BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
// some useful events
/*
 EVT_MOTION(wxImagePanel::mouseMoved)
 EVT_LEFT_DOWN(wxImagePanel::mouseDown)
 EVT_LEFT_UP(wxImagePanel::mouseReleased)
 EVT_RIGHT_DOWN(wxImagePanel::rightClick)
 EVT_LEAVE_WINDOW(wxImagePanel::mouseLeftWindow)
 EVT_KEY_DOWN(wxImagePanel::keyPressed)
 EVT_KEY_UP(wxImagePanel::keyReleased)
 EVT_MOUSEWHEEL(wxImagePanel::mouseWheelMoved)
 */
 
// catch paint events
EVT_PAINT(wxImagePanel::paintEvent)
//Size event
EVT_SIZE(wxImagePanel::OnSize)
END_EVENT_TABLE()
 
 
// some useful events
/*
 void wxImagePanel::mouseMoved(wxMouseEvent& event) {}
 void wxImagePanel::mouseDown(wxMouseEvent& event) {}
 void wxImagePanel::mouseWheelMoved(wxMouseEvent& event) {}
 void wxImagePanel::mouseReleased(wxMouseEvent& event) {}
 void wxImagePanel::rightClick(wxMouseEvent& event) {}
 void wxImagePanel::mouseLeftWindow(wxMouseEvent& event) {}
 void wxImagePanel::keyPressed(wxKeyEvent& event) {}
 void wxImagePadataSizeronel::keyReleased(wxKeyEvent& event) {}
 */

/**
 * Create a panel for image display.
 * @param parent The frame owner of the panel.
 * @param img The image to be displayed.
 */
wxImagePanel::wxImagePanel(wxWindow* parent, wxImage img) : wxPanel(parent) {
    setImage(img);
}

/**
 * Change the image displayed in the panel.
 * @param newImage the new image to be displayed.
 */
void wxImagePanel::setImage(wxImage newImage) {
    myImage = newImage;
    if (myImage.IsOk()) {
        initializeImage();
        Refresh();
    }
    else {
        Tools::log(_T("Internal error: wxImagePanel::setImage()"));
    }
}

/**
 * Initialize the new image by remembering its width, height, and aspect ratio.
 */
void wxImagePanel::initializeImage() {
    // Remember width and height to detect resizing.
    myPreviousWidth = -1;
    myPreviousHeight = -1;
    
    // Remember aspect ratio of myImage to enable resizing.
    if (myImage.GetHeight() != 0) {
        myAspectRatio = myImage.GetWidth() / (double) myImage.GetHeight();
    }
    else {
        myAspectRatio = 1.0;
    }
}

/*
 * Called by the system or by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 * @param evt The paint event.
 */
void wxImagePanel::paintEvent(wxPaintEvent & evt) {
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}
 
/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void wxImagePanel::paintNow() {
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}
 
/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void wxImagePanel::render(wxDC& dc) {   
    wxInt32 newWidth, newHeight;
    dc.GetSize(&newWidth, &newHeight);
    
    if(newWidth != myPreviousWidth || newHeight != myPreviousHeight ) {
        // Size change. Scale and draw the image to the new size, preserving 
        // aspect ratio.
        wxInt32 w, h;
        if (myAspectRatio < 1.0) {
            w = newHeight * myAspectRatio;
            h = newHeight;
        }
        else {
            w = newWidth;
            h = newWidth / myAspectRatio;
        }
        myImageBits = wxBitmap(myImage.Scale(w, h));
        
        // Remember new width and height.
        myPreviousWidth = newWidth;
        myPreviousHeight = newHeight;
        
        // Draw.
        dc.DrawBitmap(myImageBits, 0, 0, false);
    }
    else {
        // No size change.  Refresh.
        dc.DrawBitmap(myImageBits, 0, 0, false);
    }
}
 
/*
 * Here we call refresh to tell the panel to draw itself again.
 * So when the user resizes the image panel the image should be resized too.
 */
void wxImagePanel::OnSize(wxSizeEvent& event) {
    Refresh();
    event.Skip();
}
 