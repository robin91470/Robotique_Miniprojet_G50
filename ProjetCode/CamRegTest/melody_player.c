#include <ch.h>
#include <hal.h>
#include <audio/play_sound_file.h>
#include <audio/play_melody.h>
#include <selector.h>
#include <melody_player.h>

static music music_to_play = NO_SONG;


//undertale Megalovania melody from Manel Navola partition
static const uint16_t megalovania_melody[] = {
  NOTE_D4, NOTE_D4, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0,
  NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,

  NOTE_C4, NOTE_C4, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0,
  NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,

  NOTE_B3, NOTE_B3, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0,
  NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,

  NOTE_AS3, NOTE_AS3, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0,
  NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
  //repeat of first part
  NOTE_D4, NOTE_D4, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0,
  NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,

  NOTE_C4, NOTE_C4, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0,
  NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,

  NOTE_B3, NOTE_B3, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0,
  NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,

  NOTE_AS3, NOTE_AS3, NOTE_D5, NOTE_A4, 0, NOTE_GS4, 0,
  NOTE_G4, 0, NOTE_F4, NOTE_D4, NOTE_F4, NOTE_G4,
  //end of repeat
  NOTE_E5, NOTE_F5, NOTE_F5, NOTE_F5, 0, NOTE_F5, 0,
  NOTE_E5, NOTE_F5, NOTE_D5, 0, NOTE_D5,

  NOTE_F5, NOTE_F5, NOTE_F5, 0, NOTE_G5, 0, NOTE_GS5,
  NOTE_G5, NOTE_GS5, NOTE_G5, NOTE_D5, NOTE_F5, NOTE_G5, 0,

  NOTE_F5, NOTE_F5, NOTE_F5, 0, NOTE_G5, 0, NOTE_GS5,
  0, NOTE_A5, 0, NOTE_C6, 0, NOTE_A5,

  NOTE_D6, NOTE_D6, NOTE_D6, NOTE_A5, NOTE_D6, NOTE_C6, NOTE_C6,

};

//undertale Megalovania tempo
static const float megalovania_tempo[] = {
  20, 20, 20/3, 20/3, 20, 20, 20,
  20, 20, 10, 20, 20, 20,

  20, 20, 20/3, 20/3, 20, 20, 20,
  20, 20, 10, 20, 20, 20,

  20, 20, 20/3, 20/3, 20, 20, 20,
  20, 20, 10, 20, 20, 20,

  20, 20, 20/3, 20/3, 20, 20, 20,
  20, 20, 10, 20, 20, 20,
  //repeat of first part
  20, 20, 20/3, 20/3, 20, 20, 20,
  20, 20, 10, 20, 20, 20,

  20, 20, 20/3, 20/3, 20, 20, 20,
  20, 20, 10, 20, 20, 20,

  20, 20, 20/3, 20/3, 20, 20, 20,
  20, 20, 10, 20, 20, 20,

  20, 20, 20/3, 20/3, 20, 20, 20,
  20, 20, 10, 20, 20, 20,
  //end of repeat

  80, 10, 20, 20, 20, 20, 20,
  80, 10, 20, 20, 4,

  10, 20, 20, 20, 20, 20, 10,
  40, 40, 40/3, 20, 20, 20, 10,

  10, 20, 40/3, 20, 20, 20, 20,
  20, 40/3, 20, 20, 20, 20/3,

  10, 10, 20, 20, 20, 5, 4,

};

// Pokemon trainer battle melody from Bespinben partition
static const uint16_t pokemon_trainer_battle_melody[] = {

  NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_A5, NOTE_E5, NOTE_F5, NOTE_E5,
  NOTE_A5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_A5, NOTE_D5, NOTE_DS5, NOTE_D5,

  NOTE_A5, NOTE_CS5, NOTE_D5, NOTE_CS5, NOTE_A5, NOTE_C5, NOTE_CS5, NOTE_C5,
  NOTE_A5, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A5, NOTE_AS4, NOTE_B4, NOTE_AS4,

  NOTE_AS4, 0, NOTE_E4, 0, NOTE_F4, 0,

  NOTE_D4, NOTE_E4, 0, NOTE_F4, 0, NOTE_C5, 0,
};

