#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <main.h>
#include <camera/po8030.h>

#include <process_image.h>



static unsigned int line_position = IMAGE_BUFFER_SIZE/2;
static uint8_t barcode_lines = 0;

static uint8_t detection_lines(uint8_t* image);
//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);


static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
	po8030_advanced_config(FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
	dcmi_enable_double_buffering();
	dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
	dcmi_prepare();

    while(1){
//    	unsigned int timetime = chVTGetSystemTime();
        //starts a capture
		dcmi_capture_start();
		//waits for the capture to be done
		wait_image_ready();
//		unsigned int Timing = chVTGetSystemTime()-timetime;
//		chprintf((BaseSequentialStream *)&SDU1, "%Timing=%d \r \n", Timing);
		//signals an image has been captured
		chBSemSignal(&image_ready_sem);

    }
}


static THD_WORKING_AREA(waProcessImage, 1024);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	uint8_t *img_buff_ptr;
//	uint8_t image_bleu[IMAGE_BUFFER_SIZE] = {0};
	uint8_t image_rouge[IMAGE_BUFFER_SIZE] = {0};
//	uint8_t image_vert[IMAGE_BUFFER_SIZE] = {0};
	// inits barcode line counter


    while(1){

    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);
        chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565    
		img_buff_ptr = dcmi_get_last_image_ptr();
		for(unsigned int i=0;i<IMAGE_BUFFER_SIZE*2;i+=2){
			image_rouge[i/2] = img_buff_ptr[i] >> 3;
			//image_bleu[i/2] = img_buff_ptr[i+1] &0x1F;
			//prends les trois premiers bits du msb vert et adapte leurs valeur par rapport à la datasheet
			//image_vert[i/2] = img_buff_ptr[i] &0x07 *2 * 2 * 2;
			//rajoute les 3 bits du msb situé sur l'indice suivant sur img_buff_ptr
			//image_vert[i/2] += img_buff_ptr[i+1] >> 5;
		}
		SendUint8ToComputer(image_rouge, IMAGE_BUFFER_SIZE);
		barcode_lines = detection_lines(image_rouge);

		//chprintf((BaseSequentialStream *)&SD3, "%middle=%d and width=%d \r \n", line_position, width);
		/*
		*	To complete
		*/
    }
}


void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}

//cette fonction compte le nombre de lignes afin de detecter un code barre
static uint8_t detection_lines(uint8_t* image) {
	// first_begin permet de trouver le départ de la première ligne
	uint16_t width = 0, begin = 0, end = 0, first_begin = 0;
	uint16_t mean = 0;
	//compte le nombre de ligne
	uint8_t line_counter = 0;
	bool stop = 0, not_found = 0;
	uint16_t counter = 0;

	for(unsigned int i=0; i < IMAGE_BUFFER_SIZE; i++){
		mean += image[i];
	}
	mean = mean/IMAGE_BUFFER_SIZE;
	do{
			//search for a begin
			while(stop == 0 && counter < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE))
			{
				//the slope must at least be WIDTH_SLOPE wide and is compared
			    //to the mean of the image
			    if(image[counter] > mean && image[counter+WIDTH_SLOPE] < mean)
			    {
			    	if(!first_begin){
			    		first_begin = counter;
			    	}
			        begin = counter;
			        stop = 1;

			    }
			    counter ++;
			}
			//if a begin was found, search for an end
			if (counter < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE) && begin)
			{
			    stop = 0;

			    while(stop == 0 && counter < IMAGE_BUFFER_SIZE)
			    {
			        if(image[counter] > mean && image[counter-WIDTH_SLOPE] < mean)
			        {
			            end = counter;
			            stop = 1;
			            if((end-begin) < LINE_WIDTH_MIN){
			            	line_counter ++;
			            }

			        }
			        counter ++;
			    }
			    //if an end was not found and there is no other lines
			    if ((counter > IMAGE_BUFFER_SIZE || !end) && !first_begin)
			    {
			        not_found = 1;
			    }
			}
			else if(!first_begin)//if no begin was found
			{
			    not_found = 1;
			}

			//if a line too small has been detected, continues the search
			if(!not_found && (end-begin) < LINE_WIDTH_MIN){
				counter = end;
				begin = 0;
				end = 0;
				stop = 0;
			}
		}while(counter < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE));

		if(not_found){
			first_begin = 0;
			begin = 0;
			end = 0;
			line_counter = 0;
		}else{
			line_position = (first_begin + end)/2; //gives the line position.

		}

	return line_counter;
}
uint16_t get_line_position(void){
	return line_position;
}

uint8_t get_barcode(void){
	return barcode_lines;
}
