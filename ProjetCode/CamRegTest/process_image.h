#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

#define NB_SAMPLE_SCAN		    10
#define IMAGE_BUFFER_SIZE		640
#define WIDTH_SLOPE				5
#define LINE_WIDTH_MIN			30


void process_image_start(void);
uint16_t get_line_position(void);
bool get_line_detection_red(void);
bool get_line_detection_blue(void);

#endif /* PROCESS_IMAGE_H */
