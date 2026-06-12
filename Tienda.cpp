#include "Tienda.h"
#include "Personaje.h"
#include <iostream>
#include <string>
#include <cmath>

///=============================================================///
///   CONSTRUCTOR - Prepara la tienda con su posición
///=============================================================///
Tienda::Tienda(sf::Vector2f posicion_de_la_tienda_en_el_mapa) {

    ///=========================================================///
    ///   INICIALIZAMOS EL ARRAY DE ITEMS EN CERO
    ///=========================================================///
    for (int i = 0; i < CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA; i++) {
        _items_en_venta[i] = nullptr; // Cada slot empieza vacío
    }

    _cantidad_de_items_cargados_en_la_tienda = 0;     // Al inicio no hay items cargados
    _indice_del_item_seleccionado_en_la_tienda = 0;     // El primer item está seleccionado
    _cantidad_que_quiere_comprar_el_jugador = 1;     // Por defecto quiere comprar 1
    _el_jugador_esta_cerca_de_la_tienda = false; // Al inicio el jugador no está cerca
    _la_tienda_esta_abierta = false; // Al inicio la tienda está cerrada

    ///=========================================================///
    ///   IMAGEN DEL SPRITE DE LA TIENDA
    ///=========================================================///
    bool se_pudo_cargar_la_imagen = _textura_de_la_tienda.loadFromFile("assets/tienda.png");
    if (se_pudo_cargar_la_imagen == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA IMAGEN DE LA TIENDA" << std::endl;
    }

    _sprite_de_la_tienda.setTexture(_textura_de_la_tienda); // Le asignamos la imagen al sprite
    _sprite_de_la_tienda.setPosition(posicion_de_la_tienda_en_el_mapa); // Lo ponemos en el mapa

    ///=========================================================///
    ///   ZONA DE DETECCIÓN - El rectángulo invisible que detecta al jugador
    ///   Cambiá los 4 números para ajustar la zona:
    ///   Primer número: X | Segundo: Y | Tercero: ancho | Cuarto: alto
    ///=========================================================///
    _zona_de_interaccion_de_la_tienda = sf::FloatRect(220.f, 380.f, 100.f, 100.f);
}

///=============================================================///
///   DESTRUCTOR - Libera la memoria de los items al cerrar el juego
///=============================================================///
Tienda::~Tienda() {
    for (int i = 0; i < CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA; i++) {
        if (_items_en_venta[i] != nullptr) {
            delete _items_en_venta[i]; // Borramos cada item de la memoria
            _items_en_venta[i] = nullptr; // Lo ponemos en cero
        }
    }
}

///=============================================================///
///   AGREGAR ITEM EN VENTA - Agrega un item a la lista de la tienda
///=============================================================///
void Tienda::agregarItemEnVenta(Item* item_para_agregar) {

    if (item_para_agregar == nullptr) return; // Si el item no existe no hacemos nada

    if (_cantidad_de_items_cargados_en_la_tienda >= CANTIDAD_MAXIMA_DE_ITEMS_EN_LA_TIENDA) {
        std::cout << "ERROR: LA TIENDA YA TIENE LA CANTIDAD MAXIMA DE ITEMS" << std::endl;
        return; // Si ya está llena no agregamos más
    }

    _items_en_venta[_cantidad_de_items_cargados_en_la_tienda] = item_para_agregar; // Guardamos el item
    _cantidad_de_items_cargados_en_la_tienda = _cantidad_de_items_cargados_en_la_tienda + 1; // Contamos uno más
}

///=============================================================///
///   CARGAR FUENTE Y CARTEL - Se llama una sola vez al crear la tienda
///=============================================================///
void Tienda::cargar_fuente_y_cartel() {

    bool se_pudo_cargar_la_fuente = _fuente_del_cartel_de_la_tienda.loadFromFile("assets/NorthEternal.otf");
    if (se_pudo_cargar_la_fuente == false) {
        std::cout << "ERROR: NO SE PUDO CARGAR LA FUENTE DEL CARTEL" << std::endl;
    }

    _cartel_de_la_tienda.setFont(_fuente_del_cartel_de_la_tienda); // Le asignamos el tipo de letra
    _cartel_de_la_tienda.setCharacterSize(14);                     // Tamaño de la letra
    _cartel_de_la_tienda.setFillColor(sf::Color::Yellow);          // Color amarillo
    _cartel_de_la_tienda.setString("Presiona E para abrir la tienda"); // Lo que dice el cartel
}

