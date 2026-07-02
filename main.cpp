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

struct Habilidades {
    int cooldownCruz;
    int cooldownArea;
};

struct player {
    string nome; 
    int turnosJogados; 
    Habilidades habilidades; 
    navio destroyers[qtdDestroyers];
    navio cruzadores[qtdCruzadores]; 
    navio portaavioes[qtdPortaavioes];

    struct map {
        char def[mapx][mapy];
        char atk[mapx][mapy];
    } map;
} player1, player2;

struct RegistroRanking {
    string nome;
    int turnos;
};

const int MAX_RANKING = 100;
RegistroRanking ranking[MAX_RANKING];
int qtdRegistrosRanking = 0;

void atualizarRanking(string nome, int turnos) {
    if (qtdRegistrosRanking < MAX_RANKING) {
        ranking[qtdRegistrosRanking].nome = nome;
        ranking[qtdRegistrosRanking].turnos = turnos;
        qtdRegistrosRanking++;
        for (int i = 0; i < qtdRegistrosRanking - 1; i++) {
            for (int j = 0; j < qtdRegistrosRanking - i - 1; j++) {
                if (ranking[j].turnos > ranking[j + 1].turnos) {
                    RegistroRanking aux = ranking[j];
                    ranking[j] = ranking[j + 1];
                    ranking[j + 1] = aux;
                }
            }
        }
    }
}

// Exibe a tabela de líderes
void exibirRanking() {
    cout << "===== RANKING DE VITORIAS (Menos Rodadas) =====" << endl;
    if (qtdRegistrosRanking == 0) {
        cout << "Nenhum recorde registrado ainda!" << endl;
    } else {
        for (int i = 0; i < qtdRegistrosRanking; i++) {
            cout << i + 1 << "º Lugar: " << ranking[i].nome << " - " << ranking[i].turnos << " rodadas." << endl;
        }
    }
}

// Imprime o menu e permite configurar o tamanho do mapa
void menu() {
    if (!run) {
        exibirRanking(); // Exibe o ranking sempre antes de começar
        cout << "Seja bem-vindo ao Jogo: Batalha Naval!" << endl;
        cout << "Deseja iniciar o jogo? (S): ";
        char escolha;
        cin >> escolha;
        if (escolha == 'S' || escolha == 's') {
            run = true;
            
            cout << "Digite o nome do Jogador 1: ";
            cin >> player1.nome;

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

//Verifica qual jogador venceu (Mantido o padrão original, recebendo a struct correspondente)
void verificarVitoria(player &defensor, bool &statusJogo, string nomeGanhador, int turnosVencedor) {
    for (int i = 0; i < tamanhoy; i++) {
        for (int j = 0; j < tamanhox; j++) {
            if (defensor.map.def[i][j] == 'D' || defensor.map.def[i][j] == 'C' || defensor.map.def[i][j] == 'P') {
                return; 
            }
        }
    }
    cout << "FIM DE JOGO!" << nomeGanhador << "VENCEU A BATALHA EM " << turnosVencedor << " RODADAS!" << endl;
    atualizarRanking(nomeGanhador, turnosVencedor);
    statusJogo = false; 
}

// Processa o impacto de forma segura dentro dos limites do tabuleiro
void processarImpactoTiro(player &atacante, player &defensor, int l, int c) {
    if (l >= 0 && l < tamanhoy && c >= 0 && c < tamanhox) {
        char alvo = defensor.map.def[l][c];
        if (alvo == '~') {
            if (atacante.map.atk[l][c] == '~') {
                atacante.map.atk[l][c] = 'O'; 
                defensor.map.def[l][c] = 'O';
            }
        } else if (alvo == 'D' || alvo == 'C' || alvo == 'P') {
            atacante.map.atk[l][c] = 'X'; 
            defensor.map.def[l][c] = 'X';
            cout << "FOGO em [" << char('A' + c) << " " << l + 1 << "]! Navio atingido!" << endl;
        }
    }
}

void realizarAtaque(player &atacante, player &defensor, bool bot) {
    char letraColuna;
    int numLinha, indiceColuna = -1;
    int tipoTiro = 1; // 1: Normal, 2: Cruz, 3: Área 3x3

    // Reduz os tempos de recarga no início da rodada do atacante
    if (atacante.habilidades.cooldownCruz > 0) atacante.habilidades.cooldownCruz--;
    if (atacante.habilidades.cooldownArea > 0)  atacante.habilidades.cooldownArea--;

    if (bot) {
        // Inteligência básica do bot para usar os tiros especiais
        if (atacante.habilidades.cooldownArea == 0) {
            tipoTiro = 3;
            atacante.habilidades.cooldownArea = 5;
        } else if (atacante.habilidades.cooldownCruz == 0) {
            tipoTiro = 2;
            atacante.habilidades.cooldownCruz = 3;
        } else {
            tipoTiro = 1;
        }

        do {
            numLinha = rand() % tamanhoy;
            indiceColuna = rand() % tamanhox;
        } while (atacante.map.atk[numLinha][indiceColuna] != '~');
        
        cout << "A maquina usou o Disparo Tipo " << tipoTiro << " em " << char('A' + indiceColuna) << " " << numLinha + 1 << endl;
    } else {
        // Menu de tipos de tiros para o jogador humano
        cout << "Escolha o tipo de disparo para esta rodada:" << endl;
        cout << "1 - Tiro Comum (1 bloco)" << endl;
        
        if (atacante.habilidades.cooldownCruz == 0) cout << "2 - Tiro em Cruz [+] (Pronto)" << endl;
        else cout << "2 - Tiro em Cruz [+] (Recarregando: " << atacante.habilidades.cooldownCruz << " rodadas)" << endl;

        if (atacante.habilidades.cooldownArea == 0) cout << "3 - Bomba em Area 3x3 [■] (Pronto)" << endl;
        else cout << "3 - Bomba em Area 3x3 [■] (Recarregando: " << atacante.habilidades.cooldownArea << " rodadas)" << endl;
        
        cout << "Opcao: ";
        cin >> tipoTiro;

        // Se tentar usar um tiro em recarga, reverte para o normal
        if (tipoTiro == 2 && atacante.habilidades.cooldownCruz > 0) {
            cout << "Habilidade em recarga! Disparando tiro normal." << endl;
            tipoTiro = 1;
        } else if (tipoTiro == 3 && atacante.habilidades.cooldownArea > 0) {
            cout << "Habilidade em recarga! Disparando tiro normal." << endl;
            tipoTiro = 1;
        }

        // Ativa o cooldown para os turnos seguintes
        if (tipoTiro == 2) atacante.habilidades.cooldownCruz = 4; 
        if (tipoTiro == 3) atacante.habilidades.cooldownArea = 6;

        bool coordenadaValida = false;
        while (!coordenadaValida) {
            cout << "Digite a coordenada central do tiro (Ex: B 4): ";
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
                coordenadaValida = true;
            } else {
                cout << "Coordenada fora do mapa! Tente novamente." << endl;
            }
        }
    }

    // Incrementa a rodada jogada pelo atacante
    atacante.turnosJogados++;

    // Aplicação dos tipos de tiros baseado na escolha
    if (tipoTiro == 1) {
        char alvoAnterior = defensor.map.def[numLinha][indiceColuna];
        processarImpactoTiro(atacante, defensor, numLinha, indiceColuna);
        if (alvoAnterior == '~') cout << "AGUA!" << endl;
    } 
    else if (tipoTiro == 2) {
        // Formato em cruz (+)
        processarImpactoTiro(atacante, defensor, numLinha, indiceColuna);     // Centro
        processarImpactoTiro(atacante, defensor, numLinha - 1, indiceColuna); // Cima
        processarImpactoTiro(atacante, defensor, numLinha + 1, indiceColuna); // Baixo
        processarImpactoTiro(atacante, defensor, numLinha, indiceColuna - 1); // Esquerda
        processarImpactoTiro(atacante, defensor, numLinha, indiceColuna + 1); // Direita
    } 
    else if (tipoTiro == 3) {
        // Formato Quadrado 3x3
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                processarImpactoTiro(atacante, defensor, numLinha + i, indiceColuna + j);
            }
        }
    }
}

