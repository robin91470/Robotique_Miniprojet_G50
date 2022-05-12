/*
 * scan.h
 *
 *  Created on: 12 mai. 2022
 *      Author: alexis
 */

#ifndef SCAN_H_
#define SCAN_H_
void scan_start(void);
typedef enum {
    COULEUR_AUTRE = 0x00,
    COULEUR_ROUGE = 0x01,
    COULEUR_BLEU = 0x02
} couleur;
#define SCAN_DIST				250.0f  //[mm]
#define MAX_SCAN_DIST			600.0f  //[mm]
#define ROTATION_SPEED			700 	//[step/s]
#define SPEED_APPROACH			1000	//[step/s]

couleur get_color_mode(void);
void set_color_mode(couleur color_mode);
bool get_good_color(void);

#endif /* SCAN_H_ */
