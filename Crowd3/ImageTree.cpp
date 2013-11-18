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
#include "ImageTree.h"
#include "Tools.h"
#include "ImageDB.h"

/** The on-screen folder tree of source image files. */
static ImageTree* myTree = NULL;

/** Has the tree selection changed since the last call to getSelectedPeopleFiles()? */
static bool myTreeSelectionChanged = true;

/** The list of people image files associated with the image tree selections. */
static wxArrayString* mySelectedFiles = new wxArrayString();

/** Create the sole instance of the image tree control. */
ImageTree* ImageTree::create(wxWindow* parent, 
        wxWindowID id, const wxPoint& pos, const wxSize& size, long style) {
    if (myTree == NULL) {
        myTree = (ImageTree*) new wxTreeCtrl(parent, id, pos, size, style);
        
        // Monitor changes to tree selection.
        myTree->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED,
            wxCommandEventHandler(ImageTree::selectionMonitor), NULL, myTree);
    }
    return myTree;
}

/** Return the sole instance of the image tree control. */
ImageTree* ImageTree::t() {
    return myTree;
}

/** Build the image file tree from scratch using values in the Image database. */
void ImageTree::buildImageTree() {
    // Empty the tree and create a root.
    myTree->DeleteAllItems();
    myTree->AddRoot(_T("People images"));

    // Read all the records from the image database and send them one at a time
    // to the receiveRecord function.
    ImageDB::readAllRecords(receiveRecord);
    
    // Sort the image tree.
    sortImageTree();
    
    // Select the root node.
    myTree->SelectItem(myTree->GetRootItem(), true);
}

/** Receive records from the image database and add paths to the image tree. */
wxInt32 ImageTree::receiveRecord(void *a_param, int argc, char **argv, char **column) {
    // The path is the first column in the record.
    char* pathChars = argv[0];
    wxString aPath = Tools::cstar2wx(pathChars);
    
    // The first and last image IDs are 3rd and 4th columns.
    wxInt32 firstID = Tools::cstar2int(argv[2]);
    wxInt32 lastID  = Tools::cstar2int(argv[3]);
    
    // Add the path and its image IDs to the image tree.
    write(aPath, firstID, lastID);
    return 0; // OK
}

/**
 * Read a record from the image database.
 * @param path A pathname - the record key.
 * @param date a Date/Time.
 * @param firstID The first image ID.
 * @param lastID The last image ID.
 * @return true if a record was found else return false.
 */
bool ImageTree::read(wxString path, wxString& date, wxInt32& firstID, wxInt32& lastID) {
    return ImageDB::read(path, date, firstID, lastID);
}

/**
 * Write a record to the image tree and database.
 * @param path A pathname - the record key.
 * @param date A timestamp.
 * @param firstID The first image ID.
 * @param lastID The last image ID.
 */
void ImageTree::write(wxString path, wxString date, wxInt32 firstID, wxInt32 lastID) {
    ImageDB::write(path, date, firstID, lastID);
    write(path, firstID, lastID);
}

/**
 * Add a list of folders and a filename from a path to the image tree.
 * Create the folder items if they do not already exist.
 * Add the first and last image IDs to the filename's tree data.
 * @param aPath The full path to a file.
 * @param firstID The first file ID.
 * @param lastID The last file ID.
 */
void ImageTree::write(wxString aPath, wxInt32 firstID, wxInt32 lastID) {
    wxFileName fullPath = wxFileName(aPath);
    
    // Find the tree node id of the each folder in the path. Start with root
    // and descend the tree.
    wxTreeItemId aNode = myTree->GetRootItem();
    wxArrayString folderList = fullPath.GetDirs();
    for (wxInt32 i = 0; i < folderList.GetCount(); i++) {
        wxString aFolder = folderList.Item(i);
        aNode = getTreeID(aNode, aFolder);
    }
    // Add the filename under the ultimate folder's node id.
    aNode = getTreeID(aNode, fullPath.GetFullName());
    
    // Add the file IDs to the filename's node.
    ImageData* aNodeData = new ImageData(firstID, lastID);
    myTree->SetItemData(aNode, aNodeData);
}

/**
 * Delete a path from the image tree and database.
 * @param aPath The path to delete.
 */
void ImageTree::remove(wxString aPath) {
    // Remove database record.
    ImageDB::remove(aPath);

    // Find the tree node id of the each folder in the path. Start with root
    // and descend the tree.
    wxFileName fullPath = wxFileName(aPath);
    wxTreeItemId aNode = myTree->GetRootItem();
    wxArrayString folderList = fullPath.GetDirs();
    for (wxInt32 i = 0; i < folderList.GetCount(); i++) {
        wxString aFolder = folderList.Item(i);
        aNode = getTreeID(aNode, aFolder);
    }
    
    // Get and delete the ID of the path to delete.
    wxTreeItemId target = getTreeID(aNode, fullPath.GetFullName());
    myTree->Delete(target);
}

/**
 * Get the tree node id of a string in a particular tree node. Create if it
 * does not exist.
 * @param aNode The id of the particular tree node.
 * @param aString The string to find.
 * @return The id of the tree node containing the string.
 */
