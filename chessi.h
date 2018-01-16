#ifndef __CHESSI_H__
#define __CHESSI_H__
#include "base.h"

int isCheck(ChessSt *game);
int isMenaced(ChessSt *game, unsigned char pos, Color color);
int firstPieceAt(ChessSt *game, unsigned char from, Direction dir);
int isValidCastle(ChessSt *game, unsigned char pos1, unsigned char pos2);
int validPieceMove(ChessSt *game, unsigned char pos1, unsigned char pos2, unsigned char piece);
int validMove(ChessSt *game, Move move);
int doMove(ChessSt *game, Move move);

#endif
