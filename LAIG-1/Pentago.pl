:- use_module(library(random)).

/** Regra de jogo inicial */
play :-
	board(X),	
	show_board(X),
	play(1,X).

/** Regra para jogo de um jogador */
playSP :-
	board(X),	
	show_board(X),
	playSP(1,X).
    
board([' ',' ',' ',' ', ' ', ' ', ' ',' ',' ',' ', ' ', ' ',' ',' ',' ',' ', ' ', ' ',' ',' ',' ',' ', ' ', ' ',' ',' ',' ',' ', ' ', ' ',' ',' ',' ',' ', ' ', ' ']).

/** Verifica se alguem ganhou */
checkVictory(Board, Winner) :-  (checkDraw(Board), Winner is 3,!);(checkVictoryBlack(Board),Winner is 2,!);(checkVictoryWhite(Board),Winner is 1,!);Winner is 0.

/** Imprime no ecrã o vencedor */
writeWinner(Player) :- (Player =:= 1, write('Jogador 1 ganhou!'), !); write('Jogador 2 ganhou!'),!.

/** Regra de jogo principal que dá início a cada jogada */
play(_, Result) :- ((checkVictory(Result),!);(checkDraw(Result), nl, write('Empate!'),nl)). 
play(Player, Result):- readPiece(Player, Result).

/** Para Single Player */
playSP(Player, Result) :- (checkDraw(Result), nl, write('Empate!'),nl) ; (checkVictory(Result, _),!, writeWinner(Player), !). 
playSP(Player, Result):- readPieceSP(Player, Result).

/** 2 regras que verificam se o elemento de coordenadas X,Y no tabuleiro está livre */
verifyElement([L|_], 1) :- L == ' '; !,fail.
verifyElement([_|Ls], Z) :- Z > 1, Z1 is Z-1, verifyElement(Ls, Z1). 
freeSpace(Board, X, Y) :- Z is ((Y-1)*6+X), verifyElement(Board, Z).

/** Verificam se o input do utilizador é válido para a colocação de uma peça e rotação do quadrante, respectivamente */
verifyPiece(Player, Board,  X,Y) :- X > 0, X < 7, Y > 0, Y < 7, freeSpace(Board, X, Y), write('Input valido!'), nl; write('Input invalido!'), nl, readPiece(Player, Board).
verifyPieceSP(Player, Board,  X,Y) :- X > 0, X < 7, Y > 0, Y < 7, freeSpace(Board, X, Y), write('Input valido!'), nl; write('Input invalido!'), nl, readPieceSP(Player, Board).
verifyQuadrant(Player, Board,  Q,S) :- (Q > 0, Q < 5, S > 0, S < 3, write('Input valido!'), nl); write('Input invalido!'), nl, readQuadrant(Player, Board).

/** Lê o input do utilizador para colocar a peça */
readPiece(Player, Board) :- 
	(Player = 1 -> write('Jogador 1, Introduza as coordenadas da posicao onde quer inserir a peca: '), nl;
	 Player = 2 -> write('Jogador 2, Introduza as coordenadas da posicao onde quer inserir a peca: '), nl),
		write('X: '), read(X),
		write('Y: '), read(Y), nl,
		verifyPiece(Player, Board, X,Y),
		placePiece(Player, X, Y, Board).

/** Para Single Player */
readPieceSP(Player, Board) :- 
	(Player = 1 -> write('Jogador, Introduza as coordenadas da posicao onde quer inserir a peca: '), nl,
        write('X: '), read(X),
		write('Y: '), read(Y), nl,
		verifyPieceSP(Player, Board, X,Y);
	 Player = 2 -> generatePiece(Board, X, Y)),	
	placePieceSP(Player, X, Y, Board).

/** Gera uma posição para colocar a peça */
generatePiece(Board, X, Y):-
    (random(1,7,X), %random entre 1 e 6
    random(1,7,Y),
    freeSpace(Board, X, Y));
    generatePiece(Board, X, Y).

