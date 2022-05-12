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

#endif /* SCAN_H_ */
