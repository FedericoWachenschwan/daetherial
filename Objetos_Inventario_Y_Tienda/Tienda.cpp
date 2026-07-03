#include "Tienda.h"
#include <iostream>
#include <string>

///=============================================================///
///   #1 - CONSTRUCTOR
///=============================================================///
// #1
Tienda::Tienda(sf::Vector2f posicion_de_la_tienda_en_el_mapa) {

    _cantidad_de_items_cargados_en_la_tienda = 0; // Sin items en venta al comenzar
    _indice_del_item_seleccionado_en_la_tienda = 0; // El primer item empieza seleccionado
    _cantidad_que_quiere_comprar_el_jugador = 1; // Empieza queriendo comprar 1 unidad
    _el_jugador_esta_cerca_de_la_tienda = false; // El jugador empieza lejos
    _la_tienda_esta_abierta = false; // El panel empieza cerrado

    if (_textura_de_la_tienda.loadFromFile("assets/Tienda.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA IMAGEN DE LA TIENDA" << std::endl;
    }

    _sprite_de_la_tienda.setTexture(_textura_de_la_tienda); // Asigna la imagen al sprite
    float escala_de_la_tienda = 128.f / _textura_de_la_tienda.getSize().x; // Escala para que mida 128px en el mundo
    _sprite_de_la_tienda.setScale(escala_de_la_tienda, escala_de_la_tienda);

    _zona_de_interaccion_de_la_tienda = sf::FloatRect(303.f, 1660.f, 100.f, 100.f); // Rectangulo donde se puede abrir la tienda

    float centro_x_de_la_zona = _zona_de_interaccion_de_la_tienda.left + _zona_de_interaccion_de_la_tienda.width  / 2.f;
    float centro_y_de_la_zona = _zona_de_interaccion_de_la_tienda.top  + _zona_de_interaccion_de_la_tienda.height / 2.f;
    float ancho_del_sprite    = _sprite_de_la_tienda.getGlobalBounds().width;
    float alto_del_sprite     = _sprite_de_la_tienda.getGlobalBounds().height;
    _sprite_de_la_tienda.setPosition(centro_x_de_la_zona - ancho_del_sprite / 2.f, centro_y_de_la_zona - alto_del_sprite / 2.f); // Centra el sprite sobre la zona de interaccion
}

///=============================================================///
///   #2 - AGREGAR ITEM EN VENTA
///=============================================================///
// #2
void Tienda::agregar_item_en_venta(const Item& item_para_agregar) {

    if (_cantidad_de_items_cargados_en_la_tienda >= CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA) {
        std::cout << "ERROR: LA TIENDA YA TIENE LA CANTIDAD MAXIMA DE ITEMS" << std::endl;
        return; // No hay lugar para mas items
    }

    _items_en_venta[_cantidad_de_items_cargados_en_la_tienda] = item_para_agregar; // Guarda el item en el catalogo
    _cantidad_de_items_cargados_en_la_tienda++; // Hay un item mas en venta
}

///=============================================================///
///   #3 - CARGAR FUENTE Y CARTEL
///=============================================================///
// #3
void Tienda::cargar_fuente_y_cartel() {

    if (_fuente_del_cartel_de_la_tienda.loadFromFile("assets/NorthEternal.otf") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA FUENTE DEL CARTEL" << std::endl;
    }

    if (_fuente_del_panel_de_la_tienda.loadFromFile("C:/Windows/Fonts/arial.ttf") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR ARIAL PARA EL PANEL DE LA TIENDA" << std::endl;
        _fuente_del_panel_de_la_tienda = _fuente_del_cartel_de_la_tienda; // fallback a NorthEternal si falla
    }

    _cartel_de_la_tienda.setFont(_fuente_del_cartel_de_la_tienda); // Asigna la tipografia al texto
    _cartel_de_la_tienda.setCharacterSize(14); // Tamanio de letra del cartel
    _cartel_de_la_tienda.setFillColor(sf::Color::Yellow); // El cartel es amarillo
    _cartel_de_la_tienda.setString("Presiona E para abrir la tienda"); // Mensaje que ve el jugador
}

///=============================================================///
///   #4 - ACTUALIZAR TIENDA
///=============================================================///
// #4
void Tienda::actualizar_tienda(sf::Vector2f posicion_actual_del_jugador) {

    bool jugador_dentro_de_la_zona = _zona_de_interaccion_de_la_tienda.contains(posicion_actual_del_jugador); // Verdadero si el jugador esta en la zona

    if (jugador_dentro_de_la_zona == true) {
        _el_jugador_esta_cerca_de_la_tienda = true; // El jugador puede abrir la tienda
    }
    else {
        _el_jugador_esta_cerca_de_la_tienda = false; // El jugador se fue de la zona
        _la_tienda_esta_abierta = false; // Cierra la tienda si el jugador se alejo
    }

    float posicion_x_del_cartel = _zona_de_interaccion_de_la_tienda.left; // X del cartel
    float posicion_y_del_cartel = _zona_de_interaccion_de_la_tienda.top - 30.f; // Un poco arriba de la zona
    _cartel_de_la_tienda.setPosition(posicion_x_del_cartel, posicion_y_del_cartel); // Coloca el cartel en el mundo
}

///=============================================================///
///   #5 - DIBUJAR SPRITE EN EL MAPA
///=============================================================///
// #5
void Tienda::dibujar_sprite_en_el_mapa(sf::RenderWindow& ventana_del_juego, bool mostrar_zona_de_deteccion) {

    ventana_del_juego.draw(_sprite_de_la_tienda); // Dibuja el edificio de la tienda

    if (mostrar_zona_de_deteccion == true) {
        sf::RectangleShape hud_de_la_zona_de_deteccion;
        hud_de_la_zona_de_deteccion.setPosition(_zona_de_interaccion_de_la_tienda.left, _zona_de_interaccion_de_la_tienda.top + 20); // Posicion de la zona
        hud_de_la_zona_de_deteccion.setSize(sf::Vector2f(_zona_de_interaccion_de_la_tienda.width, _zona_de_interaccion_de_la_tienda.height)); // Tamanio de la zona
        hud_de_la_zona_de_deteccion.setFillColor(sf::Color(0, 255, 0, 50)); // Verde semitransparente para ver la zona
        hud_de_la_zona_de_deteccion.setOutlineColor(sf::Color::Green); // Borde verde
        hud_de_la_zona_de_deteccion.setOutlineThickness(1.f); // Grosor del borde
        ventana_del_juego.draw(hud_de_la_zona_de_deteccion); // Dibuja la zona de deteccion
    }

    if (_el_jugador_esta_cerca_de_la_tienda == true && _la_tienda_esta_abierta == false) {
        ventana_del_juego.draw(_cartel_de_la_tienda); // Muestra el cartel solo si la tienda esta cerrada
    }
}

///=============================================================///
///   #6 - DIBUJAR INTERFAZ DE COMPRA
///=============================================================///
// #6
void Tienda::dibujar_interfaz_de_compra(sf::RenderWindow& ventana_del_juego, sf::Font& fuente_del_hud) {

    if (_la_tienda_esta_abierta == false) return; // Si esta cerrada, no dibuja nada

    float ancho_del_panel = 620.f; // Ancho del panel de compra
    float alto_del_panel = 220.f; // Alto del panel de compra
    float posicion_x_del_panel = (1280.f - ancho_del_panel) / 2.f; // Centra el panel horizontalmente
    float posicion_y_del_panel = 720.f - alto_del_panel - 20.f; // Lo ubica cerca del borde inferior

    sf::RectangleShape panel_de_fondo;
    panel_de_fondo.setPosition(posicion_x_del_panel, posicion_y_del_panel); // Posicion del panel
    panel_de_fondo.setSize(sf::Vector2f(ancho_del_panel, alto_del_panel)); // Tamanio del panel
    panel_de_fondo.setFillColor(sf::Color(20, 20, 20, 220)); // Fondo oscuro semitransparente
    panel_de_fondo.setOutlineColor(sf::Color(200, 160, 50)); // Borde dorado
    panel_de_fondo.setOutlineThickness(2.f); // Grosor del borde
    ventana_del_juego.draw(panel_de_fondo); // Dibuja el fondo del panel

    sf::Text titulo_de_la_tienda;
    titulo_de_la_tienda.setFont(_fuente_del_panel_de_la_tienda);
    titulo_de_la_tienda.setCharacterSize(15);
    titulo_de_la_tienda.setFillColor(sf::Color(200, 160, 50));
    titulo_de_la_tienda.setString("TIENDA  |  Flechas: navegar   E: comprar   ESC: cerrar");
    titulo_de_la_tienda.setPosition(posicion_x_del_panel + 10.f, posicion_y_del_panel + 6.f);
    ventana_del_juego.draw(titulo_de_la_tienda);

    float ancho_del_recuadro = 180.f; // Ancho de cada recuadro de item
    float alto_del_recuadro = 150.f; // Alto de cada recuadro de item
    float margen_entre_recuadros = 20.f; // Espacio entre recuadros
    float inicio_x_de_los_recuadros = posicion_x_del_panel + 15.f; // Desde donde empiezan los recuadros
    float inicio_y_de_los_recuadros = posicion_y_del_panel + 35.f; // Altura de los recuadros

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

        sf::Sprite icono_en_tienda = _items_en_venta[i].getSprite(); // Copia del sprite
        float ancho_del_icono = icono_en_tienda.getGlobalBounds().width; // Ancho real despues de la escala
        float x_centrado = posicion_x_del_recuadro + (ancho_del_recuadro - ancho_del_icono) / 2.f; // Centra horizontalmente
        float y_icono = (_items_en_venta[i].getTipo() == TipoDeItem::BACULO_ARCANO) ? posicion_y_del_recuadro + 75.f : posicion_y_del_recuadro + 65.f;
        icono_en_tienda.setPosition(x_centrado, y_icono);
        ventana_del_juego.draw(icono_en_tienda);

        sf::Text texto_del_nombre_del_item;
        texto_del_nombre_del_item.setFont(_fuente_del_panel_de_la_tienda);
        texto_del_nombre_del_item.setCharacterSize(14);
        texto_del_nombre_del_item.setFillColor(sf::Color::White);
        texto_del_nombre_del_item.setString(_items_en_venta[i].getNombre());
        texto_del_nombre_del_item.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 8.f);
        ventana_del_juego.draw(texto_del_nombre_del_item);

        sf::Text texto_del_precio_del_item;
        texto_del_precio_del_item.setFont(_fuente_del_panel_de_la_tienda);
        texto_del_precio_del_item.setCharacterSize(13);
        texto_del_precio_del_item.setFillColor(sf::Color::Yellow);
        texto_del_precio_del_item.setString(std::to_string(_items_en_venta[i].getPrecio()) + " oro");
        texto_del_precio_del_item.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 30.f);
        ventana_del_juego.draw(texto_del_precio_del_item);

        if (_items_en_venta[i].getTipo() == TipoDeItem::BACULO_ARCANO) {
            sf::Text texto_req;
            texto_req.setFont(_fuente_del_panel_de_la_tienda);
            texto_req.setCharacterSize(12);
            texto_req.setFillColor(sf::Color(180, 100, 255));
            texto_req.setString("Requiere: Gema Arcana");
            texto_req.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 50.f);
            ventana_del_juego.draw(texto_req);
        }

        if (i == _indice_del_item_seleccionado_en_la_tienda) {

            if (_items_en_venta[i].getTipo() == TipoDeItem::BACULO_ARCANO) {
                sf::Text texto_costo_baculo;
                texto_costo_baculo.setFont(_fuente_del_panel_de_la_tienda);
                texto_costo_baculo.setCharacterSize(13);
                texto_costo_baculo.setFillColor(sf::Color(255, 200, 0));
                texto_costo_baculo.setString("Total: 150 oro + Gema");
                texto_costo_baculo.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 110.f);
                ventana_del_juego.draw(texto_costo_baculo);
            }
            else {
                sf::Text texto_de_cantidad;
                texto_de_cantidad.setFont(_fuente_del_panel_de_la_tienda);
                texto_de_cantidad.setCharacterSize(13);
                texto_de_cantidad.setFillColor(sf::Color::Cyan);
                texto_de_cantidad.setString("Cant: " + std::to_string(_cantidad_que_quiere_comprar_el_jugador));
                texto_de_cantidad.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 55.f);
                ventana_del_juego.draw(texto_de_cantidad);

                int costo_total = _items_en_venta[i].getPrecio() * _cantidad_que_quiere_comprar_el_jugador;
                sf::Text texto_del_costo_total;
                texto_del_costo_total.setFont(_fuente_del_panel_de_la_tienda);
                texto_del_costo_total.setCharacterSize(13);
                texto_del_costo_total.setFillColor(sf::Color(255, 200, 0));
                texto_del_costo_total.setString("Total: " + std::to_string(costo_total) + " oro");
                texto_del_costo_total.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 100.f);
                ventana_del_juego.draw(texto_del_costo_total);
            }
        }
    }
}

