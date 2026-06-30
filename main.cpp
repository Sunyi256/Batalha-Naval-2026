#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>

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

            if (tamanhox > mapx) tamanhox = mapx;
            if (tamanhoy > mapy) tamanhoy = mapy;
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
            mapaGlobal[i][j] = '~';
        }
    }
}

// Imprime os Mapas
void printMap(char mapa[mapx][mapy]) {
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
            cout << mapa[i][j] << " ";
        }
        cout << endl;
    }
}

//recebe o "player" por referência para salvar no mapa do jogador correto
void posicionarNavio(player &p, navio* vetorNavios, int quantidade, int tamanho, char simbolo, string nomeNavio, bool bot) {
    char letraColuna, direcao;
    int numLinha;
    char ultimaLetraMaiuscula = 'A' + (tamanhox - 1);
    char ultimaLetraMinuscula = 'a' + (tamanhox - 1);

    for (int i = 0; i < quantidade; i++) {
        bool jogadaValida = false;

        while (!jogadaValida) {
            int indiceColuna = -1;

            if (bot) {
                numLinha = rand() % tamanhoy;
                indiceColuna = rand() % tamanhox;
                direcao = (rand() % 2 == 0) ? 'H' : 'V';
            } else {
                cout << "Posicionando " << nomeNavio << " " << i + 1 << " (Tamanho: " << tamanho << ")" << endl;
                cout << "Digite a coordenada inicial (Ex: C 3 para Coluna C, Linha 3): ";
                if (!(cin >> letraColuna >> numLinha)) {
                    // Limpa erro do cin caso digitem caracteres inválidos
                    cin.clear();
                    cin.ignore();
                    cout << "Entrada invalida! Use o formato Letra Numero (Ex: B 4)." << endl;
                    continue;
                }
                numLinha--; // Ajuste para índice 0 da matriz

                // Converte a letra da coluna em um índice numérico
                if (letraColuna >= 'A' && letraColuna <= ultimaLetraMaiuscula) indiceColuna = letraColuna - 'A';
                else if (letraColuna >= 'a' && letraColuna <= ultimaLetraMinuscula) indiceColuna = letraColuna - 'a';

                direcao = 'H';
                if (tamanho > 1) {
                    cout << "Digite a direcao (H para Horizontal, V para Vertical): ";
                    cin >> direcao;
                }
            }

            bool cabeNoMapa = true;
            if (indiceColuna == -1 || numLinha < 0 || numLinha >= tamanhoy) {
                cabeNoMapa = false;
            } 
            else if ((direcao == 'H' || direcao == 'h') && (indiceColuna + tamanho > tamanhox)) {
                cabeNoMapa = false; 
            } 
            else if ((direcao == 'V' || direcao == 'v') && (numLinha + tamanho > tamanhoy)) {
                cabeNoMapa = false; 
            } 
            else if (direcao != 'H' && direcao != 'h' && direcao != 'V' && direcao != 'v') {
                cabeNoMapa = false;
            }

            // Validação de colisão com outros navios no mapa de defesa do próprio jogador
            bool semColisao = true;
            if (cabeNoMapa) {
                for (int t = 0; t < tamanho; t++) {
                    int l;
                    if (direcao == 'V' || direcao == 'v') {
                        l = numLinha + t;
                    } else {
                        l = numLinha;
                    }
                
                    int c;
                    if (direcao == 'H' || direcao == 'h') {
                        c = indiceColuna + t;
                    } else {
                        c = indiceColuna;
                    } 

                    if (p.map.def[l][c] != '~') {
                        semColisao = false;
                    }
                }
            }

            if (cabeNoMapa && semColisao) {
                for (int t = 0; t < tamanho; t++) {
                    int l;
                    if (direcao == 'V' || direcao == 'v') {
                        l = numLinha + t;
                    } else {
                        l = numLinha;
                    }
                    int c;
                    if (direcao == 'H' || direcao == 'h') {
                        c = indiceColuna + t;
                    } else {
                        c = indiceColuna;
                    }
                    vetorNavios[i].partes[t].x = l;
                    vetorNavios[i].partes[t].y = c;
                    p.map.def[l][c] = simbolo;
                }
                jogadaValida = true;
                if (!bot) cout << "Navio posicionado com sucesso!" << endl;
            } else {
                if (!bot) cout << "Erro! Posicao/Direcao invalida ou ha outro navio no caminho. Tente novamente." << endl;
            }
        }
    }
}

void inputNavio(player &p, bool bot) {
    posicionarNavio(p, p.destroyers, qtdDestroyers, 1, 'D', "Destroyer", bot);
    posicionarNavio(p, p.cruzadores, qtdCruzadores, 2, 'C', "Cruzador", bot);
    posicionarNavio(p, p.portaavioes, qtdPortaavioes, 3, 'P', "Porta-Avioes", bot);
}

