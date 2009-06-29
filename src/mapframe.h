/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel and Joerg Thalheim  *
 *   oh.devs@googlemail.com   *
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
 
 #ifndef _MAPFRAME_H
 #define _MAPFRAME_H
 
 #include <QtGui/QGraphicsView>
 #include <QtGui/QGraphicsScene>
 #include <QtGui/QGraphicsItem>
 
 #include <QtGui/QMouseEvent>
 #include <QtGui/QDialog>
 #include <QtGui/QFileDialog>

 namespace NameFilters{ enum NFs{Img, Map, Save};}
 
 class MapFrame : public QGraphicsView
 {
 Q_OBJECT

 public:
 void initMap();
 QGraphicsScene *szene;
 QSize mapgr;
 QGraphicsPixmapItem *activeItem;
 QString typ;
 QString filename;
 QString tooltip;
 int x, y;
 QPoint ziel;
  QFileDialog *fd;



 public slots:
 void newMap();
 void saveMap(QString);
 void loadMap(QString);

 void newObject();
 void newObject(QString, QString, QString);
 void fileDialog(int);
// void fileDialog(NameFilters::NFs);

 void setToolTipString(QString);
 void setFileString(QString);

 protected:
 void mousePressEvent(QMouseEvent*);
 };
 #endif
