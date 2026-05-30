#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h" // Porque necesitamos conocer a Consumible, Mueble, etc.

class ItemManager {
private:
    // Guardamos las texturas acá para no saturar la memoria RAM
    sf::Texture _texturaPocionVida;
    sf::Texture _texturaHorno;

public:
    ItemManager(); // El constructor va a cargar las imágenes al arrancar el juego

    // 🏭 Métodos de fabricación (Devuelven el objeto ya configurado)
    Item* crearPocionVida();
    Item* crearHorno();

    // 🖼️ Getters para que el ObjectsManager pueda usar las texturas
    sf::Texture& getTexturaPocionVida() { return _texturaPocionVida; }
    sf::Texture& getTexturaHorno() { return _texturaHorno; }
};


/*Caché de Texturas (Flyweight Pattern): 
Guardar sf::Texture como variables privadas del manager y pasar una referencia (&) 
con el getter es la forma perfecta de manejar gráficos en SFML. Si cada poción cargara su propia textura, 
al tirar 100 pociones al piso te quedarías sin memoria de video (VRAM). De esta forma, tenés 100 pociones leyendo de 1 sola textura. 
¡Excelente optimización!

La Fábrica Pura: Como hablábamos, tu ItemManager es estrictamente una fábrica. Nace, carga las texturas, y se dedica a escupir punteros (crearPocionVida()).
*/