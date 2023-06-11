#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <fcntl.h>     // Needed for SPI port
#include <sys/ioctl.h> // Needed for SPI port
#include <errno.h>     // linux errors
#include <unistd.h>    // sleep()
#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>    // for  uint8_t,  uint16_t etc
#include <linux/spi/spidev.h>
#include <cstring>
#include <iostream>

#include <sys/statvfs.h>


#include <QMainWindow>

#include <QtDebug>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QDesktopServices>
#include <QUrl>
#include <QKeyEvent>
#include <QtMultimedia/QSound>
#include <QList>
#include <QNetworkInterface>

#include <QProcess>
#include <QPainter>
#include <QStorageInfo> 

//#include <filesystem>


#include "ros/ros.h"
#include <ros/console.h>
#include "rosbag/bag.h"
#include <rosbag/recorder.h>
#include "sensor_msgs/Imu.h"


#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "cmat2qimage.h"

#include <sstream>
#include <algorithm>
#include <functional>
#include <sys/reboot.h>

#include "opencv2/opencv.hpp"
//#include "opencv2/freetype.hpp"
#include "opencv2/photo/cuda.hpp"
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/freetype.hpp"

#include <regex>


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

#include <pcl/common/common_headers.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>

#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/extract_indices.h>
#include <pcl_ros/transforms.h> 

#include <pcl/visualization/pcl_visualizer.h>
#include <vtkRenderWindow.h>


#include "sensor_msgs/PointCloud2.h"

#include "crdv_qthread.h"

#include "crdv_get_git_info.h"


#include "dialoginfo.h" // 추가

#if 0
#define BOOST_LOG_DYN_LINK 1
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/move/utility.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/support/date_time.hpp>
#endif

#include <omp.h>

#define PATH_USB_DEV		"/dev/sda1"
#define PATH_USB_DISK_BASE	"/media/rdv/daq/"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
	void updatePicture(cv::Mat &image);

	//ROS callback
	void Callback_Ros_Zed( const sensor_msgs::ImageConstPtr& msg ) ;
	void Callback_Ros_Zed2( const sensor_msgs::ImageConstPtr& msg ) ;
	void Callback_Ros_Zed_Depth( const sensor_msgs::ImageConstPtr& msg ) ;
	void Callback_Ros_Zed_IMU(const sensor_msgs::Imu::ConstPtr& msg)  ;

	void Callback_Ros_Lucid_RGB( const sensor_msgs::ImageConstPtr& msg ) ;
	void Callback_Ros_Lucid_NIR( const sensor_msgs::ImageConstPtr& msg ) ;

	void pcdCallback(const sensor_msgs::PointCloud2ConstPtr& msg) ;
	void imuCallback(const sensor_msgs::Imu::ConstPtr& msg)  ;	
	void canCallback(const std_msgs::String::ConstPtr& msg)  ;
		
	void colorize(const pcl::PointCloud<pcl::PointXYZ> &pc,
              pcl::PointCloud<pcl::PointXYZRGB> &pc_colored,
              const std::vector<int> &color) ;

	void pcd2Mat(const pcl::PointCloud<pcl::PointXYZ> &pc) ;
	
#if 0
protected:
    void keyPressEvent(QKeyEvent *event);
#endif

private:
	CRdvThread *m_rdv_thread;

    boost::thread* p_thread ;
    void ThreadFunction();

	boost::thread* p_thread_time ;
    void ThreadFunction_Time();

	cv::Mat m_display_image ;

	//image list
	cv::Mat m_image_zed_rgb ;
	cv::Mat m_image_zed_rgb2 ;
	cv::Mat m_image_zed_depth ;
	cv::Mat m_image_lidar ;
	QImage m_qimage_lidar ;
	cv::Mat m_image_camera_rgb ;
	cv::Mat m_image_camera_nir ;

	//image mutex
	boost::mutex m_mutex_image_zed_rgb;
	boost::mutex m_mutex_image_zed_rgb2;
	boost::mutex m_mutex_image_zed_depth;
	boost::mutex m_mutex_image_lucid_rgb ;
	boost::mutex m_mutex_image_lucid_nir ;
	boost::mutex m_mutex_image_lidar ;
	
	CMat2QImage m_cls_mat_2_qimage ;

	//time
	boost::mutex m_mutex_str_now;
	std::string m_str_now ;

	boost::mutex m_mutex_str_info;
	std::string m_str_info ;

	//font
	//cv::Ptr<cv::freetype::FreeType2> m_ft2 ;
	//cv::Ptr<cv::freetype::FreeType2> m_ft2_info ;
	double Get_Free_Disk_Space(const std::string path) ;
	double Get_Total_Disk_Space(const std::string path) ;
	
	std::vector<std::string> GetDataList(const std::string str_path) ;
	std::vector<std::string> Del_rosbag(const std::string date) ;
	
	QImage *m_p_qimage;
	unsigned char *m_p_buffer ;
	unsigned int m_i_buffer_size ;

	//usb disk
	boost::mutex m_mutex_str_bag_path;
	std::string m_str_bag_path;

	//imu data
	boost::mutex m_mutex_str_imu;
	std::string m_str_imu_anglular_x;
	std::string m_str_imu_anglular_y;
	std::string m_str_imu_anglular_z;
	std::string m_str_imu_linear_x;
	std::string m_str_imu_linear_y;
	std::string m_str_imu_linear_z;

	boost::mutex m_mutex_str_zed_imu;
	std::string m_str_zed_imu_anglular_x;
	std::string m_str_zed_imu_anglular_y;
	std::string m_str_zed_imu_anglular_z;
	std::string m_str_zed_imu_linear_x;
	std::string m_str_zed_imu_linear_y;
	std::string m_str_zed_imu_linear_z;

	
	//GPS data
	boost::mutex m_mutex_str_gps;
	std::string m_str_latitude;
	std::string m_str_longitude;

	//engine speed
	boost::mutex m_mutex_str_es;
	std::string m_str_es;

	boost::mutex m_mutex_str_et;
	std::string m_str_et;

	//vehible speed
	boost::mutex m_mutex_str_vs;
	std::string m_str_vs;

	//히치제어유닛
	boost::mutex m_mutex_str_hitch_wheel_angle;
	std::string m_str_hitch_wheel_angle;
	

	//can data
	boost::mutex m_mutex_str_can_raw;
	std::string m_str_can_raw;

	std::vector<std::string> StrSplit(std::string str, char Delimiter)  ;
	
private slots:
	void Update(void);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
