#pragma once
#include <SFML/Graphics.hpp> // Para usar sf::Sprite, sf::Texture, etc.
#include <vector>            // Para crear la matriz del mapa
#include <string>            // Para manejar rutas de archivos
#include "Colisionable.h"    // Interfaz polimórfica

using namespace std;

// 🧱 Clase para los bloques sólidos del mapa
class BloqueMapa : public Colisionable {
private:
	sf::FloatRect _hitbox; // La caja de colisión real en el mundo
public:
	BloqueMapa(float x, float y, float tam)
		: _hitbox(x, y, tam, tam) {
	}

	// Implementación obligatoria de la interfaz
	sf::FloatRect getBounds() const override { return _hitbox; }
};

class Map {
private:
	// Atributos privados del mapa
	vector<vector<int>> mapa; // Matriz del mapa
	int _filas;               // Filas del mapa
	int _columnas;            // Columnas del mapa
	int _tamTile;             // Tamaño del tile en píxeles
	float _escala;            // Escala para agrandar el tile

	sf::Texture _texture;     // La imagen del tileset	
	sf::Sprite _spriteTile;   // Sprite auxiliar para dibujar cada tile

	// 🌟 El contenedor unificado de colisiones (¡Asegurate de que esté acá!)
	vector<BloqueMapa> _bloquesSolidos;

public:
	// Metodos públicos del mapa
	Map(int tamTile = 32, float escala = 1.0f);

	// Cargar el mapa desde un archivo CSV
	bool cargarMapa(const string& csvPath, const string& texturaPath);

	// 🛡️ Mantenemos la función de Fede declarada acá para que no rompa el Personaje
	bool hayColision(float x, float y);

	// Nueva sobrecarga: chequear colisión con un rectángulo AABB
	bool hayColision(const sf::FloatRect& rect) const;

	// Getter para que el GameManager pueda pedirle al mapa sus bloques sólidos más adelante
	const vector<BloqueMapa>& getBloquesSolidos() const { return _bloquesSolidos; }

	// Funciones de dibujado
	void dibujarMapa(sf::RenderWindow& ventana);
	void dibujarDebug(sf::RenderWindow& ventana);
};