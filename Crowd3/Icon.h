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

#ifndef ICON_H
#define	ICON_H

#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/mstream.h>
#include <wx/image.h>

/**
 * Manage the in-memory icons created from .png files.<p>
 * See icons/runpng2c.sh, icons/png2c.pl.<p>
 * Usage:<p><code>
 * Icon::getBitmap(Icon::iconIdentifier);<p>
 * Icon::getIcon(Icon::iconIdentifier);<p></code>
 */
class Icon {
public:
    Icon();
    Icon(const Icon& orig);
    virtual ~Icon();
    
    static wxBitmap getBitmap(wxInt32 iconID);
    static wxIcon getIcon(wxInt32 iconID);
        
    /** Icon identifiers. Each icon needs an include and a case: in the .cpp. */
    enum {
        iCancel = 0,
        iCrowd3,
        iExit,
        iFind,
        iHelp,
        iMake,
        iMake2,
        iSave,  
        iShuffle
    };
private:
    static wxBitmap getIconBitmap(const unsigned char *data, wxInt32 dataLength);
};

#endif	/* ICON_H */

