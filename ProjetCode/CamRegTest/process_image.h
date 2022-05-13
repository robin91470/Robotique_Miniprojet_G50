#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

#define IMAGE_BUFFER_SIZE		640
#define WIDTH_SLOPE				5
#define LINE_WIDTH_MIN			30
#define PXTOCM					1570.0f


void process_image_start(void);
uint16_t get_line_position(void);
uint8_t get_barcode(void);


#endif /* PROCESS_IMAGE_H */
