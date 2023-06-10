#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h> 
#include <time.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) ,
    m_rdv_thread(NULL) ,
    m_p_qimage(nullptr) ,
    m_p_buffer(nullptr) ,
    m_i_buffer_size(0)
{
    ui->setupUi(this);

	ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
	ui->label_time->setStyleSheet("QLabel { color : QColor(101,193,192); }");
	//ui->label_time->setStyleSheet("QLabel { color : white; }");
	ui->label_bag_path->setStyleSheet("QLabel { color : white; }");
	ui->label_version->setStyleSheet("QLabel { color : white; }");

	ui->label_can_info_1->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_2->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_3->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_4->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_5->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_6->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_8->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_9->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_10->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_11->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_12->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_13->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_14->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_15->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_16->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_17->setStyleSheet("QLabel { color : white; }");
	ui->label_can_raw->setStyleSheet("QLabel { color : gray; }");

	//can data
	ui->label_can_info_engine_speed->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_engine_torque->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_vehicle_speed->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_hitch_wheel_angle->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_gps_status->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_latitude->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_longitude->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_imu_angular_velocity_x->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_imu_angular_velocity_y->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_imu_angular_velocity_z->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_imu_linear_acceleration_x->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_imu_linear_acceleration_y->setStyleSheet("QLabel { color : white; }");
	ui->label_can_info_imu_linear_acceleration_z->setStyleSheet("QLabel { color : white; }");
			
	ui->label_info_top_left->setStyleSheet("QLabel { color : gray; }");
	ui->label_info_top_middle->setStyleSheet("QLabel { color : gray; }");
	ui->label_info_top_right->setStyleSheet("QLabel { color : gray; }");
	ui->label_info_bottom_left->setStyleSheet("QLabel { color : gray; }");
	ui->label_info_bottom_middle->setStyleSheet("QLabel { color : gray; }");
	ui->label_info_bottom_right->setStyleSheet("QLabel { color : gray; }");

	#if 0
	ui->label_image_top_left->setStyleSheet("QLabel { background-color : green; }");
	ui->label_image_top_middle->setStyleSheet("QLabel { background-color : green; }");
	ui->label_image_top_right->setStyleSheet("QLabel { background-color : green; }");
	ui->label_image_bottom_left->setStyleSheet("QLabel { background-color : green; }");
	ui->label_image_bottom_middle->setStyleSheet("QLabel { background-color : green; }");
	ui->label_image_bottom_right->setStyleSheet("QLabel { background-color : green; }");
	#endif
		
    //showMaximized();
    //setWindowState(Qt::WindowFullScreen);
	
	printf("MainWindow : Init : Start Camera Thread\n") ;
	//BOOST_LOG_TRIVIAL(info) << "MainWindow : Init : Start Camera Thread";
   	p_thread = new boost::thread(boost::bind(&MainWindow::ThreadFunction, this));
	p_thread_time = new boost::thread(boost::bind(&MainWindow::ThreadFunction_Time, this));
		
	printf("MainWindow : Init : Start Draw Thread\n") ;
	//BOOST_LOG_TRIVIAL(info) << "MainWindow : Init : Start Draw Thread";
	m_rdv_thread = new CRdvThread() ;
	//connect(m_rdv_thread, SIGNAL(Update()),this, SLOT(update()));
	connect(m_rdv_thread, SIGNAL(Update()),this, SLOT(Update()));
    m_rdv_thread->start();
	
	printf("Init. Done\n") ;
	//BOOST_LOG_TRIVIAL(info) << "Init. Done";
}

MainWindow::~MainWindow()
{
	if( m_p_qimage )	delete m_p_qimage ;
	if( m_p_buffer )	delete [] m_p_buffer ;
	
    delete ui;
}

std::vector<std::string> MainWindow::StrSplit(std::string str, char Delimiter) 
{
    std::istringstream iss(str);             // istringstream에 str을 담는다.
    std::string buffer;                      // 구분자를 기준으로 절삭된 문자열이 담겨지는 버퍼
 
    std::vector<std::string> result;
 
    // istringstream은 istream을 상속받으므로 getline을 사용할 수 있다.
    while (std::getline(iss, buffer, Delimiter)) {
        result.push_back(buffer);               // 절삭된 문자열을 vector에 저장
    }
 
    return result;
}



void MainWindow::Update(void)
{
#if 1
#if 0
	cv::Mat image_input ;
	cv::Mat image_result ;

	//-------------------------------------------------------
	//check display image
	if( !m_display_image.empty() )
	{
		if( m_display_image.cols != ui->label_image->width() ||  m_display_image.rows != ui->label_image->height() )
		{
			m_display_image.release() ;
		}
	}

	//printf("Update 1\n") ;

	if( m_display_image.empty() )
	{		
		//printf("Update 1-1 : %d, %d\n", ui->label_image->width(), ui->label_image->height()) ;

		try
		{
			//m_display_image = cv::Mat(cv::Size(ui->label_image->width(), ui->label_image->height()), CV_8UC3) ;
			m_display_image = cv::Mat(ui->label_image->height(), ui->label_image->width(), CV_8UC3) ;
		}
		catch( cv::Exception& e )
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
		}

		//printf("Update 1-2\n") ; 
	}


	//printf("Update 2\n") ;

	//black
	m_display_image = cv::Scalar(0,0,0) ;

	//Zed Camera
	int count_w_image = 3 ;
	int margin_image = 10 ;
	int width_margin = margin_image * (count_w_image+1) ;
	int width_image = (m_display_image.cols-width_margin)/count_w_image ;

	int start_h_image = 100 ;
	int count_h_image = 2 ;
	int height_margin = margin_image * (count_h_image+1) ;
	int height_image = (m_display_image.rows-start_h_image-height_margin*2)/count_h_image ;
		
	cv::Rect rect_image_top_left = 			cv::Rect(margin_image, start_h_image, width_image, height_image ) ;
	cv::Rect rect_image_top_middle = 		cv::Rect(rect_image_top_left.x+rect_image_top_left.width+margin_image, start_h_image, width_image, height_image ) ;
	cv::Rect rect_image_top_right = 		cv::Rect(rect_image_top_middle.x+rect_image_top_middle.width+margin_image, start_h_image, width_image, height_image ) ;

	cv::Rect rect_image_bottom_left = 		cv::Rect(rect_image_top_left.x, rect_image_top_left.y+rect_image_top_left.height+height_margin , width_image, height_image ) ;
	cv::Rect rect_image_bottom_middle = 	cv::Rect(rect_image_top_middle.x, rect_image_top_middle.y+rect_image_top_middle.height+height_margin , width_image, height_image ) ;
	cv::Rect rect_image_bottom_right = 		cv::Rect(rect_image_top_right.x, rect_image_top_right.y+rect_image_top_right.height+height_margin , width_image, height_image ) ;

	m_display_image(rect_image_top_left) = cv::Scalar(255,0,0) ;
	m_display_image(rect_image_top_middle) = cv::Scalar(255,0,0) ;
	m_display_image(rect_image_top_right) = cv::Scalar(255,0,0) ;

	m_display_image(rect_image_bottom_left) = cv::Scalar(0,255,0) ;
	m_display_image(rect_image_bottom_middle) = cv::Scalar(0,255,0) ;
	m_display_image(rect_image_bottom_right) = cv::Scalar(0,255,0) ;
