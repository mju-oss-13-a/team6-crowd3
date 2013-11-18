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

#ifndef IMAGETREE_H
#define	IMAGETREE_H

#include <wx/treectrl.h>

/**
 * Maintain an on-screen tree of source image folders. Maintain a database that
 * mirrors the values in the tree.<br>
 * Usage:<br><code>
 * ImageTree* i = ImageTree::create(<wxTreeCtrl options>);<br>
 * ImageTree::buildImageTree();<br>
 * ImageTree::write(...);<br>
 * ImageTree::sortImageTree();<br>
 * wxArrayString list = ImageTree::getSelectedImageFiles();<br>
 * ImageTree::OtherFunction();
 * ImageTree::t()->wxTreeCtrlFunction();</code><br>
 */
class ImageTree : public wxTreeCtrl {
public:
    static ImageTree* create(
                wxWindow* parent, 
                wxWindowID id, 
                const wxPoint& pos, 
                const wxSize& size, 
                long style);
    static ImageTree* t();
    static void buildImageTree();
    static bool read(wxString path, wxString& date, wxInt32& firstID, wxInt32& lastID);
    static void write(wxString path, wxString date, wxInt32 firstID, wxInt32 lastID);
    static void remove(wxString aPath);
    static void sortImageTree();
    static wxArrayString* getSelectedPeopleFiles();
    void selectionMonitor(wxCommandEvent &event);

private:    
    static wxTreeItemId getTreeID(wxTreeItemId aNode, wxString aString);
    static void write(wxString aPath, wxInt32 first, wxInt32 last);
    static wxInt32 receiveRecord(void *a_param, int argc, char **argv, char **column);
    static void sortImageNode(wxTreeItemId aNode);
    static void addPeopleFiles(wxTreeItemId aSelection, wxArrayString* aList);
};

/** Data stored with ImageTree items that are source image filenames. */
class ImageData : public wxTreeItemData {
public:
    ImageData(wxInt32 firstID, wxInt32 lastID);
    /** Get the first image ID for a source image file. */
    inline wxInt32 getFirst() {return first;}
    /** Get the last image ID for a source image file. */
    inline wxInt32 getLast() {return last;}
private:
    /** First image ID. */
    wxInt32 first;
    /** Last image ID. */
    wxInt32 last;
};

#endif	/* IMAGETREE_H */