//Pokemon trainer battle tempo
static const float pokemon_trainer_battle_tempo[] = {

  30, 30, 30, 30, 30, 30, 30, 30,
  30, 30, 30, 30, 30, 30, 30, 30,

  30, 30, 30, 30, 30, 30, 30, 30,
  30, 30, 30, 30, 30, 30, 30, 30,

  15, 15/2, 15, 15/2, 15, 15,
  15, 15, 15, 15, 15/2, 15, 15,
};

// Victory Fanfare melody from Nobuo Uematsu partition
static const uint16_t victory_fanfare_melody[] = {

  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_A4, NOTE_C5,
  NOTE_D5, NOTE_C5, NOTE_D5,

};

//Victory Fanfare tempo
static const float victory_fanfare_tempo[] = {

  15, 15, 15, 5, 5, 5,
  5, 10, 5/3,
};

// The Tavern Song melody from Chrasher partition
static const uint16_t the_tavern_song_melody[] = {

  NOTE_FS5, NOTE_CS6, NOTE_CS6, NOTE_B5, NOTE_CS6, NOTE_CS6, NOTE_DS6,
  NOTE_E6, NOTE_FS6, NOTE_E6, NOTE_DS6, NOTE_CS6, NOTE_B5,

  NOTE_FS5, NOTE_CS6, NOTE_CS6, NOTE_B5, NOTE_CS6, NOTE_DS6, NOTE_CS6,
  NOTE_B5, NOTE_GS5, NOTE_E5, NOTE_GS5, NOTE_FS5,

  NOTE_FS5, NOTE_CS6, NOTE_CS6, NOTE_B5, NOTE_CS6, NOTE_CS6, NOTE_DS6,
  NOTE_E6, NOTE_FS6, NOTE_E6, NOTE_DS6, NOTE_CS6, NOTE_B5,

  NOTE_D6, NOTE_E6, NOTE_D6, NOTE_CS6, NOTE_B5, NOTE_A5,
  NOTE_FS5, 0,

  NOTE_CS5, NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_FS5, NOTE_GS5,
  NOTE_A5, NOTE_B5, NOTE_A5, NOTE_GS5, NOTE_FS5, NOTE_E5,

  NOTE_CS5, NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_FS5, NOTE_GS5,
  NOTE_A5, NOTE_E5, NOTE_CS5, NOTE_A5, NOTE_GS5,

  NOTE_CS5, NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_FS5, NOTE_GS5,
  NOTE_A5, NOTE_B5, NOTE_A5, NOTE_GS5, NOTE_FS5, NOTE_E5,

  NOTE_A5, NOTE_FS5, NOTE_A5, NOTE_B5, NOTE_A5, NOTE_GS5,
  NOTE_FS5, 0,

  NOTE_FS4, NOTE_CS5, NOTE_CS5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_CS5,
  NOTE_DS5, NOTE_CS5, NOTE_B4, NOTE_CS5,

  NOTE_FS4, NOTE_GS4, NOTE_A4, NOTE_GS4, NOTE_A4, NOTE_B4,
  NOTE_CS5, NOTE_B4, NOTE_A4, NOTE_GS4,

  NOTE_FS4, NOTE_CS5, NOTE_CS5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_CS5,
  NOTE_DS5, NOTE_CS5, NOTE_B4, NOTE_CS5,

  NOTE_A4, NOTE_B4, NOTE_A4, NOTE_GS4, NOTE_FS4, NOTE_E4,
  NOTE_FS4, 0,

};

//The Tavern Song tempo
static const float the_tavern_song_tempo[] = {

  10, 10, 20, 20, 5, 20, 20,
  10, 10, 10, 20, 20, 5,

  10, 10, 20, 20, 5, 20, 20,
  10, 20, 10, 20, 10/3,

  10, 10, 20, 20, 5, 20, 20,
  10, 10, 10, 20, 20, 5,

  10, 10, 10, 20/3, 20, 10,
  5/2, 5,

  10, 10, 20, 20, 5, 10,
  10, 10, 10, 20, 20, 5,

  10, 10, 20, 20, 5, 10,
  10, 20, 10, 20, 10/3,

  10, 10, 20, 20, 5, 10,
  10, 10, 10, 20, 20, 5,

  10, 10, 10, 20/3, 20, 10,
  5/2, 5,

  10, 10, 20, 20, 10, 10, 10,
  20/3, 20, 10, 10/3,

  20/3, 20, 10, 10, 10, 10,
  20/3, 20, 10, 10/3,

  10, 10, 20, 20, 10, 10, 10,
  20/3, 20, 10, 10/3,

  20/3, 20, 10, 20/3, 20, 10,
  5/2, 5,

};




