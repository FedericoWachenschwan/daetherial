#include "AdministradorDeObjetos.h"
#include <iostream>
#include <cmath>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
AdministradorDeObjetos::AdministradorDeObjetos() {
    _circulo_de_la_moneda.setRadius(8.f); // Radio de la moneda en pixeles
    _circulo_de_la_moneda.setFillColor(sf::Color(255, 215, 0)); // Color dorado
    _circulo_de_la_moneda.setOutlineColor(sf::Color(160, 120, 0)); // Borde dorado oscuro
    _circulo_de_la_moneda.setOutlineThickness(2.f); // Grosor del borde
    _circulo_de_la_moneda.setOrigin(8.f, 8.f); // El origen en el centro para posicionarla bien
}

///=============================================================///
///   QUITAR ITEM Y CORRER LOS SIGUIENTES
///=============================================================///
void AdministradorDeObjetos::quitar_item_y_correr_los_siguientes(int indice_a_quitar) {
    for (int i = indice_a_quitar; i < _cantidad_de_items_en_el_mundo - 1; i++) {
        _items_en_el_mundo[i] = _items_en_el_mundo[i + 1]; // Corre cada item un lugar hacia adelante
    }
    _cantidad_de_items_en_el_mundo--; // Hay un item menos en el mundo
}

///=============================================================///
///   QUITAR MONEDA Y CORRER LAS SIGUIENTES
///=============================================================///
void AdministradorDeObjetos::quitar_moneda_y_correr_las_siguientes(int indice_a_quitar) {
    for (int i = indice_a_quitar; i < _cantidad_de_monedas_en_el_piso - 1; i++) {
        _oro_en_el_piso[i] = _oro_en_el_piso[i + 1]; // Corre cada moneda un lugar hacia adelante
    }
    _cantidad_de_monedas_en_el_piso--; // Hay una moneda menos en el suelo
}

///=============================================================///
///   AGREGAR ITEM AL MUNDO
///=============================================================///
void AdministradorDeObjetos::agregar_item_al_mundo(const Item& nuevo_item, float posicion_x, float posicion_y, sf::FloatRect hitbox_personalizada) {

    if (_cantidad_de_items_en_el_mundo >= CANTIDAD_MAXIMA_DE_ITEMS_EN_EL_MUNDO) {
        std::cout << "ERROR: NO HAY MAS LUGAR PARA ITEMS TIRADOS EN EL MUNDO." << std::endl;
        return; // No hay espacio en el array
    }

    _items_en_el_mundo[_cantidad_de_items_en_el_mundo] = nuevo_item; // Copia el item al array
    _items_en_el_mundo[_cantidad_de_items_en_el_mundo].colocar_en_el_mundo(posicion_x, posicion_y, hitbox_personalizada); // Lo posiciona en el mapa
    _cantidad_de_items_en_el_mundo++; // Hay un item mas en el mundo
}

///=============================================================///
///   DIBUJAR ITEMS
///=============================================================///
void AdministradorDeObjetos::dibujar_items(sf::RenderWindow& ventana_del_juego) const {
    for (int i = 0; i < _cantidad_de_items_en_el_mundo; i++) {
        _items_en_el_mundo[i].dibujar(ventana_del_juego); // Dibuja cada item tirado en el suelo
    }
}

///=============================================================///
///   CHEQUEAR INTERACCIONES
///=============================================================///
void AdministradorDeObjetos::chequear_interacciones(Personaje& jugador, const InputManager& entrada_del_jugador) {
    if (entrada_del_jugador.getEl_jugador_quiere_interactuar() == false) return; // Solo actua si se presiono la tecla de interaccion

    for (int i = _cantidad_de_items_en_el_mundo - 1; i >= 0; i--) {

        bool el_jugador_esta_tocando_el_item = jugador.calcular_caja_de_colision().intersects(_items_en_el_mundo[i].getCaja_de_colision()); // Verdadero si hay contacto

        if (el_jugador_esta_tocando_el_item == true) {

            if (_items_en_el_mundo[i].getEs_agarrable() == true) {
                bool se_pudo_guardar = jugador.getMochila().agarrar_item(_items_en_el_mundo[i]); // Intenta guardar en la mochila
                if (se_pudo_guardar == true) {
                    std::cout << "GUARDASTE EN LA MOCHILA: " << _items_en_el_mundo[i].getNombre() << std::endl;
                    quitar_item_y_correr_los_siguientes(i); // Lo elimina del suelo
                }
            }
            else {
                _items_en_el_mundo[i].usar(jugador); // No es agarrable, se usa directamente
            }

            break; // Solo interactua con un item por presion
        }
    }
}

