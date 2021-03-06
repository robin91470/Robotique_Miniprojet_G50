#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

#define NB_SAMPLE_SCAN		    10
#define IMAGE_BUFFER_SIZE		640
#define WIDTH_SLOPE				5
#define LINE_WIDTH_MIN			30


void ProcessImage_start_thd(void);
uint16_t get_line_position(void);
// detecte l'absence de rouge sur l'image de la camera
bool get_line_detection_red(void);
// detecte l'absence de bleu sur l'image de la camera
bool get_line_detection_blue(void);
void ProcessImage_pause_thd(void);
void ProcessImage_resume_thd(void);

#endif /* PROCESS_IMAGE_H */
