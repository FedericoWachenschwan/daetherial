#pragma once
#include <SFML/Graphics.hpp>    // Necesitamos SFML para el sprite y la textura

///=======================================================///
///         CLASE CURSOR - Reemplaza el cursor del sistema
///=======================================================///
class Cursor_Visual {
private:

    sf::Texture _textura_cursor;    // La imagen del cursor cargada en memoria
    sf::Sprite  _sprite_cursor;     // Lo que se dibuja en pantalla siguiendo al mouse

public:

    Cursor_Visual();

    // sf::RenderWindow es la ventana del juego que maneja todo lo visual.
    // Se pasa con & (referencia) porque no queremos copiarla, queremos trabajar
    // sobre la misma ventana que ya existe en GameManager. Por eso no llega
    // con un tipo nuevo, ya tiene el suyo: sf::RenderWindow
    void actualizar(sf::RenderWindow& ventana_del_juego);   // Mueve el sprite a donde está el mouse
    void dibujar(sf::RenderWindow& ventana_del_juego);      // Dibuja el cursor en pantalla
};