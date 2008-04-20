/*
 * This file is part of the KDE project.
 *
 * Copyright (C) 2008 Laurent Montel <montel@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef WEBKITGLOBAL_H
#define WEBKITGLOBAL_H
class WebKitGlobal;
class WebKitPart;
class KAboutData;
#include <KComponentData>

class WebKitGlobal
{
public:
    WebKitGlobal();
    ~WebKitGlobal();

    static void registerPart( WebKitPart *part );
    static void deregisterPart( WebKitPart *part );
    void initGlobalSettings();
    static const KComponentData &componentData();

private:
    static void ref();
    static void deref();
private:
    static unsigned long s_refcnt;
    static WebKitGlobal *s_self;
    static KAboutData *s_about;
    static KComponentData *s_componentData;
};

#endif /* WEBKITGLOBAL_H */

