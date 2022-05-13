/*
 * scan.h
 *
 *  Created on: 13 mai 2022
 *      Author: alexis
 */

#ifndef SCAN_H_
#define SCAN_H_
void scan_start(void);
// It's the color to scan
typedef enum {
    COULEUR_AUTRE = 0x00,
    COULEUR_ROUGE = 0x01,
    COULEUR_BLEU = 0x02
} couleur;

//These parameters can be change for different environment
#define SCAN_DIST				200.0f  //[mm]
#define MAX_SCAN_DIST			400.0f  //[mm]
#define MIN_SCAN_DIST			50.0f 	//[mm]
#define ROTATION_SPEED			200 	//[step/s]
#define SPEED_APPROACH			600	//[step/s]
#define TIME_TO_SCAN			500 		//[ms]
#define CM_TO_MM				10

couleur get_color_mode(void);
void set_color_mode(couleur color_mode);
bool get_good_color(void);
void distance_approach(int16_t dist_approach);

#endif /* SCAN_H_ */
