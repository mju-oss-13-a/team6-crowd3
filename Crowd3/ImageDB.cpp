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

#include "ImageDB.h"

/** The (sqlite) image database. */
sqlite3 *myImageDB;

ImageDB::ImageDB() {}
ImageDB::ImageDB(const ImageDB& orig) {}
ImageDB::~ImageDB() {}

/**
 * Open the image database, create it if it does not exist.  Prepare tables.
 * @return true if database is open and ready, false if error.
 */
bool ImageDB::open() {
    string dbName = Tools::wx2str(Tools::crowd3Folder() + SEPARATOR + DATABASE);
    if (sqlite3_open(dbName.c_str(), &myImageDB) == SQLITE_OK) {
        // Database opened. Create imageDB table.
        string aSQL = "create table if not exists imageDB "
                "(Path   TEXT PRIMARY KEY, "
                "Date    TEXT, "
                "FirstID INTEGER, "
                "LastID  INTEGER);";
        wxInt32 result = sqlite3_exec(myImageDB, aSQL.c_str(), NULL, NULL, NULL);
        if(result != SQLITE_OK) {
            string errMsg = sqlite3_errmsg(myImageDB);
            Tools::log(Tools::str2wx(errMsg) + 
                _T("\nThe Crowd3 database table could not be created."));
            return false;
        }
        fix1(); // Apply bug fix to database.
    }
    else {
        string errMsg = sqlite3_errmsg(myImageDB);
        Tools::log(Tools::str2wx(errMsg) + 
                _T("\nThe Crowd3 database could not be opened."));
        return false;
    }
    return true;
}

/** Close the image database. */
void ImageDB::close() {
    sqlite3_close(myImageDB);  
}

/**
 * Write a record to the image database.
 * @param path A pathname - the record key.
 * @param date A timestamp, written as "04-Mar-2012 09:24:15 AM"
 * @param firstImage The first image ID.
 * @param lastImage The last image ID.
 */
void ImageDB::write(wxString path, wxString date, wxInt32 firstImage, wxInt32 lastImage) {
    string aSQL = "insert into imageDB (Path, Date, FirstID, LastID) values ("
            "'" + Tools::wx2str(filter(path)) + "', "
            "'" + Tools::wx2str(date) + "', " +
            Tools::int2str(firstImage) + ", " +
            Tools::int2str(lastImage) + ");";
    wxInt32 result = sqlite3_exec(myImageDB, aSQL.c_str(), NULL, NULL, NULL);
    if (result != SQLITE_OK) {
        string errMsg = sqlite3_errmsg(myImageDB);
        Tools::log(Tools::str2wx(errMsg) + _T("\n") +
                Tools::str2wx(aSQL) + _T("\nError writing to database"));
    }
}

/**
 * Update a record in the image database.
 * @param path A pathname - the record key.
 * @param date A timestamp, written as "04-Mar-2012 09:24:15 AM"
 * @param firstImage The first image ID.
 * @param lastImage The last image ID.
 */
void ImageDB::update(wxString path, wxString date, wxInt32 firstImage, wxInt32 lastImage) {
    string aSQL = "UPDATE imageDB SET "
            "Date = '" + Tools::wx2str(date) + "', " +
            "FirstID = " + Tools::int2str(firstImage) + ", " +
            "LastID = " + Tools::int2str(lastImage) + 
            " WHERE Path= '" + Tools::wx2str(filter(path)) + "'";
    wxInt32 result = sqlite3_exec(myImageDB, aSQL.c_str(), NULL, NULL, NULL);
    if (result != SQLITE_OK) {
        string errMsg = sqlite3_errmsg(myImageDB);
        Tools::log(Tools::str2wx(errMsg) + _T("\n") +
                Tools::str2wx(aSQL) + _T("\nError writing to database"));
    }
}

/**
 * Delete a record from the image database.
 * @param path A pathname - the record key.
 */
void ImageDB::remove(wxString path) {
    string aSQL = "DELETE from imageDB where Path = '" + 
            Tools::wx2str(filter(path)) + "';";
    wxInt32 result = sqlite3_exec(myImageDB, aSQL.c_str(), NULL, NULL, NULL);
    if(result != SQLITE_OK) {
        string errMsg = sqlite3_errmsg(myImageDB);
        Tools::log(Tools::str2wx(errMsg) + _T("\n") +
                Tools::str2wx(aSQL) + _T("\nError deleting from database"));
    }
}

/**
 * Read a record from the image database.
 * @param path A pathname - the record key.
 * @param date a Date.
 * @param firstImage The first image ID.
 * @param lastImage The last image ID.
 * @return true if a record was found else return false.
 */
