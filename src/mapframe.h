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
 QString bgi_filename; // Backgroundimage-FN
 QString cityname;
 QString mapnorth, mapwest, mapsouth, mapeast;
 QList <QGraphicsItem *> itemList;
 QSize mapSize;
 
 void initMap();
 QGraphicsScene *szene;

 QGraphicsItem *activeItem;
 bool itemSelected;
 QString object_typ;
 QString object_filename;
 QString object_tooltip;
 int x, y;
 QPoint ziel;

 QFileDialog *fd;
 QDialog *createObjectDialog;
 QString fd_filename;


 public slots:
 void newMap();
 void saveMap(QString);
 void loadMap(QString);

 void newObject();
 void newObject(QString, QString, QString);
 void fileDialog(int);
// void fileDialog(NameFilters::NFs);

 void setTyp(QString);
 void setToolTipString(QString);
 void setFileString(QString);
 
 void setXPos(int);
 void setYPos(int);
 
 protected:
 void mousePressEvent(QMouseEvent*);
 void mouseMoveEvent(QMouseEvent*);
 
 signals:
 void newObjectCreated();
 void fileStringChanged(QString);
 };
 #endif