#endif
	
	//printf("Update 2 - 1\n") ;
	//lidar
	#if 0
	cv::Mat image_lidar;
	m_mutex_image_lidar.lock();
	if( !m_image_lidar.empty())	m_image_lidar.copyTo(image_lidar) ;
	m_mutex_image_lidar.unlock() ;
	#else
	QImage qimage_lidar;
	m_mutex_image_lidar.lock();
	qimage_lidar = m_qimage_lidar.copy() ;
	m_mutex_image_lidar.unlock() ;
	#endif
	
	//Zed rgb	
	cv::Mat image_zed_rgb ;
	m_mutex_image_zed_rgb.lock();
	if( !m_image_zed_rgb.empty())	m_image_zed_rgb.copyTo(image_zed_rgb) ;
	m_mutex_image_zed_rgb.unlock() ;

	//Zed depth
	cv::Mat image_zed_depth ;
	m_mutex_image_zed_depth.lock();
	if( !m_image_zed_depth.empty())	m_image_zed_depth.copyTo(image_zed_depth) ;
	m_mutex_image_zed_depth.unlock() ;

	//Lucid rgb
	cv::Mat image_lucid_rgb ;
	m_mutex_image_lucid_rgb.lock();
	if( !m_image_camera_rgb.empty())	m_image_camera_rgb.copyTo(image_lucid_rgb) ;
	m_mutex_image_lucid_rgb.unlock() ;

	//Lucid nir
	cv::Mat image_lucid_nir ;
	m_mutex_image_lucid_nir.lock();
	if( !m_image_camera_nir.empty())	m_image_camera_nir.copyTo(image_lucid_nir) ;
	m_mutex_image_lucid_nir.unlock() ;

	//printf("Update 2 - 2\n") ;
	
	//resize
	//lidar
	//if( !image_lidar.empty() )
	{
	#if 0
		//printf("Update 2 - 2 - 11 : image_zed_rgb : %d x %d\n", image_zed_rgb.cols, image_zed_rgb.rows) ;
		//printf("Update 2 - 2 - 11 : rect_image_bottom_middle : %d x %d\n", rect_image_bottom_middle.width, rect_image_bottom_middle.height) ;
		
		cv::Mat resize_image_zed_rgb = cv::Mat(rect_image_bottom_middle.height, rect_image_bottom_middle.width, CV_8UC3) ;

		//printf("Update 2 - 2 - 2\n") ;
		cv::resize(image_zed_rgb, resize_image_zed_rgb, cv::Size(rect_image_bottom_middle.width, rect_image_bottom_middle.height)) ;

		//image copy
		//printf("Update 2 - 2 - 3\n") ;
		resize_image_zed_rgb.copyTo(m_display_image(rect_image_bottom_middle)) ;
	#endif
		
		if( qimage_lidar.width() != ui->label_image_top_left->width() || qimage_lidar.height() != ui->label_image_top_left->height() )
		{
			qimage_lidar = qimage_lidar.scaled(ui->label_image_top_left->width(), ui->label_image_top_left->height(), Qt::IgnoreAspectRatio);
		}
		ui->label_image_top_left->setPixmap(QPixmap::fromImage(qimage_lidar));
	}
	
	if( !image_zed_rgb.empty() )
	{
	#if 0
		//printf("Update 2 - 2 - 11 : image_zed_rgb : %d x %d\n", image_zed_rgb.cols, image_zed_rgb.rows) ;
		//printf("Update 2 - 2 - 11 : rect_image_bottom_middle : %d x %d\n", rect_image_bottom_middle.width, rect_image_bottom_middle.height) ;
		
		cv::Mat resize_image_zed_rgb = cv::Mat(rect_image_bottom_middle.height, rect_image_bottom_middle.width, CV_8UC3) ;

		//printf("Update 2 - 2 - 2\n") ;
		cv::resize(image_zed_rgb, resize_image_zed_rgb, cv::Size(rect_image_bottom_middle.width, rect_image_bottom_middle.height)) ;

		//image copy
		//printf("Update 2 - 2 - 3\n") ;
		resize_image_zed_rgb.copyTo(m_display_image(rect_image_bottom_middle)) ;
	#endif
		
		QImage qt_display_image((const uchar *) image_zed_rgb.data, image_zed_rgb.cols, image_zed_rgb.rows, image_zed_rgb.step, QImage::Format_RGB888);
		qt_display_image.bits(); // enforce deep copy, see documentation 

		if( image_zed_rgb.cols != ui->label_image_bottom_middle->width() || image_zed_rgb.rows != ui->label_image_bottom_middle->height() )
		{
			qt_display_image = qt_display_image.scaled(ui->label_image_bottom_middle->width(), ui->label_image_bottom_middle->height(), Qt::IgnoreAspectRatio);
		}
		ui->label_image_bottom_middle->setPixmap(QPixmap::fromImage(qt_display_image));
	}

	//printf("Update 3\n") ;
	
	if( !image_zed_depth.empty() )
	{
		#if 0
		cv::Mat resize_image_zed_depth = cv::Mat(rect_image_bottom_left.height, rect_image_bottom_left.width, CV_8UC3) ;
		cv::resize(image_zed_depth, resize_image_zed_depth, cv::Size(rect_image_bottom_left.width, rect_image_bottom_left.height)) ;

		//image copy
		resize_image_zed_depth.copyTo(m_display_image(rect_image_bottom_left)) ;
		#endif
		QImage qt_display_image((const uchar *) image_zed_depth.data, image_zed_depth.cols, image_zed_depth.rows, image_zed_depth.step, QImage::Format_RGB888);
		qt_display_image.bits(); // enforce deep copy, see documentation 

		if( image_zed_depth.cols != ui->label_image_bottom_left->width() || image_zed_depth.rows != ui->label_image_bottom_left->height() )
		{
			qt_display_image = qt_display_image.scaled(ui->label_image_bottom_left->width(), ui->label_image_bottom_left->height(), Qt::IgnoreAspectRatio);
		}
		ui->label_image_bottom_left->setPixmap(QPixmap::fromImage(qt_display_image));
		
	}

	//printf("Update 4\n") ;

	if( !image_lucid_rgb.empty() )
	{
		#if 0
		//printf("Update 4-1\n") ;
		cv::Mat resize_image_lucid_rgb = cv::Mat(rect_image_top_middle.height, rect_image_top_middle.width, CV_8UC3) ;
		//printf("Update 4-2\n") ;
		cv::resize(image_lucid_rgb, resize_image_lucid_rgb, cv::Size(rect_image_top_middle.width, rect_image_top_middle.height)) ;
		//printf("Update 4-3\n") ;
		
		//image copy
		resize_image_lucid_rgb.copyTo(m_display_image(rect_image_top_middle)) ;
		//printf("Update 4-4\n") ;
		#endif
		QImage qt_display_image((const uchar *) image_lucid_rgb.data, image_lucid_rgb.cols, image_lucid_rgb.rows, image_lucid_rgb.step, QImage::Format_RGB888);
		qt_display_image.bits(); // enforce deep copy, see documentation 

		if( image_lucid_rgb.cols != ui->label_image_top_middle->width() || image_lucid_rgb.rows != ui->label_image_top_middle->height() )
		{
			qt_display_image = qt_display_image.scaled(ui->label_image_top_middle->width(), ui->label_image_top_middle->height(), Qt::IgnoreAspectRatio);
		}
		ui->label_image_top_middle->setPixmap(QPixmap::fromImage(qt_display_image));
	}

	//printf("Update 5\n") ;

	if( !image_lucid_nir.empty() )
	{
		#if 0
		cv::Mat resize_image_lucid_nir = cv::Mat(rect_image_top_right.height, rect_image_top_right.width, CV_8UC3) ;
		cv::resize(image_lucid_nir, resize_image_lucid_nir, cv::Size(rect_image_top_right.width, rect_image_top_right.height)) ;

		//image copy
		resize_image_lucid_nir.copyTo(m_display_image(rect_image_top_right)) ;
		#endif
		QImage qt_display_image((const uchar *) image_lucid_nir.data, image_lucid_nir.cols, image_lucid_nir.rows, image_lucid_nir.step, QImage::Format_RGB888);
		qt_display_image.bits(); // enforce deep copy, see documentation 

		if( image_lucid_nir.cols != ui->label_image_top_right->width() || image_lucid_nir.rows != ui->label_image_top_right->height() )
		{
			qt_display_image = qt_display_image.scaled(ui->label_image_top_right->width(), ui->label_image_top_right->height(), Qt::IgnoreAspectRatio);
		}
		ui->label_image_top_right->setPixmap(QPixmap::fromImage(qt_display_image));
		
	}

	//printf("Update 6\n") ;
