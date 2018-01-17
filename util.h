#ifndef __UTIL_H__
#define __UTIL_H__

void initGame(ChessSt *game);
PieceType parsePiece(char c);
void parseGame(ChessSt *game, char *str);
char pieceToString(PieceType p);
void printGame(ChessSt *game);
unsigned char parsePos(char *str);
Move parseMove(char *str);

#endif