#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Item.h" // Porque necesitamos conocer a Consumible, Mueble, etc.

class ItemManager {
private:
	// Una sola textura para todos los items, con cada item leyendo de una sección diferente (idTextura)
    sf::Texture _texturaMaestraItems;

	// ==== NUEVO ====
	std::string _rutaArchivoItems = "assets/items.dat"; // Ruta al archivo de datos de items

public:
    ItemManager();

	// ---- Métodos de ABML ---
	bool guardarRegistro(const ItemReg registro) const; // ALTA
	bool modificarRegistro(const ItemReg registro) const; // MODIFICACIÓN
	ItemReg leerRegistro(int posicion) const; // LECTURA
	std::vector<ItemReg> leerTodos() const; // Carga todos los registros activos para el juego

    // Metodos auxiliares utiles
	int contarRegistros() const; // Para saber cuántos registros hay en el archivo (incluyendo los inactivos
	int buscarPorId(int id) const; // Devuelve la posición del registro con el ID dado, o -1 si no se encuentra)

    // 🏭 Métodos de fabricación
	Item* crearItemPorId(int id) const; // Crea un item a partir de su ID leyendo el registro del archivo)

	// 🖼️ Getter para la textura maestra (Flyweight Pattern)
    sf::Texture& getTexturaMaestra() { return _texturaMaestraItems; }
 
};


/*Caché de Texturas (Flyweight Pattern): 
Guardar sf::Texture como variables privadas del manager y pasar una referencia (&) 
con el getter es la forma perfecta de manejar gráficos en SFML. Si cada poción cargara su propia textura, 
al tirar 100 pociones al piso te quedarías sin memoria de video (VRAM). De esta forma, tenés 100 pociones leyendo de 1 sola textura. 
¡Excelente optimización!

La Fábrica Pura: Como hablábamos, tu ItemManager es estrictamente una fábrica. Nace, carga las texturas, y se dedica a escupir punteros (crearPocionVida()).
*/