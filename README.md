# Buscaminas en C++ con sistema de gestión de jugadores

Aplicación de consola desarrollada en C++ que combina el juego de Buscaminas con un sistema completo de gestión de jugadores y registro de partidas.

## Descripción

El programa permite:
- Registrar, dar de baja y modificar jugadores, con validación de cédula uruguaya y fecha de nacimiento.
- Jugar partidas de Buscaminas en un tablero de 8x8 con generación aleatoria de minas.
- Registrar el historial de partidas (ganadas, perdidas, abandonadas) por jugador y por fecha.
- Consultar listados de jugadores y de partidas jugadas.

## Características técnicas

- **Recursividad**: revelado de casillas vacías mediante un algoritmo de flood-fill recursivo.
- **Validación de datos**: verificación de formato de cédula (X.XXX.XXX-X) y fecha (dd/mm/aaaa) carácter por carácter.
- **Lógica de "primer movimiento seguro"**: el tablero se regenera si la primera casilla descubierta contiene una mina, evitando que el jugador pierda en su primer clic.
- **Gestión de estado con structs**: manejo de jugadores activos e inactivos mediante arreglos paralelos.
- **Algoritmo de ordenamiento**: implementación manual de bubble sort para listar jugadores por nickname.

## Tecnologías

- C++ (estándar de la librería `iostream`, `time.h`, `string`)

## Cómo compilarlo y ejecutarlo

1. Cloná o descargá el repositorio.
   
2. Compilá con g++ (o el compilador de C++ que tengas):
g++ BUSCAMINAS-Xaamira.cpp -o buscaminas

3. Ejecutá:
./buscaminas

## Posibles mejoras

- Reemplazar arreglos de tamaño fijo por `std::vector` para manejo dinámico de jugadores y partidas.
- Encriptar las contraseñas en lugar de almacenarlas en texto plano.
- Encapsular la lógica del juego en clases en lugar de variables globales.
  
## Contexto

Proyecto desarrollado en 2024 para el curso de Principios de Programación — Tecnólogo en Informática, UTEC.
