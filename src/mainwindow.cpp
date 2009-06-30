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
 
#include "mainwindow.h"

#include <QtGui/QMenuBar>
#include <QtGui/QAction>
//#include <QtGui/QToolBar>
#include <QtGui/QStatusBar>

#include <QtGui/QHBoxLayout>
//#include <QtGui/QVBoxLayout>
//#include <QtGui/QFrame>
//#include <QtGui/QFileDialog>

//#include <QtCore/QDir>


MainWindow::MainWindow()
 {

	QWidget *zentralwidget = new QWidget();
	setCentralWidget(zentralwidget);


	SideBar = new SideBarClass();
	SideBar->setParent(zentralwidget);
	SideBar->setFixedWidth(400);

	MapView = new MapFrame();
	MapView->setParent(zentralwidget); 
	MapView->initMap();

	QHBoxLayout *layout = new QHBoxLayout(zentralwidget);
	layout->setMargin(5); 
     
	layout->addWidget(MapView);
	layout->addWidget(SideBar);

	zentralwidget->setLayout(layout);


	createActions();
	createMenus();

	QString message = tr("A context menu is available by right-clicking");
	statusBar()->showMessage(message);

	setWindowTitle(tr("Karteneditor"));
	setMinimumSize(480, 360);
	resize(800, 600);
 
	connect(MapView, SIGNAL(newObjectCreated()), this, SLOT(addNewObjectToList()));
	connect(SideBar->itemListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(updateItemList(int)));
	connect(SideBar->selectFileButton, SIGNAL(clicked()), this, SLOT(sideBar_SelectFile()));
	connect(SideBar->XBox, SIGNAL(editingFinished()), this, SLOT(spinboxHandler()));
	connect(SideBar->YBox, SIGNAL(editingFinished()), this, SLOT(spinboxHandler()));
	connect(SideBar->ZBox, SIGNAL(editingFinished()), this, SLOT(spinboxHandler()));
}

void MainWindow::createActions()
 {
  newAct = new QAction(tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new map"));
  connect(newAct, SIGNAL(triggered()), MapView, SLOT(newMap()));

  openAct = new QAction(tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing map"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the map with the current name to disk"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  saveAsAct = new QAction(tr("&Save As ..."), this);
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  saveAsAct->setStatusTip(tr("Save the map to disk"));
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(save()));

  quitAct = new QAction(tr("E&xit"), this);
  quitAct->setShortcut(tr("Ctrl+Q"));
  setStatusTip(tr("Close the application"));
  connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
 }

 void MainWindow::newFile()
 {
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
	disconnect(saveAct, SIGNAL(triggered()), this, SLOT(savef()));
	MapView->newMap();
 }

 void MainWindow::open()
 {

 }

 void MainWindow::save()
 {
 MapView->fileDialog(NameFilters::Map|NameFilters::Save);

  //::getSaveFileName(this, tr("Save File"), home() ,;

 //MapView->saveMap(MapView->filename);
 connect(MapView->fd, SIGNAL(accepted()), this, SLOT(savef()));
 
 }
 
 void MainWindow::savef()
 {
	disconnect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(savef()));

	disconnect(MapView->fd, SIGNAL(accepted()), this, SLOT(savef()));
	mapfilename = MapView->fd_filename;
	setWindowTitle(tr("Karteneditor: ").append(mapfilename));
	MapView->saveMap(mapfilename);
 }

void MainWindow::createMenus()
 {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));

  helpMenu = menuBar()->addMenu(tr("&Help"));
 }
 
void MainWindow::addNewObjectToList()
{

QGraphicsItem *entry = MapView->itemList.last();

SideBar->MapEntries << entry->data(0).toString().append(entry->data(1).toString()).append(entry->data(2).toString());
SideBar->itemListWidget->clear();
SideBar->itemListWidget->addItems(SideBar->MapEntries);
}

void MainWindow::updateItemList(int selectedItemRow)
{
	if(selectedItemRow == 1)	//mapbackgroundimagefilepath
	{
		if(MapView->bgi_filename.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->bgi_filename).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->bgi_filename);
		
		//SideBar->editToolTip->setToolTip(tr("Comment"));
		SideBar->editToolTip->setEnabled(false);
		SideBar->editToolTip->setText(QString());
		SideBar->selectFileButton->setEnabled(true);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(true);
		SideBar->XBox->setEnabled(true);
		SideBar->XBox->setValue(MapView->mapSize.width());
		SideBar->YBox->setValue(MapView->mapSize.height());
	}

	if(selectedItemRow == 0)	//city-name
	{
		SideBar->editToolTip->setEnabled(true);
		SideBar->selectFileButton->setEnabled(false);
		if(MapView->cityname.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->cityname).right(39).prepend("..."));
		}
		else
			SideBar->editToolTip->setText(MapView->cityname);
		SideBar->fileView->setText(QString());
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(true);
		SideBar->XBox->setEnabled(true);
		SideBar->XBox->setValue(MapView->mapSize.width());
		SideBar->YBox->setValue(MapView->mapSize.height());
	}

	if(selectedItemRow == 2)	//Map: North
	{
		SideBar->editToolTip->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		if(MapView->mapnorth.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->mapnorth).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->mapnorth);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);

