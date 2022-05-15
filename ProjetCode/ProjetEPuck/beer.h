
#ifndef BEER_H_
#define BEER_H_

#define BAR_DIST		25.0f//[mm]
#define BEER_WAIT 		5//[s] time to wait to be serve

void beer_start(void);
// Permet de savoir si la tache est finie
bool get_beer_served(void);
void stop_beer(void);



#endif /* BEER_H_ */
