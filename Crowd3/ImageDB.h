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

#ifndef IMAGEDB_H
#define	IMAGEDB_H

#include <wx/wx.h>
#include <sqlite3.h>
#include "const.h"
#include "Tools.h"
#include <wx/progdlg.h>
using namespace std;

/**
 * Provide access to a SQL database for managing image files and the person
 * images found within them.<p>
 * The database contains records with these fields:<p>
 * - Path:    TEXT (PRIMARY KEY) - The full path to an image file.<p>
 * - Date:    TEXT - The modification date/time of Path in text format.<p>
 * - FirstID: INTEGER - The first unique id associated with Path or -1.<p>
 * - LastID:  INTEGER - the last unique id associated with Path.<p>
 * The database file is stored in the Crowd3 folder.<p>
 * Usage: (all calls are static)<p><code>
 * bool s = open();<p>
 * write(path, moddate, first, last);<p>
 * bool s = read(path, moddate, first, last);<p>
 * remove(path);<p>
 * readAllRecords(callback);<p>
 * close()<p></code>
 * 
 */
class ImageDB {
public:
    ImageDB();
    ImageDB(const ImageDB& orig);
    virtual ~ImageDB();
    static bool open();
    static void close();
    static bool read(wxString path, wxString& date, wxInt32& firstD, wxInt32& lastID);
    static void write(wxString path, wxString date, wxInt32 firstID, wxInt32 lastID);
    static void remove(wxString path);
    static void readAllRecords(int callback(void*, int, char**, char**));
private:
    static wxString filter(wxString in);
    static void fix1();
    static wxInt32 fix1ReceiveRecord(void *a_param, int argc, char **argv, char **column);
    static void update(wxString path, wxString date, wxInt32 firstID, wxInt32 lastID);
};

#endif	/* IMAGEDB_H */