/** Lê o input do utilizador para rodar um quadrante */		
readQuadrant(Player, Board) :- 
	write('Introduza o quadrante a rodar (1-4): '), nl,
	write('Quadrant: '), read(Quadrant),
	write('Introduza o sentido (1-Ponteiros do relogio, 2-Sentido contrario): '), nl,
	write('Sentido: '), read(Sentido),
	verifyQuadrant(Player, Board, Quadrant,Sentido),
	rotateQuadrantAux(Player, Board, Quadrant, Sentido).
  
 /** Para Single Player */  
readQuadrantSP(Player, Board) :- 
	write('Introduza o quadrante a rodar (1-4): '), nl,
	write('Quadrant: '), read(Quadrant),
	write('Introduza o sentido (1-Ponteiros do relogio, 2-Sentido contrario): '), nl,
	write('Sentido: '), read(Sentido),
	verifyQuadrant(Player, Board, Quadrant,Sentido),
	rotateQuadrantAuxSP(Player, Board, Quadrant, Sentido).
    
/** Gera um valor para o quadrante a rodar e o sentido **/
generateQuadrant(Board, NL) :-
    random(1,5,Quadrant),
    random(1,3,Sentido),
    rotateQuadrantAuxSP(1, Board, Quadrant, Sentido, NL).

/** Tratam de efectuar as jogadas, colocar peça e rodar quadrante, respectivamente*/
placePiece(Player, X, Y, L) :- 
	 Player =:= 2 -> Z is ((Y-1)*6+X-1),  replace(L, Z, 'B', NL), show_board(NL), ((checkVictory(NL), writeWinner(Player)) ; readQuadrant(1, NL));
	 Player =:= 1 -> Z is ((Y-1)*6+X-1),  replace(L, Z, 'W', NL), show_board(NL), ((checkVictory(NL), writeWinner(Player)) ; readQuadrant(2, NL)).
rotateQuadrantAux(Player, Board, Quadrant, Sentido):-
	rotateQuadrant(Board, Quadrant, Sentido, NL), show_board(NL), play(Player, NL).
 
/** Para Single Player */  
placePieceSP(Player, X, Y, L) :- 
	 Player =:= 2 -> Z is ((Y-1)*6+X-1),  replace(L, Z, 'B', NL), ((checkVictory(NL), writeWinner(Player)) ; (nl, write('Jogada do computador'),nl, generateQuadrant(NL)));
	 Player =:= 1 -> Z is ((Y-1)*6+X-1),  replace(L, Z, 'W', NL), show_board(NL), ((checkVictory(NL), writeWinner(Player)) ; readQuadrantSP(2, NL)).
rotateQuadrantAuxSP(Player, Board, Quadrant, Sentido, NL):-
	rotateQuadrant(Board, Quadrant, Sentido, NL).
	 
/** Substitui uma posicao de indice I da lista recebida pelo elemento X */
replace([_|T], 0, X, [X|T]).
replace([H|T], I, X, [H|R]):- I > 0, I1 is I-1, replace(T, I1, X, R).

/** SHOW_BOARD e regra auxiliar - Imprimem tabuleiro Pentago */
show_board(Board) :-
    nl, write('       1           2           3            4           5           6          '),nl,
    write(' |-----------|-----------|-----------||-----------|-----------|-----------|'),nl,
    write('1|'),
    showAux(Board,1,1).

showAux(B,N,L):-
	L =:= 8,nl,
showAux(B,N,L).

showAux([H|T],18,L):-
	L1 is L + 1,
	write('     '), write(H), write('     '),
	write('|'),nl,
	write(' |-----------|-----------|-----------||-----------|-----------|-----------|'),nl,
	write(' |-----------|-----------|-----------||-----------|-----------|-----------|'),nl,
	write(L1), write('|'),
	showAux(T,19,L1).

