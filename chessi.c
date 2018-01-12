// Estado del juego
typedef enum Color { WHITE, BLACK, COLOR_NB } Color;
typedef enum PieceType { Pawn, Rook, Knight, Bishop, Queen, King, UnmovedSpecial } PieceType;
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
			};
#define MOVE1_POSMK WKING_POSMK
#define MOVE2_POSMK BKING_POSMK

#define FLAG(bitarray, n) (((bitarray) >> (n)) & 0x1)
#define BITPACK(bitarray, first, length) (((bitarray) >> (first)) & (((unsigned long long)-1)>>(64-length)))
#define POS1(move) ((move) & MOVE1_POSMK)
#define POS2(move) (((move) & MOVE2_POSMK) >> 6)
#define TURN(game) (FLAG((game)->king_pos, TURN_BIT))
#define PRESENT(game, pos, color) (FLAG((game)->piece_present[color], pos))
#define OCUPPIED(game, pos) (PRESENT(game, pos, WHITE) || PRESENT(game, pos, BLACK))
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
#define PASSANT_COL ((game)->castling_passant & PASSANT_COLUMN_MASK)

/*
typedef struct Direction {
	int d;
	int horiz_overflow;
};
const Direction N = {8, 0};
*/

typedef enum Direction { N = 8, S = -8, E = 1 , O = -1, NE = 9, NO = 7, SE = -7, SE = -9 };
char KNIGHT_MOVES[8] = {
	8+8+1, 8+2, -8+2, -8-8+1,
	8+8-1, 8-2, -8-2, -8-8-1
};

// Primera pieza desde una posición en una dirección.
// Se proyecta la dirección buscando la primera pieza desde la posición
// Se devuelve la primera posición en esa dirección con una pieza o -1 si no hay posible
int firstPieceAt(ChessSt *game, unsigned char from, Direction dir) {
	unsigned char best, before, next, before_col, next_col;
	before = from;
	before_col = COLUMN(before);
	best = -1;
	while(1) {
		next = before + dir;
		next_col = COLUMN(next);
		if(ABS(next_col - before_col) > 1 || OOB(next)) return best; //out of board
		if(PIECE_AT(game, next)) return next;
		before = next;
		before_col = next_col;
	}
}

int validMove(ChessSt *game, Move move) {
	//1. la posición de partida del movimiento debe tener pieza del color del turno
	//2. la posición de llegada del mov. NO puede tener pieza del color del turno
	//3. posiciones deben guardar tipo de movimiento válido en relación al tipo de pieza
	//4. la jugada no debe dejar al color del turno en estado de jaque
	int valid = MYCOLOR_AT(game, POS1(move)) 
				&& !(PIECE_COLOR(PIECE_AT(game, POS2(move))) & CURR_TURN(game))
	int valid = game->piece_present[CURR_TURN(game)] & (1 << (move & MOVE1_POSMK));
}

int isValidCastle(ChessSt *game, unsigned char pos1, unsigned char pos2) {
	unsigned char col, row;
	int dpos;
	Color color;
	int i;
	col = COLUMN(pos1);
	row = ROW(pos1);
	dpos = pos1 - pos2;
	color = TURN(game);
	//5.1. movimiento enroque
	if((col == 4 && (row == color?7:0)) && ABS(dpos) == 2) {				
		if(dpos > 0) { //5.1.1. enroque largo
			if(!CANCASTLE(game, LEFT)) return -1;
			for(i = 1; i < 4; i++) {
				if(OCUPPIED(game, pos1 - i)) return -1;
			}
			for(i = 0; i < 5; i++) {
				if(isMenaced(game, pos1 - i, OPONENT(color))) return -1;
			}
		} else { //5.1.2. enroque corto
			//XXX aquí me quedé, refactorizar y unir los dos casos
		}
	}
}

int validPieceMove(ChessSt *game, unsigned char pos1, unsigned char pos2, unsigned char piece) {
	int i, j;
	unsigned char p, col, row;
	Color color = TURN(game);
	col = COLUMN(pos1);
	switch(piece) {
		// 1. si es torre, el camino entre posición 1 y posición 2 por alguna de las direcciones {N, E, S, O} debe estar libre de piezas
		case Rook:
			return firstPieceAt(game, pos2, N) == pos1 ||
				firstPieceAt(game, pos2, E) == pos1 ||
				firstPieceAt(game, pos2, S) == pos1 ||
				firstPieceAt(game, pos2, O) == pos1;
		// 2. si es alfil, " " " " por las direcciones {NE, SE, SO, NO}
		case Bishop:
			return firstPieceAt(game, pos2, NE) == pos1 ||
				firstPieceAt(game, pos2, SE) == pos1 ||
				firstPieceAt(game, pos2, SO) == pos1 ||
				firstPieceAt(game, pos2, NO) == pos1;
		// 3. si es reina, " " por todas las direcciones
		case Queen:
			return validPieceMove(game, pos1, pos2, Rook) ||
				validPieceMove(game, pos1, pos2, Bishop);
		// 4. si es caballo, la posición 2 debe estar en un conjunto de posibles determinadas por posición 1
		case Knight:
			for(i = 0; i < 8; i++) {
				p = KNIGHT_MOVES[i] + pos1; 
				if(pos2 == p && !(ABS(col - COLUMN(p)) > 2 || OOB(p))) return 1;
			}
			return 0;
		case King:
			//5. movimiento normal del rey
			for(i = -1; i <= 1; i++)
				for(j = -1; j <= 1; j++) {
					if(!i && !j) continue; //no movimiento
					p = pos1 + j + i<<3;
					if(pos2 == p && !(ABS(col - COLUMN(p)) > 2 || OOB(p))) return 1;
				}
			

	}

}