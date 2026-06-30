#include "Tienda.h"
#include <iostream>
#include <string>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
Tienda::Tienda(sf::Vector2f posicion_de_la_tienda_en_el_mapa) {

    _cantidad_de_items_cargados_en_la_tienda = 0;
    _indice_del_item_seleccionado_en_la_tienda = 0;
    _cantidad_que_quiere_comprar_el_jugador = 1;
    _el_jugador_esta_cerca_de_la_tienda = false;
    _la_tienda_esta_abierta = false;

    if (_textura_de_la_tienda.loadFromFile("assets/tienda.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA IMAGEN DE LA TIENDA" << std::endl;
    }

    _sprite_de_la_tienda.setTexture(_textura_de_la_tienda);
    _sprite_de_la_tienda.setPosition(posicion_de_la_tienda_en_el_mapa);

    _zona_de_interaccion_de_la_tienda = sf::FloatRect(220.f, 380.f, 100.f, 100.f);
}

///=============================================================///
///   AGREGAR ITEM EN VENTA
///=============================================================///
void Tienda::agregar_item_en_venta(const Item& item_para_agregar) {

    if (_cantidad_de_items_cargados_en_la_tienda >= CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA) {
        std::cout << "ERROR: LA TIENDA YA TIENE LA CANTIDAD MAXIMA DE ITEMS" << std::endl;
        return;
    }

    _items_en_venta[_cantidad_de_items_cargados_en_la_tienda] = item_para_agregar;
    _cantidad_de_items_cargados_en_la_tienda++;
}

///=============================================================///
///   CARGAR FUENTE Y CARTEL
///=============================================================///
void Tienda::cargar_fuente_y_cartel() {

    if (_fuente_del_cartel_de_la_tienda.loadFromFile("assets/NorthEternal.otf") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA FUENTE DEL CARTEL" << std::endl;
    }

    _cartel_de_la_tienda.setFont(_fuente_del_cartel_de_la_tienda);
    _cartel_de_la_tienda.setCharacterSize(14);
    _cartel_de_la_tienda.setFillColor(sf::Color::Yellow);
    _cartel_de_la_tienda.setString("Presiona E para abrir la tienda");
}

///=============================================================///
///   ACTUALIZAR TIENDA
///=============================================================///
void Tienda::actualizar_tienda(sf::Vector2f posicion_actual_del_jugador) {

    bool jugador_dentro_de_la_zona = _zona_de_interaccion_de_la_tienda.contains(posicion_actual_del_jugador);

    if (jugador_dentro_de_la_zona == true) {
        _el_jugador_esta_cerca_de_la_tienda = true;
    }
    else {
        _el_jugador_esta_cerca_de_la_tienda = false;
        _la_tienda_esta_abierta = false;
    }

    float posicion_x_del_cartel = _zona_de_interaccion_de_la_tienda.left;
    float posicion_y_del_cartel = _zona_de_interaccion_de_la_tienda.top - 30.f;
    _cartel_de_la_tienda.setPosition(posicion_x_del_cartel, posicion_y_del_cartel);
}

///=============================================================///
///   DIBUJAR SPRITE EN EL MAPA
///=============================================================///
void Tienda::dibujar_sprite_en_el_mapa(sf::RenderWindow& ventana_del_juego, bool mostrar_zona_de_deteccion) {

    ventana_del_juego.draw(_sprite_de_la_tienda);

    if (mostrar_zona_de_deteccion == true) {
        sf::RectangleShape hud_de_la_zona_de_deteccion;
        hud_de_la_zona_de_deteccion.setPosition(_zona_de_interaccion_de_la_tienda.left, _zona_de_interaccion_de_la_tienda.top + 20);
        hud_de_la_zona_de_deteccion.setSize(sf::Vector2f(_zona_de_interaccion_de_la_tienda.width, _zona_de_interaccion_de_la_tienda.height));
        hud_de_la_zona_de_deteccion.setFillColor(sf::Color(0, 255, 0, 50));
        hud_de_la_zona_de_deteccion.setOutlineColor(sf::Color::Green);
        hud_de_la_zona_de_deteccion.setOutlineThickness(1.f);
        ventana_del_juego.draw(hud_de_la_zona_de_deteccion);
    }

    if (_el_jugador_esta_cerca_de_la_tienda == true && _la_tienda_esta_abierta == false) {
        ventana_del_juego.draw(_cartel_de_la_tienda);
    }
}

///=============================================================///
///   DIBUJAR INTERFAZ DE COMPRA
///=============================================================///
void Tienda::dibujar_interfaz_de_compra(sf::RenderWindow& ventana_del_juego, sf::Font& fuente_del_hud) {

    if (_la_tienda_esta_abierta == false) return;

    float ancho_del_panel = 620.f;
    float alto_del_panel = 220.f;
    float posicion_x_del_panel = (1280.f - ancho_del_panel) / 2.f;
    float posicion_y_del_panel = 720.f - alto_del_panel - 20.f;

    sf::RectangleShape panel_de_fondo;
    panel_de_fondo.setPosition(posicion_x_del_panel, posicion_y_del_panel);
    panel_de_fondo.setSize(sf::Vector2f(ancho_del_panel, alto_del_panel));
    panel_de_fondo.setFillColor(sf::Color(20, 20, 20, 220));
    panel_de_fondo.setOutlineColor(sf::Color(200, 160, 50));
    panel_de_fondo.setOutlineThickness(2.f);
    ventana_del_juego.draw(panel_de_fondo);

    sf::Text titulo_de_la_tienda;
    titulo_de_la_tienda.setFont(fuente_del_hud);
    titulo_de_la_tienda.setCharacterSize(16);
    titulo_de_la_tienda.setFillColor(sf::Color(200, 160, 50));
    titulo_de_la_tienda.setString("TIENDA - Usa flechas para navegar, E para comprar, ESC para cerrar");
    titulo_de_la_tienda.setPosition(posicion_x_del_panel + 10.f, posicion_y_del_panel + 5.f);
    ventana_del_juego.draw(titulo_de_la_tienda);

    float ancho_del_recuadro = 180.f;
    float alto_del_recuadro = 150.f;
    float margen_entre_recuadros = 20.f;
    float inicio_x_de_los_recuadros = posicion_x_del_panel + 15.f;
    float inicio_y_de_los_recuadros = posicion_y_del_panel + 35.f;

    for (int i = 0; i < _cantidad_de_items_cargados_en_la_tienda; i++) {

        float posicion_x_del_recuadro = inicio_x_de_los_recuadros + i * (ancho_del_recuadro + margen_entre_recuadros);
        float posicion_y_del_recuadro = inicio_y_de_los_recuadros;

        sf::RectangleShape recuadro_del_item;
        recuadro_del_item.setPosition(posicion_x_del_recuadro, posicion_y_del_recuadro);
        recuadro_del_item.setSize(sf::Vector2f(ancho_del_recuadro, alto_del_recuadro));
        recuadro_del_item.setFillColor(sf::Color(40, 40, 40, 200));

        if (i == _indice_del_item_seleccionado_en_la_tienda) {
            recuadro_del_item.setOutlineColor(sf::Color::Yellow);
            recuadro_del_item.setOutlineThickness(3.f);
        }
        else {
            recuadro_del_item.setOutlineColor(sf::Color(100, 100, 100));
            recuadro_del_item.setOutlineThickness(1.f);
        }

        ventana_del_juego.draw(recuadro_del_item);

        sf::Text texto_del_nombre_del_item;
        texto_del_nombre_del_item.setFont(fuente_del_hud);
        texto_del_nombre_del_item.setCharacterSize(12);
        texto_del_nombre_del_item.setFillColor(sf::Color::White);
        texto_del_nombre_del_item.setString(_items_en_venta[i].getNombre());
        texto_del_nombre_del_item.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 8.f);
        ventana_del_juego.draw(texto_del_nombre_del_item);

        sf::Text texto_del_precio_del_item;
        texto_del_precio_del_item.setFont(fuente_del_hud);
        texto_del_precio_del_item.setCharacterSize(12);
        texto_del_precio_del_item.setFillColor(sf::Color::Yellow);
        std::string precio_como_texto = std::to_string(_items_en_venta[i].getPrecio()) + " oro";
        texto_del_precio_del_item.setString(precio_como_texto);
        texto_del_precio_del_item.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 30.f);
        ventana_del_juego.draw(texto_del_precio_del_item);

        if (i == _indice_del_item_seleccionado_en_la_tienda) {
            sf::Text texto_de_cantidad;
            texto_de_cantidad.setFont(fuente_del_hud);
            texto_de_cantidad.setCharacterSize(12);
            texto_de_cantidad.setFillColor(sf::Color::Cyan);
            texto_de_cantidad.setString("Cant: " + std::to_string(_cantidad_que_quiere_comprar_el_jugador));
            texto_de_cantidad.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 55.f);
            ventana_del_juego.draw(texto_de_cantidad);

            sf::Text texto_de_flechas;
            texto_de_flechas.setFont(fuente_del_hud);
            texto_de_flechas.setCharacterSize(10);
            texto_de_flechas.setFillColor(sf::Color(150, 150, 150));
            texto_de_flechas.setString("Arr/Ab para cantidad");
            texto_de_flechas.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 75.f);
            ventana_del_juego.draw(texto_de_flechas);

            int costo_total = _items_en_venta[i].getPrecio() * _cantidad_que_quiere_comprar_el_jugador;
            sf::Text texto_del_costo_total;
            texto_del_costo_total.setFont(fuente_del_hud);
            texto_del_costo_total.setCharacterSize(12);
            texto_del_costo_total.setFillColor(sf::Color(255, 200, 0));
            texto_del_costo_total.setString("Total: " + std::to_string(costo_total) + " oro");
            texto_del_costo_total.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 100.f);
            ventana_del_juego.draw(texto_del_costo_total);
        }
    }
}

