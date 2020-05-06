#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

#define PI 3.14159265
#define TAU 2 * PI

/* 
 * Please note that the Eigen library does not initialize 
 *   VectorXd or MatrixXd objects with zeros upon creation.
 */

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  //predict the state
  x_ = F_*x_;
  MatrixXd Ft_ = F_.transpose();
  P_ = F_*P_*Ft_+Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  //update the state by using Kalman Filter equations
  VectorXd y_ = z - (H_ * x_);
  MatrixXd Ht_ = H_.transpose();
  MatrixXd S_ = H_ * P_ * Ht_ + R_;
  MatrixXd Si_ = S_.inverse();
  MatrixXd K_ =  P_ * Ht_ * Si_;
  
  //new state
  long x_size = x_.size();
  MatrixXd I_ = MatrixXd::Identity(x_size, x_size);
  
  x_ = x_ + (K_ * y_);
  P_ = (I_ - K_ * H_) * P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
   //update the state by using Extended Kalman Filter equations
  float px = x_(0);
  float py = x_(1);
  float vx = x_(2);
  float vy = x_(3);
   
  float rho = sqrt(px*px+py*py);
  
  //avoid division by zero
  if (rho < 0.00001) {
    px += 0.001;
    py += 0.001;
    rho = sqrt(px*px+py*py);
  }
  float theta = atan2(py, px);
  float rho_dot = (px*vx+py*vy) / rho;
  
  VectorXd z_pred = VectorXd(3);
  z_pred << rho, theta, rho_dot;
  
  VectorXd y_ = z - z_pred;
  
  //Normalize angle to within -pi and pi if needed
  while (y_(1) > PI || y_(1) < -PI)
  {
    if (y_(1) > PI)
    {
      y_(1) -= TAU;
    }
    else if (y_(1) < -PI)
    {
      y_(1) += TAU;
    }
  }
  
  MatrixXd Ht_ = H_.transpose();
  MatrixXd S_ = H_ * P_ * Ht_ + R_;
  MatrixXd Si_ = S_.inverse();
  MatrixXd K_ =  P_ * Ht_ * Si_;
  
  //new state
  long x_size = x_.size();
  MatrixXd I_ = MatrixXd::Identity(x_size, x_size);
  
  x_ = x_ + (K_ * y_);
  P_ = (I_ - K_ * H_) * P_;
}