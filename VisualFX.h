#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Particula {
	sf::Sprite sprite;
	sf::Vector2f velocidad; // Hacia donde se mueve en los ejes
	float opacidad = 0.f;			// Entre 255 a 0
	float velocidadFade = 0.f;	// Que tan rapido desaparece
	bool usarGlow = false;			// BlendAdd o Mezcla Normal
};

class VisualFX {
	private:
	
		std::vector<Particula> _particulas;

	public:
		VisualFX();

		// 1. Para el Dash y la Bola de Fiegp (se quedan quietas donde naces y se desvancen)
		void agregarRastro(sf::Sprite spriteBase, sf::Color, float velocidadFade = 500.f, bool glow = true);
		// 2. Para chispas, sangre, etc (tienen movimiento propio y nacen de una textura chiquita)
		void agregarParticulaDinamica(const sf::Texture& textura, sf::Vector2f posicion, sf::Vector2f velocidadMovimiento, sf::Color color, float velocidadFade, bool glow);
		// 3.Para efectos como luciérnagas, polvo o partículas de ambientación
		void agregarParticulasAmbiente(sf::Texture& textura, sf::Vector2f areaSpawn, int cantidad, sf::Color color);


	// --- METODOS PRINCIPALES ---
		void actualizar(float dt);
		void dibujar(sf::RenderWindow& ventana);
};