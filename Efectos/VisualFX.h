#pragma once
#include <SFML/Graphics.hpp>

///=================================================================///
///   PARTICULA - Un destello visual que vive por un momento y
///   desaparece (rastro del dash, chispa de la bola de fuego)
///=================================================================///
struct Particula {
    sf::Sprite sprite_de_la_particula; // Lo que se dibuja en pantalla
    sf::Vector2f velocidad_de_la_particula; // Hacia donde y a que velocidad se mueve
    float opacidad_actual = 0.f; // De 0 (invisible) a 255 (opaco)
    float velocidad_de_desvanecimiento = 0.f; // 0 = nunca desaparece (para particulas de ambiente)
    bool usar_modo_glow = false; // Si es verdadero, se dibuja con BlendAdd (brilla)
    float tiempo_hasta_cambio_de_direccion = 0.f; // Solo lo usan las particulas de ambiente
};

///=================================================================///
///   PORTAL_SPAWN - Una animacion de portal que aparece y
///   desaparece en un punto del mapa
///=================================================================///
struct PortalSpawn {
    sf::Sprite sprite_del_portal; // Imagen del portal en el mundo
    float tiempo_de_vida_actual = 0.f; // Cuanto tiempo le queda antes de desaparecer
    float tiempo_de_vida_maximo = 1.f; // Duracion total de la animacion
    int cantidad_de_frames = 1; // Cuantos fotogramas tiene la animacion
    bool usar_modo_glow = true; // Los portales brillan por defecto
};

///=================================================================///
///   VISUAL_FX - Maneja todas las particulas del juego con arrays
///   fijos. Igual que el resto del proyecto: cada casillero puede
///   estar ocupado o libre
///=================================================================///
class VisualFX {
private:

    sf::Texture _textura_de_particulas; // Imagen compartida por todas las particulas
    sf::Texture _textura_de_portal; // Imagen del spritesheet del portal

 ///=============================================================///
 ///   PARTICULAS DE ACCION - Rastros y chispas que nacen y
 ///   desaparecen solos (del dash, la bola de fuego, etc)
 ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_PARTICULAS_DE_ACCION = 500; // Limite del array
    Particula _particulas_de_accion[CANTIDAD_MAXIMA_DE_PARTICULAS_DE_ACCION]; // Array de particulas activas
    int _cantidad_de_particulas_de_accion_en_uso = 0; // Cuantas estan vivas ahora

 ///=============================================================///
 ///   PARTICULAS DE AMBIENTE - Lucierngas, polvo. Se crean al
 ///   cargar el mapa y bailan para siempre
 ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_PARTICULAS_DE_AMBIENTE = 100; // Limite del array
    Particula _particulas_de_ambiente[CANTIDAD_MAXIMA_DE_PARTICULAS_DE_AMBIENTE]; // Array de particulas del ambiente
    int _cantidad_de_particulas_de_ambiente_en_uso = 0; // Cuantas hay activas

 ///=============================================================///
 ///   PORTALES
 ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_PORTALES = 32; // Maximo de portales al mismo tiempo
    PortalSpawn _portales[CANTIDAD_MAXIMA_DE_PORTALES]; // Array de portales activos
    int _cantidad_de_portales_en_uso = 0; // Cuantos portales hay ahora

    void actualizar_particulas_de_accion(float tiempo_transcurrido); // Mueve y desvanece las chispas
    void actualizar_particulas_de_ambiente(float tiempo_transcurrido); // Mueve las luciernagras
    void actualizar_portales(float tiempo_transcurrido); // Avanza la animacion de portales

public:

    VisualFX();

    void agregarRastro(sf::Sprite sprite_base, sf::Color color, float velocidad_de_desvanecimiento = 500.f, bool glow = true);
    void agregarParticulaDinamica(const sf::Texture& textura, sf::Vector2f posicion, sf::Vector2f velocidad, sf::Color color, float velocidad_de_desvanecimiento, bool glow);
    void agregarParticulasAmbiente(sf::Vector2f area_de_aparicion, int cantidad, sf::Color color);
    void agregarPortal(sf::Vector2f posicion, float duracion = 1.f, int frames = 6, bool glow = true);

    void actualizar(float tiempo_transcurrido); // Actualiza todos los efectos de una vez
    void dibujar(sf::RenderWindow& ventana_del_juego); // Dibuja todos los efectos en pantalla
};
