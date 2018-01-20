# Diseño en pseudocódigo de ajedrez

http://buildingskills.itmaybeahack.com/book/python-2.6/html/p05/p05c06_chess.html

## Algoritmo principal

 inicializar estado
 repetir
	leer jugada
	si no es jugada válida => error (o volver a leer, etc)
	si es jugada válida
		hacer jugada (calcular nuevo estado)
	hasta fin de partida

### es jugada valida
	parámetros: jugada = posicion, posicion
	condiciones:
	1. posición 1 debe contener pieza de color del turno
	2. posición 2 no puede contener pieza de color del turno
	3. el movimiento debe ajustarse al de la pieza
	4. la jugada no puede dejar en estado de jaque
### movimiento se ajusta a pieza
	parámetros: jugada, pieza
	1. si es torre, el camino entre posición 1 y posición 2 por alguna de las direcciones {N, E, S, O} debe estar libre de piezas
	2. si es alfil, " " " " por las direcciones {NE, SE, SO, NO}
	3. si es reina, " " por todas las direcciones
	4. si es caballo, la posición 2 debe estar en un conjunto de posibles determinadas por posición 1
	5. si es rey, ídem
		5.1. ó diferencia de posiciones = 2 y fila = 1/8
			5.1.1. diferencia de posiciones > 0 y posibilidad de enroque largo para jugador
			5.1.2. diferencia de posiciones < 0 y posibilidad de enroque corto para jugador
	6. si es peón, dir = N para blancas, S para negras
		6.1. ó posición 2 = posición 1 + N/S, en posición 2 no debe haber pieza
		6.2. ó posición 2 = posición 1 + NE ó NO / SE ó SO, y en posición 2 debe haber pieza
		6.3. ó posición 2 = posición 1 + NE ó NO / SE ó SO, y fila de posición 1 = 5/3 y pasante en columna de posicion 2
		6.4. ó fila de posición 1 = 2/6 y el camino entre p1 y p2 libre por dirección N/S y p2 libre
### jaque
	1. posición del rey del turno amenazada
### posición amenazada
	parámetros: posición, color
	1. camino entre posición en direcciones NESO no debe acabar en reina o torre de color contrario
	2. " " NE,SE,NO,SO alfil o reina de color contrario
	3. posiciones de caballo desde posición no deben contener caballo contrario
	4. rey contrario no adyacente
	5. posición + {NE,NO}/{SE,SO} no debe contener peón contrario
### posibilidad de enroque
	parámetros: color, tipo = largo, corto
	1. aún posibilidad de enroque concreto (se guarda en estado)
	2. y ninguna posición desde rey a torre amenazadas por color contrario ni ocupadas
### hacer jugada
	parámetros: jugada
	1. mover pieza de pos1 a pos2 eliminando lo que haya en pos2
	2. si posicion 1 = {pos de  4 torres y 2 reyes} y posibilidad de enroque actualizar posibilidades
	3. si pieza movimiento largo de peón marcar pasante(columna)
	4. si es peón comiendo en pasante eliminar peón contrario
	5. si es peón coronando sustituír por reina o caballo
	6. avanzar turno
### fin de partida
	= es mate o es tablas
### es mate
	1. posición rey amenazado
	2. todas posiciones de movimiento del rey ocupadas o amenazadas
	3. enumerar amenazas, si más de 1 es mate
	4. si es una o no está amenazada por el color del turno o lo está pero hacer esa jugada deja también en jaque
### es tablas
	1. hay sólo 2 reyes o combinación de piezas de tablas
	2. no se encuentra movimiento posible, no hace falta contemplar enroque o salida doble de peón
		2.1. basta probar movimientos mínimos
		2.2. para cada pieza es pieza movible?
### pieza movible <= problemático
	parámetros: posicion
	1. torre +1*{NESO}
	2. alfil +1*{NE,SE,NO,SO}
	3. caballo, movs caballo
	4. peon N o NE,NO comiendo /S SE SO
	5. rey y reina +1*todas

## Resumen de vocabulario
(Hay un "contexto" Chess implícito en todos, el estado)

	- pos1, pos2 :: Move -> Pos
	- turn :: Color
	- present? :: Color -> Bool
	- pieceAt :: Pos -> Piece
	- oob? :: Pos -> Bool -- Out Of Bounds
	- menaced? :: Pos x Color -> Maybe Pos
	- validMovePiece? :: Move x Piece -> Bool
	- validMove? :: Move -> Bool
	- castlingAvailable? :: CastlingType -> Bool
	- passant :: Maybe Int
	- makeMove :: Move -> ()
	- check?, mate?, draw? :: Bool
	- firstPieceAt :: Pos x Direction -> Maybe Pos
