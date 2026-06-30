#pragma once
#include <SFML/Graphics.hpp>

///=================================================================///
///   PARTICULA - Un destello visual que vive por un momento y
///   desaparece (rastro del dash, chispa de la bola de fuego)
///=================================================================///
struct Particula {
    sf::Sprite sprite_de_la_particula;
    sf::Vector2f velocidad_de_la_particula;
    float opacidad_actual = 0.f;
    float velocidad_de_desvanecimiento = 0.f; // 0 = nunca desaparece (para particulas de ambiente)
    bool usar_modo_glow = false;
    float tiempo_hasta_cambio_de_direccion = 0.f; // Solo lo usan las particulas de ambiente
};

///=================================================================///
///   PORTAL_SPAWN - Una animacion de portal que aparece y
///   desaparece en un punto del mapa
///=================================================================///
struct PortalSpawn {
    sf::Sprite sprite_del_portal;
    float tiempo_de_vida_actual = 0.f;
    float tiempo_de_vida_maximo = 1.f;
    int cantidad_de_frames = 1;
    bool usar_modo_glow = true;
};

///=================================================================///
///   VISUAL_FX - Maneja todas las particulas del juego con arrays
///   fijos. Igual que el resto del proyecto: cada casillero puede
///   estar ocupado o libre
///=================================================================///
class VisualFX {
private:

    sf::Texture _textura_de_particulas;
    sf::Texture _textura_de_portal;

    ///=============================================================///
    ///   PARTICULAS DE ACCION - Rastros y chispas que nacen y
    ///   desaparecen solos (del dash, la bola de fuego, etc)
    ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_PARTICULAS_DE_ACCION = 500;
    Particula _particulas_de_accion[CANTIDAD_MAXIMA_DE_PARTICULAS_DE_ACCION];
    int _cantidad_de_particulas_de_accion_en_uso = 0;

    ///=============================================================///
    ///   PARTICULAS DE AMBIENTE - Lucierngas, polvo. Se crean al
    ///   cargar el mapa y bailan para siempre
    ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_PARTICULAS_DE_AMBIENTE = 100;
    Particula _particulas_de_ambiente[CANTIDAD_MAXIMA_DE_PARTICULAS_DE_AMBIENTE];
    int _cantidad_de_particulas_de_ambiente_en_uso = 0;

    ///=============================================================///
    ///   PORTALES
    ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_PORTALES = 32;
    PortalSpawn _portales[CANTIDAD_MAXIMA_DE_PORTALES];
    int _cantidad_de_portales_en_uso = 0;

    void actualizar_particulas_de_accion(float tiempo_transcurrido);
    void actualizar_particulas_de_ambiente(float tiempo_transcurrido);
    void actualizar_portales(float tiempo_transcurrido);

public:

    VisualFX();

    void agregarRastro(sf::Sprite sprite_base, sf::Color color, float velocidad_de_desvanecimiento = 500.f, bool glow = true);
    void agregarParticulaDinamica(const sf::Texture& textura, sf::Vector2f posicion, sf::Vector2f velocidad, sf::Color color, float velocidad_de_desvanecimiento, bool glow);
    void agregarParticulasAmbiente(sf::Vector2f area_de_aparicion, int cantidad, sf::Color color);
    void agregarPortal(sf::Vector2f posicion, float duracion = 1.f, int frames = 6, bool glow = true);

    void actualizar(float tiempo_transcurrido);
    void dibujar(sf::RenderWindow& ventana_del_juego);
};