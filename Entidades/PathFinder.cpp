#include "PathFinder.h"
#include <cmath>

///=============================================================///
///   CALCULAR CAMINO - El algoritmo A*
///=============================================================///
int PathFinder::calcular_camino(Map& mapa_del_juego, sf::Vector2f posicion_de_inicio, sf::Vector2f posicion_de_destino, sf::Vector2f camino_resultado[CANTIDAD_MAXIMA_DE_PASOS_EN_UN_CAMINO]) {

    const int TAMANO_DE_CADA_CASILLERO_EN_PIXELES = 16; // Cada casillero del mapa mide 16x16 px

    sf::Vector2i casillero_de_inicio((int)posicion_de_inicio.x / TAMANO_DE_CADA_CASILLERO_EN_PIXELES, (int)posicion_de_inicio.y / TAMANO_DE_CADA_CASILLERO_EN_PIXELES); // Convierte pixeles a casillero de inicio
    sf::Vector2i casillero_de_destino((int)posicion_de_destino.x / TAMANO_DE_CADA_CASILLERO_EN_PIXELES, (int)posicion_de_destino.y / TAMANO_DE_CADA_CASILLERO_EN_PIXELES); // Convierte pixeles a casillero destino

    if (el_casillero_se_puede_caminar(mapa_del_juego, casillero_de_destino) == false) { // Si el destino es una pared
        return 0; // No hay camino posible
    }

    std::vector<Nodo> todos_los_nodos_que_exploramos; // Lista de todos los casilleros visitados

    Nodo nodo_inicial; // Crea el nodo del punto de inicio
    nodo_inicial.posicion_en_la_grilla = casillero_de_inicio; // Le asigna la posicion de inicio
    nodo_inicial.esta_en_la_lista_abierta = true; // Lo marca para explorar
    todos_los_nodos_que_exploramos.push_back(nodo_inicial); // Lo agrega a la lista

    sf::Vector2i direcciones_posibles[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; // Las 4 direcciones: arriba, abajo, izquierda, derecha

    int indice_del_nodo_destino_encontrado = -1; // -1 significa que todavia no llego al destino

    while (true) { // Repite hasta encontrar el camino o quedarse sin casilleros

        int indice_del_mejor_nodo_abierto = -1; // Casillero con menor costo total aun no explorado

        for (int i = 0; i < (int)todos_los_nodos_que_exploramos.size(); i++) { // Recorre todos los nodos
            if (todos_los_nodos_que_exploramos[i].esta_en_la_lista_abierta == true) { // Si todavia no fue explorado
                if (indice_del_mejor_nodo_abierto == -1 || todos_los_nodos_que_exploramos[i].costo_total < todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].costo_total) {
                    indice_del_mejor_nodo_abierto = i; // Guarda el de menor costo total
                }
            }
        }

        if (indice_del_mejor_nodo_abierto == -1) { // Si no queda ningun nodo por explorar
            break; // No se encontro camino, sale del bucle
        }

        todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].esta_en_la_lista_abierta = false; // Lo saca de pendientes
        todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].esta_en_la_lista_cerrada = true; // Lo marca como explorado

        if (todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].posicion_en_la_grilla == casillero_de_destino) { // Si es el destino
            indice_del_nodo_destino_encontrado = indice_del_mejor_nodo_abierto; // Guarda el indice del destino
            break; // Salio: encontro el camino
        }

        for (int i = 0; i < 4; i++) { // Revisa los 4 vecinos
            sf::Vector2i posicion_del_vecino = todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].posicion_en_la_grilla + direcciones_posibles[i]; // Calcula la posicion del vecino

            if (el_casillero_se_puede_caminar(mapa_del_juego, posicion_del_vecino) == false) { // Si el vecino es una pared
                continue; // Lo ignora y pasa al siguiente
            }

            int castigo_por_estar_cerca_de_paredes = 0; // Penalizacion por casilleros cerca de paredes
            for (int desplazamiento_y = -2; desplazamiento_y <= 2; desplazamiento_y++) { // Recorre filas cercanas
                for (int desplazamiento_x = -2; desplazamiento_x <= 2; desplazamiento_x++) { // Recorre columnas cercanas
                    if (mapa_del_juego.getEs_solido(posicion_del_vecino.y + desplazamiento_y, posicion_del_vecino.x + desplazamiento_x)) {
                        castigo_por_estar_cerca_de_paredes += 5; // Suma penalizacion por cada pared cercana
                    }
                }
            }

            int costo_de_llegar_hasta_el_vecino = todos_los_nodos_que_exploramos[indice_del_mejor_nodo_abierto].costo_de_caminos_recorridos + 10 + castigo_por_estar_cerca_de_paredes; // Costo G del vecino

            int indice_del_vecino = -1; // Busca si el vecino ya fue registrado
            for (int j = 0; j < (int)todos_los_nodos_que_exploramos.size(); j++) { // Recorre todos los nodos
                if (todos_los_nodos_que_exploramos[j].posicion_en_la_grilla == posicion_del_vecino) { // Si ya existe
                    indice_del_vecino = j; // Guarda su indice
                    break; // No necesita seguir buscando
                }
            }

            if (indice_del_vecino == -1) { // Si el vecino es nuevo (nunca fue registrado)
                Nodo nodo_nuevo; // Crea un nuevo nodo
                nodo_nuevo.posicion_en_la_grilla = posicion_del_vecino; // Le asigna su posicion
                nodo_nuevo.esta_en_la_lista_abierta = true; // Lo marca para explorar
                nodo_nuevo.indice_del_padre = indice_del_mejor_nodo_abierto; // Recuerda de donde vino
                nodo_nuevo.costo_de_caminos_recorridos = costo_de_llegar_hasta_el_vecino; // Guarda el costo G
                nodo_nuevo.costo_estimado_hasta_el_destino = calcular_distancia_estimada_entre_dos_casilleros(posicion_del_vecino, casillero_de_destino); // Calcula H
                nodo_nuevo.costo_total = nodo_nuevo.costo_de_caminos_recorridos + nodo_nuevo.costo_estimado_hasta_el_destino; // F = G + H
                todos_los_nodos_que_exploramos.push_back(nodo_nuevo); // Agrega el nodo a la lista
            }
            else if (todos_los_nodos_que_exploramos[indice_del_vecino].esta_en_la_lista_cerrada == false && costo_de_llegar_hasta_el_vecino < todos_los_nodos_que_exploramos[indice_del_vecino].costo_de_caminos_recorridos) {
 // Si ya existia pero encontramos un camino mas barato para llegar
                todos_los_nodos_que_exploramos[indice_del_vecino].esta_en_la_lista_abierta = true; // Vuelve a marcarlo para explorar
                todos_los_nodos_que_exploramos[indice_del_vecino].indice_del_padre = indice_del_mejor_nodo_abierto; // Actualiza de donde vino
                todos_los_nodos_que_exploramos[indice_del_vecino].costo_de_caminos_recorridos = costo_de_llegar_hasta_el_vecino; // Actualiza el costo G
                todos_los_nodos_que_exploramos[indice_del_vecino].costo_total = todos_los_nodos_que_exploramos[indice_del_vecino].costo_de_caminos_recorridos + todos_los_nodos_que_exploramos[indice_del_vecino].costo_estimado_hasta_el_destino; // Actualiza F
            }
        }
    }

    if (indice_del_nodo_destino_encontrado == -1) { // Si no se encontro ningun camino
        return 0; // Devuelve 0 pasos
    }

    int cantidad_de_pasos_encontrados = 0; // Contador de pasos del camino
    int indice_actual_para_reconstruir = indice_del_nodo_destino_encontrado; // Empieza desde el destino

    while (indice_actual_para_reconstruir != -1 && cantidad_de_pasos_encontrados < CANTIDAD_MAXIMA_DE_PASOS_EN_UN_CAMINO) { // Recorre hacia atras hasta el origen

        const Nodo& nodo_actual = todos_los_nodos_que_exploramos[indice_actual_para_reconstruir]; // Nodo actual en la reconstruccion

        float posicion_x_en_pixeles = (nodo_actual.posicion_en_la_grilla.x * TAMANO_DE_CADA_CASILLERO_EN_PIXELES) + (TAMANO_DE_CADA_CASILLERO_EN_PIXELES / 2.f); // Centro del casillero en X
        float posicion_y_en_pixeles = (nodo_actual.posicion_en_la_grilla.y * TAMANO_DE_CADA_CASILLERO_EN_PIXELES) + (TAMANO_DE_CADA_CASILLERO_EN_PIXELES / 2.f); // Centro del casillero en Y

        camino_resultado[cantidad_de_pasos_encontrados] = sf::Vector2f(posicion_x_en_pixeles, posicion_y_en_pixeles); // Guarda el punto en pixeles
        cantidad_de_pasos_encontrados++; // Cuenta un paso mas

        indice_actual_para_reconstruir = nodo_actual.indice_del_padre; // Sube al nodo padre (hacia el inicio)
    }

    for (int i = 0; i < cantidad_de_pasos_encontrados / 2; i++) { // Invierte el array (estaba del destino al origen)
        sf::Vector2f temporal = camino_resultado[i]; // Guarda temporalmente
        camino_resultado[i] = camino_resultado[cantidad_de_pasos_encontrados - 1 - i]; // Pone el del final
        camino_resultado[cantidad_de_pasos_encontrados - 1 - i] = temporal; // Pone el guardado
    }

    return cantidad_de_pasos_encontrados; // Devuelve cuantos pasos tiene el camino
}

///=============================================================///
///   DISTANCIA ESTIMADA
///=============================================================///
int PathFinder::calcular_distancia_estimada_entre_dos_casilleros(sf::Vector2i casillero_a, sf::Vector2i casillero_b) {
    return (std::abs(casillero_a.x - casillero_b.x) + std::abs(casillero_a.y - casillero_b.y)) * 10; // Distancia Manhattan multiplicada por 10
}

///=============================================================///
///   SE PUEDE CAMINAR
///=============================================================///
bool PathFinder::el_casillero_se_puede_caminar(Map& mapa_del_juego, sf::Vector2i posicion_del_casillero) {
    return mapa_del_juego.getEs_solido(posicion_del_casillero.y, posicion_del_casillero.x) == false; // true si no es pared
}
