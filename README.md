# Daetherial

Juego de acción y aventura **2D con vista desde arriba**, ambientado en un mundo de hielo. Hecho en **C++ con SFML** para Programación II (Tecnicatura Universitaria en Programación, UTN).

![Daetherial](https://federicowachenschwan.github.io/portfolio/img/daetherial/fondo-menu.jpg)

## Características

- **Máquina de estados** para las pantallas: intro, menú, historia de 20 ilustraciones, juego, muerte, logros y créditos.
- Un **mago** jugable con animaciones por dirección, vida, maná, oro y ataque de bola de fuego.
- Enemigos y un **jefe final, el Golem de hielo**, que persigue al jugador con el algoritmo **A\*** (búsqueda de caminos propia).
- Mascota que acompaña al jugador, **tienda e inventario**.
- Mapa hecho con **Tiled** (colisiones leídas desde CSV), cámara que sigue al personaje y niebla.
- **Logros persistentes** guardados en un archivo binario.

## Organización del código

Unas 6.000 líneas de C++ separadas en módulos: `Control_Del_Juego`, `Entidades`, `Mundo`, `Objetos_Inventario_Y_Tienda`, `Menus_Y_Paneles_En_Pantalla` y `Efectos`.
Herencia entre entidades: `EntidadViva` → `Personaje`, `Enemigo`, `Golem`, `Mascota`.

## Tecnologías

C++ · SFML 2 · Visual Studio · Tiled · Git

## Compilación

Abrir `Daetherial.slnx` con Visual Studio y compilar. La librería SFML ya está incluida en la carpeta `SFML`, y las imágenes, la música y los efectos de sonido están en `assets`.

## Trailer

[Ver el trailer en mi portfolio](https://federicowachenschwan.github.io/portfolio/#proyectos)

Proyecto grupal · Programación II · UTN
