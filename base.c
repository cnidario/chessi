#include "base.h"
//Modificadores
void setPiece(ChessSt *game, unsigned char pos, PieceType piece) {
	if(pos % 2) {
		game->pieces[pos/2] = (game->pieces[pos/2] & 0xF) | (piece << 4);
	} else {
		game->pieces[pos/2] = (game->pieces[pos/2] & 0xF0) | piece;
	}
}
void setColor(ChessSt *game, unsigned char pos, Color color) {
	game->piece_present[color] |= 1L << pos;
	game->piece_present[OPONENT(color)] &= ~(1L << pos);
}
void unsetColor(ChessSt *game, unsigned char pos, Color color) {
	game->piece_present[color] &= ~(1L << pos);
}
void setPassant(ChessSt *game, unsigned char column) {
	game->castling_passant |= 1 << PASSANT_BIT;
	game->castling_passant = (game->castling_passant & ~0x7) | column;
}
void unsetPassant(ChessSt *game) {
	game->castling_passant &= ~(1 << PASSANT_BIT);
}
void setCastle(ChessSt *game, Color color, unsigned char side) {
	game->castling_passant |= 1 << CASTLING_BIT - side - (color<<1);
}
void unsetCastle(ChessSt *game, Color color, unsigned char side) {
	game->castling_passant &= ~(1 << CASTLING_BIT - side - (color<<1));
}
void setKing(ChessSt *game, unsigned char pos, Color color) {
	if(color)
		game->king_pos = (game->king_pos & ~BKING_POSMK) | pos << POS_LEN;
	else
		game->king_pos = (game->king_pos & ~WKING_POSMK) | pos;
}