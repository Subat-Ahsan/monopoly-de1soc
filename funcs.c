void draw_player(Player p){
	for (short x = p.x; x<playerSize; x++){
        for (short y = p.y; y<playerSize; y++){
            plot_pixel(x,y, p.color);
        }
    }
	
	
};
