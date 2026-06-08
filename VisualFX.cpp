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
    // Reservamos memoria de antemano
    _particulas.reserve(500);
    _portales.reserve(32);
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

// ======================================================================
// Actualizar ahora se encarga o de dar FX a una accion o a algo ambiental
// ======================================================================
void VisualFX::actualizar(float dt) {
    // 🌟 EL RELOJ GLOBAL DE LOS EFECTOS (Se suma UNA SOLA VEZ por frame)
    static float tiempoTotal = 0.f;
    tiempoTotal += dt;
    for (int i = 0; i < (int)_particulas.size(); i++) {

        // 1. El movimiento físico lo hacen TODAS (magia y ambiente)
        _particulas[i].sprite.move(_particulas[i].velocidad * dt);

        // 2. ¿Tiene velocidad de desvanecimiento? Es una partícula de ACCIÓN
        if (_particulas[i].velocidadFade > 0.f) {
            _particulas[i].opacidad -= _particulas[i].velocidadFade * dt;

            if (_particulas[i].opacidad <= 0.f) {
                _particulas.erase(_particulas.begin() + i);
                i--; // Corregimos el índice porque borramos un elemento
            }
            else {
                actualizarAccion(_particulas[i]);
            }
        }
        // 3. Si no tiene velocidadFade (es 0), es de AMBIENTE (Luciérnaga)
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
    particula.sprite.setScale(particula.sprite.getScale() * 0.95f); // Se achica el fuego/dash
}

void VisualFX::actualizarAmbiente(Particula& particula, float dt, float tiempoTotal) {
    // 🌟 LA DANZA FLUIDA (Movimiento circular suave)
    float velocidadDanzaX = std::cos(tiempoTotal * 1.5f + particula.fase) * 35.f;
    float velocidadDanzaY = std::sin(tiempoTotal * 1.2f + particula.fase) * 35.f;

    particula.sprite.move(velocidadDanzaX * dt, velocidadDanzaY * dt);

    // 🌟 LA RESPIRACIÓN RELAJANTE (Pulso de luz)
    float pulso = (std::sin(tiempoTotal * 1.5f + particula.fase) + 1.f) / 2.f;

    sf::Color color = particula.sprite.getColor();
    // Nunca llega a 0 (mínimo 40) para que no desaparezca de golpe
    color.a = static_cast<sf::Uint8>(pulso * 215 + 30);
    particula.sprite.setColor(color);


}

void VisualFX::actualizarPortal(float dt) {
    for (int i = 0; i < (int)_portales.size(); i++) {
        _portales[i].tiempoVidaActual -= dt;

        // Si se acabó el tiempo, lo borramos
        if (_portales[i].tiempoVidaActual <= 0.f) {
            _portales.erase(_portales.begin() + i);
            i--;
            continue;
        }

        // --- CÁLCULO DE FRAME PARA GRILLA ---
        // 1. Calculamos el frame actual basado en el tiempo
        float progreso = 1.0f - (_portales[i].tiempoVidaActual / _portales[i].tiempoVidaMaximo);
        int frame = static_cast<int>(progreso * _portales[i].frames);
        if (frame >= _portales[i].frames) frame = _portales[i].frames - 1;

        // 2. Traducimos frame (0-5) a coordenadas de grilla (col/fila)
        int columnas = 3;
        int frameW = _portalTexture.getSize().x / columnas; // 32
        int frameH = _portalTexture.getSize().y / 2;        // 32

        int col = frame % columnas; // 0, 1, 2, 0, 1, 2...
        int row = frame / columnas; // 0, 0, 0, 1, 1, 1...

        // 3. Aplicamos el recorte
        _portales[i].sprite.setTextureRect(sf::IntRect(col * frameW, row * frameH, frameW, frameH));
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

void VisualFX::agregarParticulasAmbiente(sf::Vector2f areaSpawn, int cantidad, sf::Color color) {
    for (int i = 0; i < cantidad; i++) {
        Particula particula;
        particula.sprite.setTexture(_ParticleFX);
        particula.sprite.setOrigin(_ParticleFX.getSize().x / 2.f, _ParticleFX.getSize().y / 2.f);
        particula.sprite.setScale(0.8f, 0.8f);
        float posX = static_cast<float>(rand() % (int)areaSpawn.x);
        float posY = static_cast<float>(rand() % (int)areaSpawn.y);
        particula.sprite.setPosition(posX, posY);

        // 🌟 CORRECCIÓN: Velocidad base más alta para que naden y no queden quietas
        float velX = static_cast<float>(rand() % 80 - 20);
        float velY = static_cast<float>(rand() % 80 - 20);
        particula.velocidad = sf::Vector2f(velX, velY);

        particula.sprite.setColor(color);
        particula.opacidad = 255.f;
        particula.velocidadFade = 0.f; // Las de ambiente no desaparecen, están siempre ahí
        particula.usarGlow = true;     // ¡Luciérnagas con GLOW obligatorio!

        // 🌟 LA MAGIA: Cada luciérnaga recibe un número de desfasaje entre 0 y 100
        particula.fase = static_cast<float>(rand() % 1000) / 100.f;

        _particulas.push_back(particula);
    }
}

void VisualFX::agregarPortal(const sf::Vector2f& posicion, float duracion, int frames, bool glow) {
    if (_portalTexture.getSize().x == 0) return; // textura no cargada

    PortalSpawn nuevoPortal;
    nuevoPortal.tiempoVidaActual = duracion;
    nuevoPortal.tiempoVidaMaximo = duracion;
    nuevoPortal.frames = std::max(1, frames);
    nuevoPortal.frameTime = duracion / static_cast<float>(nuevoPortal.frames);
    nuevoPortal.currentFrame = 0;
    nuevoPortal.usarGlow = glow;

    nuevoPortal.sprite.setTexture(_portalTexture);

    // Configuración del spritesheet
    int columnas = 3;
    int filas = 2;
    int frameW = _portalTexture.getSize().x / columnas; // 96/3 = 32
    int frameH = _portalTexture.getSize().y / filas;    // 64/2 = 32

    // frame inicial (col 0, row 0)
    nuevoPortal.sprite.setTextureRect(sf::IntRect(0, 0, frameW, frameH));
    nuevoPortal.sprite.setOrigin(frameW / 2.f, frameH / 2.f);
    nuevoPortal.sprite.setPosition(posicion);
    nuevoPortal.sprite.setScale(2.5f, 2.5f);
    nuevoPortal.sprite.
    sf::Color color = nuevoPortal.sprite.getColor();
    color.a = 255;
    nuevoPortal.sprite.setColor(color);

    _portales.push_back(std::move(nuevoPortal));
}