wxTreeItemId ImageTree::getTreeID(wxTreeItemId aNode, wxString aString) {
    wxTreeItemId found;
    wxTreeItemIdValue cookie;
    wxInt32 childCount = myTree->GetChildrenCount(aNode, false);
    for (wxInt32 c = 0; c < childCount; c++) {
        // Check each child of aNode for aString
        if (c == 0) {
            found = myTree->GetFirstChild(aNode, cookie);
        }
        else {
            found = myTree->GetNextChild(aNode, cookie);
        }
        if (found.IsOk()) {
            if (myTree->GetItemText(found).IsSameAs(aString)) {
                // Found aString as a child of aNode.
                return found;
            }
        }
    }
    // None of the child nodes match aString.  Create a new child.
    // (Doing this in two steps works around a wxWidgets assert failure:
    // assert "m_heightText != -1" failed in GetTextHeight(): must call CalculateSize() first
    // See http://forums.wxwidgets.org/viewtopic.php?f=1&t=27785)
    wxTreeItemId t = myTree->AppendItem(aNode, _T(""));
    myTree->SetItemText(t, aString);
    return t;
}

/** Sort the image tree recursively. */
void ImageTree::sortImageTree() {
    wxTreeItemId aNode = myTree->GetRootItem();
    sortImageNode(aNode);
}

/** Sort a node of the image tree recursively. */
void ImageTree::sortImageNode(wxTreeItemId aNode) {
    // Sort the children of the given node.
    myTree->SortChildren(aNode);
    
    // Get the children of this node.
    wxTreeItemId aChild;
    wxTreeItemIdValue cookie;
    wxInt32 childCount = myTree->GetChildrenCount(aNode, false);
    for (wxInt32 c = 0; c < childCount; c++) {
        if (c == 0) {
            aChild = myTree->GetFirstChild(aNode, cookie);
        }
        else {
            aChild = myTree->GetNextChild(aNode, cookie);
        }
        if (aChild.IsOk()) {
            // Sort each child.
            sortImageNode(aChild);
        }
    }
}

/**
 * Get the people image files associated with a selected tree item. (If a tree node is
 * selected then recurse on the node's children.) Append the files to the given list.
 * @param aSelection A selected tree item.
 * @param aList A list of people image files.
 */
void ImageTree::addPeopleFiles(wxTreeItemId aSelection, wxArrayString* aList) {
    wxTreeItemId aChild;
    wxTreeItemIdValue cookie;
    wxInt32 childCount = myTree->GetChildrenCount(aSelection, false);
    for (wxInt32 c = 0; c < childCount; c++) {
        if (c == 0) {
            aChild = myTree->GetFirstChild(aSelection, cookie);
        }
        else {
            aChild = myTree->GetNextChild(aSelection, cookie);
        }
        if (aChild.IsOk()) {
            // Recurse for each child of a selected item.
            addPeopleFiles(aChild, aList);
        }
    }
    // If aSelection has no children then it MIGHT be a filename with people files.
    if (childCount == 0) {
        ImageData* iData = (ImageData*) myTree->GetItemData(aSelection);
        if (iData != NULL) {
            wxInt32 firstID = iData->getFirst();
            wxInt32 lastID = iData->getLast();
            // Test each people image file for existence.  If it exists add it to aList.
            // firstID==-1 means there are no associated people image files.
            if (firstID != -1) {
                for (wxInt32 id = firstID; id <= lastID; id++) {
                    wxString aFilename = Tools::int2wx(id) + _T(".png");
                    if (wxFileExists(Tools::crowd3Folder() + SEPARATOR + aFilename)) {
                        aList->Add(aFilename);
                    }
                }
            }
        }
    }
}

/**
 * Get the people image files that are associated with the selected tree items.
 * @return The list of people image files.
 */
wxArrayString* ImageTree::getSelectedPeopleFiles() {
    if (myTreeSelectionChanged) {
        myTreeSelectionChanged = false;
        mySelectedFiles->Empty();
        
        // Get the list of selected tree items.
        wxArrayTreeItemIds* treeSelections = new wxArrayTreeItemIds();
        wxInt32 selectionCount = myTree->GetSelections(*treeSelections);
        
        // If nothing is selected then pretend everything is selected.
        if (selectionCount == 0) {
            wxTreeItemId rNode = myTree->GetRootItem();
            addPeopleFiles(rNode, mySelectedFiles);
        }
        
        // For each selected tree item, add the people image files associated 
        // with the tree item to mySelectedFiles.
        for (wxInt32 i = 0; i < selectionCount; i++) {
            wxTreeItemId aSelection = treeSelections->Item(i);
            addPeopleFiles(aSelection, mySelectedFiles);
        }
    }
    return mySelectedFiles;
}

/** When tree selection changes set myTreeSelectionChanged = true. */
void ImageTree::selectionMonitor(wxCommandEvent &event) {
    myTreeSelectionChanged = true;
}

/**
 * Store first and last image IDs with each source file tree entry.
 * @param firstID First image ID.
 * @param lastID Last image ID.
 */
ImageData::ImageData(wxInt32 firstID, wxInt32 lastID) {
    first = firstID;
    last = lastID;
}