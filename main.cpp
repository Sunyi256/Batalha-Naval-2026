#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

//implementar multiplayer*
const int mapx = 10;
const int mapy = 10;
char map[mapx][mapy];
bool run = false;
int qtdD = 0;

struct player{
	struct destroyer{
	int destroyerx;
	int destroyery;
	}
	destroyer;
}
player1;

void menu(){
	if(run == false){
		cout << "Seja bem-vindo a Batalha Naval!" << endl;
		cout << "Deseja iniciar o jogo?" << endl;
		cout << "S/N?";
		char escolha;
		cin >> escolha;
		if(escolha == 'S' || escolha == 's'){
			run = true;
		}else if(escolha == 'N' || escolha == 'n'){
			exit(0);
		}
	}
}

void preencherMap(){
	for (int i = 0; i < mapx; ++i) {
        for (int j = 0; j < mapy; ++j) {
            map[i][j] = '~';
        }
    }
}

//Imprimir o Mapa
void printMap() {
	cout << "   ";
    for (int i = 0; i < mapx; ++i) {
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < mapy; ++i) {
        char letrinha = 'A' + i; 
        cout << letrinha << "  ";

        for (int j = 0; j < mapx; ++j) {
            cout << map[i][j] << " ";
        }
        cout << endl;
	}
}

//Selecionar local das Embarcações
void inputNavio() {
    bool inputD = true;
    char letraLinha;
    int numColuna;

    // Calcula dinamicamente qual a última letra permitida baseada no tamanho do mapa
    char ultimaLetraMaiuscula = 'A' + (mapy - 1);
    char ultimaLetraMinuscula = 'a' + (mapy - 1);

    while (inputD) {
        cout << "Digite a posicao do Destroyer (Ex: 3 C): " << endl;
        cin >> numColuna >> letraLinha;

        int indiceLinha = -1;

        // Validação dinâmica da letra
        if (letraLinha >= 'A' && letraLinha <= ultimaLetraMaiuscula) {
            indiceLinha = letraLinha - 'A';
        } else if (letraLinha >= 'a' && letraLinha <= ultimaLetraMinuscula) {
            indiceLinha = letraLinha - 'a';
        } else {
            cout << "Letra invalida! Para este tamanho de mapa, use de A ate " << ultimaLetraMaiuscula << endl;
			cout << qtdD;
		}
        // Validação dinâmica do número da coluna
        if (numColuna >= 0 && numColuna < mapx) {
            player1.destroyer.destroyerx = indiceLinha;
            player1.destroyer.destroyery = numColuna;
            qtdD = qtdD + 1;
        } else {
            cout << "Numero invalido! Use de 0 ate " << (mapx - 1) << endl;
        }
    }
	if(qtdD > 3){
		inputD = false;
	}
    // Insere no mapa
    map[player1.destroyer.destroyerx][player1.destroyer.destroyery] = 'D';
}

int main(){
	menu();
	while(run){
		preencherMap();
		inputNavio();
		printMap();
	}
}