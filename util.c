#include <stdio.h>
#include "base.h"

void initGame(ChessSt *game) {
	int i;
	game->piece_present[WHITE] = 0;
	game->piece_present[BLACK] = 0;
	/*for(i = 0; i < 32; i++) {}*/ //innecesario
	game->king_pos = 0;
	game->castling_passant = 0;
}
//c minmúscula sólo
PieceType parsePiece(char c) {
	switch(c) {
		case 'p': return Pawn;
		case 'r': return Rook;
		case 'n': return Knight;
		case 'b': return Bishop;
		case 'q': return Queen;
		case 'k': return King;
	}
	return -1;
}
Color parseColor(char c) {
	return c >= 'a' && c <= 'z';
}
char pieceToString(PieceType p) {
	switch(p) {
		case Pawn: return 'p';
		case Rook: return 'r';
		case Knight: return 'n';
		case Bishop: return 'b';
		case Queen: return 'q';
		case King: return 'k';
	}
	//error
	return -1;
}
void parseGame(ChessSt *game, char *str) {
	//RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr
	unsigned char pos;
	char c;
	PieceType piece;
	Color color;
	initGame(game);
	for(pos = 0; pos < 64; pos++) {
		c = str[pos];
		if(c != '.') {
			color = parseColor(c);
			if(!color) c = c - 'A' + 'a';
			piece = parsePiece(c);
			// assert piece != -1
			setColor(game, pos, color);
			setPiece(game, pos, piece);
		}
	}
	setCastle(game, WHITE, LEFT);
	setCastle(game, WHITE, RIGHT);
	setCastle(game, BLACK, LEFT);
	setCastle(game, BLACK, RIGHT);
}
void printGame(ChessSt *game) {
	int i, j;
	unsigned char pos;
	char c;
	PieceType piece;
	Color color;
	for(i = 7; i >= 0; i--)
		for(j = 0; j < 8; j++) {
			pos = (i<<3) | j;
			if(OCCUPIED(game, pos)) {
				color = COLOR(game, pos);
				piece = PIECE_AT(game, pos);
				c = pieceToString(piece);
				if(!color) c = c - 'a' + 'A';
				putchar(c);
			} else {
				putchar('.');
			}
			if(j == 7) putchar('\n');
		}
}