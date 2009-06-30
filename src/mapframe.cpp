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

 #include <QtCore/QDebug>
 #include <QtCore/QList>
 #include <QtCore/QDir>
 #include <QtCore/QFile>
 #include <QtCore/QTextStream>
 #include <QtGui/QComboBox>
 #include <QtGui/QLabel>
 #include <QtGui/QGridLayout>
 #include <QtGui/QHBoxLayout>
 #include <QtGui/QPushButton>
 #include <QtGui/QLineEdit>
 #include <QtGui/QSpinBox>

 #include <QStringList> 
 #include <QtCore/QSignalMapper>


 #include <QtGui/QPixmap>
 
 
 void MapFrame::initMap()
 {
 szene = new QGraphicsScene();
 setScene(szene);
 newMap();
 }

 void MapFrame::newMap()
 {
 szene->clear();
 mapSize = QSize(1000,1000);
 setSceneRect(0,0,1000,1000);
 }

void MapFrame::saveMap(QString save_filename)
{
	if(!save_filename.endsWith(".ohm"))
	{
		save_filename.append(".ohm");
	}
//	QList<QGraphicsItem*> savelist = itemList();


	QString save_bgi_filename = bgi_filename;		// Hintergrundfilenamensbearbeitung
	QString save_dir_string = save_filename;

	  {
		int n = 0;
		int y = 0;
		while(n < 1)
		{
			y++;
			n = save_bgi_filename.right(y).count("/");
		}
		save_bgi_filename = save_bgi_filename.right(y).prepend("img");
		n = 0;
		y = 0;
		while(n < 1)
		{
			y++;
			n = save_dir_string.right(y).count("/");
		}
		save_dir_string = save_dir_string.left(save_dir_string.size() - y);
	  }
	QDir save_dir = QDir(save_dir_string);
	if(!save_dir.cd("img"))
	{
		if(!save_dir.mkdir("img"))
		{
			QDialog *saveFailedDialog = new QDialog(this);
			QPushButton *ok = new QPushButton (saveFailedDialog);
			connect(ok, SIGNAL(clicked()), saveFailedDialog, SLOT(deleteLater()));
			return;
		}
	}
	if(! QFile(save_bgi_filename).exists())
		{
		QFile(bgi_filename).copy(QString(save_bgi_filename).prepend(save_dir_string));
		}
	
	QFile savefile(save_filename);
	savefile.open(QIODevice::WriteOnly);
	QTextStream savestream(&savefile);
	savestream << "<map>\n";

  savestream << "<maphintergrund>\n";
  savestream << save_bgi_filename;
  savestream << "\n</maphintergrund>\n";
  if(!cityname.isEmpty())
  {
  savestream << "<stadtname>\n";
  savestream << cityname;
  savestream << "\n</stadtname>\n";
  }
  savestream << "<mapbreite>\n";
  savestream << mapSize.width();
  savestream << "\n</mapbreite>\n";
  savestream << "<maphoehe>\n";
  savestream << mapSize.height();
  savestream << "\n</maphoehe>\n";
  //hier die Mapprops reinschreiben (Groesse, File, etc.)
  QGraphicsItem *saveitem;
  foreach(saveitem, itemList)
 {
	 savestream << "<objekt>\n";
	 savestream << "<objektfunktion>\n";
	 savestream << saveitem->data(0).toString();
	 savestream << "\n</objektfunktion>\n";
	 if(!saveitem->data(1).toString().isEmpty())
	 {
		  savestream << "<objekttooltip>\n";
		 savestream << saveitem->data(1).toString();
		 savestream << "\n</objekttooltip>\n";
 	 }
	  {
		QString img_filename = saveitem->data(2).toString();
		int n = 0;
		int y = 0;
		while(n < 1)
		{
			y++;
			n = img_filename.right(y).count("/");
		}
		img_filename = img_filename.right(y).prepend("img");

		savestream << "<objektdatei>\n";
		savestream << img_filename;
		savestream << "\n</objektdatei>\n";
		
		if(! QFile(img_filename).exists())
		{
		qWarning() << "Copying Object File:" << QString(img_filename).prepend(save_dir_string);
		QFile(saveitem->data(2).toString()).copy(QString(img_filename).prepend(save_dir_string));
		}
		
	  }
	 


  	savestream << "<objektpositionx>\n";
 	savestream << saveitem->data(3).toInt();
 	savestream << "\n</objektpositionx>\n";

 	savestream << "<objektpositiony>\n";
 	savestream << saveitem->data(4).toInt();
 	savestream << "\n</objektpositiony>\n";
 	
 	savestream << "<objekthohe>\n";
 	savestream << saveitem->zValue();
 	savestream << "\n</objekthohe>\n";
 	savestream << "</objekt>\n";
 }
 savestream << "</map>";
 }
 
 void MapFrame::loadMap(QString load_filename)
 {
 }
 
 void MapFrame::mousePressEvent(QMouseEvent *event)
 {
 object_tooltip = QString();
 object_filename = QString();
 fd_filename = QString();
 ziel = event->pos();
	QList<QGraphicsItem *> QGIlistAtClick = items(event->pos());
	if(QGIlistAtClick.isEmpty())
	{
		createObjectDialog = new QDialog();
		createObjectDialog->setModal(true);
		createObjectDialog->setWindowTitle(tr("Create new Object ..."));

		QGridLayout *cODlayout = new QGridLayout(createObjectDialog);

		QLabel *labelimg = new QLabel(createObjectDialog);
		labelimg->setText(tr("Image File"));
		cODlayout->addWidget(labelimg, 0, 0, 1, 1);

		QLabel *filelabel = new QLabel(createObjectDialog);
		filelabel->setFrameShape(QFrame::Box);
		cODlayout->addWidget(filelabel, 0, 1, 1, 3);

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
		
		QSpinBox *XBox_MV = new QSpinBox(createObjectDialog);
		XBox_MV->setRange(0, mapSize.width());
		XBox_MV->setValue(ziel.x());
		
		QSpinBox *YBox_MV = new QSpinBox(createObjectDialog);
		YBox_MV->setRange(0, mapSize.height());
		YBox_MV->setValue(ziel.y());
		
		QHBoxLayout spinboxlayout;
//		spinboxlayout.addSpacing(20);
		QLabel *XLabel = new QLabel(tr("X-Pos:"),createObjectDialog);
		spinboxlayout.addWidget(XLabel);
		spinboxlayout.addWidget(XBox_MV);
//		spinboxlayout.addSpacing(40);
		QLabel *YLabel = new QLabel(tr("Y-Pos:"),createObjectDialog);
		spinboxlayout.addWidget(YLabel);
		spinboxlayout.addWidget(YBox_MV);
//		spinboxlayout.addSpacing(20);
		cODlayout->setRowStretch(3,2);
		cODlayout->addLayout(&spinboxlayout, 4, 0, 2, 5);

		QPushButton *ok = new QPushButton(tr("Ok"), createObjectDialog);
		QPushButton *abort = new QPushButton(tr("Abort"), createObjectDialog);
				
		QHBoxLayout buttonlayout;
		buttonlayout.addSpacing(20);
		buttonlayout.addWidget(ok);
		buttonlayout.addSpacing(40);
		buttonlayout.addWidget(abort);
		buttonlayout.addSpacing(20);
		cODlayout->setRowStretch(5,2);
		cODlayout->addLayout(&buttonlayout, 6, 0, 2, 5);



		createObjectDialog->setLayout(cODlayout);
		createObjectDialog->show();

		connect(ok, SIGNAL(clicked()), this, SLOT(newObject()));
		connect(abort, SIGNAL(clicked()), createObjectDialog, SLOT(close()));
		connect(abort, SIGNAL(clicked()), createObjectDialog, SLOT(deleteLater()));

		QSignalMapper *selectFileMapper = new QSignalMapper(createObjectDialog);
		selectFileMapper->setMapping(selectFileButton, NameFilters::Img);
		
		connect(selectFileButton, SIGNAL(clicked()), selectFileMapper, SLOT(map()));
		connect(selectFileMapper,SIGNAL(mapped(int)),this, SLOT(fileDialog(int)));
		
		connect(this, SIGNAL(fileStringChanged(QString)), filelabel, SLOT(setText(QString)));

		connect(fkt, SIGNAL(currentIndexChanged(QString)), this, SLOT(setTyp(QString)));
		connect(objToolTip, SIGNAL(textEdited(QString)), this, SLOT(setToolTipString(QString)));
		connect(XBox_MV, SIGNAL(valueChanged(int)), this, SLOT(setXPos(int)));
		connect(YBox_MV, SIGNAL(valueChanged(int)), this, SLOT(setYPos(int)));
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

void MapFrame::setTyp(QString text)
{
object_typ = text;
}

void MapFrame::setXPos(int xpos)
{
ziel.setX(xpos);
}

void MapFrame::setYPos(int ypos)
{
ziel.setY(ypos);
}

void MapFrame::setToolTipString(QString ttstring)
{
object_tooltip = ttstring;
}


void MapFrame::newObject()
{
object_filename = fd_filename;
	if(!object_typ.isEmpty() && !object_filename.isEmpty())
	{
		newObject(object_typ, object_filename, object_tooltip);
	}
	else
	{
	QDialog *missingArgs = new QDialog(this);
	missingArgs->setModal(true);
	missingArgs->show();
	}
}


 void MapFrame::newObject(QString otyp, QString ofilename, QString otooltip)
 {
 createObjectDialog->close();
 delete createObjectDialog;
  QGraphicsPixmapItem *itemtoAdd = szene->addPixmap(QPixmap(ofilename));
  itemtoAdd->setPos(ziel);
  itemtoAdd->setToolTip(otooltip);
  itemtoAdd->setData(0, QVariant(otyp));
  itemtoAdd->setData(1, QVariant(otooltip));
  itemtoAdd->setData(2, QVariant(ofilename));
  itemtoAdd->setData(3, QVariant(ziel.x()));
  itemtoAdd->setData(4, QVariant(ziel.y()));
  itemList << itemtoAdd;
  emit newObjectCreated();

 }


/*void MapFrame::fileDialog(int i)
{
fileDialog(i);
}*/

// void MapFrame::fileDialog(NameFilters::NFs filterarg)
void MapFrame::fileDialog(int filterarg)
{
	fd = new QFileDialog(this, Qt::Dialog);
//	fd->setFilter(filter);
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
}

void MapFrame::setFileString(QString fileString)
{
fd_filename = fileString;
emit fileStringChanged(fileString);
}
