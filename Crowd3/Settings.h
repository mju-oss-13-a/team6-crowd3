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

#ifndef SETTINGS_H
#define	SETTINGS_H

#include <wx/config.h>
#include <wx/display.h>
#include "const.h"

/** Save user preferences between executions of Crowd3.<p>
 * Usage: (all functions are static):<p><code>
 * s = new Settings();<p>
 * Settings::setOption(o);<p>
 * v = Settings::getOption();<p></code>
 * etc.
 */
class Settings {
public:
    Settings();
    Settings(const Settings& orig);
    virtual ~Settings();
    
    static void setWindowSize(wxSize s);
    static wxSize getWindowSize();
    static void setWindowLocation(wxPoint p);
    static wxPoint getWindowLocation();
    
    static void setMakerSize(wxSize s);
    static wxSize getMakerSize();
    static void setMakerLocation(wxPoint p);
    static wxPoint getMakerLocation();
    
    static void setPersonPath(wxString path);
    static wxString getPersonPath();
    static void setCrowdPath(wxString path);
    static wxString getCrowdPath();
    
    static void setBackgroundPath(wxString path);
    static wxString getBackgroundPath();
    
    static void setPeopleCount(wxInt32 count);
    static wxInt32 getPeopleCount();
    
    static void setPerspective(bool value);
    static bool getPerspective();
    
    static void setCrowdImageSize(wxSize s);
    static wxSize getCrowdImageSize();
    
    static void setImageID(wxInt32 i);
    static wxInt32 getImageID();
    
private:

};

#endif	/* SETTINGS_H */

