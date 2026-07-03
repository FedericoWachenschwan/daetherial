#include "Cursor_Visual.h"
#include <iostream> // Para avisar en consola si algo falla

///=======================================================///
///     #1 - CONSTRUCTOR - Carga la imagen y oculta el cursor de Windows
///=======================================================///
// #1
Cursor_Visual::Cursor_Visual() {

 // Intentamos cargar la imagen del cursor desde la carpeta assets
    if (!_textura_cursor.loadFromFile("assets/cursor.png")) {
        std::cout << "Error: no se pudo cargar assets/cursor.png" << std::endl;
    }

    _sprite_cursor.setTexture(_textura_cursor); // Le asignamos la imagen al sprite
    _sprite_cursor.setScale(0.15f, 0.15f); // Escala para el cursor de gauntlet (405x376 -> ~60x56 px)
}

///====================================================================///
///     #2 - ACTUALIZAR - Mueve el sprite a la posición actual del mouse
///====================================================================///
// #2
void Cursor_Visual::actualizar(sf::RenderWindow& ventana_del_juego) {
 // sf::RenderWindow es la clase de SFML que representa la ventana del juego.
 // Maneja todo: dibujar, leer el mouse, cambiar vistas, etc.
 // La recibimos con & para trabajar sobre la original, no sobre una copia.

 // Vector2i es un par de coordenadas X e Y de tipo entero (int).
 // El mouse devuelve píxeles exactos de pantalla, que siempre son números enteros
    sf::Vector2i posicion_mouse_en_pantalla = sf::Mouse::getPosition(ventana_del_juego);

 // El sprite de SFML necesita posiciones en float (número con decimales)
 // porque internamente SFML trabaja con coordenadas de punto flotante
 // para que los movimientos sean suaves. Por eso convertimos el int a float con (float).
    _sprite_cursor.setPosition(
        (float)posicion_mouse_en_pantalla.x, // Convertimos X de int a float
        (float)posicion_mouse_en_pantalla.y // Convertimos Y de int a float
    );
}

///=======================================================///
///     #3 - DIBUJAR - Renderiza el cursor encima de todo lo demás
///=======================================================///
// #3
void Cursor_Visual::dibujar(sf::RenderWindow& ventana_del_juego) {

 // getDefaultView() devuelve la vista original de la ventana, que muestra
 // exactamente lo que entra en la pantalla sin ninguna cámara ni desplazamiento.
 // La necesitamos acá porque durante el juego la ventana usa la vista de la
 // cámara que sigue al personaje. Si dibujamos el cursor con esa vista,
 // el cursor se movería con el mundo en vez de quedarse fijo en la pantalla.
 // Al cambiar a la vista por defecto, el cursor siempre queda pegado a la pantalla.
    ventana_del_juego.setView(ventana_del_juego.getDefaultView());

    ventana_del_juego.draw(_sprite_cursor); // Dibujamos el cursor encima de todo
}