///=============================================================///
///   ACTUALIZAR TIENDA - Chequea si el jugador está dentro de la zona
///=============================================================///
void Tienda::actualizar_tienda(sf::Vector2f posicion_actual_del_jugador) {

    bool jugador_dentro_de_la_zona = _zona_de_interaccion_de_la_tienda.contains(posicion_actual_del_jugador);

    if (jugador_dentro_de_la_zona == true) {
        _el_jugador_esta_cerca_de_la_tienda = true; // El jugador entró, mostramos el cartel
    }
    else {
        _el_jugador_esta_cerca_de_la_tienda = false; // El jugador salió, ocultamos el cartel
        _la_tienda_esta_abierta = false; // Si se aleja cerramos la tienda también
    }

    ///=========================================================///
    ///   POSICIÓN DEL CARTEL - Siempre aparece encima de la zona
    ///=========================================================///
    float posicion_x_del_cartel = _zona_de_interaccion_de_la_tienda.left;
    float posicion_y_del_cartel = _zona_de_interaccion_de_la_tienda.top - 30.f;
    _cartel_de_la_tienda.setPosition(posicion_x_del_cartel, posicion_y_del_cartel);
}

///=============================================================///
///   DIBUJAR SPRITE EN EL MAPA - Dibuja el edificio y la zona de detección
///=============================================================///
void Tienda::dibujar_sprite_en_el_mapa(sf::RenderWindow& ventana_del_juego, bool mostrar_zona_de_deteccion) {

    ventana_del_juego.draw(_sprite_de_la_tienda); // Dibujamos el edificio

    ///=========================================================///
    ///   ZONA DE DETECCIÓN VISIBLE - Solo con F3 activado
    ///   sf::RectangleShape es un rectángulo que SFML dibuja en pantalla
    ///=========================================================///
    if (mostrar_zona_de_deteccion == true) {
        sf::RectangleShape hud_de_la_zona_de_deteccion; // Creamos el rectángulo verde
        hud_de_la_zona_de_deteccion.setPosition(_zona_de_interaccion_de_la_tienda.left, _zona_de_interaccion_de_la_tienda.top + 20);
        hud_de_la_zona_de_deteccion.setSize(sf::Vector2f(_zona_de_interaccion_de_la_tienda.width, _zona_de_interaccion_de_la_tienda.height));
        hud_de_la_zona_de_deteccion.setFillColor(sf::Color(0, 255, 0, 50));   // Verde semitransparente
        hud_de_la_zona_de_deteccion.setOutlineColor(sf::Color::Green);         // Borde verde
        hud_de_la_zona_de_deteccion.setOutlineThickness(1.f);                  // Grosor del borde
        ventana_del_juego.draw(hud_de_la_zona_de_deteccion);
    }

    ///=========================================================///
    ///   CARTEL - Solo cuando el jugador está cerca y la tienda no está abierta
    ///=========================================================///
    if (_el_jugador_esta_cerca_de_la_tienda == true && _la_tienda_esta_abierta == false) {
        ventana_del_juego.draw(_cartel_de_la_tienda); // Dibujamos el cartel
    }
}

