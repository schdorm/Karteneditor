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
SideBar->setFixedWidth(300);

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
 setMinimumSize(300, 200);
 resize(800, 600);
 
 connect(MapView, SIGNAL(newObjectCreated()), this, SLOT(addNewObjectToList()));
 connect(SideBar->itemList, SIGNAL(currentRowChanged(int)), this, SLOT(updateItemList(int)));
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
  saveAct->setStatusTip(tr("Save the map to disk"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

  quitAct = new QAction(tr("E&xit"), this);
  quitAct->setShortcut(tr("Ctrl+Q"));
  setStatusTip(tr("Close the application"));
  connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
 }

 void MainWindow::newFile()
 {

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
 }

void MainWindow::createMenus()
 {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));

  helpMenu = menuBar()->addMenu(tr("&Help"));
 }
 
void MainWindow::addNewObjectToList()
{

QGraphicsItem *entry = MapView->items().last();

SideBar->MapEntries << entry->data(3).toString();
SideBar->itemList->clear();
SideBar->itemList->addItems(SideBar->MapEntries);
}

void MainWindow::updateItemList(int selectedItemRow)
{
if(selectedItemRow == 0)
{
}

}
