
#ifndef SCAN_H_
#define SCAN_H_
void scan_start(void);
// It's the color to scan
typedef enum {
    COULEUR_AUTRE = 0x00,
    COULEUR_ROUGE = 0x01,
    COULEUR_BLEU = 0x02,
	COULEUR_NOIR = 0x03
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
//Permet de s?lectionner la couleur ? trouver
void set_color_mode(couleur color_mode);
// Permet de savoir si on a trouv? la bonne couleur
bool get_good_color(void);
//Fonction permettant de se rapprocher d'une certaine distance d'un objet
void distance_approach(int16_t dist_approach);
// Permet de stopper la fonction de scan si besoin
void stop_scan(void);

#endif /* SCAN_H_ */
