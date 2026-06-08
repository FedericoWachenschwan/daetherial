#include "VisualFX.h"
#include <iostream>

VisualFX::VisualFX() {
    if (!_ParticleFX.loadFromFile("assets/particleFX.png")) {
        std::cout << "Error: No se pudo cargar particleFX en VisualFX" << std::endl;
    }
    if (!_portalTexture.loadFromFile("assets/PortalSpawn.png")) {
        std::cout << "Error: No se pudo cargar Portal Spawn en VisualFX" << std::endl;
    }
    else {
        _portalTexture.setSmooth(true);
    }
    _particulas.reserve(500);
    _portales.reserve(32);
}

void VisualFX::agregarRastro(sf::Sprite spriteBase, sf::Color color, float velocidadFade, bool glow) {
    Particula particula;
    particula.sprite = spriteBase;
    particula.sprite.setColor(color);
    particula.velocidad = sf::Vector2f(0.f, 0.f);
    particula.opacidad = 255.f;
    particula.velocidadFade = velocidadFade;
    particula.usarGlow = glow;
    _particulas.push_back(particula);
}

void VisualFX::actualizar(float dt) {
    static float tiempoTotal = 0.f;
    tiempoTotal += dt;

    for (int i = 0; i < (int)_particulas.size(); i++) {
        _particulas[i].sprite.move(_particulas[i].velocidad * dt);

        if (_particulas[i].velocidadFade > 0.f) {
            _particulas[i].opacidad -= _particulas[i].velocidadFade * dt;

            if (_particulas[i].opacidad <= 0.f) {
                _particulas.erase(_particulas.begin() + i);
                i--;
            }
            else {
                actualizarAccion(_particulas[i]);
            }
        }
        else {
            actualizarAmbiente(_particulas[i], dt, tiempoTotal);
        }
    }
    actualizarPortal(dt);
}

void VisualFX::actualizarAccion(Particula& particula) {
    sf::Color color = particula.sprite.getColor();
    color.a = static_cast<sf::Uint8>(particula.opacidad);
    particula.sprite.setColor(color);
    particula.sprite.setScale(particula.sprite.getScale() * 0.95f);
}

void VisualFX::actualizarAmbiente(Particula& particula, float dt, float tiempoTotal) {
    float velocidadDanzaX = std::cos(tiempoTotal * 1.5f + particula.fase) * 35.f;
    float velocidadDanzaY = std::sin(tiempoTotal * 1.2f + particula.fase) * 35.f;
    particula.sprite.move(velocidadDanzaX * dt, velocidadDanzaY * dt);

    float pulso = (std::sin(tiempoTotal * 1.5f + particula.fase) + 1.f) / 2.f;
    sf::Color color = particula.sprite.getColor();
    color.a = static_cast<sf::Uint8>(pulso * 215 + 30);
    particula.sprite.setColor(color);
}

void VisualFX::actualizarPortal(float dt) {
    for (int i = 0; i < (int)_portales.size(); i++) {
        _portales[i].tiempoVidaActual -= dt;

        if (_portales[i].tiempoVidaActual <= 0.f) {
            _portales.erase(_portales.begin() + i);
            i--;
            continue;
        }

        float progreso = 1.0f - (_portales[i].tiempoVidaActual / _portales[i].tiempoVidaMaximo);
        int frame = static_cast<int>(progreso * _portales[i].frames);
        if (frame >= _portales[i].frames) frame = _portales[i].frames - 1;

        int columnas = 3;
        int frameW = _portalTexture.getSize().x / columnas;
        int frameH = _portalTexture.getSize().y / 2;

        int col = frame % columnas;
        int row = frame / columnas;

        _portales[i].sprite.setTextureRect(sf::IntRect(col * frameW, row * frameH, frameW, frameH));
    }
}

void VisualFX::dibujar(sf::RenderWindow& ventana) {
    for (auto& particula : _particulas) {
        if (particula.usarGlow) {
            ventana.draw(particula.sprite, sf::BlendAdd);
        }
        else {
            ventana.draw(particula.sprite);
        }
    }

    for (auto& portal : _portales) {
        if (portal.usarGlow)
            ventana.draw(portal.sprite, sf::RenderStates(sf::BlendAdd));
        else
            ventana.draw(portal.sprite);
    }
}

void VisualFX::agregarParticulaDinamica(const sf::Texture& textura, sf::Vector2f posicion, sf::Vector2f velocidadMov, sf::Color color, float velocidadFade, bool glow) {
    Particula particula;
    particula.sprite.setTexture(textura);
    particula.sprite.setOrigin(textura.getSize().x / 2.f, textura.getSize().y / 2.f);
    particula.sprite.setScale(0.08f, 0.08f);
    particula.sprite.setPosition(posicion);
    particula.sprite.setColor(color);
    particula.velocidad = velocidadMov;
    particula.opacidad = 150.f;
    particula.velocidadFade = velocidadFade;
    particula.usarGlow = glow;
    _particulas.push_back(particula);
}

void VisualFX::agregarParticulasAmbiente(sf::Vector2f areaSpawn, int cantidad, sf::Color color) {
    for (int i = 0; i < cantidad; i++) {
        Particula particula;
        particula.sprite.setTexture(_ParticleFX);
        particula.sprite.setOrigin(_ParticleFX.getSize().x / 2.f, _ParticleFX.getSize().y / 2.f);
        particula.sprite.setScale(0.8f, 0.8f);
        float posX = static_cast<float>(rand() % (int)areaSpawn.x);
        float posY = static_cast<float>(rand() % (int)areaSpawn.y);
        particula.sprite.setPosition(posX, posY);

        float velX = static_cast<float>(rand() % 80 - 20);
        float velY = static_cast<float>(rand() % 80 - 20);
        particula.velocidad = sf::Vector2f(velX, velY);

        particula.sprite.setColor(color);
        particula.opacidad = 255.f;
        particula.velocidadFade = 0.f;
        particula.usarGlow = true;
        particula.fase = static_cast<float>(rand() % 1000) / 100.f;
        _particulas.push_back(particula);
    }
}

void VisualFX::agregarPortal(const sf::Vector2f& posicion, float duracion, int frames, bool glow) {
    if (_portalTexture.getSize().x == 0) return;

    PortalSpawn nuevoPortal;
    nuevoPortal.tiempoVidaActual = duracion;
    nuevoPortal.tiempoVidaMaximo = duracion;
    nuevoPortal.frames = std::max(1, frames);
    nuevoPortal.frameTime = duracion / static_cast<float>(nuevoPortal.frames);
    nuevoPortal.currentFrame = 0;
    nuevoPortal.usarGlow = glow;

    nuevoPortal.sprite.setTexture(_portalTexture);

    int columnas = 3;
    int filas = 2;
    int frameW = _portalTexture.getSize().x / columnas;
    int frameH = _portalTexture.getSize().y / filas;

    nuevoPortal.sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
    nuevoPortal.sprite.setOrigin(frameW / 2.f, frameH / 2.f);
    nuevoPortal.sprite.setPosition(posicion);
    nuevoPortal.sprite.setScale(2.5f, 2.5f);

    sf::Color color = nuevoPortal.sprite.getColor(); // Obtenemos el color actual del sprite
    color.a = 255; // Le ponemos opacidad completa
    nuevoPortal.sprite.setColor(color); // Aplicamos el color al sprite

    _portales.push_back(std::move(nuevoPortal));
}