#pragma once
#include <SFML/Graphics.hpp>

class Colisionable {
public:
    virtual ~Colisionable() = default;
    // ----📜 EL CONTRATO OBLIGATORIO----
    virtual sf::FloatRect getBounds() const = 0;
    //------------------------------------------------------------------------
    virtual sf::FloatRect getColision() const {    // 🤝 LA AYUDA PARA EL EQUIPO (Alias en español)
        return getBounds();
    }
    virtual sf::Vector2f getCentroFisico() const {
        sf::FloatRect limites = getColision();
        return sf::Vector2f(limites.left + (limites.width / 2.f), limites.top + (limites.height / 2.f));
    }     // 🎯 CÁLCULO DEL CENTRO (Traído desde tu .cpp)
    virtual bool chequearColision(const Colisionable& otra) const {
        return this->getColision().intersects(otra.getColision());
    }
};

/*
 FloatRect es la AABB (Axis-Aligned Bounding Box) usada por SFML.
 getBounds() es la API canónica de SFML que devuelve una caja de colisión
 en coordenadas del mundo. Para facilitar la lectura del equipo hemos añadido
 getColision() como alias en español que delega en getBounds().
 Mantener getBounds() garantiza compatibilidad con librerías y snippets externos.
*/

