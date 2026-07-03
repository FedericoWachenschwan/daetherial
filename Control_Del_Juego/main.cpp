#include "GameManager.h"
#include <cstdlib>

// #1
int main() {
    system("chcp 65001 > nul"); // UTF-8 en consola: tildes y ñ se muestran bien en cout
    GameManager juego; // crea el juego: ventana, texturas, musica y estado inicial
    ejecutar(juego); // bucle principal: eventos -> actualizar -> renderizar, hasta cerrar
    return 0; // programa termino sin errores
}