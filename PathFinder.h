#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "map.h" // Necesitamos tu mapa para leer dónde están las paredes

// ============================================================================
// 🌟 ESTRUCTURA NODO: Representa un cuadradito (baldosa) en la grilla del mapa
// ============================================================================
struct Nodo {
    sf::Vector2i posicionGrilla; // Coordenada (x, y) en la matriz lógica
    int costoG; // Esfuerzo desde el inicio
    int costoH; // Distancia estimada al destino
    int costoF; // G + H (El puntaje final)

    Nodo* padre; // El nodo anterior que pisamos para llegar a este (como migas de pan)

    // Constructor del nodo
    Nodo(sf::Vector2i pos, Nodo* p = nullptr)
        : posicionGrilla(pos), costoG(0), costoH(0), costoF(0), padre(p) {
    }
};

// ============================================================================
// 🌟 CLASE PATHFINDER: El GPS matemático
// ============================================================================
class Pathfinder {
public:
    // Es estático ('static') para que podamos llamarlo desde cualquier lado 
    // sin tener que crear una variable Pathfinder nueva todo el tiempo.
    static std::vector<sf::Vector2f> calcularCamino(Map& mapa, sf::Vector2f posInicio, sf::Vector2f posDestino);

private:
    // Funciones matemáticas internas para que el código quede limpio
    static int calcularDistanciaHeuristica(sf::Vector2i nodoA, sf::Vector2i nodoB);
    static bool esCaminable(Map& mapa, sf::Vector2i posGrilla);
};

/*Concepto general del Pathfinder : > Imaginemos que es un GPS.El algoritmo A* (A - Star)
funciona de la misma manera: el objetivo es llevar a la IA del punto A (el enemigo) al punto B (el personaje) calculando la ruta más corta, pero esquivando obstáculos.
Para lograrlo, a cada tile (baldosa) del mapa se le calcula un puntaje llamado COSTO F. El algoritmo siempre va a elegir caminar por donde le salga más barato. 
Este puntaje se calcula sumando el COSTO G (cansancio), que son los pasos reales que tiene que dar hasta llegar ahí, y el COSTO H (intuición),
que es una adivinanza de cuántos pasos faltan en línea recta hasta el destino usando la Distancia Manhattan (que es contar casilleros en forma de L, sin diagonales).
Los personajes se mueven en el plano X/Y, pero para la IA sería lentísimo pensar en píxeles. Entonces, la primera parte del código traduce el mundo real a una cuadrícula. 
Divide la posición por TAMANO_TILE (16) para saber en qué casillero exacto está parado. Acá hay una validación clave: si el algoritmo detecta que el casillero de DESTINO FINAL es una colisión (una pared), cancela todo de entrada para no calcular un viaje imposible a lo bobo. 
Si el destino está libre, arranca a buscar y simplemente rodea los obstáculos que se cruza en el camino.
También cuenta con una LISTA ABIERTA (Frontera), que son las baldosas que el algoritmo "ve" pero todavía no pisó (es como mirar a tu alrededor en la vida real),
y una LISTA CERRADA, que son las baldosas que ya pisó, analizó y descartó para no dar vueltas en círculos.
Lo que además implementamos fue un Costo de Incomodidad. De la forma tradicional, el NPC es tan vago que costea los bordes del mapa para acortar camino, 
lo que hace que parezca que se desliza rozando las paredes. Lo que hicimos es que el NPC mire un área de 5x5 a su alrededor; 
si detecta que hay paredes cerca, le suma puntos de castigo a esa baldosa. 
Como el A* siempre busca el puntaje más bajo, el personaje va a preferir caminar por el medio de los pasillos anchos en lugar de ir pegado a la pared 
(logrando un movimiento mucho más natural).
Y después, una vez que llega a la meta, usamos el concepto de Hansel y Gretel: ir leyendo las "migas de pan" nodo por nodo hacia atrás
(el padre del padre) para reconstruir el camino final. Finalmente, para hacerlo eficiente de verdad y que no nos mate la CPU ni la memoria RAM, 
el código hace una limpieza masiva con delete de todos esos nodos temporales que ya no sirven, liberando la memoria.*/