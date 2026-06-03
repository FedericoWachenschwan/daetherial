#include "menu.h" // Incluye la definición de la clase Menu (archivo .h correspondiente)
#include <iostream> 

Menu::Menu(float width, float height) { // Constructor del menú, recibe el ancho y alto de la ventana
    font.loadFromFile("assets/NorthEternal-yYl4V.otf");
    // Carga la fuente desde el archivo especificado (necesaria para mostrar texto)

    // Cargando la imagen de fondo del menu
    if ( !fondoTexture.loadFromFile("assets/fondo-menu3.jpeg"))
    {
        //Mensaje que se muestra en caso de que no se pueda cargar la imagen de fondo
        std::cout << "Error cargando fondo del menu" << std::endl;
    }

	fondoSprite.setTexture(fondoTexture);

    // Esto hace que la imagen ocupe toda la pantalla
	fondoSprite.setScale(
       width / fondoTexture.getSize().x,
       height / fondoTexture.getSize().y
    );

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

        // Cálculo de la posición del texto para centrarlo y distribuirlo verticalmente
        float centroX = width / 2.0f;
        float inicioY = height * 0.35f;   // baja todo el menú
        float separacion = 70.0f;         // espacio entre opciones

        text.setPosition(
            centroX - 80,
            inicioY + i * separacion
        );

        // Posiciona el texto en la pantalla
        // width / 2 - 100 -> centra horizontalmente con un pequeño ajuste
        // height / (5 + 1) * (i + 1) -> distribuye las opciones verticalmente

        if (i == 0)
            text.setFillColor(sf::Color(180, 50, 30)); // Al seleccionar una opcion
        // Si es la primera opción, se pinta de rojo (indica selección inicial)
        else
            text.setFillColor(sf::Color(60, 40, 20));  // Opciones no seleccionadas 
        // Las demás opciones se pintan de blanco

        opciones.push_back(text);
        // Agrega el texto al vector de opciones del menú

        sf::RectangleShape boton;
        // Llamamos a SFML para crear al boton

        boton.setSize(sf::Vector2f(300, 60));
        // Tamaño del boton

        // Posicion del boton (La logica es igual que la del texto)
        centroX = width / 2.0f;
        inicioY = height * 0.35f;
        separacion = 70.0f;

        boton.setPosition(
            centroX - 150, // mitad del ancho del botón
            inicioY + i * separacion
        );

        // Estilos del boton
        boton.setFillColor(sf::Color(200, 180, 140)); // color pergamino
        boton.setOutlineThickness(2);
        boton.setOutlineColor(sf::Color(60, 40, 20)); // marrón oscuro

        // Hacemos que el boton se muestre
        botones.push_back(boton);

        sf::FloatRect bounds = text.getLocalBounds();
        // Obtiene los límites locales del texto (posición interna, ancho y alto)
        // bounds.left y bounds.top indican el offset interno del texto
        // bounds.width y bounds.height representan el tamaño real del texto

        text.setOrigin(bounds.left + bounds.width / 2.0f,
            bounds.top + bounds.height / 2.0f);
        // Define el origen del texto en su centro exacto
        // Se corrige usando left/top porque el texto en SFML no siempre empieza en (0,0)
        // Esto permite que el posicionamiento sea realmente centrado

        text.setPosition(
            centroX,
            inicioY + i * separacion + 30 // centro del botón
        );
        // Posiciona el texto en pantalla
        // centroX lo alinea horizontalmente al centro
        // inicioY + i * separacion distribuye las opciones verticalmente
        // +30 ajusta el texto al centro vertical del botón (ya que el botón mide 60px de alto)

    }

    selectedIndex = 0;
    // Inicializa el índice seleccionado en 0 (primera opción del menú)
}

void Menu::draw(sf::RenderWindow& window) {
    // Primero pintamos la imagen en la ventana
	window.draw(fondoSprite);
    
	// Funcion que dubuja los botones del menu
    for (auto& boton : botones)
        window.draw(boton);

    // Función que dibuja todas las opciones en la ventana

    for (auto& opcion : opciones)
        window.draw(opcion);
    // Recorre cada opción del vector y la dibuja en la ventana
}

void Menu::moveUp() {
    // Mueve la selección hacia arriba en el menú

    if (selectedIndex > 0) {
        // Verifica que no esté ya en la primera opción

        opciones[selectedIndex].setFillColor(sf::Color(60, 40, 20));
        // Restaura el color original de la opción actual

        selectedIndex--;
        // Disminuye el índice para subir una posición

        opciones[selectedIndex].setFillColor(sf::Color(180, 150, 90));
        // Marca la nueva opción seleccionada
    }
}

void Menu::moveDown() {
    // Mueve la selección hacia abajo en el menú

    if (selectedIndex < static_cast<int>(opciones.size()) - 1) {
        // Verifica que no esté en la última opción del menú

        opciones[selectedIndex].setFillColor(sf::Color(60, 40, 20));
        // Restaura el color original de la opción actual

        selectedIndex++;
        // Aumenta el índice para bajar una posición

        opciones[selectedIndex].setFillColor(sf::Color(180, 150, 90));
        // Marca la nueva opción seleccionada
    }
}

int Menu::getSelectedIndex() { // 👈 FIX
    // Devuelve el índice de la opción actualmente seleccionada

    return selectedIndex;
    // Retorna el valor del índice seleccionado
}