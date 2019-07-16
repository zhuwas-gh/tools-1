/*
** navstruct.h for tools in /home/fwt/program/myprogram/wsl_program/mscnav/submodules/tools/include
**
** Made by little fang
** Login   <fangwentao>
**
** Started on  Wed Jul 3 14:38:27 2019 little fang
** Last update Wed Jul 16 下午10:54:58 2019 little fang
*/

#ifndef NAVSTRUCT_H_
#define NAVSTRUCT_H_
#include "stdio.h"
#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <Eigen/Dense>

#include "navtime.h"

namespace utiltool
{

inline void navsleep(int _milliseconds)
{
  std::chrono::milliseconds msecond(_milliseconds);
  std::this_thread::sleep_for(msecond);
}

inline void navexit(const std::string &info = "Exitting...")
{
  std::cout << info << std::endl;
  navsleep(1000);
  exit(0);
}

enum DataType
{
  DATAUNKOWN = -1,
  IMUDATA,
  GNSSDATA,
  ODODATA,
  FEATUREDATA,
  RESULTDATA
};

enum GnssType
{
  GNSSUNKOWN = -1,
  GNSSSPP,
  GNSSPPP,
  GNSSFIXED,
  GNSSRTD,
  GNSSRAW
};

class BaseData
{
public:
  BaseData(const NavTime &time) : t0_(time) {}
  BaseData() {}
  virtual ~BaseData() {}

public:
  using Ptr = BaseData *;
  NavTime get_time() const { return t0_; }
  NavTime set_time(const NavTime &time) { t0_ = time; }
  DataType get_type() const { return data_type_; }
  virtual std::string to_string() = 0;

protected:
  DataType data_type_ = DATAUNKOWN;
  NavTime t0_;
  NavTime t1_; //bak receive time
};

class GnssData : public BaseData
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

public:
  GnssData(const NavTime &time) : BaseData(time)
  {
    data_type_ = GNSSDATA;
  }
  GnssData() : BaseData()
  {
    data_type_ = GNSSDATA;
  }
  virtual ~GnssData() {}

public:
  virtual std::string to_string() override
  {
    std::ostringstream osstream;
    osstream << t0_.Time2String() << "\t";
    osstream << std::fixed << std::setprecision(9) << std::setw(13) << pos_.transpose();
    osstream << std::fixed << std::setprecision(4) << std::setw(7) << vel_.transpose();
    osstream << std::fixed << std::setprecision(4) << std::setw(7) << pos_std_.transpose();
    osstream << std::fixed << std::setprecision(4) << std::setw(7) << vel_std_.transpose();
    return osstream.str();
  }

public:
  Eigen::Vector3d pos_{0, 0, 0};
  Eigen::Vector3d vel_{0, 0, 0};
  Eigen::Vector3d pos_std_{0, 0, 0};
  Eigen::Vector3d vel_std_{0, 0, 0};
  GnssType gnss_type_ = GNSSUNKOWN;
};

class ImuData : public BaseData
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

public:
  ImuData(const NavTime &time) : BaseData(time)
  {
    data_type_ = IMUDATA;
  }
  ImuData() : BaseData()
  {
    data_type_ = IMUDATA;
  }
  virtual ~ImuData() {}
  virtual std::string to_string() override
  {
    std::ostringstream osstream;
    osstream << t0_.Time2String() << "\t";
    osstream << std::fixed << std::setprecision(6) << std::setw(14) << gyro_.transpose();
    osstream << std::fixed << std::setprecision(6) << std::setw(14) << acce_.transpose();
    return osstream.str();
  }

public:
  Eigen::Vector3d gyro_{0, 0, 0};
  Eigen::Vector3d acce_{0, 0, 0};
};

class OdoData : public BaseData
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

public:
  OdoData(const NavTime &time) : BaseData(time)
  {
    data_type_ = ODODATA;
  }
  OdoData() : BaseData()
  {
    data_type_ = ODODATA;
  }
  virtual ~OdoData() {}

public:
  virtual std::string to_string() override
  {
    std::ostringstream osstream;
    osstream << t0_.Time2String() << "\t";
    osstream << std::fixed << std::setprecision(5) << std::setw(10) << odo_vel.transpose();
    return osstream.str();
  }

public:
  Eigen::Vector4d odo_vel{0, 0, 0, 0};
};

struct NavInfo
{
  NavTime time_;
  Eigen::Quaterniond quat_{0, 0, 0, 0};
  Eigen::Vector3d pos_{0, 0, 0};
  Eigen::Vector3d vel_{0, 0, 0};
  Eigen::Vector3d att_{0, 0, 0};
  Eigen::Vector3d wibb_{0, 0, 0};
  Eigen::Vector3d fibb_{0, 0, 0};
  Eigen::Vector3d pos_std_{0, 0, 0};
  Eigen::Vector3d vel_std_{0, 0, 0};
  Eigen::Vector3d att_std_{0, 0, 0};
  Eigen::Vector3d gyro_bias_{0, 0, 0};
  Eigen::Vector3d acce_bias_{0, 0, 0};
  Eigen::Vector3d gyro_scale_{0, 0, 0};
  Eigen::Vector3d acce_scale_{0, 0, 0};
  Eigen::Vector3d leverarm_{0, 0, 0};
  long long int result_type_;
};

} // namespace utiltool

#endif /* !NAVSTRUCT_H_ */