///=============================================================///
///   DIBUJAR INTERFAZ DE COMPRA - Dibuja el menú de items cuando está abierto
///   Esta función se llama con la vista default (coordenadas de pantalla)
///=============================================================///
void Tienda::dibujar_interfaz_de_compra(sf::RenderWindow& ventana_del_juego, sf::Font& fuente_del_hud) {

    if (_la_tienda_esta_abierta == false) return; // Si no está abierta no dibujamos nada

    ///=========================================================///
    ///   PANEL DE FONDO - El rectángulo oscuro detrás de los items
    ///=========================================================///
    float ancho_del_panel = 620.f;  // Ancho total del panel de la tienda
    float alto_del_panel = 220.f;  // Alto total del panel de la tienda
    float posicion_x_del_panel = (1280.f - ancho_del_panel) / 2.f; // Centrado en pantalla
    float posicion_y_del_panel = 720.f - alto_del_panel - 20.f;    // Abajo de la pantalla

    sf::RectangleShape panel_de_fondo; // El rectángulo oscuro de fondo
    panel_de_fondo.setPosition(posicion_x_del_panel, posicion_y_del_panel); // Lo posicionamos
    panel_de_fondo.setSize(sf::Vector2f(ancho_del_panel, alto_del_panel));   // Le damos tamaño
    panel_de_fondo.setFillColor(sf::Color(20, 20, 20, 220));                 // Negro semitransparente
    panel_de_fondo.setOutlineColor(sf::Color(200, 160, 50));                 // Borde dorado
    panel_de_fondo.setOutlineThickness(2.f);                                 // Grosor del borde
    ventana_del_juego.draw(panel_de_fondo);                                  // Dibujamos el panel

    ///=========================================================///
    ///   TITULO DE LA TIENDA
    ///=========================================================///
    sf::Text titulo_de_la_tienda;
    titulo_de_la_tienda.setFont(fuente_del_hud);
    titulo_de_la_tienda.setCharacterSize(16);
    titulo_de_la_tienda.setFillColor(sf::Color(200, 160, 50)); // Color dorado
    titulo_de_la_tienda.setString("TIENDA - Usa flechas para navegar, E para comprar, ESC para cerrar");
    titulo_de_la_tienda.setPosition(posicion_x_del_panel + 10.f, posicion_y_del_panel + 5.f);
    ventana_del_juego.draw(titulo_de_la_tienda);

    ///=========================================================///
    ///   RECUADROS DE CADA ITEM - Uno al lado del otro
    ///=========================================================///
    float ancho_del_recuadro = 180.f; // Ancho de cada recuadro de item
    float alto_del_recuadro = 150.f; // Alto de cada recuadro de item
    float margen_entre_recuadros = 20.f; // Espacio entre recuadros
    float inicio_x_de_los_recuadros = posicion_x_del_panel + 15.f; // Donde empieza el primer recuadro
    float inicio_y_de_los_recuadros = posicion_y_del_panel + 35.f; // Donde empieza en Y

    for (int i = 0; i < _cantidad_de_items_cargados_en_la_tienda; i++) {

        float posicion_x_del_recuadro = inicio_x_de_los_recuadros + i * (ancho_del_recuadro + margen_entre_recuadros); // Cada recuadro va a la derecha del anterior
        float posicion_y_del_recuadro = inicio_y_de_los_recuadros; // Todos en la misma altura

        ///=========================================================///
        ///   DIBUJAMOS EL RECUADRO DEL ITEM
        ///   El seleccionado se pone amarillo, los demás grises
        ///=========================================================///
        sf::RectangleShape recuadro_del_item;
        recuadro_del_item.setPosition(posicion_x_del_recuadro, posicion_y_del_recuadro);
        recuadro_del_item.setSize(sf::Vector2f(ancho_del_recuadro, alto_del_recuadro));
        recuadro_del_item.setFillColor(sf::Color(40, 40, 40, 200)); // Gris oscuro

        if (i == _indice_del_item_seleccionado_en_la_tienda) {
            recuadro_del_item.setOutlineColor(sf::Color::Yellow); // Borde amarillo si está seleccionado
            recuadro_del_item.setOutlineThickness(3.f);
        }
        else {
            recuadro_del_item.setOutlineColor(sf::Color(100, 100, 100)); // Borde gris si no está seleccionado
            recuadro_del_item.setOutlineThickness(1.f);
        }

        ventana_del_juego.draw(recuadro_del_item); // Dibujamos el recuadro

        ///=========================================================///
        ///   NOMBRE DEL ITEM
        ///=========================================================///
        sf::Text texto_del_nombre_del_item;
        texto_del_nombre_del_item.setFont(fuente_del_hud);
        texto_del_nombre_del_item.setCharacterSize(12);
        texto_del_nombre_del_item.setFillColor(sf::Color::White);
        texto_del_nombre_del_item.setString(_items_en_venta[i]->getNombre()); // Nombre del item
        texto_del_nombre_del_item.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 8.f);
        ventana_del_juego.draw(texto_del_nombre_del_item);

        ///=========================================================///
        ///   PRECIO DEL ITEM
        ///=========================================================///
        sf::Text texto_del_precio_del_item;
        texto_del_precio_del_item.setFont(fuente_del_hud);
        texto_del_precio_del_item.setCharacterSize(12);
        texto_del_precio_del_item.setFillColor(sf::Color::Yellow);
        std::string precio_como_texto = std::to_string(_items_en_venta[i]->getPrecio()) + " oro"; // Precio en texto
        texto_del_precio_del_item.setString(precio_como_texto);
        texto_del_precio_del_item.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 30.f);
        ventana_del_juego.draw(texto_del_precio_del_item);

        ///=========================================================///
        ///   CANTIDAD A COMPRAR - Solo para el item seleccionado
        ///=========================================================///
        if (i == _indice_del_item_seleccionado_en_la_tienda) {
            sf::Text texto_de_cantidad;
            texto_de_cantidad.setFont(fuente_del_hud);
            texto_de_cantidad.setCharacterSize(12);
            texto_de_cantidad.setFillColor(sf::Color::Cyan);
            std::string cantidad_como_texto = "Cant: " + std::to_string(_cantidad_que_quiere_comprar_el_jugador);
            texto_de_cantidad.setString(cantidad_como_texto);
            texto_de_cantidad.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 55.f);
            ventana_del_juego.draw(texto_de_cantidad);

            sf::Text texto_de_flechas;
            texto_de_flechas.setFont(fuente_del_hud);
            texto_de_flechas.setCharacterSize(10);
            texto_de_flechas.setFillColor(sf::Color(150, 150, 150));
            texto_de_flechas.setString("Arr/Ab para cantidad");
            texto_de_flechas.setPosition(posicion_x_del_recuadro + 8.f, posicion_y_del_recuadro + 75.f);
            ventana_del_juego.draw(texto_de_flechas);

            // Costo total
            int costo_total = _items_en_venta[i]->getPrecio() * _cantidad_que_quiere_comprar_el_jugador;
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
///   ABRIR Y CERRAR LA TIENDA
///=============================================================///
void Tienda::abrir_tienda() {
    _la_tienda_esta_abierta = true;                     // Abrimos la tienda
    _indice_del_item_seleccionado_en_la_tienda = 0;    // Volvemos al primer item
    _cantidad_que_quiere_comprar_el_jugador = 1;        // Reseteamos la cantidad
}

