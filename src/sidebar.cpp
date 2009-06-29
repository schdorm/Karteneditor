/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel and Joerg Thalheim  	   *
 *   oh.devs@googlemail.com 						   *
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
 
 #include "sidebar.h"
 
 #include <QtGui/QVBoxLayout>
 #include <QtGui/QHBoxLayout>
 #include <QtGui/QFrame>

 
 SideBarClass::SideBarClass()
 {
 
	QVBoxLayout *SideBarLayout = new QVBoxLayout(this);

	MapEntries << "Stadtname" << "Maphintergrund" << "Nordmap" << "Westmap" << "Suedmap" << "Ostmap";

	itemList = new QListWidget(this);
	itemList->setSelectionMode(QAbstractItemView::SingleSelection);
	itemList->addItems(MapEntries);
	SideBarLayout->addWidget(itemList);
	
	QFrame *line = new QFrame(this);
	line->setFrameShape(QFrame::HLine);
	SideBarLayout->addWidget(line);

	editToolTip = new QLineEdit(this);
	SideBarLayout->addWidget(editToolTip );
QWidget *wid = new QWidget(this);
	SideBarLayout->addWidget(wid);

	QHBoxLayout *objectFileLayout = new QHBoxLayout(wid);

	fileView = new QLabel(this);
	fileView->setFrameShape(QFrame::Box);
	objectFileLayout->addWidget(fileView);

	selectFileButton = new QPushButton ("...", this);
	objectFileLayout->addWidget(selectFileButton);
	objectFileLayout->setStretch(0,5);


//	SideBarLayout->addLayout(objectFileLayout);

 }