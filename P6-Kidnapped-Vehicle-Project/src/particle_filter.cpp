/**
 * particle_filter.cpp
 *
 * Created on: Dec 12, 2016
 * Author: Tiffany Huang
 */

#include "particle_filter.h"

#include <math.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "helper_functions.h"

using std::string;
using std::vector;

void ParticleFilter::init(double x, double y, double theta, double std[]) {
  /**
   * TODO: Set the number of particles. Initialize all particles to 
   *   first position (based on estimates of x, y, theta and their uncertainties
   *   from GPS) and all weights to 1. 
   * TODO: Add random Gaussian noise to each particle.
   * NOTE: Consult particle_filter.h for more information about this method 
   *   (and others in this file).
   */
  num_particles = 50;  // TODO: Set the number of particles
  std::default_random_engine gen;
  
  // Creating a normal (Gaussian) distribution for sensor noise.
  std::normal_distribution<double> dist_x_init(0, std[0]);
  std::normal_distribution<double> dist_y_init(0, std[1]);  
  std::normal_distribution<double> dist_theta_init(0, std[2]);
  
  // Initialize particles to first position
  for (int i = 0; i < num_particles; i++) {
    Particle current_particle;
    current_particle.id = i;
    current_particle.x = x;
    current_particle.y = y;
    current_particle.theta = theta;
    current_particle.weight = 1.0;
    
  // Add random Gaussian noise to each particle
    current_particle.x += dist_x_init(gen);
    current_particle.y += dist_y_init(gen);
    current_particle.theta += dist_theta_init(gen);
    
    particles.push_back(current_particle);
    weights.push_back(current_particle.weight);
  }
  is_initialized = true;
}

void ParticleFilter::prediction(double delta_t, double std_pos[], 
                                double velocity, double yaw_rate) {
  /**
   * TODO: Add measurements to each particle and add random Gaussian noise.
   * NOTE: When adding noise you may find std::normal_distribution 
   *   and std::default_random_engine useful.
   *  http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
   *  http://www.cplusplus.com/reference/random/default_random_engine/
   */
  std::default_random_engine gen;
  
  // Define normal distributions for sensor noise
  std::normal_distribution<double> dist_x(0, std_pos[0]);
  std::normal_distribution<double> dist_y(0, std_pos[1]);
  std::normal_distribution<double> dist_theta(0, std_pos[2]);
  
  for (int i = 0; i < num_particles; i++) {
    if (fabs(yaw_rate) < 0.00001) {  
      particles[i].x += velocity * delta_t * cos(particles[i].theta);
      particles[i].y += velocity * delta_t * sin(particles[i].theta);
    } 
    else {
      particles[i].x += velocity / yaw_rate * (sin(particles[i].theta + yaw_rate*delta_t) - sin(particles[i].theta));
      particles[i].y += velocity / yaw_rate * (cos(particles[i].theta) - cos(particles[i].theta + yaw_rate*delta_t));
      particles[i].theta += yaw_rate * delta_t;
    }
    
    // // Add random Gaussian noise to each particle
    particles[i].x += dist_x(gen);
    particles[i].y += dist_y(gen);
    particles[i].theta += dist_y(gen);
  }
}

void ParticleFilter::dataAssociation(vector<LandmarkObs> predicted, 
                                     vector<LandmarkObs>& observations) {
  /**
   * TODO: Find the predicted measurement that is closest to each 
   *   observed measurement and assign the observed measurement to this 
   *   particular landmark.
   * NOTE: this method will NOT be called by the grading code. But you will 
   *   probably find it useful to implement this method and use it as a helper 
   *   during the updateWeights phase.
   */
  for (unsigned int i = 0; i < observations.size(); i++) {
    double lowest_dist = std::numeric_limits<double>::max();
	int closest_landmark_id = -1;
	double obs_x = observations[i].x;
	double obs_y = observations[i].y;
    
    for (unsigned int j = 0; j < predicted.size(); j++) {
      double predict_x = predicted[j].x;
      double predict_y = predicted[j].y;
      int predict_id = predicted[j].id;
      double current_dist = dist(obs_x, obs_y, predict_x, predict_y);
      
      if (current_dist < lowest_dist) {
        lowest_dist = current_dist;
        closest_landmark_id = predict_id;
      }
    }
    observations[i].id = closest_landmark_id;
  }
}

