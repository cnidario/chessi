/*
	Este fichero contiene las estructuras de datos que representan el estado del juego
	así como una serie de macros que facilitan el acceso (a modo de API)
	y también otros tipos de datos para comunicarse con él

	Se utilizarán tan sólo los tipos
	Color, PieceType, Move, Direction
	además de las macros
	POS1, POS2, TURN, PRESENT, OCCUPIED, PIECE_AT, COLOR, OPONENT, OOB, COLUMN, ROW,
	ABS, REACHABLE, CANCASTLE, CANPASSANT
*/
#ifndef __BASE_H__
#define __BASE_H__
// Estado del juego
typedef enum Color { WHITE, BLACK, COLOR_NB } Color;
typedef enum PieceType { Pawn, Rook, Knight, Bishop, Queen, King } PieceType;
// UnmovedSpecial es Torre, Rey o Peón aún sin mover. También Peón recién salido en pasante.
typedef struct ChessSt {
	unsigned long long piece_present[COLOR_NB]; //64*2 bits presencia/color bitboard
	unsigned char pieces[32]; //4bits por pieza
	//posición de los reyes, 3x2=6 bits por posición x2 = 12+1=13 bit turno
	//2*2 = 4 bits posibilidad de enroque (izqda. y dcha)
	//3b + 1 = 4 indicación en passant + columna
	//total = 22bits, passant + enroque = 8, aparte
	unsigned short king_pos;
	unsigned char castling_passant;
} ChessSt;
typedef unsigned short Move;

typedef enum Mask { TURN_BIT = 12, 
					WKING_POSMK = 0x3f,
					BKING_POSMK = 0xfc0,
					COLOR_BIT = 3,
					CASTLING_BIT = 7,
					LEFT_CASTLING_BIT = 7,
					RIGHT_CASTLING_BIT = 6, //5 y 4 para negras
					PASSANT_BIT = 3,
					PASSANT_COLUMN_MASK = 0x7,
					LEFT = 0,
					RIGHT = 1
				} Mask;
#define MOVE1_POSMK WKING_POSMK
#define MOVE2_POSMK BKING_POSMK

#define FLAG(bitarray, n) (((bitarray) >> (n)) & 0x1)
#define BITPACK(bitarray, first, length) (((bitarray) >> (first)) & (((unsigned long long)-1)>>(64-length)))
#define POS1(move) ((move) & MOVE1_POSMK)
#define POS2(move) (((move) & MOVE2_POSMK) >> 6)
#define TURN(game) (FLAG((game)->king_pos, TURN_BIT))
#define PRESENT(game, pos, color) (FLAG((game)->piece_present[color], pos))
#define OCCUPIED(game, pos) (PRESENT(game, pos, WHITE) || PRESENT(game, pos, BLACK))
#define PIECE_AT(game, pos) (((pos)%2) ? \
								  ((game)->pieces[(pos)/2] & 0xF0) >> 4 \
								: ((game)->pieces[(pos)/2] & 0xF))
#define COLOR(game, pos) (PRESENT(game, pos, BLACK))
#define OPONENT(color) (((color) + 1) % 2)
#define OOB(p) ((p) >= 64) //out of bounds
#define COLUMN(pos) ((pos)%8)
#define ROW(pos) ((pos)/8)
#define ABS(x) ((x) > 0? (x) : -(x))
//casilla alcanzable desde otra en una dirección
#define REACHABLE(from, to, dir) (((to) - (from)) % (dir) && \
							((dir) > 0 && (from) < (to)) || \
							((dir) < 0 && (from) > (to)) )
#define CANCASTLE_(game, color, side) (FLAG((game)->castling_passant, CASTLING_BIT + (side) + (color)<<1))
#define CANCASTLE(game, side) (CANCASTLE_(game, TURN(game), side))
#define PASSANT(game) (FLAG((game)->castling_passant, PASSANT_BIT))
#define PASSANT_COL(game) ((game)->castling_passant & PASSANT_COLUMN_MASK)
#define CANPASSANT(game, col) (PASSANT(game) && PASSANT_COL(game) == (col))

typedef enum Direction { N = 8, S = -8, E = 1 , O = -1, NE = 9, NO = 7, SE = -7, SO = -9 } Direction;
//se cumple: NE = N + E, NO = N + O. Lo mismo para S, E y O.

#define MOVE(pos1, pos2) ((Move)((pos2)<<6)&(pos1))
//Modificadores
void setPiece(ChessSt *game, unsigned char pos, PieceType piece);
void setColor(ChessSt *game, unsigned char pos, Color color);
void unsetColor(ChessSt *game, unsigned char pos, Color color);
void setPassant(ChessSt *game, unsigned char column);
void unsetPassant(ChessSt *game);
void setCastle(ChessSt *game, Color color, unsigned char side);
void unsetCastle(ChessSt *game, Color color, unsigned char side);

#endif
