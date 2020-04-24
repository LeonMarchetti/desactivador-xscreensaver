# Desactivador de XScreensaver

Pequeño programa que detiene la activación del salvapantallas.

Ejecuta el comando `xscreensaver-command -deactivate` cada 30 segundos, con lo cual se envía una señal que simula que hubo movimiento y por lo tanto se reinicia el temporizador del salvapantallas.

Crea un ícono en el área de notificación para alternar la activación del salvapantallas.

Apretando el botón cuando el ícono es ![Stop][Stop] activa el programa y desactiva el salvapantallas, y caso opuesto cuando el ícono es ![Play][Play]

Para terminar el programa, presionar el botón derecho del mouse sobre el ícono.

Escrito en C.

XScreenSaver fue desarrollado por Jamie Zawinski http://www.jwz.org/xscreensaver/.

## Librerías utilizadas:

* gtk
* pthread

## Instalación

Después de clonar, entrar en el directorio y ejecutar `make`, para

[Play]: /icons/play-small.png
[Stop]: /icons/stop-small.png
