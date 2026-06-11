#include "Tienda.h"
#include "Personaje.h"
#include <iostream>
#include <string>

///=============================================================///
///   CONSTRUCTOR DE LA TIENDA - Carga imagen, cartel y zona de detección
///=============================================================///
Tienda::Tienda(sf::Vector2f posicion_de_la_tienda_en_el_mapa, Item* item_que_vende_la_tienda, int precio_del_item_que_vende_la_tienda) {

    ///=========================================================///
    ///   DATOS DE LA TIENDA - Guardamos el item, precio y estado inicial
    ///=========================================================///
    _item_que_vende_la_tienda = item_que_vende_la_tienda; // Guardamos el item que va a vender
    _precio_del_item_que_vende_la_tienda = precio_del_item_que_vende_la_tienda; // Guardamos cuánto cuesta el item
    _el_jugador_esta_cerca_de_la_tienda = false; // Al arrancar el juego el jugador no está cerca

    ///=========================================================///
    ///   IMAGEN DEL SPRITE DE LA TIENDA - El dibujo que se ve en el mapa
    ///=========================================================///
    bool se_pudo_cargar_la_imagen = _textura_de_la_tienda.loadFromFile("assets/tienda.png"); // Intentamos cargar la imagen

    if (se_pudo_cargar_la_imagen == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA IMAGEN DE LA TIENDA" << std::endl;
    }

    _sprite_de_la_tienda.setTexture(_textura_de_la_tienda); // Le asignamos la imagen al sprite
    _sprite_de_la_tienda.setPosition(posicion_de_la_tienda_en_el_mapa); // Lo ponemos en el mapa

    ///=========================================================///
    ///   ZONA DE DETECCIÓN DE LA TIENDA - El área invisible donde
    ///   el jugador activa el cartel al entrar.
    ///   Cambiá los 4 números para ajustar la zona:
    ///   - Primer número: posición X de la zona en el mapa
    ///   - Segundo número: posición Y de la zona en el mapa
    ///   - Tercer número: ancho de la zona en píxeles
    ///   - Cuarto número: alto de la zona en píxeles
    ///=========================================================///
    _zona_de_interaccion_de_la_tienda = sf::FloatRect(220.f, 380.f, 100.f, 100.f); // X, Y, ancho, alto
}

///=============================================================///
///   DESTRUCTOR DE LA TIENDA - Libera la memoria al cerrar el juego
///=============================================================///
Tienda::~Tienda() {
    delete _item_que_vende_la_tienda; // Borramos el item de la memoria
    _item_que_vende_la_tienda = nullptr; // Lo ponemos en cero para que no apunte a basura
}

///=============================================================///
///   CARGAR FUENTE Y CARTEL - Se llama una sola vez al crear la tienda
///=============================================================///
void Tienda::cargar_fuente_y_cartel() {

    ///=========================================================///
    ///   FUENTE DEL CARTEL - Carga el tipo de letra del texto
    ///=========================================================///
    bool se_pudo_cargar_la_fuente = _fuente_del_cartel_de_la_tienda.loadFromFile("assets/NorthEternal.otf"); // Intentamos cargar la fuente

    if (se_pudo_cargar_la_fuente == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA FUENTE DEL CARTEL" << std::endl;
    }

    ///=========================================================///
    ///   TEXTO DEL CARTEL - Lo que dice el cartel y cómo se ve
    ///=========================================================///
    _cartel_de_la_tienda.setFont(_fuente_del_cartel_de_la_tienda); // Le asignamos el tipo de letra
    _cartel_de_la_tienda.setCharacterSize(14); // Tamaño de la letra en píxeles
    _cartel_de_la_tienda.setFillColor(sf::Color::Yellow); // Color amarillo para que se vea bien

    std::string nombre_del_item = _item_que_vende_la_tienda->getNombre(); // Nombre del item
    std::string precio_como_texto = std::to_string(_precio_del_item_que_vende_la_tienda); // Precio en texto
    std::string texto_completo = "Presiona E para comprar\n" + nombre_del_item + " - " + precio_como_texto + " oro"; // Texto completo
    _cartel_de_la_tienda.setString(texto_completo); // Le asignamos el texto al cartel
}

///=============================================================///
///   ACTUALIZAR TIENDA - Chequea si el jugador pisó la zona de detección
///=============================================================///
void Tienda::actualizar_tienda(sf::Vector2f posicion_actual_del_jugador) {

    ///=========================================================///
    ///   DETECCIÓN DE CERCANÍA - Chequeamos si el jugador está
    ///   dentro del rectángulo invisible de la tienda
    ///=========================================================///
    bool jugador_dentro_de_la_zona = _zona_de_interaccion_de_la_tienda.contains(posicion_actual_del_jugador); // true si el jugador está dentro

    if (jugador_dentro_de_la_zona == true) {
        _el_jugador_esta_cerca_de_la_tienda = true; // El jugador entró, mostramos el cartel
    }
    else {
        _el_jugador_esta_cerca_de_la_tienda = false; // El jugador salió, ocultamos el cartel
    }

    ///=========================================================///
    ///   POSICIÓN DEL CARTEL - Siempre aparece encima de la zona
    ///=========================================================///
    float posicion_x_del_cartel = _zona_de_interaccion_de_la_tienda.left; // Misma X que la zona
    float posicion_y_del_cartel = _zona_de_interaccion_de_la_tienda.top - 30.f; // 30 píxeles encima de la zona

    _cartel_de_la_tienda.setPosition(posicion_x_del_cartel, posicion_y_del_cartel); // Actualizamos la posición del cartel
}

