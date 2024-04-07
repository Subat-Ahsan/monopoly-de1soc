#include <time.h>


volatile int pixel_buffer_start; // global variable
short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];

void wait_for_vsync();
void draw_line(int x0, int y0, int x1, int y1, short int color);
void clear_screen();
void plot_pixel(int x, int y, short int line_color);
void draw_rect(int x1, int y1, int x2, int y2, short int color);

typedef struct {
	short int x;
	short int y;
	short int w; 
	short int color;
	char dx;
	char dy;
	
} square;
void updateLocation(square* s){
	s->x += (-1 + 2 * s->dx);
	s->y += (-1 + 2 * s->dy);
	if (s->x == 0){
		s->dx = 1;
	}
	if (s->x == 311){
		s->dx = 0;
	}
	if (s->y == 0){
		s->dy = 1;
	}
	if (s->y == 211){
		s->dy = 0;
	}
}
	
int main(void)
{
	srand(time(NULL));
	
	//get it
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
	short int num_rectangles = 8;
	square boxes[num_rectangles]; 
	
	for (int i = 0; i<num_rectangles; i++){
		
		boxes[i].x = rand() % 312;
		boxes[i].y = rand() % 212;
		boxes[i].color = rand() % 0xFFFF;
		boxes[i].dx = rand() % 2;
		boxes[i].dy = rand() % 2;
	}
	
	//pixelbufferstuff
    /* set front pixel buffer to Buffer 1 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1; // first store the address in the  back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    /* set back pixel buffer to Buffer 2 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    while (1)
    {
        // Erase 
        clear_screen();
		
		//draw
		for (int i = 0; i<num_rectangles; i++){
			updateLocation(&boxes[i]);
			draw_rect(boxes[i].x, boxes[i].y, boxes[i].x+8, boxes[i].y+8, boxes[i].color);
			draw_line(boxes[i].x+4, boxes[i].y+4, boxes[(i+1)%num_rectangles].x+4, boxes[(i+1)%num_rectangles].y+4, 
			(boxes[i].color+boxes[i+1%num_rectangles].color)/2);
		}			
		
		//refresh
        wait_for_vsync(); 
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}

// code for subroutines (not shown)

void wait_for_vsync(){
	volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
	*pixel_ctrl_ptr = 1;
	while ((*(pixel_ctrl_ptr+3) & 1) == 1){;}
}

void draw_line(int x0, int y0, int x1, int y1, short int color){
	
	char is_steep = ((y1-y0) * (((y1-y0) < 0) ? -1: 1)) > ((x1-x0) * (((x1-x0) < 0) ? -1: 1));
	if (is_steep){
		if (x0 != y0){
			x0 ^= y0;
			y0 ^= x0;
			x0 ^= y0;
		}
		if (x1 != y1){
			x1 ^= y1;
			y1 ^= x1;
			x1 ^= y1;
		}
	}
	if (x0 > x1){
		if (x0 != x1){
			x0 ^= x1;
			x1 ^= x0;
			x0 ^= x1;
		}
		if (y0 != y1){
			y0 ^= y1;
			y1 ^= y0;
			y0 ^= y1;
		}
	}
	int deltax = x1-x0;
	int deltay = (y1-y0) * ((y1-y0 < 0) ? -1 : 1);
	int error = -(deltax/2);
	int y = y0;
	int y_step = (y0 < y1) ? 1 : -1;
	
	for (int x = x0; x<= x1; x++){
		if (is_steep){
			plot_pixel(y,x,color);
		}
		else{
			plot_pixel(x,y,color);
		}
		error = error + deltay;
		if (error > 0){
			y = y+y_step;
			error = error - deltax;
		}
	}
}
void clear_screen() {
	int max_x = 320;
	int max_y = 240;

    for (int x = 0; x<max_x; x++){
        for (int y = 0; y<max_y; y++){
            plot_pixel(x,y, 0);
        }
    }
}


void plot_pixel(int x, int y, short int line_color)
{
    volatile short int *one_pixel_address;

        one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);

        *one_pixel_address = line_color;
}

void draw_rect(int x1, int y1, int x2, int y2, short int color){
	for (int i = x1; i<= x2; i++){
		for (int j = y1; j <= y2; j++){
			plot_pixel(i,j,color);
		}
	}
}