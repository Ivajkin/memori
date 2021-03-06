// vim: set ts=4 sts=4 sw=4 et:
/* This file is part of the KDE project
   Copyright (C) 2000 David Faure <faure@kde.org>
   Copyright (C) 2002-2003 Alexander Kellett <lypanov@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) version 3.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef __importers_h
#define __importers_h

#include "kbookmarkmodel/commands.h"
#include <klocalizedstring.h>

#include <QtCore/QObject>

class KBookmark;

// TODO: remove QObject base class? doesn't seem to be used.
class ImportCommand : public QObject, public QUndoCommand, public IKEBCommand
{
   Q_OBJECT
public:
   ImportCommand(KBookmarkModel* model);

   virtual void import(const QString &fileName, bool folder) = 0;

   void setVisibleName(const QString& visibleName);
   QString visibleName() const { return m_visibleName; }
   virtual QString requestFilename() const = 0;

   static ImportCommand* performImport(KBookmarkModel* model, const QString &, QWidget *);
   static ImportCommand* importerFactory(KBookmarkModel* model, const QString &);

   virtual ~ImportCommand()
   {}

   void redo() Q_DECL_OVERRIDE;
   void undo() Q_DECL_OVERRIDE;
   QString affectedBookmarks() const Q_DECL_OVERRIDE;

   QString groupAddress() const { return m_group; }
   QString folder() const;

protected:
   /**
    * @param fileName HTML file to import
    * @param folder name of the folder to create. Empty for no creation (root()).
    * @param icon icon for the new folder, if @p folder isn't empty
    * @param utf8 true if the HTML is in utf-8 encoding
    */
   void init(const QString &fileName, bool folder, const QString &icon, bool utf8)
   {
      m_fileName = fileName;
      m_folder = folder;
      m_icon = icon;
      m_utf8 = utf8;
   }

   virtual void doCreateHoldingFolder(KBookmarkGroup &bkGroup);
   virtual void doExecute(const KBookmarkGroup &) = 0;

protected:
   KBookmarkModel* m_model;
   QString m_visibleName;
   QString m_fileName;
   QString m_icon;
   QString m_group;
   bool m_utf8;

private:
   bool m_folder;
   QUndoCommand *m_cleanUpCmd;
};

// part pure
class XBELImportCommand : public ImportCommand
{
public:
   XBELImportCommand(KBookmarkModel* model) : ImportCommand(model) {}
   void import(const QString &fileName, bool folder) Q_DECL_OVERRIDE = 0;
   QString requestFilename() const Q_DECL_OVERRIDE = 0;
private:
   void doCreateHoldingFolder(KBookmarkGroup &bkGroup) Q_DECL_OVERRIDE;
   void doExecute(const KBookmarkGroup &) Q_DECL_OVERRIDE;
};

class GaleonImportCommand : public XBELImportCommand
{
public:
   GaleonImportCommand(KBookmarkModel* model) : XBELImportCommand(model) { setVisibleName(i18n("Galeon")); }
   void import(const QString &fileName, bool folder) Q_DECL_OVERRIDE {
      init(fileName, folder, "", false);
   }
   QString requestFilename() const Q_DECL_OVERRIDE;
};

class KDE2ImportCommand : public XBELImportCommand
{
public:
   KDE2ImportCommand(KBookmarkModel* model) : XBELImportCommand(model) { setVisibleName(i18n("KDE")); }
   void import(const QString &fileName, bool folder) Q_DECL_OVERRIDE {
      init(fileName, folder, "", false);
   }
   QString requestFilename() const Q_DECL_OVERRIDE;
};

// part pure
class HTMLImportCommand : public ImportCommand
{
public:
   HTMLImportCommand(KBookmarkModel* model) : ImportCommand(model) {}
   void import(const QString &fileName, bool folder) Q_DECL_OVERRIDE = 0;
   QString requestFilename() const Q_DECL_OVERRIDE = 0;
private:
   void doExecute(const KBookmarkGroup &) Q_DECL_OVERRIDE;
};

class NSImportCommand : public HTMLImportCommand
{
public:
   NSImportCommand(KBookmarkModel* model) : HTMLImportCommand(model) { setVisibleName(i18n("Netscape")); }
   void import(const QString &fileName, bool folder) Q_DECL_OVERRIDE {
      init(fileName, folder, "netscape", false);
   }
   QString requestFilename() const Q_DECL_OVERRIDE;
};

class MozImportCommand : public HTMLImportCommand
{
public:
   MozImportCommand(KBookmarkModel* model) : HTMLImportCommand(model) { setVisibleName(i18n("Mozilla")); }
   void import(const QString &fileName, bool folder) Q_DECL_OVERRIDE {
      init(fileName, folder, "mozilla", true);
   }
   QString requestFilename() const Q_DECL_OVERRIDE;
};

class IEImportCommand : public ImportCommand
{
public:
   IEImportCommand(KBookmarkModel* model) : ImportCommand(model) { setVisibleName(i18n("IE")); }
   void import(const QString &fileName, bool folder) Q_DECL_OVERRIDE {
      init(fileName, folder, "", false);
   }
   QString requestFilename() const Q_DECL_OVERRIDE;
private:
   void doExecute(const KBookmarkGroup &) Q_DECL_OVERRIDE;
};

class OperaImportCommand : public ImportCommand
{
public:
   OperaImportCommand(KBookmarkModel* model) : ImportCommand(model) { setVisibleName(i18n("Opera")); }
   void import(const QString &fileName, bool folder) Q_DECL_OVERRIDE {
      init(fileName, folder, "opera", false);
   }
   QString requestFilename() const Q_DECL_OVERRIDE;
private:
   void doExecute(const KBookmarkGroup &) Q_DECL_OVERRIDE;
};

#endif