void Tienda::cerrar_tienda() {
    _la_tienda_esta_abierta = false;                    // Cerramos la tienda
    _cantidad_que_quiere_comprar_el_jugador = 1;        // Reseteamos la cantidad
}

///=============================================================///
///   NAVEGACIÓN - Cambia el item seleccionado
///=============================================================///
void Tienda::seleccionar_item_siguiente() {
    if (_indice_del_item_seleccionado_en_la_tienda < _cantidad_de_items_cargados_en_la_tienda - 1) {
        _indice_del_item_seleccionado_en_la_tienda = _indice_del_item_seleccionado_en_la_tienda + 1; // Pasamos al siguiente
        _cantidad_que_quiere_comprar_el_jugador = 1; // Reseteamos la cantidad al cambiar de item
    }
}

void Tienda::seleccionar_item_anterior() {
    if (_indice_del_item_seleccionado_en_la_tienda > 0) {
        _indice_del_item_seleccionado_en_la_tienda = _indice_del_item_seleccionado_en_la_tienda - 1; // Pasamos al anterior
        _cantidad_que_quiere_comprar_el_jugador = 1; // Reseteamos la cantidad al cambiar de item
    }
}

///=============================================================///
///   CANTIDAD - Cambia cuántos quiere comprar el jugador
///=============================================================///
void Tienda::aumentar_cantidad_a_comprar() {
    _cantidad_que_quiere_comprar_el_jugador = _cantidad_que_quiere_comprar_el_jugador + 1; // Subimos uno
}

