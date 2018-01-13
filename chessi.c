#include "chessi.h"

void parseGame(ChessSt *game, char *str) {
	//XXXX próximo: parsear game de string,
	//RNBQKBNRPPPPPPPPXXXX...XXXXPPPPPPPPRNBQKBNR
}

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

int isValidCastle(ChessSt *game, unsigned char pos1, unsigned char pos2) {
	unsigned char col, row;
	int dpos, side, p0, count, i;
	Color color;
	col = COLUMN(pos1);
	row = ROW(pos1);
	dpos = pos1 - pos2;
	color = TURN(game);
	if((col != 4 || (row != color?7:0)) || ABS(dpos) != 2) return 0; //5.1. movimiento enroque
	side = dpos < 0; //dpos > 0 enroque largo = izqda, sino enroque corto
	p0 = side? pos1 : pos2 - 2; //posición menor, los recorridos los hacemos todos en sentido creciente
	count = side? 2 : 3; //cuenta de posiciones adicionales además de la del rey y la torre
	if(!CANCASTLE(game, side)) return 0;
	for(i = 1; i <= count; i++) {
		if(OCUPPIED(game, p0 + i)) return 0;
	}
	for(i = 0; i <= count + 2; i++) {
		if(isMenaced(game, p0 + i, OPONENT(color))) return 0;
	}
	return 1;
}

char KNIGHT_MOVES[8] = {
	8+8+1, 8+2, -8+2, -8-8+1,
	8+8-1, 8-2, -8-2, -8-8-1
};

int validPieceMove(ChessSt *game, unsigned char pos1, unsigned char pos2, unsigned char piece) {
	int i, j;
	unsigned char p, q, col, row, pawn_dir;
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
			return isValidCastle(game, pos1, pos2);
		case Pawn:
			pawn_dir = color ? S : N;
			//6.1. ó posición 2 = posición 1 + N/S, en posición 2 no debe haber pieza
			p = pos1 + pawn_dir;
			if(pos2 == p && !OCCUPIED(game, p)) return 1;
			//6.2. ó posición 2 = posición 1 + NE ó NO / SE ó SO, y en posición 2 debe haber pieza
			//6.3. ó posición 2 = posición 1 + NE ó NO / SE ó SO, y fila de posición 1 = 5/3 y pasante en columna de posicion 2
			row = ROW(pos1);
			if(pos2 == p + E || pos2 == p + O) {
				if(PRESENT(game, pos2, OPONENT(color))) return 1; //6.2
				col2 = COLUMN(pos2);
				if(row == (color? 3 : 5) && CANPASSANT(game, COLUMN(pos2))) return 1;
			}
			//6.4. ó fila de posición 1 = 2/6 y el camino entre p1 y p2 libre por dirección N/S y p2 libre
			if(row == (color? 6 : 2) && firstPieceAt(game, pos2, color ? N : S) == pos1) return 1;
			return 0;
	}
}

int validMove(ChessSt *game, Move move) {
	int pos1, pos2;
	Color color;
	pos1 = POS1(game, move);
	pos2 = POS2(game, move);
	color = TURN(game);
	//1. la posición de partida del movimiento debe tener pieza del color del turno
	if(COLOR(game, pos1) != color) return 0;
	//2. la posición de llegada del mov. NO puede tener pieza del color del turno
	if(COLOR(game, pos2) == color) return 0;
	//3. posiciones deben guardar tipo de movimiento válido en relación al tipo de pieza
	if(!validPieceMove(game, pos1, pos2, PIECE_AT(game, pos1))) return 0;
	//4. la jugada no debe dejar al color del turno en estado de jaque
}

