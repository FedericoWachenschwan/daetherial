#include "Colisionable.h"

bool Colisionable::chequearColision(const Colisionable& otra) const {
    // Compara la caja AABB de este objeto con la caja AABB del otro
    return this->getBounds().intersects(otra.getBounds());
}