bool ImageDB::read(wxString path, wxString &date, wxInt32 &firstImage, wxInt32 &lastImage) {
    // Attempt a read from the database with key=path.
    sqlite3_stmt *statement;
    string aSQL = "SELECT Date, FirstID, LastID from imageDB where Path = '" +
            Tools::wx2str(filter(path)) + "';";
    if(sqlite3_prepare_v2(myImageDB, aSQL.c_str(), -1, &statement, 0) != SQLITE_OK) {
        // Error on prepare.
        string errMsg = sqlite3_errmsg(myImageDB);
        Tools::log(Tools::str2wx(errMsg) + _T("\n") +
                Tools::str2wx(aSQL) + _T("\nError preparing database query"));
        sqlite3_finalize(statement);
        return false;
    }
    else {
        // Get row 1 of the sql result.
        int sqlResult = sqlite3_step(statement);
        
        // Expect DONE code if no record exists with the key.
        if (sqlResult == SQLITE_DONE) {
            sqlite3_finalize(statement);
            return false;
        }
        
        // Check for error.
        if (sqlResult == SQLITE_ERROR) {
            string errMsg = sqlite3_errmsg(myImageDB);
            Tools::log(Tools::str2wx(errMsg) + _T("\n") +
                    Tools::str2wx(aSQL) + _T("\nError reading from database"));
            sqlite3_finalize(statement);
            return false;
        }
        
        // Check for column error.
        if (3 != sqlite3_column_count(statement)) {
            Tools::log(Tools::str2wx(aSQL) + _T("\nColumn error reading from database"));
            sqlite3_finalize(statement);
            return false;
        }

        // Get data fields.
        if(sqlResult == SQLITE_ROW) {
            // Date.
            char* dateChars = (char*)sqlite3_column_text(statement, 0);
            date = Tools::cstar2wx(dateChars);
            
            // Image IDs.
            firstImage = sqlite3_column_int(statement, 1);
            lastImage = sqlite3_column_int(statement, 2);
            
            sqlite3_finalize(statement);
            return true;
        }

        // Unexpected return code.
        string errMsg = sqlite3_errmsg(myImageDB);
        Tools::log(Tools::str2wx(errMsg) + _T("\n") + Tools::int2wx(sqlResult) + 
                _T("\n") + Tools::str2wx(aSQL) + _T("\nUnexpected code from database"));
        sqlite3_finalize(statement);
        return false;
    }
}

/**
 * Read all records from the image database. Send them one at a time to the 
 * callback function.
 * @param callback The callback function.
 */
void ImageDB::readAllRecords(int callback (void*, int, char**, char**) ) {
    string aSQL = "SELECT * from imageDB;";
    wxInt32 result = sqlite3_exec(myImageDB, aSQL.c_str(), callback, NULL, NULL);
    if(result != SQLITE_OK) {
        string errMsg = sqlite3_errmsg(myImageDB);
        Tools::log(Tools::str2wx(errMsg) + _T("\n") +
                Tools::str2wx(aSQL) + _T("\nError reading all records from database"));
    }
}

/**
 * Filter SQL-unfriendly characters from strings before using in a SQL statement.
 * @param in The original string.
 * @return The safe string.
 */
wxString ImageDB::filter(wxString in) {
    in.Replace(_T("'"), _T("''"), true); // Escape ' by ''.
    return in;
}

/** A progress bar for fix functions. */
wxProgressDialog *fixProgress;

/** Fix1: Rewrite all database records using GMT instead of local time. */
void ImageDB::fix1() {
    // Apply this fix if fix1.txt exists.
    wxString fix1Name = Tools::crowd3Folder() + SEPARATOR + _T("fix1.txt");
    wxFileName fileNameObject(fix1Name);
    if ( ! fileNameObject.FileExists()) {
        return;
    }
    
    // Show a progress bar.
    fixProgress = new wxProgressDialog(
            _T("Applying Fix1, please be patient."),
            _T("Applying Fix1, please be patient."),
            100,
            NULL,
            wxPD_APP_MODAL);
    fixProgress->SetSize(fixProgress->GetSize().GetWidth() * 2,
                         fixProgress->GetSize().GetHeight());
    
    // Read all the records from the image database and send them one at a time
    // to the receiveRecord function.
    ImageDB::readAllRecords(fix1ReceiveRecord);
    
    // Delete fix1.txt.
    if ( ! wxRemoveFile(fix1Name)) {
        Tools::log(_T("\nError deleting fix1.txt.  Please delete it."));
    }
    
    // Inform user.
    fixProgress->Destroy();
    wxString doneMsg = _T("Done! If no error messages appeared then Fix1 was successful.\n");
    wxMessageDialog* done = new wxMessageDialog(NULL, doneMsg, _T("Fix1 applied"), 
            wxOK | wxSTAY_ON_TOP, wxDefaultPosition);
    done->ShowModal();
}

/** Fix1: Receive records from the image database, convert times to GMT and rewrite. */
wxInt32 ImageDB::fix1ReceiveRecord(void *a_param, int argc, char **argv, char **column) {
    // The path is the first column in the record.
    char* pathChars = argv[0];
    wxString aPath = Tools::cstar2wx(pathChars);
    
    // The date is the 2nd column.
    char* dateChars = argv[1];
    wxString aDate = Tools::cstar2wx(dateChars);
    
    // The first and last image IDs are 3rd and 4th columns.
    wxInt32 firstID = Tools::cstar2int(argv[2]);
    wxInt32 lastID  = Tools::cstar2int(argv[3]);
    
    // Continue only if the file exists.
    const wxString dbDateFormat = _T("%d-%b-%Y %H:%M:%S");
    wxFileName fileNameObject(aPath);
    if (fileNameObject.FileExists()) {
        // Get the file's modification date from the OS.
        wxDateTime osModDate = fileNameObject.GetModificationTime();
        wxString osModDateGMT = osModDate.Format(dbDateFormat, wxDateTime::UTC);
        wxString osMinSec = osModDateGMT.Right(5);

        // Get database (assumed local) modification time.
        wxDateTime dbDate;
        dbDate.ParseDateTime(aDate);
        wxString dbMinSec = aDate.Right(5);
        
        // Compare OS modification time to the database record time.
        // Skip records where the file seems to have changed since the DB record
        // was written (os mod date very different from db mod date).
        wxDateTime loTime = osModDate;
        loTime.Subtract(wxTimeSpan::Days(1));
        wxDateTime hiTime = osModDate;
        hiTime.Add(wxTimeSpan::Days(1));
        if (dbDate > loTime && dbDate < hiTime && dbMinSec.IsSameAs(osMinSec)) {
            // Match. Rewrite the record with GMT time.
            update(aPath, osModDateGMT, firstID, lastID);
        }
    }
    fixProgress->Pulse(aPath);
    return 0; // OK
}