///=============================================================///
///   #7 - ABRIR Y CERRAR
///=============================================================///
// #7
void Tienda::abrir_tienda() {
    _la_tienda_esta_abierta = true; // Muestra el panel de compra
    _indice_del_item_seleccionado_en_la_tienda = 0; // Vuelve al primer item al abrir
    _cantidad_que_quiere_comprar_el_jugador = 1; // Reinicia la cantidad a 1
}

// #8
void Tienda::cerrar_tienda() {
    _la_tienda_esta_abierta = false; // Oculta el panel de compra
    _cantidad_que_quiere_comprar_el_jugador = 1; // Reinicia la cantidad a 1
}

///=============================================================///
///   #9 - NAVEGACION
///=============================================================///
// #9
void Tienda::seleccionar_item_siguiente() {
    if (_indice_del_item_seleccionado_en_la_tienda < _cantidad_de_items_cargados_en_la_tienda - 1) {
        _indice_del_item_seleccionado_en_la_tienda++; // Mueve el cursor al item siguiente
        _cantidad_que_quiere_comprar_el_jugador = 1; // Reinicia la cantidad al cambiar de item
    }
}

// #10
void Tienda::seleccionar_item_anterior() {
    if (_indice_del_item_seleccionado_en_la_tienda > 0) {
        _indice_del_item_seleccionado_en_la_tienda--; // Mueve el cursor al item anterior
        _cantidad_que_quiere_comprar_el_jugador = 1; // Reinicia la cantidad al cambiar de item
    }
}

