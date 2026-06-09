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
struct PortalSpawn {
	sf::Sprite sprite;              // sprite con spritesheet del portal
	int frames = 6;                 // cantidad de frames en el spritesheet
	float frameTime = 0.1f;         // tiempo que dura cada frame en pantalla
	float timer = 0.f;              // ACUMULADOR DE TIEMPO para saber cuándo cambiar de frame
	int currentFrame = 0;           // frame actual
	bool usarGlow = true;           // efecto glow
	bool estaActivado = true;       // nuestro booleano de estado
};

class VisualFX {
	private:
		std::vector<Particula> _particulas; // guarda en su vector la lista de particulas en el mundo
		std::vector<PortalSpawn> _portales;

		sf::Texture _portalTexture;
		sf::Texture _ParticleFX;

		void actualizarAccion(Particula& particula);
		void actualizarAmbiente(Particula& particula, float dt, float tiempoTotal);
		void actualizarPortal(float dt);
	

	public:
		VisualFX();

		// 1. Para el Dash y la Bola de Fiegp (se quedan quietas donde naces y se desvancen)
		void agregarRastro(sf::Sprite spriteBase, sf::Color, float velocidadFade = 500.f, bool glow = true);
		// 2. Para chispas, sangre, etc (tienen movimiento propio y nacen de una textura chiquita)
		void agregarParticulaDinamica(const sf::Texture& textura, sf::Vector2f posicion, sf::Vector2f velocidadMovimiento, sf::Color color, float velocidadFade, bool glow);
		// 3.Para efectos como luciérnagas, polvo o partículas de ambientación
		void agregarParticulasAmbiente(sf::Vector2f areaSpawn, int cantidad, sf::Color color);
		// 4. Para agregar portales en el mundo (tanto como spawns como para portales dimensionales)
		void activarPortales();

		void agregarPortal(const sf::Vector2f& posicion, int frames = 6, bool glow = true, bool arrancaPrendido = true);




	// --- METODOS PRINCIPALES ---
		void actualizar(float dt);
		void dibujar(sf::RenderWindow& ventana);
};