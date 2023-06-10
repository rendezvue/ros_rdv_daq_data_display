#ifndef DIALOGRINFO_H
#define DIALOGRINFO_H

#include <QDialog>

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/asio.hpp>

#include "ros/ros.h"

namespace Ui {
class DialogInfo;
}

class DialogInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInfo(QWidget *parent = nullptr);
    ~DialogInfo();


protected:
    void showEvent(QShowEvent *ev) override;


private slots:
	void on_pushButton_save_ini() ;
	
private:
    Ui::DialogInfo *ui;
};

#endif // DIALOGINFO_H
