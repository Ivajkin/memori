/*
 * This file is part of the KDE project.
 *
 * Copyright (C) 2008 Laurent Montel <montel@kde.org>
 * Copyright 2008 Benjamin C. Meyer <ben@meyerhome.net>
 * Copyright (C) 2008 Urs Wolfer <uwolfer @ kde.org>
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

#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>

#include "ui_searchbar.h"

class QTimeLine;
class QWebView;

class SearchBar : public QWidget
{
    Q_OBJECT

public:
    SearchBar(QWidget *parent = 0);

    QString searchText() const;
    bool caseSensitive() const;
    void setFoundMatch(bool match);

public slots:
    void animateHide();
    void clear();
    void showFind();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void frameChanged(int frame);
    void notifySearchChanged();

signals:
    void searchChanged(const QString& text);
    void closeClicked();
    void findNextClicked();
    void findPreviousClicked();

private:
    void initializeSearchWidget();

    Ui::SearchBar ui;
    QWidget *m_widget;

    QWebView *m_webView;
    QTimeLine *m_timeLine;
};

#endif // SEARCHBAR_H
