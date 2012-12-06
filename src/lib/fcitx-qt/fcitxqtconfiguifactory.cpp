/***************************************************************************
 *   Copyright (C) 2012~2012 by CSSlayer                                   *
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
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#include "fcitxqtconfiguifactory.h"
#include "fcitxqtconfiguiplugin.h"
#include "fcitx-config/xdg.h"
#include "fcitx-utils/utils.h"
#include <QDir>
#include <QDebug>
#include <QLibrary>
#include <QPluginLoader>
#include <libintl.h>

FcitxQtConfigUIFactory::FcitxQtConfigUIFactory(QObject* parent): QObject(parent)
{
    scan();
}

FcitxQtConfigUIFactory::~FcitxQtConfigUIFactory()
{

}

FcitxQtConfigUIWidget* FcitxQtConfigUIFactory::create(const QString& file)
{
    char* localepath = fcitx_utils_get_fcitx_path("localedir");
    bindtextdomain(plugins[file]->domain().toUtf8().data(), localepath);
    free(localepath);
    return plugins[file]->create(file);
}

void FcitxQtConfigUIFactory::scan()
{
    QStringList dirs;
    // check plugin files
    size_t len;
    char** path = FcitxXDGGetLibPath(&len);
    for (int i = 0; i < len; i ++) {
        dirs << path[i];
    }

    if (dirs.isEmpty())
        return;
    for (QStringList::ConstIterator it = dirs.begin(); it != dirs.end(); ++it) {
        // qDebug() << QString ("Checking Qt Library Path: %1").arg (*it);
        QDir libpath (*it);
        QDir dir (libpath.filePath (QString ("qt")));
        if (!dir.exists()) {
            continue;
        }

        QStringList entryList = dir.entryList();
        // filter out "." and ".." to keep debug output cleaner
        entryList.removeAll (".");
        entryList.removeAll ("..");
        if (entryList.isEmpty()) {
            continue;
        }

        foreach (const QString & maybeFile, entryList) {
            QFileInfo fi (dir.filePath (maybeFile));

            QString filePath = fi.filePath(); // file name with path
            QString fileName = fi.fileName(); // just file name

            qDebug() << filePath;

            if (!QLibrary::isLibrary (filePath)) {
                continue;
            }

            QPluginLoader* loader = new QPluginLoader (filePath, this);
            qDebug() << loader->load();
            qDebug() << loader->errorString();
            FcitxQtConfigUIFactoryInterface* plugin = qobject_cast< FcitxQtConfigUIFactoryInterface* > (loader->instance());
            if (plugin) {
                QStringList list = plugin->files();
                foreach(const QString& s, list) {
                    plugins[s] = plugin;
                }
            }
        }
    }
}