#if 0
	//Text Now
	int fontHeight = 25;
	int thickness = -1;
	int linestyle = 16;
	int baseline = 0;

	m_mutex_str_now.lock(); 
	std::string str_text = m_str_now ;
	m_mutex_str_now.unlock() ;

	//printf("Update str_text = %s\n", str_text.c_str()) ;

	//cv::Size textSize = m_ft2->getTextSize(str_text, fontHeight, thickness, &baseline);

	//printf("Update 2\n") ;
	m_ft2->putText(m_display_image, str_text, cv::Point(10,20+fontHeight), fontHeight, cv::Scalar(192,193,101), thickness, linestyle, true) ;

	
	//printf("Update 7\n") ;
	
	//작업 정보	
	fontHeight = 20;
	m_mutex_str_info.lock(); 
	std::string str_job_info = m_str_info ;
	m_mutex_str_info.unlock() ;
	
	//cv::Size textSize = m_ft2->getTextSize(str_job_info, fontHeight, thickness, &baseline);
	//m_ft2->putText(m_display_image, str_job_info, cv::Point(840 -textSize.width -10 ,20+fontHeight+10), fontHeight, cv::Scalar(192,193,101), thickness, linestyle, true) ;
	
	//printf("Update 8\n") ;
	
	//Sensor Info
	cv::Scalar color_text = cv::Scalar(255,255,255) ;
	fontHeight = 20 ;
	std::string str_info = "LIDAR" ;
	//textSize = m_ft2_info->getTextSize(str_info, fontHeight, thickness, &baseline);
	m_ft2_info->putText(m_display_image, str_info, cv::Point(rect_image_top_left.x,rect_image_top_left.y-3), fontHeight, color_text, thickness, linestyle, true) ;
	
	str_info = "RGB" ;
	//textSize = m_ft2_info->getTextSize(str_info, fontHeight, thickness, &baseline);
	m_ft2_info->putText(m_display_image, str_info, cv::Point(rect_image_top_middle.x,rect_image_top_middle.y-3), fontHeight, color_text, thickness, linestyle, true) ;

	str_info = "NIR" ;
	//textSize = m_ft2_info->getTextSize(str_info, fontHeight, thickness, &baseline);
	m_ft2_info->putText(m_display_image, str_info, cv::Point(rect_image_top_right.x,rect_image_top_right.y-3), fontHeight, color_text, thickness, linestyle, true) ;

	str_info = "STEREO : DEPTH" ;
	//textSize = m_ft2_info->getTextSize(str_info, fontHeight, thickness, &baseline);
	m_ft2_info->putText(m_display_image, str_info, cv::Point(rect_image_bottom_left.x,rect_image_bottom_left.y-3), fontHeight, color_text, thickness, linestyle, true) ;

	str_info = "STEREO : RGB" ;
	//textSize = m_ft2_info->getTextSize(str_info, fontHeight, thickness, &baseline);
	m_ft2_info->putText(m_display_image, str_info, cv::Point(rect_image_bottom_middle.x,rect_image_bottom_middle.y-3), fontHeight, color_text, thickness, linestyle, true) ;

	str_info = "ETC." ;
	//textSize = m_ft2_info->getTextSize(str_info, fontHeight, thickness, &baseline);
	m_ft2_info->putText(m_display_image, str_info, cv::Point(rect_image_bottom_right.x,rect_image_bottom_right.y-3), fontHeight, color_text, thickness, linestyle, true) ;

