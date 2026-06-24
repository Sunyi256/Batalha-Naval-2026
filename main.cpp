#include <iostream>
#include <ctime>

using namespace std;

//implementar multiplayer*
//Structs e Variáveis Globais
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

//Imprime o menu
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
void posicionarNavio(navio* vetorNavios, int quantidade, int tamanho, char simbolo, string nomeNavio) {
    char letraLinha, direcao;
    int numColuna;

    char ultimaLetraMaiuscula = 'A' + (mapy - 1);
    char ultimaLetraMinuscula = 'a' + (mapy - 1);

    for (int i = 0; i < quantidade; i++) { //Roda enquanto houverem naves a serem posicionadas
        bool jogadaValida = false;

        while (!jogadaValida) {
            cout << "Posicionando " << nomeNavio << " " << i + 1 << " (Tamanho: " << tamanho << ")" << endl;
            cout << "Digite a coordenada inicial (Ex: 3 C): ";
            cin >> numColuna >> letraLinha;

            int indiceLinha = -1;
            if (letraLinha >= 'A' && letraLinha <= ultimaLetraMaiuscula) indiceLinha = letraLinha - 'A';
            else if (letraLinha >= 'a' && letraLinha <= ultimaLetraMinuscula) indiceLinha = letraLinha - 'a';

            //Define se o navio será colocado na vertical ou horizontal e se ele cabe no mapa
            direcao = 'H';
            if (tamanho > 1) {
                cout << "Digite a direcao (H para Horizontal, V para Vertical): ";
                cin >> direcao;
            }

            bool cabeNoMapa = true;
            if (indiceLinha == -1 || numColuna < 0 || numColuna >= mapx) {
                cabeNoMapa = false;
            } 

            else if ((direcao == 'H' || direcao == 'h') && (numColuna + tamanho > mapx)) {
                cabeNoMapa = false; 
            } 

            else if ((direcao == 'V' || direcao == 'v') && (indiceLinha + tamanho > mapy)) {
                cabeNoMapa = false; 
            } 

            else if (direcao != 'H' && direcao != 'h' && direcao != 'V' && direcao != 'v') {
                cabeNoMapa = false;
            }

            bool semColisao = true;
            if (cabeNoMapa) {
                for (int t = 0; t < tamanho; t++) {
                    int l = (direcao == 'V' || direcao == 'v') ? indiceLinha + t : indiceLinha;
                    int c = (direcao == 'H' || direcao == 'h') ? numColuna + t : numColuna;
                    if (map[l][c] != '~') {
                        semColisao = false;
                        break;
                    }
                }
            }

            if (cabeNoMapa && semColisao) { //Define se o espaço está vazio ou com um navio
                for (int t = 0; t < tamanho; t++) {
                    int l = (direcao == 'V' || direcao == 'v') ? indiceLinha + t : indiceLinha;
                    int c = (direcao == 'H' || direcao == 'h') ? numColuna + t : numColuna;

                    vetorNavios[i].partes[t].x = l;
                    vetorNavios[i].partes[t].y = c;
                    map[l][c] = simbolo; 
                }
                jogadaValida = true;
            } else {
                cout << "Erro! Posicao/Direcao invalida ou ha outro navio no caminho. Tente novamente." << endl;
            }
        }
    }
}

//define todos os inputs de todos os navios na função posicionarNavio
void inputNavio() {
    posicionarNavio(player1.destroyers, qtdDestroyers, 1, 'D', "Destroyer");
    posicionarNavio(player1.cruzadores, qtdCruzadores, 2, 'C', "Cruzador");
    posicionarNavio(player1.portaavioes, qtdPortaavioes, 3, 'P', "Porta-Avioes");
}

int main(){
	menu();
	for (int i = 0; i < mapx; ++i) {
        for (int j = 0; j < mapy; ++j) {
            map[i][j] = '~';
        }
    }
	while(run){
		printMap();
        inputNavio();
		printMap();
        cout << "Posicionamento concluido!" << endl;
        run = false;
	}
}
