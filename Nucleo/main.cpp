#include "GameManager.h"
#include <cstdlib>


//OBJETIVO PRINCIPAL: Arquitectura del programa ::
//-------------------------------------------------------------------------------------------------------------------


int main() {
	system("chcp 65001 > nul"); // Configuramos la consola para ver (Emojis) en la consola.
    GameManager game; // GameManager es nuestro Motor del juego;
    game.run();       // encendemos el motor

    return 0;
}