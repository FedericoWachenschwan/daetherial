#include "Pathfinder.h"
#include <cmath>
#include <algorithm>
#include <iostream>

// El tamaño de tus baldosas en el mapa (16)
const int TAMANO_TILE = 16;

// ============================================================================
// FUNCIÓN PRINCIPAL: El motor matemático del algoritmo A*
// ============================================================================
std::vector<sf::Vector2f> Pathfinder::calcularCamino(Map& mapa, sf::Vector2f posInicio, sf::Vector2f posDestino) {
    std::vector<sf::Vector2f> caminoFinal;

    // 1. Traducimos los píxeles exactos a coordenadas de grilla (Ej: el pixel 65,33 es la baldosa 2,1)
    sf::Vector2i inicioGrilla(static_cast<int>(posInicio.x) / TAMANO_TILE,
        static_cast<int>(posInicio.y) / TAMANO_TILE);

    sf::Vector2i destinoGrilla(static_cast<int>(posDestino.x) / TAMANO_TILE,
        static_cast<int>(posDestino.y) / TAMANO_TILE);

    // Si el destino es una pared, abortamos para no calcular al infinito
    if (!esCaminable(mapa, destinoGrilla)) {
        return caminoFinal; // Devuelve lista vacía
    }

    // 2. Creamos las listas de memoria del algoritmo
    std::vector<Nodo*> listaAbierta;
    std::vector<Nodo*> listaCerrada;

    // Metemos el nodo de origen para empezar a buscar
    Nodo* nodoInicial = new Nodo(inicioGrilla);
    listaAbierta.push_back(nodoInicial);

    // Las 4 direcciones posibles (Arriba, Abajo, Izquierda, Derecha)
    std::vector<sf::Vector2i> direcciones = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

    // 3. EL BUCLE PRINCIPAL (Busca mientras haya caminos posibles)
    while (!listaAbierta.empty()) {

        // A. Buscar el nodo con el menor costo F (el mejor candidato)
        auto iteradorMejorNodo = listaAbierta.begin();
        Nodo* nodoActual = *iteradorMejorNodo;

        for (auto it = listaAbierta.begin(); it != listaAbierta.end(); ++it) {
            if ((*it)->costoF < nodoActual->costoF) {
                nodoActual = *it;
                iteradorMejorNodo = it;
            }
        }

        // B. Lo sacamos de la Abierta y lo metemos en la Cerrada (ya lo estamos pisando)
        listaCerrada.push_back(nodoActual);
        listaAbierta.erase(iteradorMejorNodo);

        // C. ¡CONDICIÓN DE VICTORIA! Si el nodo actual es el destino, armamos la ruta
        if (nodoActual->posicionGrilla == destinoGrilla) {
            Nodo* actual = nodoActual;
            while (actual != nullptr) {
                // Traducimos la baldosa de vuelta a píxeles (centrado en la baldosa)
                float pixelX = (actual->posicionGrilla.x * TAMANO_TILE) + (TAMANO_TILE / 2.f);
                float pixelY = (actual->posicionGrilla.y * TAMANO_TILE) + (TAMANO_TILE / 2.f);

                caminoFinal.push_back(sf::Vector2f(pixelX, pixelY));
                actual = actual->padre; // Volvemos sobre nuestros pasos
            }
            // Como armamos el camino desde el final hacia el principio, lo damos vuelta
            std::reverse(caminoFinal.begin(), caminoFinal.end());
            break; // Salimos del bucle
        }

        // D. Revisar a los vecinos (Arriba, Abajo, Izquierda, Derecha)
        for (const auto& dir : direcciones) {
            sf::Vector2i posVecino = nodoActual->posicionGrilla + dir;

            // Si es una pared, lo ignoramos
            if (!esCaminable(mapa, posVecino)) continue;

            // Si ya está en la lista cerrada, lo ignoramos
            bool enCerrada = false;
            for (Nodo* n : listaCerrada) {
                if (n->posicionGrilla == posVecino) {
                    enCerrada = true;
                    break;
                }
            }
            if (enCerrada) continue;

            // Calculamos el costo G (El esfuerzo hasta el vecino es el esfuerzo actual + 1)
            int nuevoCostoG = nodoActual->costoG + 10; // Usamos 10 puntos por paso recto

            // Buscamos si el vecino ya estaba en la lista abierta
            Nodo* vecino = nullptr;
            bool enAbierta = false;
            for (Nodo* n : listaAbierta) {
                if (n->posicionGrilla == posVecino) {
                    vecino = n;
                    enAbierta = true;
                    break;
                }
            }

            // Si no estaba en la abierta, o si encontramos un camino más rápido hacia él
            if (!enAbierta || nuevoCostoG < vecino->costoG) {
                if (!enAbierta) {
                    vecino = new Nodo(posVecino);
                    listaAbierta.push_back(vecino);
                }

                // Actualizamos sus datos (F = G + H)
                vecino->padre = nodoActual; // Le decimos de dónde vino
                vecino->costoG = nuevoCostoG;
                vecino->costoH = calcularDistanciaHeuristica(vecino->posicionGrilla, destinoGrilla);
                vecino->costoF = vecino->costoG + vecino->costoH;
            }
        }
    }

    // 4. LIMPIEZA DE MEMORIA (¡Importantísimo en C++ para no tener Memory Leaks!)
    for (Nodo* n : listaAbierta) delete n;
    for (Nodo* n : listaCerrada) delete n;

    return caminoFinal;
}

// ============================================================================
// HEURÍSTICA: Distancia Manhattan (Ideal para movimientos en 4 direcciones)
// ============================================================================
int Pathfinder::calcularDistanciaHeuristica(sf::Vector2i nodoA, sf::Vector2i nodoB) {
    // Multiplicamos por 10 para mantener la escala con el costo G
    return (std::abs(nodoA.x - nodoB.x) + std::abs(nodoA.y - nodoB.y)) * 10;
}

// ============================================================================
// VALIDACIÓN: ¿La baldosa es pisable o hay una pared de colisión?
// ============================================================================
bool Pathfinder::esCaminable(Map& mapa, sf::Vector2i posGrilla) {
    // ESTRICTO: Fila = Y, Columna = X
    return !mapa.esSolido(posGrilla.y, posGrilla.x);
}