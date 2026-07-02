#include "Tienda.h"
#include <iostream>
#include <string>

///=============================================================///
///   CONSTRUCTOR
///=============================================================///
Tienda::Tienda(sf::Vector2f posicion_de_la_tienda_en_el_mapa) {

    _cantidad_de_items_cargados_en_la_tienda = 0; // Sin items en venta al comenzar
    _indice_del_item_seleccionado_en_la_tienda = 0; // El primer item empieza seleccionado
    _cantidad_que_quiere_comprar_el_jugador = 1; // Empieza queriendo comprar 1 unidad
    _el_jugador_esta_cerca_de_la_tienda = false; // El jugador empieza lejos
    _la_tienda_esta_abierta = false; // El panel empieza cerrado

    if (_textura_de_la_tienda.loadFromFile("assets/tienda.png") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA IMAGEN DE LA TIENDA" << std::endl;
    }

    _sprite_de_la_tienda.setTexture(_textura_de_la_tienda); // Asigna la imagen al sprite
    _sprite_de_la_tienda.setPosition(posicion_de_la_tienda_en_el_mapa); // Coloca el edificio en el mapa

    _zona_de_interaccion_de_la_tienda = sf::FloatRect(220.f, 380.f, 100.f, 100.f); // Rectangulo donde se puede abrir la tienda
}

///=============================================================///
///   AGREGAR ITEM EN VENTA
///=============================================================///
void Tienda::agregar_item_en_venta(const Item& item_para_agregar) {

    if (_cantidad_de_items_cargados_en_la_tienda >= CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA) {
        std::cout << "ERROR: LA TIENDA YA TIENE LA CANTIDAD MAXIMA DE ITEMS" << std::endl;
        return; // No hay lugar para mas items
    }

    _items_en_venta[_cantidad_de_items_cargados_en_la_tienda] = item_para_agregar; // Guarda el item en el catalogo
    _cantidad_de_items_cargados_en_la_tienda++; // Hay un item mas en venta
}

///=============================================================///
///   CARGAR FUENTE Y CARTEL
///=============================================================///
void Tienda::cargar_fuente_y_cartel() {

    if (_fuente_del_cartel_de_la_tienda.loadFromFile("assets/NorthEternal.otf") == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA FUENTE DEL CARTEL" << std::endl;
    }

    _cartel_de_la_tienda.setFont(_fuente_del_cartel_de_la_tienda); // Asigna la tipografia al texto
    _cartel_de_la_tienda.setCharacterSize(14); // Tamanio de letra del cartel
    _cartel_de_la_tienda.setFillColor(sf::Color::Yellow); // El cartel es amarillo
    _cartel_de_la_tienda.setString("Presiona E para abrir la tienda"); // Mensaje que ve el jugador
}