int main() {
    srand(time(NULL));
    
    // Loop para permitir rodar o jogo várias vezes sem perder o ranking da sessão
    while (true) {
        run = false;
        menu();
        if (!run){
            return 0;
        }
        
        int modoJogo;
        cout << "Escolha o modo de jogo: " << endl << "1 - Contra outro jogador" << endl << "2 - Contra a MaquinaOpcao: " << endl;
        cin >> modoJogo;
        
        // Configuração inicial de nomes e variáveis de controle
        player1.turnosJogados = 0;
        player1.habilidades.cooldownCruz = 0;
        player1.habilidades.cooldownArea = 0;

        if (modoJogo == 1) {
            cout << "Digite o nome do Jogador 2: ";
            cin >> player2.nome;
        } else {
            player2.nome = "A MAQUINA";
        }
        player2.turnosJogados = 0;
        player2.habilidades.cooldownCruz = 0;
        player2.habilidades.cooldownArea = 0;

        fillMap();
        
        cout << "--- " << player1.nome << ": POSICIONE SEUS NAVIOS ---" << endl;
        inputNavio(player1, false);
        printMap(player1.map.def);
        
        if (modoJogo == 1) {
            cout << "--- " << player2.nome << ": POSICIONE SEUS NAVIOS ---" << endl;
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
                cout << "=== TURNO DE: " << player1.nome << " ===" << endl;
                cout << "Seu mapa de ATAQUES dados ao inimigo:" << endl;
                printMap(player1.map.atk); 
                
                realizarAtaque(player1, player2, false);
                verificarVitoria(player2, jogoRodando, player1.nome, player1.turnosJogados);
                
                turno = 2;
            } 
            else {
                cout << "=== TURNO DE: " << player2.nome << " ===" << endl;
                if (modoJogo == 1) {
                    cout << "Seu mapa de ATAQUES dados ao inimigo:" << endl;
                    printMap(player2.map.atk);
                    realizarAtaque(player2, player1, false);
                } else {
                    realizarAtaque(player2, player1, true); 
                }
                verificarVitoria(player1, jogoRodando, player2.nome, player2.turnosJogados);
                
                turno = 1;
            }
        }

        cout << "Deseja jogar novamente para atualizar o ranking? (S): ";
        char jogarNovamente;
        cin >> jogarNovamente;
        if (jogarNovamente != 'S' && jogarNovamente != 's') {
            exit(0);
        }
    }

    return 0;
}
