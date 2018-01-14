#include <stdio.h>
#include "base.h"
#include "util.h"


void assertTrue(char *title, int condition) {
	if(!condition) {
		puts(title);
	}
}
int test1() {
	ChessSt game;
	unsigned char pos;
	Color color;
	initGame(&game);
	for(pos = 0; pos < 64; pos++) {
		setColor(&game, pos, BLACK);
		assertTrue("posición debe contener pieza", OCCUPIED(&game, pos));
		assertTrue("posición debe contener pieza negra", PRESENT(&game, pos, BLACK));
		assertTrue("posición no debe contener pieza blanca", !PRESENT(&game, pos, WHITE));
	}
	initGame(&game);
	for(pos = 0; pos < 64; pos++) {
		setColor(&game, pos, WHITE);
		assertTrue("posición debe contener pieza", OCCUPIED(&game, pos));
		assertTrue("posición debe contener pieza blanca", PRESENT(&game, pos, WHITE));
		assertTrue("posición no debe contener pieza negra", !PRESENT(&game, pos, BLACK));
	}
	initGame(&game);
	for(pos = 0; pos < 64; pos++) {
		if(pos % 5 >= 3) color = WHITE;
		else if(pos % 5 >= 1) color = BLACK;
		else continue;
		setColor(&game, pos, color);
		//printf("seteando pos %d color %s\n", pos, color?"black":"white");
		assertTrue("posición debe contener pieza", OCCUPIED(&game, pos));
		assertTrue("posición debe contener pieza del color", PRESENT(&game, pos, color));
		assertTrue("posición no debe contener pieza del oponente", !PRESENT(&game, pos, OPONENT(color)));
	}
}
int test1_2() {
	ChessSt game;
	initGame(&game);
	setColor(&game, 58, WHITE);
	assertTrue("posición 58 debe contener pieza blanca", PRESENT(&game, 58, WHITE));
}
int test2() {
	ChessSt game;
	parseGame(&game, "RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr");
	printGame(&game);
}

int main(int argc, char *argv[]) {
	test2();
}