// 		SideBar->XBox->setValue(MapView->mapSize.width());
// 		SideBar->YBox->setValue(MapView->mapSize.height());
	}
	if(selectedItemRow == 3)	//Map: West
	{
		SideBar->editToolTip->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		if(MapView->mapwest.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->mapwest).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->mapwest);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);
// 		SideBar->XBox->setValue(MapView->mapSize.width());
// 		SideBar->YBox->setValue(MapView->mapSize.height());
	}
	if(selectedItemRow == 4)	//Map: South
	{
		SideBar->editToolTip->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		if(MapView->mapsouth.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->mapsouth).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->mapsouth);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);
// 		SideBar->XBox->setValue(MapView->mapSize.width());
// 		SideBar->YBox->setValue(MapView->mapSize.height());
	}
	if(selectedItemRow == 5)	//Map: East
	{
		SideBar->editToolTip->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		if(MapView->mapeast.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->mapeast).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->mapeast);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);
// 		SideBar->XBox->setValue(MapView->mapSize.width());
// 		SideBar->YBox->setValue(MapView->mapSize.height());
	}
	if(selectedItemRow > 5)
	{
		SideBar->editToolTip->setEnabled(true);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->YBox->setEnabled(true);
		SideBar->XBox->setEnabled(true);
		SideBar->ZBox->setEnabled(true);
		SideBar->XBox->setValue(MapView->itemList.value(selectedItemRow - 6)->x());
		SideBar->YBox->setValue(MapView->itemList.value(selectedItemRow - 6)->y());

		SideBar->editToolTip->setText(MapView->itemList.value(selectedItemRow - 6)->data(1).toString());
		
		if(MapView->itemList.value(selectedItemRow - 6)->data(2).toString().size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->itemList.value(selectedItemRow - 6)->data(2).toString()).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->itemList.value(selectedItemRow - 6)->data(2).toString());
	}

}

void MainWindow::sideBar_SelectFile()
{
	if(SideBar->itemListWidget->currentRow() == 1 || SideBar->itemListWidget->currentRow() > 5)
	{
		MapView->fileDialog(NameFilters::Img);
	}
	else
	{
		MapView->fileDialog(NameFilters::Map);
	}
	connect(MapView->fd, SIGNAL(accepted()), this, SLOT(sideBar_FileSelected()));
}

void MainWindow::sideBar_FileSelected()
{
	disconnect(MapView->fd, SIGNAL(accepted()), this, SLOT(sideBar_FileSelected()));
	SideBar->fileView->setText(MapView->fd_filename);
	if(SideBar->itemListWidget->currentRow() > 5)
	{
		MapView->itemList.value(SideBar->itemListWidget->currentRow() - 6)->setData(1, QVariant(MapView->fd_filename));
	}
	if(SideBar->itemListWidget->currentRow() == 1)
	{
		MapView->bgi_filename = MapView->fd_filename;
		MapView->szene->setBackgroundBrush(QBrush(QImage(MapView->bgi_filename)));
	}
	if(SideBar->itemListWidget->currentRow() == 2)
	{
		MapView->mapnorth = MapView->fd_filename;
	}
	if(SideBar->itemListWidget->currentRow() == 3)
	{
		MapView->mapwest = MapView->fd_filename;
	}
	if(SideBar->itemListWidget->currentRow() == 4)
	{
		MapView->mapsouth = MapView->fd_filename;
	}
	if(SideBar->itemListWidget->currentRow() == 5)
	{
		MapView->mapeast = MapView->fd_filename;
	}

}

void MainWindow::spinboxHandler()
{
	if(SideBar->itemListWidget->currentRow() > 5)
	{
		MapView->itemList.value(SideBar->itemListWidget->currentRow() - 6)->setPos(SideBar->XBox->value(), SideBar->YBox->value());
		MapView->itemList.value(SideBar->itemListWidget->currentRow() - 6)->setZValue(SideBar->ZBox->value());
	}
	if(SideBar->itemListWidget->currentRow() <= 5)
	{
		MapView->mapSize = QSize(SideBar->XBox->value(), SideBar->YBox->value());
		MapView->setSceneRect(0, 0, MapView->mapSize.width(), MapView->mapSize.height());
	}


}
