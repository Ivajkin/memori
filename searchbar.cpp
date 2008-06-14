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
 */

#include "searchbar.h"

#include <QResizeEvent>
#include <QShortcut>
#include <QTimeLine>

#include <KColorScheme>

SearchBar::SearchBar(QWidget *parent)
    : QWidget(parent)
    , m_widget(0)
    , m_webView(0)
    , m_timeLine(new QTimeLine(150, this))
{
    initializeSearchWidget();

    // we start off hidden
    setMaximumHeight(0);
    m_widget->setGeometry(0, -1 * m_widget->height(),
                          m_widget->width(), m_widget->height());
    hide();

    connect(m_timeLine, SIGNAL(frameChanged(int)),
            this, SLOT(frameChanged(int)));

    new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(animateHide()));
}

void SearchBar::initializeSearchWidget()
{
    m_widget = new QWidget(this);
    m_widget->setContentsMargins(0, 0, 0, 0);
    ui.setupUi(m_widget);
    ui.closeButton->setIcon(KIcon("dialog-close"));
    ui.previousButton->setIcon(KIcon("go-up-search"));
    ui.previousButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui.nextButton->setIcon(KIcon("go-down-search"));
    ui.nextButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui.searchInfo->setText(QString());
    connect(ui.nextButton, SIGNAL(clicked()),
            this, SIGNAL(findNextClicked()));
    connect(ui.previousButton, SIGNAL(clicked()),
            this, SIGNAL(findPreviousClicked()));
    connect(ui.searchLineEdit, SIGNAL(returnPressed()),
            this, SIGNAL(findNextClicked()));
    connect(ui.closeButton, SIGNAL(clicked()),
            this, SLOT(animateHide()));

    setMinimumWidth(m_widget->minimumWidth());
    setMaximumWidth(m_widget->maximumWidth());
    setMinimumHeight(m_widget->minimumHeight());
}

void SearchBar::clear()
{
    ui.searchLineEdit->setText(QString());
}

void SearchBar::showFind()
{
    if (isVisible())
        return;

    show();
    ui.searchLineEdit->setFocus();
    ui.searchLineEdit->selectAll();

    m_timeLine->setFrameRange(-1 * m_widget->height(), 0);
    m_timeLine->setDirection(QTimeLine::Forward);
    disconnect(m_timeLine, SIGNAL(finished()),
               this, SLOT(hide()));
    m_timeLine->start();
}

void SearchBar::resizeEvent(QResizeEvent *event)
{
    if (event->size().width() != m_widget->width())
        m_widget->resize(event->size().width(), m_widget->height());
    QWidget::resizeEvent(event);
}

void SearchBar::animateHide()
{
    m_timeLine->setDirection(QTimeLine::Backward);
    m_timeLine->start();
    connect(m_timeLine, SIGNAL(finished()), this, SLOT(hide()));
}

void SearchBar::frameChanged(int frame)
{
    if (!m_widget)
        return;
    m_widget->move(0, frame);
    int height = qMax(0, m_widget->y() + m_widget->height());
    setMinimumHeight(height);
    setMaximumHeight(height);
}

void SearchBar::notifySearchChanged()
{
    emit searchChanged(searchText());
}

QString SearchBar::searchText() const
{
    return ui.searchLineEdit->text();
}

bool SearchBar::caseSensitive() const
{
    return ui.matchCaseCheckBox->isChecked();
}

void SearchBar::setFoundMatch(bool match)
{
    if (!match && !searchText().isEmpty()) {
        KStatefulBrush backgroundBrush(KColorScheme::View, KColorScheme::NegativeBackground);

        QString styleSheet = QString("QLineEdit{ background-color:%1 }")
                             .arg(backgroundBrush.brush(ui.searchLineEdit).color().name());

        ui.searchLineEdit->setStyleSheet(styleSheet);
    } else {
        ui.searchLineEdit->setStyleSheet(QString());
    }
}

#include "searchbar.moc"
