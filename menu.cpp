#include "menu.h" // Incluye la definición de la clase Menu (archivo .h correspondiente)

Menu::Menu(float width, float height) { // Constructor del menú, recibe el ancho y alto de la ventana
    font.loadFromFile("assets/NorthEternal-yYl4V.otf");
    // Carga la fuente desde el archivo especificado (necesaria para mostrar texto)

    std::string items[] = {
        "Inicio",
        "Logros",
        "Creditos",
        "Salir"
    };
    // Arreglo de strings que contiene las opciones del menú

    for (int i = 0; i < 4; i++)
    {
        sf::Text text;
        // Crea un objeto de texto (representa una opción del menú)

        text.setFont(font);
        // Asigna la fuente cargada anteriormente al texto

        text.setString(items[i]);
        // Define el contenido del texto usando el arreglo de opciones

        text.setCharacterSize(40);
        // Define el tamaño de la letra

        text.setPosition(width / 2 - 100, height / (5 + 1) * (i + 1));
        // Posiciona el texto en la pantalla
        // width / 2 - 100 -> centra horizontalmente con un pequeño ajuste
        // height / (5 + 1) * (i + 1) -> distribuye las opciones verticalmente

        if (i == 0)
            text.setFillColor(sf::Color::Red);
        // Si es la primera opción, se pinta de rojo (indica selección inicial)
        else
            text.setFillColor(sf::Color::White);
        // Las demás opciones se pintan de blanco

        opciones.push_back(text);
        // Agrega el texto al vector de opciones del menú
    }

    selectedIndex = 0;
    // Inicializa el índice seleccionado en 0 (primera opción del menú)
}

void Menu::draw(sf::RenderWindow& window) {
    // Función que dibuja todas las opciones en la ventana

    for (auto& opcion : opciones)
        window.draw(opcion);
    // Recorre cada opción del vector y la dibuja en la ventana
}

void Menu::moveUp() {
    // Mueve la selección hacia arriba en el menú

    if (selectedIndex > 0) {
        // Verifica que no esté ya en la primera opción

        opciones[selectedIndex].setFillColor(sf::Color::White);
        // Quita el color rojo de la opción actual (la deselecciona)

        selectedIndex--;
        // Disminuye el índice para subir una posición

        opciones[selectedIndex].setFillColor(sf::Color::Red);
        // Marca la nueva opción seleccionada en rojo
    }
}

void Menu::moveDown() {
    // Mueve la selección hacia abajo en el menú

    if (selectedIndex < opciones.size() - 1) {
        // Verifica que no esté en la última opción del menú

        opciones[selectedIndex].setFillColor(sf::Color::White);
        // Quita el color rojo de la opción actual

        selectedIndex++;
        // Aumenta el índice para bajar una posición

        opciones[selectedIndex].setFillColor(sf::Color::Red);
        // Marca la nueva opción seleccionada en rojo
    }
}

int Menu::getSelectedIndex() { // 👈 FIX
    // Devuelve el índice de la opción actualmente seleccionada

    return selectedIndex;
    // Retorna el valor del índice seleccionado
}