///=============================================================///
///   ABRIR Y CERRAR
///=============================================================///
void Tienda::abrir_tienda() {
    _la_tienda_esta_abierta = true;
    _indice_del_item_seleccionado_en_la_tienda = 0;
    _cantidad_que_quiere_comprar_el_jugador = 1;
}

void Tienda::cerrar_tienda() {
    _la_tienda_esta_abierta = false;
    _cantidad_que_quiere_comprar_el_jugador = 1;
}

///=============================================================///
///   NAVEGACION
///=============================================================///
void Tienda::seleccionar_item_siguiente() {
    if (_indice_del_item_seleccionado_en_la_tienda < _cantidad_de_items_cargados_en_la_tienda - 1) {
        _indice_del_item_seleccionado_en_la_tienda++;
        _cantidad_que_quiere_comprar_el_jugador = 1;
    }
}

void Tienda::seleccionar_item_anterior() {
    if (_indice_del_item_seleccionado_en_la_tienda > 0) {
        _indice_del_item_seleccionado_en_la_tienda--;
        _cantidad_que_quiere_comprar_el_jugador = 1;
    }
}

///=============================================================///
///   CANTIDAD
///=============================================================///
void Tienda::aumentar_cantidad_a_comprar() {
    _cantidad_que_quiere_comprar_el_jugador++;
}