void Tienda::disminuir_cantidad_a_comprar() {
    if (_cantidad_que_quiere_comprar_el_jugador > 1) {
        _cantidad_que_quiere_comprar_el_jugador = _cantidad_que_quiere_comprar_el_jugador - 1; // Bajamos uno, mínimo 1
    }
}

///=============================================================///
///   INTENTAR COMPRAR - Ejecuta la compra si el jugador puede pagar
///=============================================================///
void Tienda::intentar_comprar_item_seleccionado(Personaje& jugador) {

    if (_la_tienda_esta_abierta == false) return; // Si la tienda no está abierta no hacemos nada

    Item* item_seleccionado = _items_en_venta[_indice_del_item_seleccionado_en_la_tienda]; // El item que eligió

    if (item_seleccionado == nullptr) return; // Si el item no existe no hacemos nada

    int costo_total_de_la_compra = item_seleccionado->getPrecio() * _cantidad_que_quiere_comprar_el_jugador; // Calculamos cuánto cuesta
    int oro_que_tiene_el_jugador = jugador.getOro(); // Cuánto oro tiene

    if (oro_que_tiene_el_jugador < costo_total_de_la_compra) {
        std::cout << "NO TENES SUFICIENTE ORO. NECESITAS " << costo_total_de_la_compra << " Y TENES " << oro_que_tiene_el_jugador << std::endl;
        return; // Si no alcanza el oro salimos
    }

    ///=========================================================///
    ///   EJECUTAMOS LA COMPRA - Restamos el oro y damos el item
    ///=========================================================///
    int oro_que_le_queda_al_jugador = oro_que_tiene_el_jugador - costo_total_de_la_compra; // Calculamos cuánto le queda
    jugador.setOro(oro_que_le_queda_al_jugador); // Le actualizamos el oro

    // Agregamos los items al inventario uno por uno
    for (int i = 0; i < _cantidad_que_quiere_comprar_el_jugador; i++) {
        Item* copia_del_item = new Item( // Creamos una copia del item para el inventario
            item_seleccionado->getId(),
            item_seleccionado->getNombre(),
            item_seleccionado->getTipo(),
            item_seleccionado->getPrecio(),
            1,
            item_seleccionado->getMaxStack(),
            item_seleccionado->esAgarrable(),
            item_seleccionado->getPuntosDeCluracion(),
            item_seleccionado->getBonusDeAtaque(),
            item_seleccionado->getBonusDeDefensa()
        );
        const sf::Texture* textura_del_item_original = item_seleccionado->getSprite().getTexture(); // Obtenemos el puntero a la textura
        if (textura_del_item_original != nullptr) {
            copia_del_item->getSprite().setTexture(*textura_del_item_original); // El * convierte el puntero en referencia para que setTexture lo acepte
        }
        copia_del_item->getSprite().setTextureRect(item_seleccionado->getSprite().getTextureRect()); // Copiamos el recorte
        jugador.getInventario().agarrarItem(copia_del_item); // Lo mandamos al inventario
    }

    std::cout << "COMPRASTE " << _cantidad_que_quiere_comprar_el_jugador << "x " << item_seleccionado->getNombre() << ". TE QUEDAN " << oro_que_le_queda_al_jugador << " DE ORO." << std::endl;
    _cantidad_que_quiere_comprar_el_jugador = 1; // Reseteamos la cantidad después de comprar
}

///=============================================================///
///   GETTERS
///=============================================================///
bool Tienda::getJugadorEstaCercaDeLaTienda() const { return _el_jugador_esta_cerca_de_la_tienda; }
bool Tienda::getLaTiendaEstaAbierta() const { return _la_tienda_esta_abierta; }