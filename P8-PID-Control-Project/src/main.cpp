#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include <valarray> 
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// For resetting vehicle simulator
void reset_simulator(uWS::WebSocket<uWS::SERVER>& ws, PID& pid) {

  // Reset simulator
  std::string msg("42[\"reset\", {}]");
  ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
}

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;
  
  PID pid;
  
  pid.Init(0.13, 0.0002, 3.0);
  
  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value;
                
          // DEBUG
          //std::cout << "CTE: " << cte << " Steering Value: " << steer_value 
                    //<< std::endl;
        
          pid.counter += 1;
            
          //Update p_error, i_error, d_error values
          pid.UpdateError(cte);
 
          //Get total PID error
          double pid_error = pid.TotalError();
            
          //Add pid_error to total_error variable.
          pid.total_error += pid_error;
            
          json msgJson;
            
          msgJson["steering_angle"] = pid_error;
          msgJson["throttle"] = 0.3;
            
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          
          if (pid.twiddle && pid.counter > pid.max_counter) {
            std::cout << "Total dp: " << pid.dp[0]+pid.dp[1]+pid.dp[2] << std::endl;      
            if (pid.dp[0]+pid.dp[1]+pid.dp[2] > pid.tolerance) {
              int i = pid.i;
              std::cout << "Iteration: " << pid.iterations << " i: " << i << " what: " << pid.what << std::endl;
              assert(i < pid.p.size());
              switch (pid.what) {
              case 1: {
                // Try increasing p
                pid.p[i] += pid.dp[i];
                pid.what = 2;
                break;
              }
              case 2: {
                if (abs(pid.total_error) < pid.best_error) {
                  // Increasing p worked, new best error
                  pid.best_error = abs(pid.total_error);
                  pid.dp[i] *= pid.increase_factor;
                  std::cout << "New error: " << pid.best_error << std::endl;
                  std::cout << "New p: " << pid.p[0] << " " << pid.p[1] << " " << pid.p[2] << std::endl;
                  std::cout << "New dp: " << i << " " << pid.dp[i] << std::endl;
                  // Next i
                  pid.i = (pid.i+1) % (pid.p.size());
                  pid.what = 1;
                } else {
                  // Increasing didn't work, try decreasing p
                  pid.p[i] -= 2 * pid.dp[i];
                  pid.what = 3;
                }
                break;
              }
              case 3: {
                if (abs(pid.total_error) < pid.best_error) {
                  // Decreasing p worked, new best error
                  pid.best_error = abs(pid.total_error);
                  pid.dp[i] *= pid.increase_factor;
                  std::cout << "New error: " << pid.best_error << std::endl;
                  std::cout << "New p: "<< pid.p[0] << " " << pid.p[1] << " " << pid.p[2] << std::endl;
                } else {
                  // Neither increasing nor decreasing p reduced the error, back to the old value
                  pid.p[i] += pid.dp[i];
                  pid.dp[i] *= pid.decrease_factor;
                }
                std::cout << "New dp: " << i << " " << pid.dp[i] << std::endl;
                // Next i
                pid.i = (pid.i+1) % (pid.p.size());
                pid.what = 1;
                break;
              }
              default: { // should never get here
                std::cout << "Error, what: " << pid.what << std::endl;
                assert(0);
                break;
              }
              }
              pid.iterations += 1;  
              reset_simulator(ws, pid);
              pid.counter = 0;
              pid.total_error = 0.0;
            } else {
              pid.twiddle = false;
            }
          }
        }  // end "telemetry" if
        
      } 
      
      else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage
  

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}
