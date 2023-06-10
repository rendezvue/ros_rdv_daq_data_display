#include "dialoginfo.h"
#include "ui_dialoginfo.h"

DialogInfo::DialogInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
    ui->setupUi(this);

	connect(ui->pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui->pushButton_save_ini, SIGNAL(clicked()), this, SLOT(on_pushButton_save_ini()));
}

DialogInfo::~DialogInfo()
{
    delete ui;
}

void DialogInfo::showEvent(QShowEvent *ev)
{

	std::string str_device = "";
	std::string str_name = "";
	std::string str_job = "";
	
	if(boost::filesystem::exists("/home/rdv/.info.ini"))
	{
		//ini 파일 정보 읽기
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini( "/home/rdv/.info.ini", pt );
		str_device = pt.get<std::string>("info.str_device", "");
		str_name = pt.get<std::string>("info.str_name", "");
		str_job = pt.get<std::string>("info.str_job", "");

		ui->lineEdit_device->setText(QString::fromStdString(str_device));
		ui->lineEdit_name->setText(QString::fromStdString(str_name));
		ui->lineEdit_job->setText(QString::fromStdString(str_job));
	}
		
    QDialog::showEvent(ev) ;
}

void DialogInfo::on_pushButton_save_ini()
{
	printf("Save!!\n") ;
	
	std::time_t rawtime;
    std::tm* timeinfo;
	
	char buffer [80];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer,80,"%Y-%m-%d %H:%M:%S",timeinfo);
    std::string str_now(buffer);
	
	//save file ini
	std::string str_device = ui->lineEdit_device->text().toStdString();
	std::string str_name = ui->lineEdit_name->text().toStdString();
	std::string str_job = ui->lineEdit_job->text().toStdString();
	
	boost::property_tree::ptree pt2;
	pt2.put("info.str_device", str_device);
	pt2.put("info.str_name", str_name);
	pt2.put("info.str_job", str_job);
	pt2.put("info.save_date", str_now);
	boost::property_tree::write_ini( "/home/rdv/.info.ini", pt2 );

	ros::NodeHandle n;
	n.setParam("job_device", str_device);
	n.setParam("job_name", str_name);
	n.setParam("job_job", str_job);

	this->close() ;
}


