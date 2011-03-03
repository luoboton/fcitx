/***************************************************************************
 *   Copyright (C) 2002~2005 by Yuking                                     *
 *   yuking_net@sohu.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "X11/Xlib.h"
#include "fcitx-config/xdg.h"

static XErrorHandler   oldXErrorHandler;
static XIOErrorHandler oldXIOErrorHandler;

static int FcitxXErrorHandler (Display * dpy, XErrorEvent * event);
static int FcitxXIOErrorHandler (Display *d);

void Init (void)
{
    oldXErrorHandler = XSetErrorHandler (FcitxXErrorHandler);
    oldXIOErrorHandler = XSetIOErrorHandler (FcitxXIOErrorHandler);
}

int FcitxXIOErrorHandler (Display *d)
{
    FILE *fp;
    fp = GetXDGFileUser("crash.log","wt" , NULL);
    if ( fp ) {
        fprintf(fp, "%s: X IO error.\n", DisplayString(d));
        fclose(fp);
    }
    
    if (oldXIOErrorHandler)
        oldXIOErrorHandler(d);
    return 0;

}

int FcitxXErrorHandler (Display * dpy, XErrorEvent * event)
{
    char    str[256];
    FILE* fp = NULL;

    fp = GetXDGFileUser("crash.log","wt" , NULL);
    if ( fp ) {
        XGetErrorText (dpy, event->error_code, str, 255);
        fprintf (fp, "fcitx: %s\n", str);
    }

    SaveIM();
    
    if ( fp )
        fclose(fp);
    if (event->error_code != 3 && event->error_code != BadMatch) {  // xterm will generate 3
        if (oldXErrorHandler)
            oldXErrorHandler (dpy, event);
    }

    return 0;
}
