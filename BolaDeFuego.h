#pragma once
#include "habilidades.h"
#include <SFML/Graphics.hpp>

// ============================================================================
// CLASE: habilidad_1 (Bola de Fuego)
// Hereda de Habilidades e implementa la mecánica de un proyectil que viaja
// en línea recta hasta alcanzar su rango máximo
// ============================================================================
class habilidad_1 : public Habilidades
{
private:
	// ========== COMPONENTES VISUALES ==========
	sf::Texture _textura;      // Textura/imagen del proyectil cargada desde archivo
	sf::Sprite _sprite;        // Sprite que dibuja el proyectil en pantalla

	// ========== ESTADO DEL PROYECTIL ==========
	bool _activo;              // Flag: true = proyectil en movimiento, false = inactivo

	// ========== FÍSICA DEL PROYECTIL ==========
	sf::Vector2f _direccion;   // Vector normalizado (magnitud 1) que apunta hacia el objetivo
	float _velocidad;          // Píxeles por segundo que se mueve el proyectil
	float _distanciaRecorrida; // Distancia acumulada en píxeles (se compara con _rango para detener)

public:
	// Constructor: Inicializa todos los parámetros de la habilidad Bola de Fuego
	habilidad_1();

	// ========== MÉTODOS SOBRESCRITOS DE LA CLASE BASE ==========

	// activar(): Lanza el proyectil si el cooldown está disponible
	// Parámetros:
	//   - inicio: Posición del personaje (origen del proyectil)
	//   - objetivo: Posición del cursor del ratón (punto al que apunta)
	void activar(sf::Vector2f inicio, sf::Vector2f objetivo) override;

	// subirNivel(): Mejora las estadísticas de la habilidad al subir de nivel
	void subirNivel() override;

	// ========== MÉTODOS ADICIONALES ESPECÍFICOS DE HABILIDAD_1 ==========

	// actualizar(): Actualiza el estado del proyectil cada frame
	// IMPORTANTE: Esta función:
	//   1. Actualiza el cooldown (llama a cdActualizar)
	//   2. Mueve el proyectil si está activo
	//   3. Verifica si alcanzó el rango máximo
	//   4. Desactiva el proyectil cuando alcanza el rango
	void actualizar(float deltaTime);

	// dibujar(): Renderiza el proyectil en pantalla si está activo
	void dibujar(sf::RenderWindow& ventana);
};