//Verifica qual jogador venceu
void verificarVitoria(player &defensor, bool &statusJogo, string nomeGanhador) {
    for (int i = 0; i < tamanhoy; i++) {
        for (int j = 0; j < tamanhox; j++) {
            if (defensor.map.def[i][j] == 'D' || defensor.map.def[i][j] == 'C' || defensor.map.def[i][j] == 'P') {
                return; 
            }
        }
    }
    cout << "   FIM DE JOGO! " << nomeGanhador << " VENCEU A BATALHA!" << endl;
    statusJogo = false; 
}

void realizarAtaque(player &atacante, player &defensor, bool bot) {
    char letraColuna;
    int numLinha, indiceColuna = -1;

    if (bot) {
        do {
            numLinha = rand() % tamanhoy;
            indiceColuna = rand() % tamanhox;
        } while (atacante.map.atk[numLinha][indiceColuna] != '~');
        
        cout << "A maquina atirou em " << 'A' + indiceColuna << " " << numLinha + 1 << endl;
    } else {
        bool coordenadaValida = false;
        while (!coordenadaValida) {
            cout << "Digite a coordenada do tiro (Ex: B 4): ";
            if (!(cin >> letraColuna >> numLinha)) {
                cin.clear();
                cin.ignore();
                cout << "Entrada invalida!" << endl;
                continue;
            }
            numLinha--;

            if (letraColuna >= 'A' && letraColuna <= 'A' + (tamanhox - 1)) indiceColuna = letraColuna - 'A';
            else if (letraColuna >= 'a' && letraColuna <= 'a' + (tamanhox - 1)) indiceColuna = letraColuna - 'a';

            if (indiceColuna >= 0 && numLinha >= 0 && numLinha < tamanhoy) {
                if (atacante.map.atk[numLinha][indiceColuna] == '~') {
                    coordenadaValida = true;
                } else {
                    cout << "Voce ja atirou ai! Escolha outra coordenada." << endl;
                }
            } else {
                cout << "Coordenada fora do mapa! Tente novamente." << endl;
            }
        }
    }

    char alvo = defensor.map.def[numLinha][indiceColuna];

    if (alvo == '~') {
        cout << "AGUA!" << endl;
        atacante.map.atk[numLinha][indiceColuna] = 'O'; 
        defensor.map.def[numLinha][indiceColuna] = 'O';
    } else {
        cout << "FOGO! Um navio foi atingido!" << endl;
        atacante.map.atk[numLinha][indiceColuna] = 'X'; 
        defensor.map.def[numLinha][indiceColuna] = 'X';
    }
}

int main() {
    srand(time(NULL));
    menu();
    if (!run){
        return 0;
    }
    
    int modoJogo;
    cout << "Escolha o modo de jogo: " << endl << "1 - Contra outro jogador" << endl << "2 - Contra a Maquina Opcao: " << endl;
    cin >> modoJogo;
    fillMap();
    
    cout << "--- JOGADOR 1: POSICIONE SEUS NAVIOS ---" << endl;
    inputNavio(player1, false);
    printMap(player1.map.def); // Mostra como ficou o mapa do P1
    
    if (modoJogo == 1) {
        cout << "--- JOGADOR 2: POSICIONE SEUS NAVIOS ---" << endl;
        inputNavio(player2, false);
        printMap(player2.map.def);
    } else {
        cout << "--- MAQUINA POSICIONANDO NAVIOS... ---" << endl;
        inputNavio(player2, true);
        cout << "Navios da maquina posicionados!" << endl;
    }

    // Loop de ataques
    bool jogoRodando = true;
    int turno = 1;

    while (jogoRodando) {
        if (turno == 1) {
            cout << "=== TURNO DO JOGADOR 1 ===" << endl;
            cout << "Seu mapa de ATAQUES dados ao inimigo:" << endl;
            printMap(player1.map.atk); 
            
            realizarAtaque(player1, player2, false);
            verificarVitoria(player2, jogoRodando, "JOGADOR 1");
            
            turno = 2;
        } 
        else {
            cout << "=== TURNO DO OPONENTE ===" << endl;
            if (modoJogo == 1) {
                cout << "Seu mapa de ATAQUES dados ao inimigo (Jogador 2):" << endl;
                printMap(player2.map.atk);
                realizarAtaque(player2, player1, false);
                verificarVitoria(player1, jogoRodando, "JOGADOR 2");
            } else {
                realizarAtaque(player2, player1, true); 
                verificarVitoria(player1, jogoRodando, "A MAQUINA");
            }
            
            turno = 1;
        }
    }

    return 0;
}