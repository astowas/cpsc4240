#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gps.h>
#include <math.h>

int main(){
    int rc;
    struct gps_data_t gps;

    if((rc = gps_open("localhost", "2947", &gps)) == -1){
        printf("couldn't open\n");
        return EXIT_FAILURE;
    }

    gps_stream(&gps, WATCH_ENABLE | WATCH_JSON, NULL);

    while(1){

        if (gps_waiting(&gps, 20000000)) {
            if (gps_read(&gps) == -1) {
                printf("couldn't read\n");
                return EXIT_FAILURE;
            } else {
                if ((gps.status == STATUS_FIX) &&
                    (gps.fix.mode == MODE_2D || gps.fix.mode == MODE_3D) &&
                    !isnan(gps.fix.latitude) &&
                    !isnan(gps.fix.longitude)) {
                        //gettimeofday(&tv, NULL); EDIT: tv.tv_sec isn't actually the timestamp!
                        printf("latitude: %f, longitude: %f, speed: %f, timestamp: %lf\n", gps.fix.latitude, gps.fix.longitude, gps.fix.speed, gps.fix.time); //EDIT: Replaced tv.tv_sec with gps_data.fix.time
                } else {
                    printf("no GPS data available. status=%d\n", gps.status);
                }
            }
        }

        sleep(3);

    }

    gps_stream(&gps, WATCH_DISABLE, NULL);
    gps_close(&gps);
}

/*#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <gps.h>

int main(void){

   struct gps_data_t gps_data;
   int ret = 0;

   ret = gps_open("localhost", "5056", &gps_data);

   gps_stream(&gps_data, WATCH_ENABLE | WATCH_JSON, NULL);

   if (gps_waiting (&gps_data, 500)) {


       if (gps_read (&gps_data) == -1) {

           fprintf(stdout, "Error #3: No GPS data available! Retrying ...\n");

       }
       else {

           fprintf(stdout, "GPS-Data: Mode: %d, Latitude: %f, Longitude: %f, Altitude: %.1f, Timestamp: %ld\n", gps_data.fix.mode, gps_data.fix.latitude, gps_data.fix.longitude, gps_data.fix.altitude);

       }
   }

   /* When you are done...
   gps_stream(&gps_data, WATCH_DISABLE, NULL);
   gps_close (&gps_data);

   return 0;
} */