static const melody_t melody[NB_EXTERNAL_SONGS] = {
		  //MEGALOVANIA
		  {
		    .notes = megalovania_melody,
		    .tempo = megalovania_tempo,
		    .length = sizeof(megalovania_melody)/sizeof(uint16_t),

		  },
		  //POKEMON_TRAINER_BATTLE
		  {
			.notes = pokemon_trainer_battle_melody,
			.tempo = pokemon_trainer_battle_tempo,
			.length = sizeof(pokemon_trainer_battle_melody)/sizeof(uint16_t),

		  },
		  //VICTORY_FANFARE
		  {
		  	.notes = victory_fanfare_melody,
		  	.tempo = victory_fanfare_tempo,
		  	.length = sizeof(victory_fanfare_melody)/sizeof(uint16_t),

		  },
			//THE_TAVERN_SONG
		  {
			.notes = the_tavern_song_melody,
			.tempo = the_tavern_song_tempo,
			.length = sizeof(the_tavern_song_melody)/sizeof(uint16_t),

		  },
};

static THD_WORKING_AREA(waMelodyPlayer, 1024);
static THD_FUNCTION(Melody_player, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    while (1) {
    	//Permet de detecter un changement de musique
    	static music temp = NO_SONG;
    	//La fonction get_play implémenté dans la libraire e-puck2_main_processor est
    	//nécessaire afin de savoir quand on doit rejouer la musique

    	switch (music_to_play){
    		default:
    			stopCurrentMelody();
    			temp = NO_SONG;
    			break;
    		case TAVERN_SONG:
    			if(temp!=TAVERN_SONG){
    				playMelody(EXTERNAL_SONG, ML_FORCE_CHANGE, &melody[THE_TAVERN_SONG]);
    			}else{
    				playMelody(EXTERNAL_SONG, ML_SIMPLE_PLAY, &melody[THE_TAVERN_SONG]);
    			}
    			temp = TAVERN_SONG;
    			break;
    		case PURSUIT_SONG:
    			if(temp!=PURSUIT_SONG){
    				playMelody(EXTERNAL_SONG, ML_FORCE_CHANGE, &melody[MEGALOVANIA]);
    			}else{
    				playMelody(EXTERNAL_SONG, ML_SIMPLE_PLAY, &melody[MEGALOVANIA]);
    			}
    			temp = PURSUIT_SONG;
    			break;
    		case ENEMY_DETECTION_SONG:
    			if(temp!=ENEMY_DETECTION_SONG){
    				playMelody(EXTERNAL_SONG, ML_FORCE_CHANGE, &melody[POKEMON_TRAINER_BATTLE]);
    			}else{
    				playMelody(EXTERNAL_SONG, ML_SIMPLE_PLAY, &melody[POKEMON_TRAINER_BATTLE]);
    			}
    			temp = ENEMY_DETECTION_SONG;
    			break;
    		case VICTORY_SONG:
    			if(temp!=VICTORY_SONG){
    				playMelody(EXTERNAL_SONG, ML_FORCE_CHANGE, &melody[VICTORY_FANFARE]);
    			}else{
    				playMelody(EXTERNAL_SONG, ML_SIMPLE_PLAY, &melody[VICTORY_FANFARE]);
    			}
    			temp = VICTORY_SONG;
    			break;
    		}


        chThdSleepMilliseconds(100);
    }
}

void melody_player_start(void){
	chThdCreateStatic(waMelodyPlayer, sizeof(waMelodyPlayer), NORMALPRIO, Melody_player, NULL);
}
music get_music_to_play(void){
	return music_to_play;
}

void set_music_to_play(uint8_t music_to_set){
	music_to_play = music_to_set;
}
