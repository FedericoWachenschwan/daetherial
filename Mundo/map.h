#pragma once
#include <SFML/Graphics.hpp>
#include <vector> 
#include <string>
#include "Colisionable.h"    //llamamos a nuestra clase abstracta
#include "VisualFX.h"

using namespace std;

// ACTUALIZACION GRANDE, AHORA EL MAPA PUEDE TENER BloqueSolido de 8x8 4x4
// 🧱 Clase para los bloques sólidos del mapa
class BloqueMapa : public Colisionable {
private:
	sf::FloatRect _hitbox; // La caja de colisión real en el mundo

public:
	BloqueMapa(float x, float y, float ancho, float alto)
		: _hitbox(x, y, ancho, alto) {}
	// Implementación obligatoria de la interfaz
	sf::FloatRect getBounds() const override { return _hitbox; }
};

class Map {
private:
	vector<vector<int>> mapa; // Matriz del mapa
	int _filas;               // Filas del mapa
	int _columnas;            // Columnas del mapa
	int _tamTile;             // Tamaño del tile en píxeles
	float _escala;            // Escala para agrandar el tile
	sf::Texture _texture;     // La imagen del tileset	
	sf::Sprite _spriteTile;   // Sprite auxiliar para dibujar cada tile
	vector<BloqueMapa> _bloqueSolido; // El contenedor unificado de colisiones

public:
	Map(int tamTile = 32, float escala = 1.0f);
	bool cargarMapa(const string& csvPath, const string& texturaPath);
	void dibujarMapa(sf::RenderWindow& ventana) const;
	void dibujarDebug(sf::RenderWindow& ventana) const;
	void generarClima(VisualFX& vfx);
	bool esSolido(int f, int c) const;  // Función para Pathfinder si una baldosa es sólida sin coordenadas del mundo
	const vector<BloqueMapa>& getBloqueSolido() const { return _bloqueSolido; } // Getter para que el GameManager pueda pedirle al mapa sus bloques sólidos más adelante


};