#endif	
	m_mutex_str_now.lock(); 
	std::string str_text = m_str_now ;
	m_mutex_str_now.unlock() ;
	
	ui->label_time->setText(QString::fromUtf8(str_text.c_str()))  ;

	ros::NodeHandle n;
	std::string str_param_rec_path = "" ;
   	n.getParam("rosgab_rec_path", str_param_rec_path);
   
	m_mutex_str_bag_path.lock() ;
	std::string str_bag_path = str_param_rec_path ;
	m_mutex_str_bag_path.unlock() ;
	ui->label_bag_path->setText(QString::fromUtf8(str_bag_path.c_str()))  ;

	const std::string str_version = std::string("Version : ") + CRdv_Get_Git_Info::getInstance()->GET_VERSION() ; 
	ui->label_version->setText(QString::fromUtf8(str_version.c_str()))  ;

	//CAN Data
	m_mutex_str_imu.lock() ;
	std::string str_imu_anglular_x  = m_str_imu_anglular_x; // = std::to_string(msg->angular_velocity.x) ;
	std::string str_imu_anglular_y = m_str_imu_anglular_y; // = std::to_string(msg->angular_velocity.y) ;;
	std::string str_imu_anglular_z = m_str_imu_anglular_z; // = std::to_string(msg->angular_velocity.z) ;;
	std::string str_imu_linear_x = m_str_imu_linear_x; // = std::to_string(msg->linear_acceleration.x) ;;
	std::string str_imu_linear_y = m_str_imu_linear_y; // = std::to_string(msg->linear_acceleration.y) ;;
	std::string str_imu_linear_z = m_str_imu_linear_z; // = std::to_string(msg->linear_acceleration.z) ;;
	m_mutex_str_imu.unlock() ;
	
	ui->label_can_info_imu_angular_velocity_x->setText(QString::fromUtf8(str_imu_anglular_x.c_str()));
	ui->label_can_info_imu_angular_velocity_y->setText(QString::fromUtf8(str_imu_anglular_y.c_str()));
	ui->label_can_info_imu_angular_velocity_z->setText(QString::fromUtf8(str_imu_anglular_z.c_str()));
	ui->label_can_info_imu_linear_acceleration_x->setText(QString::fromUtf8(str_imu_linear_x.c_str()));
	ui->label_can_info_imu_linear_acceleration_y->setText(QString::fromUtf8(str_imu_linear_y.c_str()));
	ui->label_can_info_imu_linear_acceleration_z->setText(QString::fromUtf8(str_imu_linear_z.c_str()));

	//GPS
	m_mutex_str_gps.lock();
	std::string str_latitude  = m_str_latitude + " °";
	std::string str_longitude = m_str_longitude + " °";
	m_mutex_str_gps.unlock();
	ui->label_can_info_latitude->setText(QString::fromUtf8(str_latitude.c_str()));
	ui->label_can_info_longitude->setText(QString::fromUtf8(str_longitude.c_str()));

	//엔진 스피드
	m_mutex_str_es.lock();
	std::string str_es = m_str_es + " RPM";
	m_mutex_str_es.unlock();
	ui->label_can_info_engine_speed->setText(QString::fromUtf8(str_es.c_str()));

	//엔진 토크
	m_mutex_str_et.lock();
	std::string str_et  = m_str_et + " %" ;
	m_mutex_str_et.unlock();
	ui->label_can_info_engine_torque->setText(QString::fromUtf8(str_et.c_str()));
	
	//차량 스피드
	m_mutex_str_vs.lock();
	std::string str_vs = m_str_vs + " km/h";
	m_mutex_str_vs.unlock();
	ui->label_can_info_vehicle_speed->setText(QString::fromUtf8(str_vs.c_str()));

	//히치제어유닛
	m_mutex_str_hitch_wheel_angle.lock();
	std::string str_hitch_wheel_angle = m_str_hitch_wheel_angle + " mV" ;
	m_mutex_str_hitch_wheel_angle.unlock();
	ui->label_can_info_hitch_wheel_angle->setText(QString::fromUtf8(str_hitch_wheel_angle.c_str()));
	
	m_mutex_str_can_raw.lock() ;
	std::string str_can_raw = m_str_can_raw ;
	m_mutex_str_can_raw.unlock() ;
	ui->label_can_raw->setText(QString::fromUtf8(str_can_raw.c_str()));
	
	//printf("Update 9\n") ;

	#if 0
	QImage qt_display_image = m_cls_mat_2_qimage.cvtMat2QImage(m_display_image, ui->label_image->width(), ui->label_image->height()) ;
	ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
	#endif
		

	//printf("Update 10\n") ;
#endif	
}

void MainWindow::colorize(const pcl::PointCloud<pcl::PointXYZ> &pc,
              pcl::PointCloud<pcl::PointXYZRGB> &pc_colored,
              const std::vector<int> &color) {

    int N              = pc.points.size();

	ROS_INFO("colorize N : %d", N);

    pc_colored.clear();
    pcl::PointXYZRGB pt_tmp;
    for (int         i = 0; i < N; ++i) {
        const auto &pt = pc.points[i];
        pt_tmp.x = pt.x;
        pt_tmp.y = pt.y;
        pt_tmp.z = pt.z;
        pt_tmp.r = color[0];
        pt_tmp.g = color[1];
        pt_tmp.b = color[2];
        pc_colored.points.emplace_back(pt_tmp);
    }
}

