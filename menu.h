#pragma once // Evita que este archivo se incluya más de una vez (previene errores de redefinición)

#include <SFML/Graphics.hpp> // Librería de gráficos de SFML (ventanas, texto, fuentes, etc.)
#include <vector> // Permite usar std::vector (arreglo dinámico)

class Menu // Declaración de la clase Menu (representa el menú del juego)
{
private:
    int selectedIndex; // Guarda el índice de la opción actualmente seleccionada (ej: 0 = primera opción)

    std::vector<sf::Text> opciones;
    // Vector que almacena todos los textos del menú (cada opción como "Inicio", "Salir", etc.)

    sf::Font font;
    // Fuente que se usará para mostrar el texto en pantalla

public: // 👈 TODO lo que usás desde main VA ACÁ

    Menu(float width, float height);
    // Constructor de la clase Menu
    // Recibe el ancho y alto de la ventana para poder posicionar correctamente las opciones

    void draw(sf::RenderWindow& window);
    // Dibuja todas las opciones del menú en la ventana
    // Recibe la ventana donde se renderiza el contenido

    void moveUp();
    // Mueve la selección del menú hacia arriba (disminuye el índice seleccionado)

    void moveDown();
    // Mueve la selección del menú hacia abajo (aumenta el índice seleccionado)

    int getSelectedIndex();
    // Devuelve el índice de la opción actualmente seleccionada
    // Sirve para saber qué opción eligió el usuario
};