///=============================================================///
///   #11 - CANTIDAD
///=============================================================///
// #11
void Tienda::aumentar_cantidad_a_comprar() {
    _cantidad_que_quiere_comprar_el_jugador++; // Suma una unidad mas a la cantidad
}

// #12
void Tienda::disminuir_cantidad_a_comprar() {
    if (_cantidad_que_quiere_comprar_el_jugador > 1) {
        _cantidad_que_quiere_comprar_el_jugador--; // Resta una unidad, sin bajar de 1
    }
}

///=============================================================///
///   #13 - INTENTAR COMPRAR
///=============================================================///
// #13
void Tienda::intentar_comprar_item_seleccionado(Personaje& jugador) {

    if (_la_tienda_esta_abierta == false) return; // No se puede comprar con la tienda cerrada

    Item& item_seleccionado = _items_en_venta[_indice_del_item_seleccionado_en_la_tienda]; // Referencia al item elegido

    int cantidad_a_comprar = _cantidad_que_quiere_comprar_el_jugador; // Cuantas unidades quiere comprar
    if (item_seleccionado.getTipo() == TipoDeItem::BACULO_ARCANO) {
        cantidad_a_comprar = 1; // El baculo siempre se compra de a uno
    }

    int costo_total_de_la_compra = item_seleccionado.getPrecio() * cantidad_a_comprar; // Precio total a pagar
    int oro_que_tiene_el_jugador = jugador.getOro(); // Cuanto oro tiene el jugador

    if (oro_que_tiene_el_jugador < costo_total_de_la_compra) {
        std::cout << "NO TENES SUFICIENTE ORO. NECESITAS " << costo_total_de_la_compra << " Y TENES " << oro_que_tiene_el_jugador << std::endl;
        return; // No tiene suficiente oro para comprar
    }

    if (item_seleccionado.getTipo() == TipoDeItem::BACULO_ARCANO) { // El baculo ademas requiere una Gema Arcana
        bool tenia_la_gema = jugador.getMochila().consumir_un_item_de_tipo(TipoDeItem::GEMA_ARCANA); // Intenta consumir la gema
        if (tenia_la_gema == false) {
            std::cout << "NECESITAS UNA GEMA ARCANA PARA COMPRAR EL BACULO. MATÁ 15 ENEMIGOS." << std::endl;
            return; // No tiene la gema, no puede comprar
        }
    }

    int oro_que_le_queda_al_jugador = oro_que_tiene_el_jugador - costo_total_de_la_compra; // Calcula el oro restante
    jugador.setOro(oro_que_le_queda_al_jugador); // Le descuenta el oro al jugador

    for (int i = 0; i < cantidad_a_comprar; i++) {
        Item copia_para_el_inventario = item_seleccionado; // Copia el item para no modificar el original
        copia_para_el_inventario.setCantidad(1); // Cada copia tiene cantidad 1
        jugador.getMochila().agarrar_item(copia_para_el_inventario); // Intenta guardarlo en la mochila
    }

    std::cout << "COMPRASTE " << cantidad_a_comprar << "x " << item_seleccionado.getNombre() << ". TE QUEDAN " << oro_que_le_queda_al_jugador << " DE ORO." << std::endl;
    _cantidad_que_quiere_comprar_el_jugador = 1; // Reinicia la cantidad despues de comprar
}
