#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const int mapx = 10;
const int mapy = 10;
char map[mapx][mapy];
bool run = false;
const int qtdDestroyers = 1;
const int qtdCruzadores = 1;
const int qtdPortaavioes = 1;

struct coordenada{
	int x;
	int y;
};
struct navio{
    coordenada partes[3];
};
struct player{
	navio destroyers[qtdDestroyers];
    navio cruzadores[qtdCruzadores];
    navio portaavioes[qtdPortaavioes];
} player1;

void menu();

void printMap();

void posicionarNavio(navio* vetorNavios, int quantidade, int tamanho, char simbolo, string nomeNavio);

void inputNavio();

#endif