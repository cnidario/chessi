#include <stdio.h>
#include "base.h"
#include "util.h"
#include "chessi.h"


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

int testPlay(ChessSt *game, char *play) {
	Move move;
	move = parseMove(play);
	printf("Jugada: %d %d\n", POS1(move), POS2(move));
	assertTrue("jugada debe ser correcta", move != -1);
	assertTrue("\n\njugada debe ser válida\n\n", validMove(game, move));
	if(validMove(game, move)) {
		doMove(game, move);
		return 1;
	}
	return 0;
}
int testPlays(ChessSt *game, char *plays, int print_game_each_step) {
	int i;
	i = 0;
	while(plays[i*6]) {
		puts(plays + i*6);
		if(testPlay(game, plays + i*6) && print_game_each_step)
			printGame(game);
		i++;
	}
}
int test_game_1() {
	ChessSt game;
	char play[] = "e2 e3";
	parseGame(&game, "RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr");
	printGame(&game);
	if(testPlay(&game, play))
		printGame(&game);
}

int test_game_2() {
	ChessSt game;
	int i;
	char *plays[] = { 
		"a2 a3", "b2 b3", "c2 c3", "d2 d3", "e2 e3", "f2 f3", "g2 g3", "h2 h3",
		"a2 a4", "b2 b4", "c2 c4", "d2 d4", "e2 e4", "f2 f4", "g2 g4", "h2 h4"
	};
	for(i = 0; i < sizeof(plays)/sizeof(char*); i++) {
		parseGame(&game, "RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr");
		if(testPlay(&game, plays[i]))
			printGame(&game);
	}
}

int test_game_3() {
	ChessSt game;
	int i;
	char *plays[] = {
		"a2 a4 a7 a5"
	};
	for(i = 0; i < sizeof(plays)/sizeof(char*); i++) {
		parseGame(&game, "RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr");
		if(testPlays(&game, plays[i], 1))
			printGame(&game);
	}	
}

int main(int argc, char *argv[]) {
	test_game_3();
}