#include <Rcpp.h>
#include <cmath>

using namespace Rcpp;

double deg2rad(double deg) {
  return deg * M_PI / 180;
}

const double a = 6378137.0;       

double distance(double lat1, double lon1, double lat2, double lon2) {
  double phi1 = deg2rad(lat1);
  double phi2 = deg2rad(lat2);
  double delta_phi = deg2rad(lat2 - lat1);
  double delta_lambda = deg2rad(lon2 - lon1);
  
  double a_ = sin(delta_phi / 2) * sin(delta_phi / 2) +
    cos(phi1) * cos(phi2) *
    sin(delta_lambda / 2) * sin(delta_lambda / 2);
  double c = 2 * atan2(sqrt(a_), sqrt(1 - a_));
  
  double distance = a * c;
  return distance;
}

// [[Rcpp::export]]
DataFrame addDistanceColumn(DataFrame df, 
                            std::string lat1_col, 
                            std::string lon1_col,
                            std::string lat2_col,
                            std::string lon2_col) {
  int n = df.nrow();
  NumericVector lat1 = df[lat1_col];
  NumericVector lon1 = df[lon1_col];
  NumericVector lat2 = df[lat2_col];
  NumericVector lon2 = df[lon2_col];
  NumericVector distances(n);
  
  for (int i = 0; i < n; i++) {
    distances[i] = distance(lat1[i], lon1[i], lat2[i], lon2[i]);
  }
  
  df["distance"] = distances;
  return df;
}