void MainWindow::pcd2Mat(const pcl::PointCloud<pcl::PointXYZ> &pc) 
{
    int N              = pc.points.size();
	if( N <= 0 ) return ;
	
	//ROS_INFO("pcd2Mat N : %d", N);

	cv::Size worldSize(10.0, 20.0); // width and height of sensor field in m
	cv::Size imageSize(1000, 2000); // corresponding top view image in pixel

	//ROS_INFO("pcd2Mat N : %d", N);
	if( !m_p_qimage )
	{
		m_p_qimage = new QImage(imageSize.width , imageSize.height, QImage::Format_RGB888);

		//ROS_INFO("pcd2Mat 3");
	}
	
	//QImage image{imageSize.width , imageSize.height, QImage::Format_RGB888};
	//image.bits() ;
	//image.setColor(0, qRgb(0, 0, 0)); // Index #0 = Red
	//image.fill(0);

	//ROS_INFO("pcd2Mat 4");
	m_p_qimage->fill(0) ;

	//ROS_INFO("pcd2Mat 5");
	QPainter painter{m_p_qimage};
	QPen pen{Qt::NoBrush, 3.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
	pen.setColor(QColor{0, 255, 0});
	painter.setPen(pen);

	//ROS_INFO("pcd2Mat 6");
	float max_ref=0;
	if( N>0 )
	{
		//check max value ;
		#if 0
		float max_x = 0.0 ;
		float max_y = 0.0 ;
		float max_z = 0.0 ;
		for (int         i = 0; i < N; ++i)
		{
			const auto &pt = pc.points[i];
			if( pt.x > max_x )	max_x = pt.x ;
			if( pt.y > max_y )	max_y = pt.y ;
			if( pt.z > max_z )	max_z = pt.z ;		
		}
		#endif
		
		// plot Lidar points into image
		//#pragma omp parallel for
		const float w2 = imageSize.width / 2 ;
		const float h2 = imageSize.height / 2 ;

		const float w3 = imageSize.width / worldSize.width ;
		const float h3 = imageSize.height / worldSize.height ;
		
		for (int         i = 0; i < N; i+=2 )
		{
			const auto &pt = pc.points[i];
			
			int y = (-pt.y * h3) + h2;
			int x = (-pt.x * w3) + w2;

			if( x >=0 && x <= imageSize.width && y >=0 && y <= imageSize.height )
			{
			//		  cv::circle(topviewImg, cv::Point(x, y), 5, cv::Scalar(0, 0, 255), -1);
				
				// TODO: 
				// 1. Change the color of the Lidar points such that 
				// X=0.0m corresponds to red while X=20.0m is shown as green.
				// 2. Remove all Lidar points on the road surface while preserving 
				// measurements on the obstacles in the scene.
				#if 0
				float zw=pt.z;

				float val=pt.x;
				//float reflectivity=it->r; // reflectivity of lidar point
				//float reflectivity=1.0 ; // reflectivity of lidar point
				float reflectivity=zw ;
				
				float maxval =worldSize.height;
				int red = std::min(255,(int)(255*abs((val-maxval)/maxval)));
				int green = std::min(255,(int)(255*(1-abs((val-maxval)/maxval))));
				int thickness=reflectivity>0.5?-1:1; // -1:ellipse arc outline 1:filled ellipse sector
				//cv::circle(topviewImg,cv::Point(x,y),5,cv::Scalar(0,green,red),thickness);
				//painter.drawEllipse(8, 8, 17, 17);
				#endif
				//pen.setColor(QColor{0, 255, 0});
				painter.drawEllipse(QPointF(x,y), 2, 2);
			}
		}

		#if 0
		//cout<<"max reflectivity: "<<max_ref<<endl;
		// plot distance markers
		float lineSpacing = 2.0; // gap between distance markers
		int nMarkers = floor(worldSize.height / lineSpacing);
		for (size_t i = 0; i < nMarkers; ++i)
		{
			int y = (-(i * lineSpacing) * imageSize.height / worldSize.height) + imageSize.height;
			//cv::line(topviewImg, cv::Point(0, y), cv::Point(imageSize.width, y), cv::Scalar(255, 0, 0));
			//painter.drawLine (0, y, imageSize.width, y);
		}
		#endif
	}

	//ROS_INFO("topviewImg end");

	painter.end();      // close Paint device

	//ROS_INFO("pcd2Mat 7");
	//return m_p_qimage ;
}

void MainWindow::imuCallback(const sensor_msgs::Imu::ConstPtr& msg) 
{
	printf("IMU Info ->\n") ;		

	//ROS_INFO("Imu Seq: [%d]", msg->header.seq);
  	//ROS_INFO("Imu Orientation x: [%f], y: [%f], z: [%f], w: [%f]", msg->orientation.x,msg->orientation.y,msg->orientation.z,msg->orientation.w);

	m_mutex_str_imu.lock() ;
	m_str_imu_anglular_x = std::to_string(msg->angular_velocity.x) ;
	m_str_imu_anglular_y = std::to_string(msg->angular_velocity.y) ;;
	m_str_imu_anglular_z = std::to_string(msg->angular_velocity.z) ;;
	m_str_imu_linear_x = std::to_string(msg->linear_acceleration.x) ;;
	m_str_imu_linear_y = std::to_string(msg->linear_acceleration.y) ;;
	m_str_imu_linear_z = std::to_string(msg->linear_acceleration.z) ;;
	m_mutex_str_imu.unlock() ;
	
	printf("<- IMU Info\n") ;
}

void MainWindow::canCallback(const std_msgs::String::ConstPtr& msg) 
{	
	printf("CAN Info ->\n") ;
	
	//ROS_INFO("Imu Seq: [%d]", msg->header.seq);
  	//ROS_INFO("Imu Orientation x: [%f], y: [%f], z: [%f], w: [%f]", msg->orientation.x,msg->orientation.y,msg->orientation.z,msg->orientation.w);
	m_mutex_str_can_raw.lock() ;
	m_str_can_raw = msg->data ;
	m_mutex_str_can_raw.unlock() ;

	std::vector<std::string> vec_str = StrSplit(m_str_can_raw, ' ')  ;

	std::vector<std::string> vec_str2 ;
	int size_str = vec_str.size() ;
	for( int i=0 ; i<size_str ; i++ )
	{
		if( !vec_str[i].empty() && vec_str[i] != " ") vec_str2.push_back(vec_str[i]) ;

		//
	}

	size_str = vec_str2.size() ;
	//printf("--------- %d\n", size_str) ;
	//printf("data = %s\n", m_str_can_raw.c_str()) ;
	
	std::string id = "" ;
	for( int i=0 ; i<size_str ; i++ )
	{
		//printf("[%d] %s\n", i, vec_str2[i].c_str()) ;

		if( vec_str2[i] == "ID:" && size_str > i+1 )
		{
			id = vec_str2[i+1] ;
		}

		//printf("ID = %s\n", id.c_str()) ;
		if( !id.empty() )
		{
			int data_length = 0 ;
			std::vector<std::string> vec_str_data ;
			if( vec_str2[i] == "DL:" && size_str > i+1 )
			{
				int index_dl = i+1 ;
				data_length = std::stoi( vec_str2[index_dl] ) ;

				if( size_str > index_dl + data_length )
				{
					for( int j=0 ; j<data_length ; j++ )
					{
						vec_str_data.push_back(vec_str2[index_dl+1+j]) ;
					}

					//GPS
					if( id == "18ff2800" ) 	//latitude 위도와 경도
					{
						//하위 4바이트가 위도
						std::string str_latitude = vec_str_data[3] + vec_str_data[2] + vec_str_data[1] + vec_str_data[0] ;
						
						unsigned int x;   
					    std::stringstream ss;
					    ss << std::hex << str_latitude ;
					    ss >> x;

						double latitude = (double)x / 10000000.0 ;
						//printf(">>>>>>>>>>> latitude : %f\n", latitude) ;
						//double latitude = 

						//상위 4바이트가 경도
						std::string str_longitude = vec_str_data[7] + vec_str_data[6] + vec_str_data[5] + vec_str_data[4] ;
						unsigned int x2;   
					    std::stringstream ss2;
					    ss2 << std::hex << str_longitude ;
					    ss2 >> x2;

						double longitude = (double)x2 / 10000000.0 ;

						m_mutex_str_gps.lock();
						m_str_latitude = std::to_string(latitude);
						m_str_longitude = std::to_string(longitude);
						m_mutex_str_gps.unlock();
						
					}
					//GPS 수신감도
					#if 0
					else if( id == "18ff2801" ) 	
					{
						//하위 4바이트가 위도
						std::string str_latitude = vec_str_data[3] + vec_str_data[2] + vec_str_data[1] + vec_str_data[0] ;
						
						unsigned int x;   
					    std::stringstream ss;
					    ss << std::hex << str_latitude ;
					    ss >> x;

						double latitude = (double)x / 10000000.0 ;
						//printf(">>>>>>>>>>> latitude : %f\n", latitude) ;
						//double latitude = 

						//상위 4바이트가 경도
						std::string str_longitude = vec_str_data[7] + vec_str_data[6] + vec_str_data[5] + vec_str_data[4] ;
						unsigned int x2;   
					    std::stringstream ss2;
					    ss2 << std::hex << str_longitude ;
					    ss2 >> x2;

						double longitude = (double)x2 / 10000000.0 ;

						m_mutex_str_gps.lock();
						m_str_latitude = std::to_string(latitude);
						m_str_longitude = std::to_string(longitude);
						m_mutex_str_gps.unlock();
						
					}
					#endif
					else if( id == "0cf00400" )	//label_can_info_engine_speed & torque
					{						
						//4..5바이트가 엔진 스피드
						std::string str_engine_speed = vec_str_data[4] + vec_str_data[3] ;

						unsigned int x;   
						std::stringstream ss;
						ss << std::hex << str_engine_speed ;
						ss >> x;

						double engine_speed = (double)x * 0.125 ;

						m_mutex_str_es.lock();
						m_str_es = std::to_string(engine_speed);
						m_mutex_str_es.unlock();

						//엔진 torque
						std::string str_engine_torque = vec_str_data[2] ;

						unsigned int x2;   
						std::stringstream ss2;
						ss2 << std::hex << str_engine_torque ;
						ss2 >> x2;

						int engine_torque = x2 - 125 ;

						m_mutex_str_et.lock();
						m_str_et = std::to_string(engine_torque);
						m_mutex_str_et.unlock();
						
						//printf(">>>>>>>>>>> latitude : %f\n", latitude) ;
						//double latitude = 
					}
					else if( id == "18fef113" )	//vehicle speed : label_can_info_vehicle_speed
					{						
						//스피드
						std::string str_vehicle_speed = vec_str_data[2] + vec_str_data[1] ;

						unsigned int x;   
						std::stringstream ss;
						ss << std::hex << str_vehicle_speed ;
						ss >> x;

						double vehicle_speed = (double)x * 0.00390625 ;

						m_mutex_str_vs.lock();
						m_str_vs = std::to_string(vehicle_speed);
						m_mutex_str_vs.unlock();
						
						//printf(">>>>>>>>>>> latitude : %f\n", latitude) ;
						//double latitude = 
					}
					else if( id == "1cff5021" )	//히치제어유닛
					{						
						//4..5바이트가 엔진 스피드
						std::string str_hitch_wheel_angle = vec_str_data[1] + vec_str_data[0] ;

						unsigned int x;   
						std::stringstream ss;
						ss << std::hex << str_hitch_wheel_angle ;
						ss >> x;

						double hitch_wheel_angle = (double)x * 4.8875855 ;

						m_mutex_str_hitch_wheel_angle.lock();
						m_str_hitch_wheel_angle = std::to_string(hitch_wheel_angle);
						m_mutex_str_hitch_wheel_angle.unlock();
	
						//printf(">>>>>>>>>>> latitude : %f\n", latitude) ;
						//double latitude = 
					}
				}
			}
		}	
	}
	//printf("-----\n") ;

	printf("<- CAN Info\n") ;
}



//pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));

void MainWindow::pcdCallback(const sensor_msgs::PointCloud2ConstPtr& msg) 
{
	printf("LIDAR Info ->\n") ;
	
	try
	{
		//pcl::PointCloud<pcl::PointXYZ> cloud;
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>());
    	pcl::fromROSMsg (*msg, *cloud);

		//pcl::PointCloud<pcl::PointXYZRGB>::Ptr src_colored(new pcl::PointCloud<pcl::PointXYZRGB>);
		
		//colorize(*cloud, *src_colored, {255, 0, 0});	
		//QImage image = pcd2Mat(*cloud) ;
		pcd2Mat(*cloud) ;
#if 1
		//if( !image.empty() )
		{
			m_mutex_image_lidar.lock() ;
			m_qimage_lidar = m_p_qimage->copy() ; //image.copyTo(m_image_lidar) ;
			m_mutex_image_lidar.unlock() ;
		}
#endif

		#if 0
		if(!src_colored->empty()) 
		{
			ROS_INFO("make image : %d x %d", src_colored->width, src_colored->height);
			
			cv::Mat image = cv::Mat( src_colored->height, src_colored->width, CV_8UC3);
			
			for (std::uint32_t j = 0; j < src_colored->width; j++)
			{
				for (std::uint32_t i = 0; i < src_colored->height; i++)
				{	
					image.at<cv::Vec3b>(i, j) = cv::Vec3b((*src_colored)(j, i).b, (*src_colored)(j, i).g,  (*src_colored)(j, i).r);
				}
			}

			m_mutex_image_lidar.lock() ;
			image.copyTo(m_image_lidar) ;
			m_mutex_image_lidar.unlock() ;
			
		    //viewer->updatePointCloud(src_colored, "sample cloud");
		    //viewer->spinOnce(100);
		}
		#endif
	}
	catch (std::runtime_error e)
	{
		ROS_ERROR_STREAM("Error in converting cloud to pcl message: " << e.what());
	}
	
	
	
	//ROS_INFO("pointcloud recieved");

	printf("<- LIDAR Info\n") ;
}