void Tienda::disminuir_cantidad_a_comprar() {
    if (_cantidad_que_quiere_comprar_el_jugador > 1) {
        _cantidad_que_quiere_comprar_el_jugador--;
    }
}

///=============================================================///
///   INTENTAR COMPRAR
///=============================================================///
void Tienda::intentar_comprar_item_seleccionado(Personaje& jugador) {

    if (_la_tienda_esta_abierta == false) return;

    Item& item_seleccionado = _items_en_venta[_indice_del_item_seleccionado_en_la_tienda];

    int costo_total_de_la_compra = item_seleccionado.getPrecio() * _cantidad_que_quiere_comprar_el_jugador;
    int oro_que_tiene_el_jugador = jugador.getOro();

    if (oro_que_tiene_el_jugador < costo_total_de_la_compra) {
        std::cout << "NO TENES SUFICIENTE ORO. NECESITAS " << costo_total_de_la_compra << " Y TENES " << oro_que_tiene_el_jugador << std::endl;
        return;
    }

    int oro_que_le_queda_al_jugador = oro_que_tiene_el_jugador - costo_total_de_la_compra;
    jugador.setOro(oro_que_le_queda_al_jugador);

    for (int i = 0; i < _cantidad_que_quiere_comprar_el_jugador; i++) {
        Item copia_para_el_inventario = item_seleccionado;
        copia_para_el_inventario.setCantidad(1);
        jugador.getMochila().agarrar_item(copia_para_el_inventario);
    }

    std::cout << "COMPRASTE " << _cantidad_que_quiere_comprar_el_jugador << "x " << item_seleccionado.getNombre() << ". TE QUEDAN " << oro_que_le_queda_al_jugador << " DE ORO." << std::endl;
    _cantidad_que_quiere_comprar_el_jugador = 1;
}