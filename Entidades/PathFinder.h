#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "map.h"

///=================================================================///
///   NODO - Representa un casillero de la grilla del mapa mientras
///   se calcula el camino. En vez de "apuntar" a su padre con un
///   puntero, guarda el NUMERO DE POSICION (el indice) de su padre
///   dentro de la lista de nodos. -1 significa "no tengo padre,
///   soy el primer casillero del camino"
///=================================================================///
struct Nodo {
    sf::Vector2i posicion_en_la_grilla; // Posicion del casillero en la grilla del mapa
    int costo_de_caminos_recorridos = 0; // Cuanto "esfuerzo" costo llegar hasta aca (G)
    int costo_estimado_hasta_el_destino = 0; // Adivinanza de cuanto falta para el destino (H)
    int costo_total = 0; // La suma de los dos anteriores (F). El A* siempre elige el mas bajo
    int indice_del_padre = -1; // De que casillero vinimos para llegar a este. -1 = no tiene padre
    bool esta_en_la_lista_abierta = false; // true si todavia falta explorarlo
    bool esta_en_la_lista_cerrada = false; // true si ya lo exploramos y descartamos
};

///=================================================================///
///   PATH_FINDER - El "GPS" del Golem. Calcula el camino mas corto
///   entre dos puntos del mapa esquivando paredes (algoritmo A*)
///
///   ACLARACION IMPORTANTE: esta es la UNICA clase de todo el
///   proyecto que usa una lista que crece sola (vector), porque
///   el algoritmo necesita explorar una cantidad de casilleros que
///   no se puede saber de antemano. No usa ningun puntero: cada
///   Nodo se guarda directamente por valor en la lista, y para
///   relacionar un nodo con su padre se guarda un numero de
///   indice, no una direccion de memoria. El resultado final (el
///   camino encontrado) se entrega en un array fijo, igual que en
///   el resto del proyecto
///=================================================================///
class PathFinder {
public:

    static const int CANTIDAD_MAXIMA_DE_PASOS_EN_UN_CAMINO = 64; // Maximo de puntos en un camino calculado

 // #1
    static int calcular_camino(Map& mapa_del_juego, sf::Vector2f posicion_de_inicio, sf::Vector2f posicion_de_destino, sf::Vector2f camino_resultado[CANTIDAD_MAXIMA_DE_PASOS_EN_UN_CAMINO]);

private:
 // #2
    static int calcular_distancia_estimada_entre_dos_casilleros(sf::Vector2i casillero_a, sf::Vector2i casillero_b);
 // #3
    static bool el_casillero_se_puede_caminar(Map& mapa_del_juego, sf::Vector2i posicion_del_casillero);
};