void MainWindow::ThreadFunction(void)
{
	printf("ThreadFunction \n") ;
#if 1
	int argc=0 ;
    char** argv = nullptr ;
    ros::init(argc,argv, "daq");
	ros::NodeHandle n;

	//zed color
	image_transport::ImageTransport it(n);
  	image_transport::Subscriber sub = it.subscribe("/zed2i/zed_node/rgb_raw/image_raw_color", 10, &MainWindow::Callback_Ros_Zed, this);

	//zed depth
	image_transport::ImageTransport it_zed_depth(n);
	image_transport::Subscriber sub_zed_depth = it_zed_depth.subscribe("/zed2i/zed_node/depth/depth_registered", 10, &MainWindow::Callback_Ros_Zed_Depth, this);

	//lucid rgb
	image_transport::ImageTransport it_lucid_rgb(n);
	image_transport::Subscriber sub_lucid_rgb = it_lucid_rgb.subscribe("/arena_camera_node/image_raw", 10, &MainWindow::Callback_Ros_Lucid_RGB, this);

	//lucid nir
	image_transport::ImageTransport it_lucid_nir(n);
	image_transport::Subscriber sub_lucid_nir = it_lucid_nir.subscribe("/arena_camera_node_2/image_raw", 10, &MainWindow::Callback_Ros_Lucid_NIR, this);

	//lidar
	ros::Subscriber sub_lidar = n.subscribe("/velodyne_points", 1000, &MainWindow::pcdCallback, this);

	//IMU
	ros::Subscriber sub_imu = n.subscribe("/can/imu", 1000, &MainWindow::imuCallback, this);

	//can raw
	ros::Subscriber sub_can_raw = n.subscribe("/can/can_raw", 1000, &MainWindow::canCallback, this);
	
	printf("ThreadFunction - Sub. OK \n") ;
	ros::spin();
#endif	
}

