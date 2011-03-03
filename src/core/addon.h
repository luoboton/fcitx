/***************************************************************************
 *   Copyright (C) 2010~2010 by CSSlayer                                   *
 *   wengxt@gmail.com                                                      *
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

#ifndef ADDON_H
#define ADDON_H

#include "core/fcitx.h"
#include "X11/Xmd.h"
#include "fcitx-config/fcitx-config.h"
#include "utils/utarray.h"
#include "backend.h"
#include "module.h"

typedef enum AddonCategory
{
    AC_INPUTMETHOD = 0,
    AC_BACKEND,
    AC_MODULE
} AddonCategory;

typedef enum AddonType
{
    AT_SHAREDLIBRARY = 0
} AddonType;

typedef struct FcitxAddon
{
    GenericConfig config;
    char *name;
    Bool bEnabled;
    AddonCategory category;
    AddonType type;
    char *library;
    char *depend;
    union {
        FcitxBackend *backend;
        FcitxModule *module;
        FcitxIM* im;
    };
} FcitxAddon;

void FreeAddon(void *v);
void LoadAddonInfo(void);
void AddonResolveDependency();
boolean AddonIsAvailable(const char* name);
FcitxAddon* GetAddonByName(const char* name);

extern UT_array *addons;

#endif
