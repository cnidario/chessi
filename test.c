#include <stdio.h>
#include "base.h"
#include "util.h"


void assertTrue(char *title, int condition) {
	if(!condition) {
		puts(title);
	}
}
int test_piecePlacing_1() {
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
int test_piecePlacing_2() {
	ChessSt game;
	initGame(&game);
	setColor(&game, 58, WHITE);
	assertTrue("posición 58 debe contener pieza blanca", PRESENT(&game, 58, WHITE));
}
int test_printGame_1() {
	ChessSt game;
	parseGame(&game, "RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr");
	printGame(&game);
}
int test_castling_1() {
	ChessSt game;
	initGame(&game);
	assertTrue("enroque a la izquierda para blancas debe estar permitido", CANCASTLE_(&game, WHITE, LEFT));
	assertTrue("enroque a la derecha para blancas debe estar permitido", CANCASTLE_(&game, WHITE, RIGHT));
	assertTrue("enroque a la izquierda para negras debe estar permitido", CANCASTLE_(&game, BLACK, LEFT));
	assertTrue("enroque a la derecha para negras debe estar permitido", CANCASTLE_(&game, BLACK, RIGHT));
}

int main(int argc, char *argv[]) {
	test_printGame_1();
}