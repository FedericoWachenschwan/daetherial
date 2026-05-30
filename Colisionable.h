#pragma once
#include <SFML/Graphics.hpp>

class Colisionable {
public:
    // Destructor virtual: Clave para que cuando se borre un objeto polimórfico no haya fugas de memoria
    virtual ~Colisionable() = default;

    // Método virtual puro: Obliga a herederos a devolver su caja de colisión (AABB)
    virtual sf::FloatRect getBounds() const = 0;

    // Función unificada para chequear si este objeto choca contra CUALQUIER otro colisionable
    bool chequearColision(const Colisionable& otra) const;
};



/*1. sf::FloatRect: El Terreno Alambrado
Imaginate que estás jugando al fútbol y querés delimitar la cancha, o que tenés un terreno y 
lo querés alambrar para que nadie se meta. En el mundo real, necesitarías estacas y metros de alambre. En SFML, 
para alambrar un objeto invisible, usás un sf::FloatRect.

La palabra viene de Float (número con coma, decimal) y Rect (Rectángulo). 
Es simplemente una estructura que guarda 4 números flotantes indispensables para ubicar un rectángulo en el espacio 2D.


Cualquier sf::FloatRect del universo se define por estos cuatro integrantes:
left (Izquierda): Es la coordenada X de la esquina superior izquierda. Te dice qué tan lejos está del borde izquierdo de la pantalla.
top (Arriba): Es la coordenada Y de la esquina superior izquierda. Te dice qué tan lejos está del techo de la pantalla.
width (Ancho): Cuántos píxeles mide de izquierda a derecha.
height (Alto): Cuántos píxeles mide de arriba a abajo.

💡 La diferencia con IntRect: Capaz con Fede vieron sf::IntRect para recortar las animaciones del spritesheet. 
Hacen exactamente lo mismo, pero el IntRect usa números enteros (píxeles puros como 0, 64, 128) porque las imágenes se recortan en píxeles enteros. 
El FloatRect usa decimales (como 100.5f) porque en la física los personajes se mueven con velocidades fraccionarias para que sea fluido.

2. getBounds(): El Agrimensor Técnico
Ahora, una cosa es el rectángulo matemático (FloatRect) y otra cosa es el personaje o el bloque de mapa que se está moviendo. 
Ahí es donde entra getBounds() (que en inglés significa "obtener los límites").

Pensalo con esta analogía:
Tu sprite es un actor de teatro que se mueve por el escenario, cambia de vestuario (texturas) y se da vuelta (espejado). 
El mánager de colisiones necesita saber todo el tiempo dónde poner el alambrado para que no se choque. 
getBounds() es el agrimensor que viene con la cinta métrica en cada frame del juego, 
mide al actor en su posición actual y te devuelve el plano del terreno alambrado (sf::FloatRect).

¿Por qué lo hicimos polimórfico con la clase abstracta?
Porque cada objeto del juego se mide de forma diferente:

El bloque del mapa: Es vago. Se crea una vez en la posición X, Y, mide 32x32 y nunca más se mueve. 
Su getBounds() siempre devuelve la misma caja fija.

El sprite completo: Si usás sprite.getGlobalBounds(), SFML mide automáticamente todo el dibujo (los 64x64 píxeles), 
incluyendo el aire que rodea la cabeza del personaje.

Tu Personaje calibrado: Como ustedes querían que la colisión fuera fina en los pies, hackearon el getBounds() para que, 
en vez de medir todo el cuerpo, agarre la posición del sprite, 
le sume un offset y dibuje una cajita matemática más chica justo en los botines del personaje.

3. ¿Cómo se conectan en la matemática de SFML?
Una vez que getBounds() te devuelve el sf::FloatRect de dos objetos, viene la magia del choque con la función .intersects().

Cuando el código ejecuta esto en tu bucle predictivo:

C++
if (personaje.getBounds().intersects(bloque.getBounds()))
En criollo le estás diciendo a la computadora:

"Personaje, dame tu alambrado de los pies" (FloatRect A).

"Bloque de mapa, dame tu alambrado de pared" (FloatRect B).

"SFML, fijate si las líneas de esos dos terrenos se cruzaron en la pantalla".

Como SFML sabe los 4 números (left, top, width, height) de cada caja, hace un cálculo de fracciones superrápido. 
Si hay superposición, se activa el true y el código del "paso fantasma" que armamos hace que el personaje vuelva un paso atrás en el tiempo, 
logrando el freno perfecto que viste en las capturas.

¡Y listo! Esos dos componentes son el corazón de cualquier motor de física 2D. 
Con este mapa mental, cuando te pregunten los profesores, 
les explicás que FloatRect es la caja de datos y getBounds() es la función polimórfica que calcula esa caja en tiempo real.*/