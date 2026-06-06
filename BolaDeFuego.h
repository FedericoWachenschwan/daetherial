#pragma once
#include "habilidades.h"
#include <SFML/Graphics.hpp>
#include <vector> // Para manejar nuestra lista de particulas


// ============================================================================
// ESTRUCTURA: ParticulaFuego
// Guarda la información visual de cada "chispa" que forma la cola del rastro
// ============================================================================
struct ParticulaFuego {
	sf::Sprite sprite;
	float opacidad;
};

// ============================================================================
// CLASE: BolaDeFuego
// Hereda de Habilidades e implementa la mecánica de un proyectil que viaja
// en línea recta hasta alcanzar su rango máximo
// ============================================================================
class VisualFX;

class BolaDeFuego : public Habilidades
{
private:
	// ========== COMPONENTES VISUALES ==========
	sf::Texture _textura;
	sf::Sprite _sprite;

	// ========== SISTEMA DE PARTÍCULAS (RASTRO) ==========
	float _relojSpawnRastro = 0.0f;           // 🌟 Temporizador para emitir

	// ========== ESTADO DEL PROYECTIL ==========
	bool _activo = false;

	// ========== FÍSICA DEL PROYECTIL ==========
	sf::Vector2f _direccion = sf::Vector2f(0.0f, 0.0f);
	float _velocidad = 0.0f;
	float _distanciaRecorrida = 0.0f;

public:
	// Constructor: Inicializa todos los parámetros de la habilidad
    BolaDeFuego();
	// ========== MÉTODOS SOBRESCRITOS DE LA CLASE BASE ==========
	void activar(sf::Vector2f inicio, sf::Vector2f objetivo, float rangoPixeles) override;
	void setRangoDinamico(float rangoPixeles) { _rango = rangoPixeles; }
	void subirNivel() override;

	// ========== MÉTODOS ADICIONALES ESPECÍFICOS ==========
	void actualizar(float deltaTime, VisualFX& vfx);
	void dibujar(sf::RenderWindow& ventana);
	bool estaActiva() const { return _activo; }
	void desactivar() { _activo = false; }
    sf::FloatRect getBounds() const { return _sprite.getGlobalBounds(); }
};