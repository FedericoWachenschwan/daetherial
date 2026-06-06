#include "VisualFX.h"

VisualFX::VisualFX() {
	//Reservamos memodia de anemano
	_particulas.reserve(500);
}

void VisualFX::agregarRastro(sf::Sprite spriteBase, sf::Color color, float velocidadFade, bool glow) {
	Particula particula;
	particula.sprite = spriteBase;
	particula.sprite.setColor(color);
	particula.velocidad = sf::Vector2f(0.f, 0.f); // Rastro quieto, se queda en el aire
	particula.opacidad = 255.f;
	particula.velocidadFade = velocidadFade;
	particula.usarGlow = glow;

	_particulas.push_back(particula);
}

void VisualFX::actualizar(float dt) {
    for (int i = 0; i < (int)_particulas.size(); i++) {
        // 1. Desvanecimiento
        _particulas[i].opacidad -= _particulas[i].velocidadFade * dt;
        // 2. Movimiento (si tiene)
        _particulas[i].sprite.move(_particulas[i].velocidad * dt);
        // 3. Destrucción si ya es invisible
        if (_particulas[i].opacidad <= 0.f) {
            _particulas.erase(_particulas.begin() + i);
            i--;
        }
        else {
            // Aplicar el nuevo nivel de transparencia al color existente
            sf::Color colorActual = _particulas[i].sprite.getColor();
            colorActual.a = static_cast<sf::Uint8>(_particulas[i].opacidad);
            _particulas[i].sprite.setColor(colorActual);

            // Efecto opcional: achicar la partícula a medida que muere (sirve para fuego y humo)
            _particulas[i].sprite.setScale(_particulas[i].sprite.getScale() * 0.95f);
        }
    }
}

void VisualFX::dibujar(sf::RenderWindow& ventana) {
    for (auto& particula : _particulas) {
        if (particula.usarGlow) {
            ventana.draw(particula.sprite, sf::BlendAdd);
        }
        else {
            ventana.draw(particula.sprite); // Mezcla normal para sangre o polvo
        }
    }
}

void VisualFX::agregarParticulaDinamica(const sf::Texture& textura, sf::Vector2f posicion, sf::Vector2f velocidadMov, sf::Color color, float velocidadFade, bool glow) {
    Particula particula;
    particula.sprite.setTexture(textura);
    particula.sprite.setOrigin(textura.getSize().x / 2.f, textura.getSize().y / 2.f); // Seteamos para que tome el centro de la textura
    particula.sprite.setScale(0.08f, 0.08f); // Achicamos el sprite para que parezca una chispita y no una bola entera
    particula.sprite.setPosition(posicion);
    particula.sprite.setColor(color);
    particula.velocidad = velocidadMov;
    particula.opacidad = 150.f;
    particula.velocidadFade = velocidadFade;
    particula.usarGlow = glow;

    _particulas.push_back(particula);
}
 
/*void agregarParticulasAmbiente(sf::Texture& textura, sf::Vector2f areaSpawn, int cantidad, sf::Color color) {
    for (int i = 0; i < cantidad; i++) {
        Particula particula;
        particula.sprite.setTexture(textura);
        
        float posX = static_cast<float>(rand() % (int)areaSpawn.x);
        float posY = static_cast<float>(rand() % (int)areaSpawn.y);
        particula.sprite.setPosition(posX, posY);
        // Movimiento muy lento y errático
        particula.velocidad = sf::Vector2f((rand() % 20 - 10) / 10.f, (rand() % 20 - 10) / 10.f);
        particula.sprite.setColor(color);
        particula.opacidad = 255.f;
        particula.velocidadFade = 0.f; // Las de ambiente no desaparecen, están siempre ahí
        particula.usarGlow = true;     // ¡Luciérnagas con GLOW obligatorio!

        _particulas.push_back(particula);
    }
}*/