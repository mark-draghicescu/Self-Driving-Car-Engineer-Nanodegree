#include "PID.h"

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {
  twiddle = false;
  counter = 0;
  max_counter = 100;
  total_error = 0.0;
  best_error = 10000.00;
  p = {0.1, 0.01, 1.0};          
  dp = {0.1, 0.01, 1.0};
  tolerance = 0.05;
  iterations = 0;
  what = 1;
  i = 0;
  increase_factor = 1.2;
  decrease_factor = 0.8;
}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
 
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on CTE.
   */
  //Update d_error or differential error. This is the difference between the current CTE and the CTE at the last time step.
  d_error = (cte - p_error);
    
  //p_error is replaced with the new CTE
  p_error = cte;
  
  //The i_error is the integrated crosstrack error. This is the sum of all previous CTE's.
  i_error += cte;
}

double PID::TotalError() {
  
  double output;
  
  output = -Kp * p_error - Kd * d_error - Ki * i_error;
  
  //Limit between -1 and 1.
  if (output > 1.0) {
    output = 1.0;
  }
  else if (output < -1.0) {
    output = -1.0;
  }
  
  return output;
}