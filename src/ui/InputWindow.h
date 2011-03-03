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
/**
 * @file   InputWindow.h
 * @author Yuking yuking_net@sohu.com
 * @date   2008-1-16
 * 
 * @brief  输入条窗口
 * 
 * 
 */

#ifndef _INPUT_WINDOW_H
#define _INPUT_WINDOW_H

#include <X11/Xlib.h>
#include <stdarg.h>
#include "IMdkit.h"
#include "tools/utf8.h"
#include <cairo.h>

#define INPUTWND_WIDTH	50
#define INPUTWND_HEIGHT	40

typedef struct InputWindow {
    Window window;
    
    uint            iInputWindowHeight;
    uint            iInputWindowWidth;
    Bool            bShowPrev;
    Bool            bShowNext;
    Bool            bShowCursor;
    
    //这两个变量是GTK+ OverTheSpot光标跟随的临时解决方案
    ///* Issue 11: piaoairy: 为适应generic_config_integer(), 改INT8 为int */
    int		iOffsetX;
    int		iOffsetY;
    
    Pixmap pm_input_bar;
    
    cairo_surface_t *cs_input_bar;
    cairo_surface_t *cs_input_back;
    cairo_t *c_back, *c_cursor;
    cairo_t *c_font[8];
} InputWindow;

extern InputWindow inputWindow;
extern int iCursorPos;

Bool            CreateInputWindow (void);
void            DisplayInputWindow (void);
void            DrawInputWindow (void);
void            CalInputWindow (void);
void            CalculateInputWindowHeight (void);
void            DrawCursor (int iPos);
void            DisplayMessageUp (void);
void            DisplayMessageDown (void);
void            ResetInputWindow (void);
void            MoveInputWindow();
void            CloseInputWindow(void);
void DestroyInputWindow();

#endif
