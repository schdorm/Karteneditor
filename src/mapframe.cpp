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
 #include <QtGui/QHBoxLayout>
 #include <QtGui/QPushButton>
 #include <QtGui/QLineEdit>

 #include <QStringList> 
 #include <QtCore/QSignalMapper>


 #include <QtGui/QPixmap>
 
 
 void MapFrame::initMap()
 {
 szene = new QGraphicsScene();
 setScene(szene);
 }

 void MapFrame::newMap()
 {
 szene->clear();
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
 	savestream << saveitem->data(3).toInt();
 	savestream << "</objektpositionx>";

 	savestream << "<objektpositiony>";
 	savestream << saveitem->data(4).toInt();
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
 ziel = event->pos();
	QList<QGraphicsItem *> QGIlistAtClick = items(event->pos());
	if(QGIlistAtClick.isEmpty())
	{
		QDialog *createObjectDialog = new QDialog();
		createObjectDialog->setModal(true);

		QGridLayout *cODlayout = new QGridLayout(createObjectDialog);

		QLabel *labelimg = new QLabel(createObjectDialog);
		labelimg->setText(tr("Image File"));
		cODlayout->addWidget(labelimg, 0, 0, 1, 1);

		QLabel *file = new QLabel(createObjectDialog);
		file->setFrameShape(QFrame::Box);
		cODlayout->addWidget(file, 0, 1, 1, 3);

		QPushButton *selectFileButton = new QPushButton ("...",createObjectDialog);
		cODlayout->addWidget(selectFileButton, 0, 4, 1, 1);


		QLabel *labelfkt = new QLabel(createObjectDialog);
		labelfkt->setText(tr("Object function"));
		cODlayout->addWidget(labelfkt, 1, 0, 1, 1);

		QComboBox *fkt = new QComboBox(createObjectDialog);
		fkt->addItem("land");
		fkt->addItem("markt");
		cODlayout->addWidget(fkt, 1, 1,1,4);


		QLabel *labeltt = new QLabel(createObjectDialog);
		labeltt->setText(tr("Object tooltip (optional)"));
		cODlayout->addWidget(labeltt, 2, 0, 1, 1);

		QLineEdit *objToolTip = new QLineEdit(createObjectDialog);
		objToolTip->setMaxLength(25);
		cODlayout->addWidget(objToolTip, 2, 1, 1, 4);

		QPushButton *ok = new QPushButton(tr("Ok"), createObjectDialog);
		QPushButton *abort = new QPushButton(tr("Abort"), createObjectDialog);
				
		QHBoxLayout buttonlayout;
		buttonlayout.addSpacing(20);
		buttonlayout.addWidget(ok);
		buttonlayout.addSpacing(40);
		buttonlayout.addWidget(abort);
		buttonlayout.addSpacing(20);
		cODlayout->setRowStretch(3,2);
		cODlayout->addLayout(&buttonlayout, 4, 0, 2, 5);



		createObjectDialog->setLayout(cODlayout);
		createObjectDialog->show();

		connect(ok, SIGNAL(clicked()), this, SLOT(newObject()));
		connect(abort, SIGNAL(clicked()), createObjectDialog, SLOT(close()));
		connect(abort, SIGNAL(clicked()), createObjectDialog, SLOT(deleteLater()));

		QSignalMapper *selectFileMapper = new QSignalMapper(createObjectDialog);
		selectFileMapper->setMapping(selectFileButton, NameFilters::Img);
		
		connect(selectFileButton, SIGNAL(clicked()), selectFileMapper, SLOT(map()));
		connect(selectFileMapper,SIGNAL(mapped(int)),this, SLOT(fileDialog(int)));


		connect(objToolTip, SIGNAL(textEdited(QString)), this, SLOT(setToolTipString(QString)));
		
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


void MapFrame::setFileString(QString fileString)
{
filename = fileString;
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
  itemtoAdd->setData(3, QVariant(ziel.x()));
  itemtoAdd->setData(4, QVariant(ziel.y()));
 }


/*void MapFrame::fileDialog(int i)
{
fileDialog(i);
}*/

// void MapFrame::fileDialog(NameFilters::NFs filterarg)
  void MapFrame::fileDialog(int filterarg)
 {
  //QString fd_filename = QString();
fd = new QFileDialog(this, Qt::Dialog);
//fd->setFilter(filter);
fd->setModal(true);

QStringList filters;
if(filterarg == NameFilters::Img)
{

	filters	<< "Image files (*.png *.jpg)";
//		<< "All files (*)";
}
if(filterarg == NameFilters::Map)
{
	filters	<< "OpenHanse Map files (*.ohm)";
//		<< "All files (*)";
}

if(filterarg == (NameFilters::Map|NameFilters::Save))
{
	fd->setAcceptMode(QFileDialog::AcceptSave);
	filters	<< "OpenHanse Map files (*.ohm)";
}

fd->setNameFilters(filters);




  fd->show();
 connect(fd,SIGNAL(fileSelected(QString)),this,SLOT(setFileString(QString)));
 connect(fd,SIGNAL(finished(int)),fd,SLOT(deleteLater()));
  //filename = fd_filename;
  //return fd_filename;
 }