///=============================================================///
///   ACTUALIZAR TIENDA
///=============================================================///
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
///   DIBUJAR SPRITE EN EL MAPA
///=============================================================///
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
///   DIBUJAR INTERFAZ DE COMPRA
///=============================================================///
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
    titulo_de_la_tienda.setFont(fuente_del_hud); // Tipografia del titulo
    titulo_de_la_tienda.setCharacterSize(16); // Tamanio del texto
    titulo_de_la_tienda.setFillColor(sf::Color(200, 160, 50)); // Color dorado
    titulo_de_la_tienda.setString("TIENDA - Usa flechas para navegar, E para comprar, ESC para cerrar"); // Instrucciones
    titulo_de_la_tienda.setPosition(posicion_x_del_panel + 10.f, posicion_y_del_panel + 5.f); // En la parte superior del panel
    ventana_del_juego.draw(titulo_de_la_tienda); // Dibuja el titulo

    float ancho_del_recuadro = 180.f; // Ancho de cada recuadro de item
    float alto_del_recuadro = 150.f; // Alto de cada recuadro de item
    float margen_entre_recuadros = 20.f; // Espacio entre recuadros
    float inicio_x_de_los_recuadros = posicion_x_del_panel + 15.f; // Desde donde empiezan los recuadros
    float inicio_y_de_los_recuadros = posicion_y_del_panel + 35.f; // Altura de los recuadros

    for (int i = 0; i < _cantidad_de_items_cargados_en_la_tienda; i++) {

        float posicion_x_del_recuadro = inicio_x_de_los_recuadros + i * (ancho_del_recuadro + margen_entre_recuadros); // Posicion X de este recuadro
        float posicion_y_del_recuadro = inicio_y_de_los_recuadros; // Misma altura para todos

        sf::RectangleShape recuadro_del_item;
        recuadro_del_item.setPosition(posicion_x_del_recuadro, posicion_y_del_recuadro); // Posicion del recuadro
        recuadro_del_item.setSize(sf::Vector2f(ancho_del_recuadro, alto_del_recuadro)); // Tamanio del recuadro
        recuadro_del_item.setFillColor(sf::Color(40, 40, 40, 200)); // Fondo oscuro

        if (i == _indice_del_item_seleccionado_en_la_tienda) {
            recuadro_del_item.setOutlineColor(sf::Color::Yellow); // Borde amarillo para el seleccionado
            recuadro_del_item.setOutlineThickness(3.f); // Borde mas grueso para destacarlo
        }
        else {
            recuadro_del_item.setOutlineColor(sf::Color(100, 100, 100)); // Borde gris para el resto
            recuadro_del_item.setOutlineThickness(1.f); // Borde fino normal
        }

        ventana_del_juego.draw(recuadro_del_item); // Dibuja el recuadro del item

        sf::Text texto_del_nombre_del_item;
        texto_del_nombre_del_item.setFont(fuente_del_hud); // Tipografia del nombre
        texto_del_nombre_del_item.setCharacterSize(12); // Tamanio del texto
        texto_del_nombre_del_item.setFillColor(sf::Color::White); // Color blanco
        texto_del_nombre_del_item.setString(_items_en_venta[i].getNombre()); // Nombre del item en venta
        texto_del_nombre_del_item.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 8.f); // En la parte superior del recuadro
        ventana_del_juego.draw(texto_del_nombre_del_item); // Dibuja el nombre

        sf::Text texto_del_precio_del_item;
        texto_del_precio_del_item.setFont(fuente_del_hud); // Tipografia del precio
        texto_del_precio_del_item.setCharacterSize(12); // Tamanio del texto
        texto_del_precio_del_item.setFillColor(sf::Color::Yellow); // Color amarillo para el precio
        std::string precio_como_texto = std::to_string(_items_en_venta[i].getPrecio()) + " oro"; // Convierte el numero a texto con unidad
        texto_del_precio_del_item.setString(precio_como_texto); // Asigna el texto con el precio
        texto_del_precio_del_item.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 30.f); // Debajo del nombre
        ventana_del_juego.draw(texto_del_precio_del_item); // Dibuja el precio

        if (i == _indice_del_item_seleccionado_en_la_tienda) {
            sf::Text texto_de_cantidad;
            texto_de_cantidad.setFont(fuente_del_hud); // Tipografia de cantidad
            texto_de_cantidad.setCharacterSize(12); // Tamanio del texto
            texto_de_cantidad.setFillColor(sf::Color::Cyan); // Color cyan para la cantidad
            texto_de_cantidad.setString("Cant: " + std::to_string(_cantidad_que_quiere_comprar_el_jugador)); // Muestra la cantidad elegida
            texto_de_cantidad.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 55.f); // Debajo del precio
            ventana_del_juego.draw(texto_de_cantidad); // Dibuja la cantidad

            sf::Text texto_de_flechas;
            texto_de_flechas.setFont(fuente_del_hud); // Tipografia de instruccion
            texto_de_flechas.setCharacterSize(10); // Tamanio pequenio
            texto_de_flechas.setFillColor(sf::Color(150, 150, 150)); // Color gris para texto secundario
            texto_de_flechas.setString("Arr/Ab para cantidad"); // Instruccion para cambiar cantidad
            texto_de_flechas.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 75.f); // Debajo de la cantidad
            ventana_del_juego.draw(texto_de_flechas); // Dibuja la instruccion

            int costo_total = _items_en_venta[i].getPrecio() * _cantidad_que_quiere_comprar_el_jugador; // Precio por cantidad
            sf::Text texto_del_costo_total;
            texto_del_costo_total.setFont(fuente_del_hud); // Tipografia del costo total
            texto_del_costo_total.setCharacterSize(12); // Tamanio del texto
            texto_del_costo_total.setFillColor(sf::Color(255, 200, 0)); // Color dorado anaranjado
            texto_del_costo_total.setString("Total: " + std::to_string(costo_total) + " oro"); // Muestra el precio total
            texto_del_costo_total.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 100.f); // En la parte inferior
            ventana_del_juego.draw(texto_del_costo_total); // Dibuja el costo total
        }
    }
}

