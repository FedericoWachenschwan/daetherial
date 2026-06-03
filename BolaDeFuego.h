#pragma once
#include "habilidades.h"
#include <SFML/Graphics.hpp>

// ============================================================================
// CLASE: BolaDeFuego
// Hereda de Habilidades e implementa la mecánica de un proyectil que viaja
// en línea recta hasta alcanzar su rango máximo
// ============================================================================
class BolaDeFuego : public Habilidades
{
private:
	// ========== COMPONENTES VISUALES ==========
	sf::Texture _textura;
	sf::Sprite _sprite;

	// ========== ESTADO DEL PROYECTIL ==========
	bool _activo;

	// ========== FÍSICA DEL PROYECTIL ==========
	sf::Vector2f _direccion;
	float _velocidad;
	float _distanciaRecorrida;

public:
	// Constructor: Inicializa todos los parámetros de la habilidad
	BolaDeFuego(); // El constructor se llama igual que la clase

	// ========== MÉTODOS SOBRESCRITOS DE LA CLASE BASE ==========
	void activar(sf::Vector2f inicio, sf::Vector2f objetivo, float rangoPixeles) override;
	void setRangoDinamico(float rangoPixeles) { _rango = rangoPixeles; }
	void subirNivel() override;

	// ========== MÉTODOS ADICIONALES ESPECÍFICOS ==========
	void actualizar(float deltaTime);
	void dibujar(sf::RenderWindow& ventana);
	bool estaActiva() const { return _activo; }
	void desactivar() { _activo = false; }
	sf::FloatRect getBounds() const {return _sprite.getGlobalBounds();}
		
};