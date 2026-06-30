#include <iostream>
#include <ctime>
#include "functions.h"

using namespace std;

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
