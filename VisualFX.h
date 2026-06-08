#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <algorithm>

struct Particula {
	sf::Sprite sprite;
	sf::Vector2f velocidad;			// Hacia donde se mueve en los ejes
	float opacidad = 0.f;			// Entre 255 a 0
	float velocidadFade = 0.f;		// Que tan rapido desaparece
	bool usarGlow = false;			// BlendAdd o Mezcla Normal
	float fase = 0.f;				// Controla el pulso de la luz
};

class VisualFX {
	private:
		std::vector<Particula> _particulas;
		sf::Texture _portalTexture;

		// Nuevo: PortalSpawn para animación de portal
		struct PortalSpawn {
			sf::Sprite sprite;           // sprite con spritesheet del portal
			float tiempoVidaActual = 0.f;
			float tiempoVidaMaximo = 1.f; // duración total en segundos
			int frames = 1;              // cantidad de frames en el spritesheet
			float frameTime = 0.1f;      // tiempo por frame
			int currentFrame = 0;
			bool usarGlow = true;
		};
		std::vector<PortalSpawn> _portales;


		void actualizarAccion(Particula& particula);
		void actualizarAmbiente(Particula& particula, float dt, float tiempoTotal);
		void actualizarPortal(float dt);
		sf::Texture _ParticleFX;

	public:
		VisualFX();

		// 1. Para el Dash y la Bola de Fiegp (se quedan quietas donde naces y se desvancen)
		void agregarRastro(sf::Sprite spriteBase, sf::Color, float velocidadFade = 500.f, bool glow = true);
		// 2. Para chispas, sangre, etc (tienen movimiento propio y nacen de una textura chiquita)
		void agregarParticulaDinamica(const sf::Texture& textura, sf::Vector2f posicion, sf::Vector2f velocidadMovimiento, sf::Color color, float velocidadFade, bool glow);
		// 3.Para efectos como luciérnagas, polvo o partículas de ambientación
		void agregarParticulasAmbiente(sf::Vector2f areaSpawn, int cantidad, sf::Color color);

		void agregarPortal(const sf::Vector2f& posicion, float duracion = 1.0f, int frames = 6, bool glow = true);



	// --- METODOS PRINCIPALES ---
		void actualizar(float dt);
		void dibujar(sf::RenderWindow& ventana);
};