///=============================================================///
///   ABRIR Y CERRAR
///=============================================================///
void Tienda::abrir_tienda() {
    _la_tienda_esta_abierta = true; // Muestra el panel de compra
    _indice_del_item_seleccionado_en_la_tienda = 0; // Vuelve al primer item al abrir
    _cantidad_que_quiere_comprar_el_jugador = 1; // Reinicia la cantidad a 1
}

void Tienda::cerrar_tienda() {
    _la_tienda_esta_abierta = false; // Oculta el panel de compra
    _cantidad_que_quiere_comprar_el_jugador = 1; // Reinicia la cantidad a 1
}

///=============================================================///
///   NAVEGACION
///=============================================================///
void Tienda::seleccionar_item_siguiente() {
    if (_indice_del_item_seleccionado_en_la_tienda < _cantidad_de_items_cargados_en_la_tienda - 1) {
        _indice_del_item_seleccionado_en_la_tienda++; // Mueve el cursor al item siguiente
        _cantidad_que_quiere_comprar_el_jugador = 1; // Reinicia la cantidad al cambiar de item
    }
}

void Tienda::seleccionar_item_anterior() {
    if (_indice_del_item_seleccionado_en_la_tienda > 0) {
        _indice_del_item_seleccionado_en_la_tienda--; // Mueve el cursor al item anterior
        _cantidad_que_quiere_comprar_el_jugador = 1; // Reinicia la cantidad al cambiar de item
    }
}

///=============================================================///
///   CANTIDAD
///=============================================================///
void Tienda::aumentar_cantidad_a_comprar() {
    _cantidad_que_quiere_comprar_el_jugador++; // Suma una unidad mas a la cantidad
}

void Tienda::disminuir_cantidad_a_comprar() {
    if (_cantidad_que_quiere_comprar_el_jugador > 1) {
        _cantidad_que_quiere_comprar_el_jugador--; // Resta una unidad, sin bajar de 1
    }
}

///=============================================================///
///   INTENTAR COMPRAR
///=============================================================///
void Tienda::intentar_comprar_item_seleccionado(Personaje& jugador) {

    if (_la_tienda_esta_abierta == false) return; // No se puede comprar con la tienda cerrada

    Item& item_seleccionado = _items_en_venta[_indice_del_item_seleccionado_en_la_tienda]; // Referencia al item elegido

    int costo_total_de_la_compra = item_seleccionado.getPrecio() * _cantidad_que_quiere_comprar_el_jugador; // Precio total a pagar
    int oro_que_tiene_el_jugador = jugador.getOro(); // Cuanto oro tiene el jugador

    if (oro_que_tiene_el_jugador < costo_total_de_la_compra) {
        std::cout << "NO TENES SUFICIENTE ORO. NECESITAS " << costo_total_de_la_compra << " Y TENES " << oro_que_tiene_el_jugador << std::endl;
        return; // No tiene suficiente oro para comprar
    }

    int oro_que_le_queda_al_jugador = oro_que_tiene_el_jugador - costo_total_de_la_compra; // Calcula el oro restante
    jugador.setOro(oro_que_le_queda_al_jugador); // Le descuenta el oro al jugador

    for (int i = 0; i < _cantidad_que_quiere_comprar_el_jugador; i++) {
        Item copia_para_el_inventario = item_seleccionado; // Copia el item para no modificar el original
        copia_para_el_inventario.setCantidad(1); // Cada copia tiene cantidad 1
        jugador.getMochila().agarrar_item(copia_para_el_inventario); // Intenta guardarlo en la mochila
    }

    std::cout << "COMPRASTE " << _cantidad_que_quiere_comprar_el_jugador << "x " << item_seleccionado.getNombre() << ". TE QUEDAN " << oro_que_le_queda_al_jugador << " DE ORO." << std::endl;
    _cantidad_que_quiere_comprar_el_jugador = 1; // Reinicia la cantidad despues de comprar
}
