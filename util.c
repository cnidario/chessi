#include <stdio.h>
#include <string.h>
#include "base.h"

void initGame(ChessSt *game) {
	int i,j;
	game->piece_present[WHITE] = 0;
	game->piece_present[BLACK] = 0;
	/*for(i = 0; i < 32; i++) {}*/ //innecesario
	game->king_pos = 0;
	game->castling_passant = 0;
	for(i = 0; i < 2; i++)
		for(j = 0; j < 2; j++)
			setCastle(game, i, j);
}
//c minúscula sólo
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
			if(piece == King) {
				setKing(game, pos, color);
			}
		}
	}
	setCastle(game, WHITE, LEFT);
	setCastle(game, WHITE, RIGHT);
	setCastle(game, BLACK, LEFT);
	setCastle(game, BLACK, RIGHT);
}
int sprintpos(char *dst, unsigned char pos) {
	unsigned char col, row;
	col = COLUMN(pos);
	row = ROW(pos);
	row++;
	col = col + 'a';
	return sprintf(dst, "%c%d", col, row);
}
void printStatus(ChessSt *game) {
	Color color;
	int passant, passant_col, i, j, castle[2][2]; //castle[color][side]
	unsigned char wking, bking;
	char passant_str[2], castle_str[2][3], kings[2][3];
	color = TURN(game);
	passant = PASSANT(game);
	passant_col = PASSANT_COL(game);
	for(i = 0; i < 2; i++)
		for(j = 0; j < 2; j++)
			castle[i][j] = CANCASTLE_(game, i, j);
	wking = WKING_POS(game);
	bking = BKING_POS(game);

	sprintf(passant_str, "%d", passant_col);
	sprintpos(kings[WHITE], wking);
	sprintpos(kings[BLACK], bking);
	for(i = 0; i < 2; i++)
		if(castle[i][LEFT] && castle[i][RIGHT]) {
			strcpy(castle_str[i], "LR");
		} else if(castle[i][LEFT]) {
			strcpy(castle_str[i], "L ");
		} else if(castle[i][RIGHT]) {
			strcpy(castle_str[i], " R");
		} else {
			strcpy(castle_str[i], "No");
		}
	printf("Turn: %s - Passant: %s - Castle: W %s; B %s - Kings: W %s; B %s\n",
		color?   "black":"white",
		passant? passant_str:"N/A",
		castle_str[WHITE], castle_str[BLACK],
		kings[WHITE], kings[BLACK]);
}
void printBoard(ChessSt *game) {
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
void printGame(ChessSt *game) {
	printBoard(game);
	printStatus(game);
}