#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gps.h>
#include <math.h>

int getCoordinates(){
    int rc;
    struct gps_data_t gps;

    if((rc = gps_open("localhost", "2947", &gps)) == -1){
        printf("couldn't open\n");
        return 0;
    }

    gps_stream(&gps, WATCH_ENABLE | WATCH_JSON, NULL);

    while(1){

        if (gps_waiting(&gps, 20000000)) {
            if (gps_read(&gps) == -1) {
                printf("couldn't read\n");
                return 0;
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

    return 1;
}

int getWireless(){
    char sig[5];
    int sigint, rc;

    FILE* pp = popen("cat /proc/net/wireless | grep wlp3s0", "r");

    rc = fscanf(pp, "%*s %*s %*s %s", sig);

    if(rc==EOF) return(0);

    int i;
    for(i = 0; sig[i] != '\0'; i++){
        if(sig[i] == '.') sig[i] = '\0';
    }

    sigint = atoi(sig);

    pclose(pp);

    return(sigint);
}

int main(){
    int rc;

    //rc = getCoordinates();


    while(1==1){
        rc = getWireless();
        fprintf(stdout, "%d\n", rc);
        sleep(1);
    }


}
