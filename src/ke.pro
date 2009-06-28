SOURCES = main.cpp \
	mainwindow.cpp\
	mapframe.cpp
HEADERS += mainwindow.h\
	mapframe.h
	
TEMPLATE = app
TARGET = KE
CONFIG += warn_on \
	  thread \
          qt

DESTDIR = bin/
