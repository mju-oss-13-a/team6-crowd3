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

#include "Icon.h"
#include "icons/cancel.png.h"
#include "icons/crowd3.png.h"
#include "icons/exit.png.h"
#include "icons/find.png.h"
#include "icons/help.png.h"
#include "icons/make.png.h"
#include "icons/make2.png.h"
#include "icons/save.png.h"
#include "icons/shuffle.png.h"

Icon::Icon() {}
Icon::Icon(const Icon& orig) {}
Icon::~Icon() {}

/**
 * Return an icon bitmap given an icon ID.
 * @param iconID An Icon identifier.
 * @return An Icon bitmap.
 */
wxBitmap Icon::getBitmap(wxInt32 iconID) {
    switch (iconID) {
        case iCancel:
            return getIconBitmap(cancel_img, sizeof(cancel_img));
        case iCrowd3:
            return getIconBitmap(crowd3_img, sizeof(crowd3_img));
        case iExit:
            return getIconBitmap(exit_img, sizeof(exit_img));
        case iFind:
            return getIconBitmap(find_img, sizeof(find_img));
        case iHelp:
            return getIconBitmap(help_img, sizeof(help_img));
        case iMake:
            return getIconBitmap(make_img, sizeof(make_img));
        case iMake2:
            return getIconBitmap(make2_img, sizeof(make2_img));
        case iSave:
            return getIconBitmap(save_img, sizeof(save_img));
        case iShuffle:
            return getIconBitmap(shuffle_img, sizeof(shuffle_img));
    }
}

/**
 * Return an icon given an icon ID.
 * @param iconID An Icon identifier.
 * @return An Icon.
 */
wxIcon Icon::getIcon(wxInt32 iconID) {
    wxIcon anIcon;
    anIcon.CopyFromBitmap(Icon::getBitmap(iconID));
    return anIcon;
}

/**
 * Return an Icon bitmap given an Icon character image array.
 * @param data The icon image array.
 * @param dataLength The length of the icon image array.
 * @return An Icon bitmap.
 */
wxBitmap Icon::getIconBitmap(const unsigned char *data, wxInt32 dataLength) {
    wxMemoryInputStream is(data, dataLength);
    return wxBitmap(wxImage(is, wxBITMAP_TYPE_PNG, -1), -1);
}