void MainWindow::ThreadFunction_Time(void)
{
	while(1)
	{
		std::time_t rawtime;
	    std::tm* timeinfo;
		
		char buffer [80];
	    std::time(&rawtime);
	    timeinfo = std::localtime(&rawtime);
	    std::strftime(buffer,80,"%Y년 %m월 %d일 - %H시 %M분 %S초",timeinfo);
	    std::string str_now(buffer);

		m_mutex_str_now.lock() ;
		m_str_now = str_now ;
		m_mutex_str_now.unlock() ;

		boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
	} ;
}


void MainWindow::Callback_Ros_Zed( const sensor_msgs::ImageConstPtr& msg ) 
{
	printf("Stereo Color Info ->\n") ;
	m_mutex_image_zed_rgb.lock();
	
	cv_bridge::CvImagePtr cv_ptr;
	try 
	{		
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
		cv_ptr->image.copyTo(m_image_zed_rgb) ;
	}
	catch ( cv_bridge::Exception& e ) 
	{
		ROS_ERROR(" cv_bridge exception: %s", e.what() );
	}

	m_mutex_image_zed_rgb.unlock();
	printf("<- Stereo Color Info\n") ;
}

void MainWindow::Callback_Ros_Zed_Depth( const sensor_msgs::ImageConstPtr& msg ) 
{
	printf("Stereo Depth Info ->\n") ;
	
	m_mutex_image_zed_depth.lock();
	
	cv_bridge::CvImagePtr cv_ptr;
	try 
	{		
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1);

		cv::Mat image ;
		cv_ptr->image.convertTo(image, CV_8UC1);
		cv::equalizeHist(image, image);
		cv::cvtColor(image, m_image_zed_depth, cv::COLOR_GRAY2RGB ) ;
		//cv::applyColorMap(image, m_image_zed_depth, cv::COLORMAP_JET);
		
#if 0
		double min;
		double max;
		cv::minMaxIdx(cv_ptr->image, &min, &max);
		cv::Mat adjMap;
		// Histogram Equalization
		float scale = 255 / (max-min);
		cv_ptr->image.convertTo(adjMap,CV_8UC1, scale, -min*scale); 

		cv::Mat falseColorsMap;
		cv::applyColorMap(adjMap, m_image_zed_depth, cv::COLORMAP_AUTUMN);
		
		//cv::Mat image ;
		//cv::normalize(cv_ptr->image, image, 0, 255, cv::NORM_MINMAX) ;
		//cv::cvtColor(falseColorsMap, m_image_zed_depth, cv::COLOR_BGR2RGB ) ;
#endif		
	}
	catch ( cv_bridge::Exception& e ) 
	{
		ROS_ERROR(" cv_bridge exception: %s", e.what() );
	}

	m_mutex_image_zed_depth.unlock();

	printf("<- Stereo Depth Info\n") ;
}

