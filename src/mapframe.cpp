/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel and Joerg Thalheim  	   *
 *   oh.devs@googlemail.com  						   *
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

 #include "mapframe.h"
 #include <QtCore/QList>
 #include <QtCore/QFile>
 #include <QtCore/QTextStream>
 #include <QtGui/QComboBox>
 #include <QtGui/QLabel>
 #include <QtGui/QGridLayout>
 #include <QtGui/QPushButton>
 #include <QtGui/QLineEdit>

 #include <QtGui/QPixmap>

 void MapFrame::newMap()
 {
 szene = new QGraphicsScene();
 setScene(szene);
 }

 void MapFrame::saveMap(QString filename)
 {
  QList<QGraphicsItem*> savelist = szene->items();
  QFile savefile(filename);
  savefile.open(QIODevice::WriteOnly);
  QTextStream savestream(&savefile);
  savestream << "<map>";
  savestream << "<maphintergrund>";
  savestream << "</maphintergrund>";
  savestream << "<mapbreite>";
  savestream << mapgr.width();
  savestream << "</mapbreite>";
  savestream << "<maphoehe>";
  savestream << mapgr.height();
  savestream << "</maphoehe>";
  //hier die Mapprops reinschreiben (Groesse, File, etc.)
  QGraphicsItem *saveitem;
  foreach(saveitem, savelist)
 {
	 savestream << "<objekt>";
	 savestream << "<objektfunktion>";
	 savestream << saveitem->data(0).toString();
	 savestream << "</objektfunktion>";
	 if(!saveitem->data(1).toString().isEmpty())
	 {
		  savestream << "<objekttooltip>";
		 savestream << saveitem->data(1).toString();
		 savestream << "</objekttooltip>";
 	 }
 	savestream << "<objektdatei>";
 	savestream << saveitem->data(2).toString();
 	savestream << "</objektdatei>";

  	savestream << "<objektpositionx>";
 	savestream << saveitem->data(3).toString();
 	savestream << "</objektpositionx>";

 	savestream << "<objektpositiony>";
 	savestream << saveitem->data(4).toString();
 	savestream << "</objektpositiony>";
 	savestream << "</objekt>";
 }
 savestream << "</map>";
 }
 
 void MapFrame::loadMap(QString filename)
 {
 }
 
 void MapFrame::mousePressEvent(QMouseEvent *event)
 {
	QList<QGraphicsItem *> QGIlistAtClick = items(event->pos());
	if(QGIlistAtClick.isEmpty())
	{
		QDialog *createObjectDialog = new QDialog();
		createObjectDialog->setModal(true);

		QGridLayout *cODlayout = new QGridLayout(createObjectDialog);
		
		QLabel *file = new QLabel(createObjectDialog);
		cODlayout->addWidget(file, 0, 0, 1, 1);

		QPushButton *selectFileButton = new QPushButton ("...",createObjectDialog);
		cODlayout->addWidget(selectFileButton, 0, 1, 1, 1);

		QComboBox *fkt = new QComboBox(createObjectDialog);
		fkt->addItem("land");
		fkt->addItem("markt");
		cODlayout->addWidget(fkt, 1, 0,1,2);

		QLineEdit *objToolTip = new QLineEdit(createObjectDialog);
		objToolTip->setMaxLength(25);
		cODlayout->addWidget(objToolTip, 2, 0, 1, 2);

		QPushButton *ok = new QPushButton(tr("Ok"),createObjectDialog);
		cODlayout->addWidget(ok, 3, 0, 1, 1);

		QPushButton *abort = new QPushButton(tr("Abort"),createObjectDialog);
		cODlayout->addWidget(abort, 3, 1, 1, 1);

		createObjectDialog->setLayout(cODlayout);
		createObjectDialog->show();

		connect(ok, SIGNAL(clicked()), this, SLOT(newObject()));
		connect(abort, SIGNAL(clicked()), createObjectDialog, SLOT(close()));
		connect(abort, SIGNAL(clicked()), createObjectDialog, SLOT(deleteLater()));
		connect(selectFileButton, SIGNAL(clicked()), this, SLOT(fileDialog()));
		connect(objToolTip, SIGNAL(textEdited(QString()), this, SLOT(setToolTipString(QString)));
	}
	else
	{
		if(QGIlistAtClick.size() == 1)
		{
			
		}


		if(QGIlistAtClick.size() >1)
		{
			
		}

	}
}

void MapFrame::setToolTipString(QString ttstring)
{
tooltip = ttstring;
}



void MapFrame::newObject()
{
newObject(typ,filename,tooltip);
}


 void MapFrame::newObject(QString otyp, QString ofilename, QString otooltip)
 {
  QGraphicsPixmapItem *itemtoAdd = szene->addPixmap(QPixmap(filename));
  itemtoAdd->setData(0, QVariant(otyp));
  itemtoAdd->setData(1, QVariant(otooltip));
  itemtoAdd->setData(2, QVariant());
 }



 QString MapFrame::fileDialog()
 {
  QString fd_filename = QString();

  //So, das habe ich fein vorbereitet, den Rest ueberlass ich dir mal ;-)
  filename = fd_filename;
  return fd_filename;
 }