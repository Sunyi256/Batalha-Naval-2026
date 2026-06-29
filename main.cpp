#include <iostream>
#include <ctime>

using namespace std;

// Variáveis Globais
const int mapx = 99;
const int mapy = 99;
char mapaGlobal[mapx][mapy];
bool run = false;
const int qtdDestroyers = 1;
const int qtdCruzadores = 1;
const int qtdPortaavioes = 1;
int tamanhox, tamanhoy;

struct coordenada {
    int x;
    int y;
};

struct navio {
    coordenada partes[3];
};

struct player {
    navio destroyers[qtdDestroyers];
    navio cruzadores[qtdCruzadores];
    navio portaavioes[qtdPortaavioes];

    struct map {
        char def[mapx][mapy];
        char atk[mapx][mapy];
    } map;
} player1, player2;

// Imprime o menu e permite configurar o tamanho do mapa
void menu() {
    if (!run) {
        cout << "Seja bem-vindo ao Jogo: Batalha Naval!" << endl;
        cout << "Deseja iniciar o jogo? (S/N): ";
        char escolha;
        cin >> escolha;
        if (escolha == 'S' || escolha == 's') {
            run = true;
            cout << "Digite o tamanho do mapa (Largura e Altura, ex: 8 8): ";
            cin >> tamanhox >> tamanhoy;
        } else {
            exit(0);
        }
    }
}

// Inicializa e redimensiona os mapas com as novas variáveis mapx e mapy
void fillMap() {
    for(int i = 0; i < mapx; i++){
        for(int j = 0; j < mapy; j++){
            player1.map.def[i][j] = '~';
            player1.map.atk[i][j] = '~';
            player2.map.def[i][j] = '~';
            player2.map.atk[i][j] = '~';
        }
    }
}

// Imprime os Mapas
void printMap() {
    cout << "  ";
    for (int i = 0; i < tamanhox; i++) {
        char letrinha = 'A' + i;
        cout << letrinha << " ";
    }
    cout << endl;
    for (int i = 0; i < tamanhoy; i++) {
        if (i < 9) { 
            cout << i + 1 << " ";
        } else {
            cout << i + 1;
        }

        for (int j = 0; j < tamanhox; j++) {
            cout << player1.map.def[i][j] << " ";
        }
        cout << endl;
    }
}

void posicionarNavio(navio* vetorNavios, int quantidade, int tamanho, char simbolo, string nomeNavio) {
    char letraLinha, direcao;
    int numColuna;

    char ultimaLetraMaiuscula = 'A' + (mapx - 1);
    char ultimaLetraMinuscula = 'a' + (mapx - 1);

    for (int i = 0; i < quantidade; i++) {
        bool jogadaValida = false;

        while (!jogadaValida) {
            cout << "Posicionando " << nomeNavio << " " << i + 1 << " (Tamanho: " << tamanho << ")" << endl;
            cout << "Digite a coordenada inicial (Ex: 3 C): ";
            cin >> numColuna >> letraLinha;
            numColuna--; // Ajuste para índice 0 (se o usuário digita 1, vira 0)

            int indiceLinha = -1;
            if (letraLinha >= 'A' && letraLinha <= ultimaLetraMaiuscula) indiceLinha = letraLinha - 'A';
            else if (letraLinha >= 'a' && letraLinha <= ultimaLetraMinuscula) indiceLinha = letraLinha - 'a';

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

                    if (mapaGlobal[l][c] != '~') {
                        semColisao = false;
                        break;
                    }
                }
            }

            if (cabeNoMapa && semColisao) {
                for (int t = 0; t < tamanho; t++) {
                    int l = (direcao == 'V' || direcao == 'v') ? indiceLinha + t : indiceLinha;
                    int c = (direcao == 'H' || direcao == 'h') ? numColuna + t : numColuna;

                    vetorNavios[i].partes[t].x = l;
                    vetorNavios[i].partes[t].y = c;
                    mapaGlobal[l][c] = simbolo; 
                    player1.map.def[l][c] = simbolo;
                }
                jogadaValida = true;
            } else {
                cout << "Erro! Posicao/Direcao invalida ou ha outro navio no caminho. Tente novamente." << endl;
            }
        }
    }
}

void inputNavio() {
    posicionarNavio(player1.destroyers, qtdDestroyers, 1, 'D', "Destroyer");
    posicionarNavio(player1.cruzadores, qtdCruzadores, 2, 'C', "Cruzador");
    posicionarNavio(player1.portaavioes, qtdPortaavioes, 3, 'P', "Porta-Avioes");
}

int main() {
    srand(time(NULL));
    menu();
    while (run) {
        fillMap();
        printMap();
        inputNavio();
        printMap();
        cout << "Posicionamento concluido!" << endl;
        run = false;
    }
    return 0;
}
