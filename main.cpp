/**
 * @file /src/main.cpp
 *
 * @brief Qt based gui.
 *
 * @date November 2010
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

//#include <qt5/QtGui/QtGui>
#include <QtGui>
#include <QApplication>
//#include "../include/qdude/main_window.hpp"
#include "mainwindow.h"

/*****************************************************************************
** Main
*****************************************************************************/

int main(int argc, char **argv) {
    /*********************
    ** Qt
    **********************/

	 qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard")); 
	
    QApplication a(argc, argv);
    MainWindow w;

	//폰트 PATH 변경이 필요함.
	if(boost::filesystem::exists("/home/rdv/catkin_ws/src/ros_rdv_daq_ui/fonts/2/NotoSansCJKkr-Bold.ttf"))
	{
		int id = QFontDatabase::addApplicationFont("/home/rdv/catkin_ws/src/ros_rdv_daq_ui/fonts/2/NotoSansCJKkr-Bold.ttf");
	    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
	     
		QFont SerifFont(family);
		QApplication::setFont(SerifFont);	
	}

	a.installEventFilter(&w);

	//w.setWindowFlags(Qt::WindowStaysOnTopHint);
    w.show();

    return a.exec();
}
