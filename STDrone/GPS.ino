void GPS_calculate(){
  if(lattitude >= 0) north_location = true;
  else north_location = false;

  if(longitude >=0) east_location = false;
  else east_location = true;

  if (lat_gps_previous == 0 && lon_gps_previous == 0) {                            //If this is the first time the GPS code is used.
    lat_gps_previous = lattitude;                                            //Set the lat_gps_previous variable to the lat_gps_actual variable.
    lon_gps_previous = longitude;                                           //Set the lon_gps_previous variable to the lon_gps_actual variable.
  }

  
}
