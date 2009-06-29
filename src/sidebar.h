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
 
 #ifndef _SIDEBAR_H
 #define _SIDEBAR_H
 
 #include <QtGui/QWidget>
 #include <QtGui/QListWidget>
 #include <QtGui/QLineEdit>
 #include <QtGui/QLabel>
 #include <QtGui/QPushButton>


 
 class SideBarClass : public QWidget
 {
 Q_OBJECT
 public:
 QStringList MapEntries;
 SideBarClass();
 
  QListWidget *itemList;
  QPushButton *selectFileButton;
  QLabel *fileView;
  QLineEdit *editToolTip;
 
 };
 #endif