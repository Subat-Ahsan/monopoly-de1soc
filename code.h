void wait_for_vsync();
void clear_screen();
void plot_pixel(short x, short y, short int line_color);
void draw_rect(short x1, short y1, short x2, short y2, short int color);
void draw_player(Player p, char erase);
void draw_players(char player, char erase);

void draw_board();

const short playerSize = 17;
typedef struct {
	short posx; 
	short posy;
	short startx;
	short starty;
	short color;
	char currentSquare;
	char num;
	short cash;
	char isJail;
	
} Player; 

typedef struct {
	short drawx;
	short drawy;
	char propertyNum;
	char goJail;
	char jail;
	short val;
} Square;

void draw_player(Player p, char erase){
	for (short x = p.posx; x<p.posx+playerSize; x++){
        for (short y = p.posy; y<p.posy+playerSize; y++){
            plot_pixel(x,y, p.color);
        }
    }
};

short calculateSquareCost(Player p, Square s){
	if (s.val != 0){return s.val;}
	if (s.num == 29){return -400 + rand()%700;};
	
	return 0;
}