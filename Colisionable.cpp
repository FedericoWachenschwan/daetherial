#include "Colisionable.h"

bool Colisionable::chequearColision(const Colisionable& otra) const {
    // Compara la caja AABB de este objeto con la caja AABB del otro
    return this->getBounds().intersects(otra.getBounds());
}

sf::Vector2f Colisionable::getCentroFisico() const {
    sf::FloatRect limites = getBounds();
    return sf::Vector2f(
        limites.left + (limites.width / 2.f),
        limites.top + (limites.height / 2.f)
    );
}