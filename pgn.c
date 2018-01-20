#include <ctype.h>
#include <string.h>
#include "base.h"

char* skipTurnSAN(char *str) {
	char c;
	int i;
	i = 0;
	do {
		c = str[i++];
	}
	while(c && (isspace(c) || isdigit(c) || c == '.'));
	return i;
}
PieceType parseMoveTypeSAN(char *str) {
	char c;
	c = str[0];
	switch(c) {
		case 'R': return Rook;
		case 'B': return Bishop;
		case 'N': return Knight;
		case 'Q': return Queen;
		case 'K': return King;
		default: return -1;
	}
}
#define SHORT_CASTLING 1
#define LONG_CASTLING 2
int checkCastlingSAN(char *str) {
	if(strncmp(str, "0-0", 3) == 0)
		if(str[3] == '\0')
			return SHORT_CASTLING
		else if(strncmp(str+3, "-0", 3) == 0)
			return LONG_CASTLING
	return 0;
}
#define PIECE_AT_POS_IS(game, pos, color, piece) (PRESENT(game, pos, color) && PIECE_AT(game, pos) == (piece))
Position searchPawnSAN(ChessSt *game, Position dest, Color color, int is_capture, unsigned char partial_row, unsigned char partial_col) {
	Position p;
	Direction pawn_dir;
	pawn_dir = color? S : N;
	if(is_capture) {
		p = dest - pawn_dir + E;
		if(PIECE_AT_POS_IS(game, p, color, Pawn))
			return p;
		p = dest - pawn_dir + O;
		if(PIECE_AT_POS_IS(game, p, color, Pawn))
			return p;
		return -1;
	}
	p = dest - pawn_dir;
	if(PIECE_AT_POS_IS(game, p, color, Pawn))
		return p;
	p = dest - 2*pawn_dir;
	if(ROW(p) == STARTPAWNROW(color) && PIECE_AT_POS_IS(game, p, color, Pawn))
		return p;
	return -1;
}
Position searchRookSAN(ChessSt *game, Position dest, Color color, int is_capture, unsigned char partial_row, unsigned char partial_col) {
	Position p;
	//buscar resto de piezas
}
//parsea movimiento en Short Algebraic Mode
Move parseSAN(char *str) {
	
}