void ParticleFilter::updateWeights(double sensor_range, double std_landmark[], 
                                   const vector<LandmarkObs> &observations, 
                                   const Map &map_landmarks) {
  /**
   * TODO: Update the weights of each particle using a mult-variate Gaussian 
   *   distribution. You can read more about this distribution here: 
   *   https://en.wikipedia.org/wiki/Multivariate_normal_distribution
   * NOTE: The observations are given in the VEHICLE'S coordinate system. 
   *   Your particles are located according to the MAP'S coordinate system. 
   *   You will need to transform between the two systems. Keep in mind that
   *   this transformation requires both rotation AND translation (but no scaling).
   *   The following is a good resource for the theory:
   *   https://www.willamette.edu/~gorr/classes/GeneralGraphics/Transforms/transforms2d.htm
   *   and the following is a good resource for the actual equation to implement
   *   (look at equation 3.33) http://planning.cs.uiuc.edu/node99.html
   */
  //Initialize variable that normalizes weights of all particles.
  
  double weight_normalizer = 0.0;
  
  //Iterating through each particle
  for (int i = 0; i < num_particles; i++) {
    
    //Getting x,y coordinates of particles
    double particle_x = particles[i].x;
    double particle_y = particles[i].y;
    double particle_theta = particles[i].theta;
    
    //Vector containing observations transformed to map co-ordinates
    vector<LandmarkObs> transformed_observations;
    
    //Transform observations from vehicle's co-ordinates to map co-ordinates.
    for (unsigned int j = 0; j < observations.size(); j++) {
      int observations_id = observations[j].id;
      double observations_x = particle_x + cos(particle_theta) * observations[j].x - sin(particle_theta) * observations[j].y;
      double observations_y = particle_y + sin(particle_theta) * observations[j].x + cos(particle_theta) * observations[j].y;
      transformed_observations.push_back(LandmarkObs{observations_id, observations_x, observations_y});
    }
    
    //Vector containing detected sensor_range landmarks from each particle.
    vector<LandmarkObs> predicted_landmarks;
    
    //Filtering map landmarks to excluse landmarks that are not within bounds of the sensor_range.
    for (unsigned int k = 0; k < map_landmarks.landmark_list.size(); k++) {
      Map::single_landmark_s current_landmark = map_landmarks.landmark_list[k];
      if ((fabs((particle_x - current_landmark.x_f)) <= sensor_range) && (fabs((particle_y - current_landmark.y_f)) <= sensor_range)) {
        predicted_landmarks.push_back(LandmarkObs {current_landmark.id_i, current_landmark.x_f, current_landmark.y_f});
      }
    }
    
    //Associate observations to predicted landmarks using nearest neighbor algorithm.
    //Associate observations with predicted landmarks
    dataAssociation(predicted_landmarks, transformed_observations);
    
    //Calculate the weight of each particle using Multivariate Gaussian distribution.
    //Reset the weight of particle to 1.0
    particles[i].weight = 1.0;
    
    double sig_x = std_landmark[0];
    double sig_y = std_landmark[1];
    double sig_x_2 = pow(sig_x, 2);
    double sig_y_2 = pow(sig_y, 2);
    double normalizer = (1.0/(2.0 * M_PI * sig_x * sig_y));
    
    for (unsigned int l = 0; l < transformed_observations.size(); l++) {
      double trans_obs_x = transformed_observations[l].x;
      double trans_obs_y = transformed_observations[l].y;
      double trans_obs_id = transformed_observations[l].id;
      double multi_prob = 1.0;
      
      for (unsigned int m = 0; m < predicted_landmarks.size(); m++) {
        double pred_landmark_x = predicted_landmarks[m].x;
        double pred_landmark_y = predicted_landmarks[m].y;
        double pred_landmark_id = predicted_landmarks[m].id;
        
        if (trans_obs_id == pred_landmark_id) {
          multi_prob = normalizer * exp(-1.0 * ((pow((trans_obs_x - pred_landmark_x), 2)/(2.0 * sig_x_2)) + (pow((trans_obs_y - pred_landmark_y), 2)/(2.0 * sig_y_2))));
          particles[i].weight *= multi_prob;
        }
      }
    }
    weight_normalizer += particles[i].weight;
  }
  
  //Normalize the weights of all particles
  for (unsigned int i = 0; i < particles.size(); i++) {
    particles[i].weight /= weight_normalizer;
    weights[i] = particles[i].weight;
  }
}

void ParticleFilter::resample() {
  /**
   * TODO: Resample particles with replacement with probability proportional 
   *   to their weight. 
   * NOTE: You may find std::discrete_distribution helpful here.
   *   http://en.cppreference.com/w/cpp/numeric/random/discrete_distribution
   */
  
  std::default_random_engine gen;
  
  //Initialize new vector
  vector<Particle> new_particles;
  
  //Get all of the current weights
  vector<double> weights;
  
  for (int i = 0; i < num_particles; i++) {
    weights.push_back(particles[i].weight);
  }
  
  //Generate random starting index for resampling wheel
  std::uniform_int_distribution<int> uniintdist(0, num_particles-1);
  auto index = uniintdist(gen);
  
  //Retrieve max weight
  double max_weight = *max_element(weights.begin(), weights.end());
  
  //Uniform random distribution [0.0, max_weight)
  std::uniform_real_distribution<double> unirealdist(0.0, max_weight);
  
  //Initialize beta variable
  double beta = 0.0;
  
  //Iterate resampling wheel
  for (int i = 0; i < num_particles; i++) {
    beta += unirealdist(gen) * 2.0;
    while (beta > weights[index]) {
      beta -= weights[index];
      index = (index + 1) % num_particles;
    }
    new_particles.push_back(particles[index]);
  }
  
  particles = new_particles;
}

void ParticleFilter::SetAssociations(Particle& particle, 
                                     const vector<int>& associations, 
                                     const vector<double>& sense_x, 
                                     const vector<double>& sense_y) {
  // particle: the particle to which assign each listed association, 
  //   and association's (x,y) world coordinates mapping
  // associations: The landmark id that goes along with each listed association
  // sense_x: the associations x mapping already converted to world coordinates
  // sense_y: the associations y mapping already converted to world coordinates
  particle.associations= associations;
  particle.sense_x = sense_x;
  particle.sense_y = sense_y;
}

string ParticleFilter::getAssociations(Particle best) {
  vector<int> v = best.associations;
  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<int>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}

string ParticleFilter::getSenseCoord(Particle best, string coord) {
  vector<double> v;

  if (coord == "X") {
    v = best.sense_x;
  } else {
    v = best.sense_y;
  }

  std::stringstream ss;
  copy(v.begin(), v.end(), std::ostream_iterator<float>(ss, " "));
  string s = ss.str();
  s = s.substr(0, s.length()-1);  // get rid of the trailing space
  return s;
}