void MainWindow::Callback_Ros_Lucid_RGB( const sensor_msgs::ImageConstPtr& msg )
{
	printf("RGB Info ->\n") ;
	
	m_mutex_image_lucid_rgb.lock();
	
	cv_bridge::CvImagePtr cv_ptr;
	try 
	{		
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
		cv_ptr->image.copyTo(m_image_camera_rgb) ;
	}
	catch ( cv_bridge::Exception& e ) 
	{
		ROS_ERROR(" cv_bridge exception: %s", e.what() );
	}

	m_mutex_image_lucid_rgb.unlock();
	printf("<- RGB Info\n") ;
}

void MainWindow::Callback_Ros_Lucid_NIR( const sensor_msgs::ImageConstPtr& msg )
{
	printf("NIR Info ->\n") ;
	m_mutex_image_lucid_nir.lock();
	
	cv_bridge::CvImagePtr cv_ptr;
	try 
	{		
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);

		cv::Mat image ;
		cv_ptr->image.copyTo(image) ;
		cv::cvtColor(image, m_image_camera_nir, cv::COLOR_GRAY2RGB ) ;
	}
	catch ( cv_bridge::Exception& e ) 
	{
		ROS_ERROR(" cv_bridge exception: %s", e.what() );
	}

	m_mutex_image_lucid_nir.unlock();
	printf("<- NIR Info\n") ;
}

double MainWindow::Get_Free_Disk_Space(const std::string path)
{
	#if 1
	printf("Get_Free_Disk_Space = %s\n", path.c_str()) ;
	boost::filesystem::space_info tmp = boost::filesystem::space(path);
	//std::filesystem::space_info tmp = std::filesystem::space("/tmp");

	double kbyte = (double)tmp.free / 1024.0 ;
	double mbyte = kbyte / 1024.0 ;
	double gbyte = mbyte / 1024.0 ;
	
    std::cout << "Free space: " << tmp.free << " Byte\n" ; 
	std::cout << "capacity space: " << tmp.capacity << " Byte\n" ; 
	std::cout << "available space: " << tmp.available << " Byte\n" ; 

	return gbyte ;
	#endif

#if 0
	QFileInfo info("/media/rdv/daq1/");
	QString qpath = info.path();
	QStorageInfo storage(qpath);

	 qDebug() << "export root path: " <<storage.rootPath();
   qDebug() << "volume name:" << storage.name();
   qDebug() << "fileSystemType:" << storage.fileSystemType();
   qDebug() << "size:" << storage.bytesTotal()/1000/1000 << "MB";
   qDebug() << "availableSize:" << storage.bytesAvailable()/1000/1000 << "MB";
#endif
	
#if 0 
	QFile usbMemoryInfo;
	QStringList usbMemoryLines;
	QStringList usbMemoryColumns;

	system("df /dev/sda1 > /tmp/usb_usage.info");
	usbMemoryInfo.setFileName( "/tmp/usb_usage.info" );

	usbMemoryInfo.open(QIODevice::ReadOnly);

	QTextStream readData(&usbMemoryInfo);

	while (!readData.atEnd())
	{
	    usbMemoryLines << readData.readLine();
	}

	usbMemoryInfo.close();

	usbMemoryColumns = usbMemoryLines.at(1).split(QRegExp("\\s+"));
	QString available_bytes = usbMemoryColumns.at(3);

	double kbyte = (double)available_bytes.toDouble() ;
	double mbyte = kbyte / 1024.0 ;
	double gbyte = mbyte / 1024.0 ;

	return gbyte ;
#endif
	return 0 ;
}

double MainWindow::Get_Total_Disk_Space(const std::string path)
{
	printf("Get_Free_Disk_Space = %s\n", path.c_str()) ;
	boost::filesystem::space_info tmp = boost::filesystem::space(path);
	//std::filesystem::space_info tmp = std::filesystem::space("/tmp");

	double kbyte = (double)tmp.capacity / 1024.0 ;
	double mbyte = kbyte / 1024.0 ;
	double gbyte = mbyte / 1024.0 ;

	return gbyte ;
}


std::vector<std::string> MainWindow::GetDataList(const std::string str_path)
{
	//장비 이름
	#if 0
	char hostname[1024];
    gethostname(hostname, 1024);
	std::string str_host_name(hostname) ;
	#endif
	//printf("%s\n", __func__) ;
	
	//boost::lock_guard<boost::mutex> lock(m_mutex);
	
	//check folder list
    std::string dir = str_path ;
    std::vector<std::string> paths;

    for( auto& p : boost::filesystem::directory_iterator(dir) )
    {
    	if( boost::filesystem::is_directory(p) )
    	{
        	//paths.push_back(p.path().filename().string());
        	paths.push_back(p.path().string());
    	}
    }

    std::sort(paths.begin(), paths.end());

	return paths ;
}

std::vector<std::string> MainWindow::Del_rosbag(const std::string date)
{
	printf("Del_rosbag : %s\n", date.c_str()) ;
	std::vector<std::string> vec_delete_file ;
	
	char hostname[1024];
    gethostname(hostname, 1024);
	std::string str_host_name(hostname) ;
	
    std::string str_full_path ;
    str_full_path = date + "/" ;

	//int count = 0 ;
	try 
	{
	    if(boost::filesystem::exists(str_full_path.c_str()) && boost::filesystem::is_directory(str_full_path.c_str()))
	    {		
			for (auto const & entry : boost::filesystem::recursive_directory_iterator(str_full_path))
	        {
	        	//이미지 파일을 찾아서 모두 삭제 한다.
	        	if (boost::filesystem::is_regular_file(entry) && (entry.path().extension() == ".bag" || entry.path().extension() == ".active") )
	            {
	            	//remove file
	            	vec_delete_file.push_back(entry.path().string()) ; 
					//count++ ;
	    		}
	        }

#if 0
			count = vec_delete_file.size() ;
			for( int i=0 ; i<count ; i++ )
			{
				boost::filesystem::remove(vec_delete_file[i]);
				printf(" - %s\n", vec_delete_file[i].c_str()) ;
			}
#endif
	    }
	}
	catch (...) 
	{
	}
	

	//printf("Del_rosbag : %d\n", count) ;
	std::sort(vec_delete_file.begin(), vec_delete_file.end());
	
	return vec_delete_file ;
}
