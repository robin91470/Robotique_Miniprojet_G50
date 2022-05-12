#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <usbcfg.h>

#include <main.h>
#include <camera/po8030.h>

#include <process_image.h>



//static uint16_t line_position = IMAGE_BUFFER_SIZE/2;
static bool line_detection_red = 0;
static bool line_detection_blue = 0;
static bool detection_line(uint8_t* image);
static void line_detection_avg(void);
//semaphore
static BSEMAPHORE_DECL(image_ready_sem, TRUE);


static THD_WORKING_AREA(waCaptureImage, 256);
static THD_FUNCTION(CaptureImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	//Takes pixels 0 to IMAGE_BUFFER_SIZE of the line 10 + 11 (minimum 2 lines because reasons)
	po8030_advanced_config(FORMAT_RGB565, 0, 10, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
	po8030_set_contrast(100);
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


static THD_WORKING_AREA(waProcessImage, 2*1024);
static THD_FUNCTION(ProcessImage, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

	uint8_t *img_buff_ptr;
	uint8_t image_bleu[IMAGE_BUFFER_SIZE] = {0};
	uint8_t image_rouge[IMAGE_BUFFER_SIZE] = {0};
//	uint8_t image_vert[IMAGE_BUFFER_SIZE] = {0};


    while(1){

    	//waits until an image has been captured
        chBSemWait(&image_ready_sem);
		//gets the pointer to the array filled with the last image in RGB565    
		img_buff_ptr = dcmi_get_last_image_ptr();
		for(unsigned int i=0;i<IMAGE_BUFFER_SIZE*2;i+=2){
			image_rouge[i/2] = img_buff_ptr[i] >> 3;
			image_bleu[i/2] = img_buff_ptr[i+1] &0x1F;
			//prends les trois premiers bits du msb vert et adapte leurs valeur par rapport à la datasheet
			//image_vert[i/2] = img_buff_ptr[i] &0x07 *2 * 2 * 2;
			//rajoute les 3 bits du msb situé sur l'indice suivant sur img_buff_ptr
			//image_vert[i/2] += img_buff_ptr[i+1] >> 5;
		}
//		SendUint8ToComputer(image_bleu, IMAGE_BUFFER_SIZE);
		line_detection_red = detection_line(image_rouge);
		line_detection_blue = detection_line(image_bleu);
		line_detection_avg();
//		chprintf((BaseSequentialStream *)&SD3, " detec ligne rouge = %d et detec ligne bleu = %d \r \n",
//				line_detection_red, line_detection_blue);
		chThdSleepMilliseconds(100);


    }
}


void process_image_start(void){
	chThdCreateStatic(waProcessImage, sizeof(waProcessImage), NORMALPRIO, ProcessImage, NULL);
	chThdCreateStatic(waCaptureImage, sizeof(waCaptureImage), NORMALPRIO, CaptureImage, NULL);
}

//cette fonction renvoie la detection d'une ligne de couleur
static bool detection_line(uint8_t* image) {
	uint16_t  begin = 0, end = 0, counter = 0;
	uint16_t mean = 0;
	bool wrong_line = 0;
	bool stop = 0, not_found = 0;
	bool detect_line_color = 0;

	for(unsigned int i=0; i < IMAGE_BUFFER_SIZE; i++){
		mean += image[i];
	}
	mean = mean/IMAGE_BUFFER_SIZE;
	do{
			wrong_line = 0;
			//search for a begin
			while(stop == 0 && counter < (IMAGE_BUFFER_SIZE - WIDTH_SLOPE))
			{
				//the slope must at least be WIDTH_SLOPE wide and is compared
			    //to the mean of the image
			    if(image[counter] > mean && image[counter+WIDTH_SLOPE] < mean)
			    {
			        begin = counter;
			        stop = 1;
			    }
			    counter++;
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
			        }
			        counter++;
			    }
			    //if an end was not found
			    if (counter > IMAGE_BUFFER_SIZE || !end)
			    {
			        not_found = 1;
			    }
			}
			else//if no begin was found
			{
			    not_found = 1;
			}

			//if a line too small has been detected, continues the search
			if(!not_found && (end-begin) < LINE_WIDTH_MIN){
				counter = end;
				begin = 0;
				end = 0;
				stop = 0;
				wrong_line = 1;
			}
		}while(wrong_line);

		if(not_found){
			begin = 0;
			end = 0;
			detect_line_color = 0;
		}else{
			detect_line_color = 1;
			//line_position = (begin + end)/2; //gives the line position.
		}

	return detect_line_color;
}

static void line_detection_avg(void){
		static bool previous_line_red[NB_SAMPLE_SCAN] = {0};
		static bool previous_line_blue[NB_SAMPLE_SCAN] = {0};
		static uint8_t circular_buffer = 0;
		uint8_t detected_line_counter_red = 0;
		uint8_t detected_line_counter_blue = 0;
		previous_line_red[circular_buffer] = line_detection_red;
		previous_line_blue[circular_buffer] = line_detection_blue;
		for(uint8_t i=0; i<NB_SAMPLE_SCAN; i++){
				if(previous_line_red[i]){
					detected_line_counter_red ++;
				}
				if(previous_line_blue[i]){
					detected_line_counter_blue ++;
				}
			}
		if(detected_line_counter_red > (NB_SAMPLE_SCAN/2)){
			line_detection_red = 1;
		}else{
			line_detection_red =  0;
		}
		if(detected_line_counter_blue > (NB_SAMPLE_SCAN/2)){
					line_detection_blue = 1;
		}else{
					line_detection_blue =  0;
		}
		circular_buffer = (circular_buffer + 1) % NB_SAMPLE_SCAN;
}
bool get_line_detection_red(void){
	return line_detection_red;
}

bool get_line_detection_blue(void){
	return line_detection_blue;
}

