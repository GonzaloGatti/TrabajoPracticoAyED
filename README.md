# TrabajoPracticoAyED
Repositorio del trabajo practico "Laberintos y Fantasmas", Algoritmos y estructuras de datos

## Pasos para jugar a "Laberintos y Fantasmas":
1. Una vez clonado el proyecto en su computadora local, debe compilar ambos proyectos incluyendo la biblioteca ws2_32 para poder utilizar los sockets:
  gcc src/servidor/*.c -o servidor.exe -lws2_32  
  gcc src/cliente/*.c  -o cliente.exe  -lws2_32  
3. Ejecutar proyecto servidor. De esta manera, el servidor estara "escuchando" en el puerto asignado (12345) una conexion.
4. Ejecutar proyecto cliente. El mismo se conectara al servidor mediante el mismo puerto (12345), de manera de poder comunicarse y llevar a cabo el servicio de base de datos.
5. Jugar. Mientras el servidor este activo, todas las partidas jugadas se estar guardando en el mismo, pudiendo obtener una respuesta del servidor, por ejemplo, al pedir el ranking de jugadores.
