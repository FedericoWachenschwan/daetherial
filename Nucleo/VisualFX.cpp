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
    // Calculamos las medidas una sola vez
    int columnas = 3;
    int frameW = _portalTexture.getSize().x / columnas;
    int frameH = _portalTexture.getSize().y / 2;

    for (int i = 0; i < (int)_portales.size(); i++) {
        auto& portal = _portales[i];

        // --- CAMINO 1: EL PORTAL ESTÁ APAGADO ---
        if (portal.estaActivado == false) {
            portal.currentFrame = 0; // Lo mantenemos en el primer dibujo
            portal.sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
        }
        // --- CAMINO 2: EL PORTAL ESTÁ PRENDIDO ---
        else {
            portal.timer += dt; // El cronómetro avanza

            // Si pasó el tiempo, cambiamos de frame
            if (portal.timer >= portal.frameTime) {
                portal.timer -= portal.frameTime;
                portal.currentFrame++;

                // Bucle infinito de la animación
                if (portal.currentFrame >= portal.frames) {
                    portal.currentFrame = 0;
                }
            }

            // Calculamos fila, columna y recortamos la imagen
            int col = portal.currentFrame % columnas;
            int row = portal.currentFrame / columnas;
            portal.sprite.setTextureRect(sf::IntRect(col * frameW, row * frameH, frameW, frameH));
        }
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
        if (portal.usarGlow) {
            sf::Sprite aura;
            aura.setTexture(*portal.sprite.getTexture());
            aura.setTextureRect(portal.sprite.getTextureRect());
            aura.setOrigin(portal.sprite.getOrigin());
            aura.setPosition(portal.sprite.getPosition());
            sf::Vector2f portalScale = portal.sprite.getScale();
            aura.setScale(portalScale.x + 2.f, portalScale.y * 2.f);
            aura.setColor(sf::Color(60, 209, 23, 200));
            ventana.draw(aura, sf::BlendAdd);
            ventana.draw(portal.sprite, sf::RenderStates(sf::BlendAdd));
        }
        else {
            ventana.draw(portal.sprite);
        }
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

void VisualFX::agregarPortal(const sf::Vector2f& posicion, int frames, bool glow, bool arrancaPrendido) {
    if (_portalTexture.getSize().x == 0) return; // Protección anti-crashes

    PortalSpawn nuevoPortal;
    nuevoPortal.frames = std::max(1, frames);

    // CORRECCIÓN: Le clavamos una velocidad fija a la animación (ej: 0.1 segundos por frame)
    nuevoPortal.frameTime = 0.1f;

    nuevoPortal.currentFrame = 0;
    nuevoPortal.timer = 0.f; // Cronómetro en cero
    nuevoPortal.usarGlow = glow;

    // Acá usamos el nuevo parámetro para decidir cómo nace el portal
    nuevoPortal.estaActivado = arrancaPrendido;

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

    _portales.push_back(std::move(nuevoPortal)); // lo manda a lo ultimo asegurando rendimiento
}

void VisualFX::activarPortales() {
    // Usamos size_t para arreglar la advertencia de signed/unsigned
    for (size_t i = 0; i < _portales.size(); i++) {
        _portales[i].estaActivado = !_portales[i].estaActivado;

        // El ! invierte el valor. 
        // Si era true (Horda), pasa a false (se apaga).
        // Si era false (Salida), pasa a true (se prende).
    }
}