///=============================================================///
///   DIBUJAR TIENDA - Dibuja el sprite, la zona de detección y el cartel
///=============================================================///
void Tienda::dibujar_tienda(sf::RenderWindow& ventana_del_juego, bool mostrar_zona_de_deteccion) {

    ///=========================================================///
    ///   DIBUJO DEL SPRITE - El edificio de la tienda en el mapa
    ///=========================================================///
    ventana_del_juego.draw(_sprite_de_la_tienda); // Dibujamos el edificio

    ///=========================================================///
    ///   DIBUJO DE LA ZONA DE DETECCIÓN - Solo se muestra cuando
    ///   el debug está activo (tecla F3)
    ///   sf::RectangleShape es un rectángulo que SFML dibuja en pantalla
    ///=========================================================///
    if (mostrar_zona_de_deteccion == true) {
        sf::RectangleShape hud_de_la_zona_de_deteccion; // Creamos el rectángulo verde
        hud_de_la_zona_de_deteccion.setPosition(_zona_de_interaccion_de_la_tienda.left, _zona_de_interaccion_de_la_tienda.top + 20); // Posición de la zona
        hud_de_la_zona_de_deteccion.setSize(sf::Vector2f(_zona_de_interaccion_de_la_tienda.width, _zona_de_interaccion_de_la_tienda.height)); // Tamaño de la zona
        hud_de_la_zona_de_deteccion.setFillColor(sf::Color(0, 255, 0, 50)); // Verde semitransparente
        hud_de_la_zona_de_deteccion.setOutlineColor(sf::Color::Green); // Borde verde
        hud_de_la_zona_de_deteccion.setOutlineThickness(1.f); // Grosor del borde
        ventana_del_juego.draw(hud_de_la_zona_de_deteccion); // Dibujamos el rectángulo
    }

    ///=========================================================///
    ///   DIBUJO DEL CARTEL - Solo aparece cuando el jugador está
    ///   dentro de la zona de detección de la tienda
    ///=========================================================///
    bool jugador_esta_cerca = _el_jugador_esta_cerca_de_la_tienda; // Copiamos el valor del atributo

    if (jugador_esta_cerca == true) {
        ventana_del_juego.draw(_cartel_de_la_tienda); // Dibujamos el cartel
    }
}

///=============================================================///
///   INTENTAR COMPRAR ITEM - Ejecuta la compra si el jugador puede pagar
///=============================================================///
void Tienda::intentar_comprar_item(Personaje& jugador_que_quiere_comprar) {

    ///=========================================================///
    ///   VERIFICACIÓN DE CERCANÍA - El jugador tiene que estar
    ///   dentro de la zona de detección para poder comprar
    ///=========================================================///
    bool jugador_esta_cerca = _el_jugador_esta_cerca_de_la_tienda; // Copiamos el valor del atributo

    if (jugador_esta_cerca == false) {
        return; // Si no está cerca salimos sin hacer nada
    }

    ///=========================================================///
    ///   VERIFICACIÓN DE ORO - El jugador tiene que tener suficiente
    ///   oro para pagar el item de la tienda
    ///=========================================================///
    int oro_que_tiene_el_jugador = jugador_que_quiere_comprar.getOro(); // Cuánto oro tiene el jugador
    int precio_del_item = _precio_del_item_que_vende_la_tienda; // Cuánto cuesta el item

    if (oro_que_tiene_el_jugador < precio_del_item) {
        std::cout << "NO TENES SUFICIENTE ORO. NECESITAS " << precio_del_item << " Y TENES " << oro_que_tiene_el_jugador << std::endl;
        return; // Salimos sin hacer la compra
    }

    ///=========================================================///
    ///   EJECUCIÓN DE LA COMPRA - Descontamos el oro y confirmamos
    ///=========================================================///
    int oro_que_le_queda_al_jugador = oro_que_tiene_el_jugador - precio_del_item; // Calculamos cuánto oro le queda
    jugador_que_quiere_comprar.setOro(oro_que_le_queda_al_jugador); // Le actualizamos el oro

    std::string nombre_del_item_comprado = _item_que_vende_la_tienda->getNombre(); // Nombre del item comprado
    std::cout << "COMPRASTE " << nombre_del_item_comprado << ". TE QUEDAN " << oro_que_le_queda_al_jugador << " DE ORO." << std::endl;
}

///=============================================================///
///   GETTER - Devuelve si el jugador está dentro de la zona de la tienda
///=============================================================///
bool Tienda::getJugadorEstaCercaDeLaTienda() {
    return _el_jugador_esta_cerca_de_la_tienda; // Devolvemos el valor del atributo
}