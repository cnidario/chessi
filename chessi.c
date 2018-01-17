#include "chessi.h"

Direction STRAIGHT_MOVES[] = { N, E, S, O };
Direction DIAGONAL_MOVES[] = { NO, NE, SE, SO };
char KNIGHT_MOVES[] = {
	8+8+1, 8+2, -8+2, -8-8+1,
	8+8-1, 8-2, -8-2, -8-8-1
};

int isCheck(ChessSt *game) {
	Color color;
	//1. posición del rey del turno amenazada
	color = TURN(game);
	return isMenaced(game, color? BKING_POS(game) : WKING_POS(game), OPONENT(color));
}

int isMenaced(ChessSt *game, unsigned char pos, Color color) {
	unsigned char p, col, dc, dr;
	int i, dp;
	Direction d;
	PieceType piece;
	//1. camino entre posición en direcciones NESO no debe acabar en reina o torre de color contrario
	for(i = 0; i < sizeof(STRAIGHT_MOVES)/sizeof(Direction); i++) {
		d = STRAIGHT_MOVES[i];
		p = firstPieceAt(game, pos, d);
		if(p != -1) {
			piece = PIECE_AT(game, p);
			if((piece == Rook || piece == Queen) && COLOR(game, p) == color)
				return p;
		}
	}
	//2. " " NE,SE,NO,SO alfil o reina de color contrario
	for(i = 0; i < sizeof(DIAGONAL_MOVES)/sizeof(Direction); i++) {
		d = DIAGONAL_MOVES[i];
		p = firstPieceAt(game, pos, d);
		if(p != -1) {
			piece = PIECE_AT(game, p);
			if((piece == Bishop || piece == Queen) && COLOR(game, p) == color)
				return p;
		}
	}
	//3. posiciones de caballo desde posición no deben contener caballo contrario
	col = COLUMN(pos);
	for(i = 0; i < sizeof(KNIGHT_MOVES)/sizeof(char); i++) {
		dp = KNIGHT_MOVES[i];
		p = pos + dp;
		if(!OVERFLOW_MOVE(col, p)) {
			piece = PIECE_AT(game, p);
			if(piece == Knight && COLOR(game, p) == color)
				return p;
		}
	}
	//4. rey contrario no adyacente
	p = color ? BKING_POS(game) : WKING_POS(game);
	dc = ABS(col - COLUMN(p));
	dr = ABS(ROW(pos) - ROW(p));
	if(dc <= 1 && dr <= 1 && (dc || dr))
		return p;
	//5. posición + {NE,NO}/{SE,SO} no debe contener peón contrario
	d = color ? N : S;
	p = pos + d + E;
	if(PIECE_AT(game, p) == Pawn && COLOR(game, p) == color)
		return p;
	p = pos + d + O;
	if(PIECE_AT(game, p) == Pawn && COLOR(game, p) == color)
		return p;
	return 0;
}
// Primera pieza desde una posición en una dirección.
// Se proyecta la dirección buscando la primera pieza desde la posición
// Se devuelve la primera posición en esa dirección con una pieza o -1 si no hay posible
int firstPieceAt(ChessSt *game, unsigned char from, Direction dir) {
	unsigned char before, next, before_col, next_col;
	before = from;
	before_col = COLUMN(before);
	while(1) {
		next = before + dir;
		next_col = COLUMN(next);
		if(ABS(next_col - before_col) > 1 || OOB(next)) return before == from? -1 : before; //out of board
		if(OCCUPIED(game, next)) return next;
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
		if(OCCUPIED(game, p0 + i)) return 0;
	}
	for(i = 0; i <= count + 2; i++) {
		if(isMenaced(game, p0 + i, OPONENT(color))) return 0;
	}
	return 1;
}

int validPieceMove(ChessSt *game, unsigned char pos1, unsigned char pos2, unsigned char piece) {
	int i, j;
	unsigned char p, q, col, col2, row, pawn_dir;
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
			for(i = 0; i < sizeof(KNIGHT_MOVES)/sizeof(char); i++) {
				p = KNIGHT_MOVES[i] + pos1; 
				if(pos2 == p && !OVERFLOW_MOVE(col, p)) return 1;
			}
			return 0;
		case King:
			//5. movimiento normal del rey
			for(i = -1; i <= 1; i++)
				for(j = -1; j <= 1; j++) {
					if(!i && !j) continue; //no movimiento
					p = pos1 + j + i<<3;
					if(pos2 == p && !OVERFLOW_MOVE(col, p)) return 1;
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
				if(row == STARTPAWNROW(!color) + pawn_dir && CANPASSANT(game, COLUMN(pos2))) return 1;
			}
			//6.4. ó fila de posición 1 = 2/6 y el camino entre p1 y p2 libre por dirección N/S y p2 libre
			if(row == STARTPAWNROW(color) && firstPieceAt(game, pos2, color ? N : S) == pos1) return 1;
			return 0;
	}
}

int validMove(ChessSt *game, Move move) {
	int pos1, pos2;
	Color color;
	ChessSt game2;
	pos1 = POS1(move);
	pos2 = POS2(move);
	color = TURN(game);
	//1. la posición de partida del movimiento debe tener pieza del color del turno
	if(COLOR(game, pos1) != color) return 0;
	//2. la posición de llegada del mov. NO puede tener pieza del color del turno
	if(PRESENT(game, pos2, color)) return 0;
	//3. posiciones deben guardar tipo de movimiento válido en relación al tipo de pieza
	if(!validPieceMove(game, pos1, pos2, PIECE_AT(game, pos1))) return 0;
	//4. la jugada no debe dejar al color del turno en estado de jaque
	copy(&game2, game);
	doMove(&game2, move);
	if(isCheck(&game2))
		return 0;
	return 1;
}

int doMove(ChessSt *game, Move move) {
	int pos1, pos2, col, row;
	Color color;
	PieceType piece;
	pos1 = POS1(move);
	pos2 = POS2(move);
	color = TURN(game);
	piece = PIECE_AT(game, pos1);
	unsetColor(game, pos1, color);
	setColor(game, pos2, color);
	setPiece(game, pos2, piece);
	col = COLUMN(pos1);
	if(CANCASTLE(game, LEFT) || CANCASTLE(game, RIGHT)) {
		if(piece == Rook && col == 0)
			unsetCastle(game, color, LEFT);
		else if(piece == Rook && col == 7)
			unsetCastle(game, color, RIGHT);
		else if(piece == King) {
			unsetCastle(game, color, LEFT);
			unsetCastle(game, color, RIGHT);
		}
	}
	if(piece == King) {
		setKing(game, pos2, color);
	}
	row = ROW(pos1);
	if(piece == Pawn && row == STARTPAWNROW(color) && ABS(ROW(pos2) - row) == 2)
		setPassant(game, col);
	else
		unsetPassant(game);
	setTurn(game, OPONENT(color));
}