showAux([H|T],N,L):-
	N mod 6 =:= 0,
	L1 is L + 1,
	L1 \= 7,
	write('     '), write(H), write('     '),
	write('|'),nl,
	write(' |-----------|-----------|-----------||-----------|-----------|-----------|'),nl,
	write(L1), write('|'),
	N1 is N + 1,
	showAux(T,N1,L1).

showAux([H|T],N,L):-
	( N mod 3 =:= 0, N \= 36 -> write('     '), write(H), write('     '),
	write('||');
	write('     '), write(H), write('     '),
	write('|')),
	N1 is N + 1,
	showAux(T,N1,L).

showAux([],_,_):-nl,
	write(' |-----------|-----------|-----------||-----------|-----------|-----------|'),
	nl,nl,nl.

checkDraw([]).
checkDraw([H|T]):-
	H \= ' ',
	checkDraw(T)
.

/** Predicados para verificação de vitória */
checkVictoryWhite(['W','W','W','W','W'|_]).
checkVictoryWhite([_,'W','W','W','W','W'|_]).
checkVictoryWhite([_,_,_,_,_,_,'W','W','W','W','W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,'W','W','W','W','W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,_,_,'W','W','W','W','W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,_,_,_,'W','W','W','W','W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'W','W','W','W','W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'W','W','W','W','W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'W','W','W','W','W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'W','W','W','W','W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'W','W','W','W','W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'W','W','W','W','W'|_]).

checkVictoryWhite(['W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W',_,_,_,_,_,'W'|_]).

checkVictoryWhite(['W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W',_,_,_,_,_,_,'W'|_]).

checkVictoryWhite([_,_,_,_,_,'W',_,_,_,_,'W',_,_,_,_,'W',_,_,_,_,'W',_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,'W',_,_,_,'W',_,_,_,'W',_,_,_,'W',_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,'W',_,_,_,_,'W',_,_,_,_,'W',_,_,_,_,'W',_,_,_,_,'W'|_]).
checkVictoryWhite([_,_,_,_,_,_,_,_,_,_,_,'W',_,_,_,_,'W',_,_,_,_,'W',_,_,_,_,'W',_,_,_,_,'W'|_]).

%--------------------------------------------------------------------------------------------

checkVictoryBlack(['B','B','B','B','B'|_]).
checkVictoryBlack([_,'B','B','B','B','B'|_]).
checkVictoryBlack([_,_,_,_,_,_,'B','B','B','B','B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,'B','B','B','B','B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,_,_,'B','B','B','B','B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,_,_,_,'B','B','B','B','B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'B','B','B','B','B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'B','B','B','B','B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'B','B','B','B','B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'B','B','B','B','B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'B','B','B','B','B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,'B','B','B','B','B'|_]).

checkVictoryBlack(['B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B',_,_,_,_,_,'B'|_]).

checkVictoryBlack(['B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B',_,_,_,_,_,_,'B'|_]).

checkVictoryBlack([_,_,_,_,_,'B',_,_,_,_,'B',_,_,_,_,'B',_,_,_,_,'B',_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,'B',_,_,_,'B',_,_,_,'B',_,_,_,'B',_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,'B',_,_,_,_,'B',_,_,_,_,'B',_,_,_,_,'B',_,_,_,_,'B'|_]).
checkVictoryBlack([_,_,_,_,_,_,_,_,_,_,_,'B',_,_,_,_,'B',_,_,_,_,'B',_,_,_,_,'B',_,_,_,_,'B'|_]).

/** Predicados para rotação dos quadrantes */
rotateQuadrant([A,B,C,E4,E5,E6,D,E,F,E10,E11,E12,G,H,I|T], 1, 1, [G,D,A,E4,E5,E6,H,E,B,E10,E11,E12,I,F,C|T]).
rotateQuadrant([A,B,C,E4,E5,E6,D,E,F,E10,E11,E12,G,H,I|T], 1, 2, [C,F,I,E4,E5,E6,B,E,H,E10,E11,E12,A,D,G|T]).

rotateQuadrant([E1,E2,E3,A,B,C,E7,E8,E9,D,E,F,E13,E14,E15,G,H,I|T], 2, 1, [E1,E2,E3,G,D,A,E7,E8,E9,H,E,B,E13,E14,E15,I,F,C|T]).
rotateQuadrant([E1,E2,E3,A,B,C,E7,E8,E9,D,E,F,E13,E14,E15,G,H,I|T], 2, 2, [E1,E2,E3,C,F,I,E7,E8,E9,B,E,H,E13,E14,E15,A,D,G|T]).

rotateQuadrant([E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15,E16,E17,E18,A,B,C,E22,E23,E24,D,E,F,E28,E29,E30,G,H,I|T], 3, 1, [E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15,E16,E17,E18,G,D,A,E22,E23,E24,H,E,B,E28,E29,E30,I,F,C|T]).
rotateQuadrant([E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15,E16,E17,E18,A,B,C,E22,E23,E24,D,E,F,E28,E29,E30,G,H,I|T], 3, 2, [E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15,E16,E17,E18,C,F,I,E22,E23,E24,B,E,H,E28,E29,E30,A,D,G|T]).

rotateQuadrant([E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15,E16,E17,E18,E19,E20,E21,A,B,C,E25,E26,E27,D,E,F,E31,E32,E33,G,H,I|T], 4, 1, [E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15,E16,E17,E18,E19,E20,E21,G,D,A,E25,E26,E27,H,E,B,E31,E32,E33,I,F,C|T]).
rotateQuadrant([E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15,E16,E17,E18,E19,E20,E21,A,B,C,E25,E26,E27,D,E,F,E31,E32,E33,G,H,I|T], 4, 2, [E1,E2,E3,E4,E5,E6,E7,E8,E9,E10,E11,E12,E13,E14,E15,E16,E17,E18,E19,E20,E21,C,F,I,E25,E26,E27,B,E,H,E31,E32,E33,A,D,G|T]).

boardTeste(['B','W','B','B', 'W', 'W', 'W','W','B','B', 'W', 'W','W','W','B','W', 'B', 'B','B','W','B','W', 'B', 'W','B','B','B','W', 'W', 'B','B','B','W','B', 'B', 'W']).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%                 Sockets                   %%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:-use_module(library(sockets)).

port(60070).

% launch me in sockets mode
server:-
	port(Port),
	socket_server_open(Port, Socket),
	socket_server_accept(Socket, _Client, Stream, [type(text)]),
	write('Accepted connection'), nl,
	serverLoop(Stream),
	socket_server_close(Socket).

% wait for commands
serverLoop(Stream) :-
	repeat,
	read(Stream, ClientMsg),
	write('Received: '), write(ClientMsg), nl,
	parse_input(ClientMsg, MyReply),
	format(Stream, '~q.~n', [MyReply]),
	write('Wrote: '), write(MyReply), nl,
	flush_output(Stream),
	(ClientMsg == quit; ClientMsg == end_of_file), !.

parse_input(comando(Arg1, Arg2), Answer) :-
	comando(Arg1, Arg2, Answer).

parse_input(checkVictory(Board), Answer) :-
	checkVictory(Board, Answer).
	
parse_input(computerPlacePiece(Board), Answer) :-
	generatePiece(Board, X, Y), !,
	Answer is ((Y-1)*6+X-1).
	
parse_input(computerRotateQuadrant(Board), Answer) :-
	generateQuadrant(Board, Answer).
	
parse_input(rotateQuadrant(Board, Quadrant, Direction), Answer) :-
	rotateQuadrant(Board, Quadrant, Direction, Answer).
	
/**parse_input().**/

parse_input(quit, ok-bye) :- !.
		
comando(Arg1, Arg2, Answer) :-
	write(Arg1), nl, write(Arg2), nl,
	Answer = 5.