///=============================================================///
///   RECIBIR ITEM SOLTADO
///=============================================================///
void AdministradorDeObjetos::recibir_item_soltado(const Item& item_soltado) {
    if (_cantidad_de_items_en_el_mundo >= CANTIDAD_MAXIMA_DE_ITEMS_EN_EL_MUNDO) {
        std::cout << "ERROR: NO HAY MAS LUGAR PARA ITEMS TIRADOS EN EL MUNDO." << std::endl;
        return; // No hay espacio para tirar el item
    }
    _items_en_el_mundo[_cantidad_de_items_en_el_mundo] = item_soltado; // Agrega el item soltado al array
    _cantidad_de_items_en_el_mundo++; // Hay un item mas en el mundo
}

///=============================================================///
///   SOLTAR ORO EN EL PISO
///=============================================================///
void AdministradorDeObjetos::soltar_oro_en_el_piso(sf::Vector2f posicion, int valor) {
    if (_cantidad_de_monedas_en_el_piso >= CANTIDAD_MAXIMA_DE_MONEDAS_EN_EL_PISO) {
        std::cout << "ERROR: NO HAY MAS LUGAR PARA MONEDAS EN EL PISO." << std::endl;
        return; // No hay espacio para mas monedas
    }

    _oro_en_el_piso[_cantidad_de_monedas_en_el_piso].posicion_de_la_moneda = posicion; // Guarda donde cayo la moneda
    _oro_en_el_piso[_cantidad_de_monedas_en_el_piso].valor_de_la_moneda = valor; // Guarda cuanto vale
    _cantidad_de_monedas_en_el_piso++; // Hay una moneda mas en el suelo
}

///=============================================================///
///   DIBUJAR ORO
///=============================================================///
void AdministradorDeObjetos::dibujar_oro(sf::RenderWindow& ventana_del_juego) {
    for (int i = 0; i < _cantidad_de_monedas_en_el_piso; i++) {
        _circulo_de_la_moneda.setPosition(_oro_en_el_piso[i].posicion_de_la_moneda); // Coloca el circulo en la posicion de la moneda
        ventana_del_juego.draw(_circulo_de_la_moneda); // Dibuja la moneda
    }
}

///=============================================================///
///   CHEQUEAR RECOGER ORO
///=============================================================///
void AdministradorDeObjetos::chequear_recoger_oro(Personaje& jugador, const InputManager& entrada_del_jugador) {

    if (entrada_del_jugador.getEl_jugador_quiere_agarrar_oro() == false) return; // Solo actua si se presiono la tecla de recoger

    sf::Vector2f posicion_del_jugador = jugador.getPosicion(); // Donde esta el personaje
    float distancia_maxima_para_agarrar = 24.f; // Rango en pixeles para recoger

    for (int i = _cantidad_de_monedas_en_el_piso - 1; i >= 0; i--) {

        float distancia_x = posicion_del_jugador.x - _oro_en_el_piso[i].posicion_de_la_moneda.x; // Diferencia en X
        float distancia_y = posicion_del_jugador.y - _oro_en_el_piso[i].posicion_de_la_moneda.y; // Diferencia en Y
        float distancia_total = std::hypot(distancia_x, distancia_y); // Distancia real

        if (distancia_total <= distancia_maxima_para_agarrar) {
            jugador.setOro(jugador.getOro() + _oro_en_el_piso[i].valor_de_la_moneda); // Suma el oro al jugador
            std::cout << "AGARRASTE " << _oro_en_el_piso[i].valor_de_la_moneda << " DE ORO." << std::endl;
            quitar_moneda_y_correr_las_siguientes(i); // Elimina la moneda del suelo
            break; // Solo recoge una moneda por presion
        }
    }
}
