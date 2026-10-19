link del video: [https://youtu.be/Yl7TDUvs6gQ](https://www.youtube.com/watch?v=Yl7TDUvs6gQ)


Ude@Book — Desafío II de Informática II

Ude@Book es una simulación de las funcionalidades básicas de una red social, ejecutada desde la consola. El proyecto se centra en la programación orientada a objetos, el manejo manual de estructuras dinámicas, la persistencia de información en archivos de texto y la medición de recursos.

La aplicación trabaja con usuarios, amistades, solicitudes, mensajes y álbumes fotográficos. Las fotografías se representan mediante sus rutas de archivo: no se abren ni se muestran las imágenes reales.
Funcionalidades
- Inicio de sesión mediante nombre de usuario y contraseña.
- Visualización de tres fotos de perfil de amigos mediante selección aleatoria ponderada, con una actualización por sesión.
- Envío, consulta, aceptación y rechazo de solicitudes de amistad.
- Consulta de relaciones de amistad y número de amigos.
- Sugerencias de amistad ordenadas por cantidad de amigos en común (entre 1 y 20).
- Envío de mensajes de hasta 512 caracteres, lectura de mensajes nuevos y búsquedas por rango de fechas.
- Consulta de álbumes públicos o visibles para amigos, recorrido circular de fotografías y modificación de likes.
- Lectura y actualización de los datos almacenados en archivos .txt.
- Reporte de iteraciones directas e indirectas y consumo estimado de memoria en las operaciones.
Diseño del programa
El sistema está organizado en seis clases principales:
Clase	Responsabilidad
Plataforma	Coordina usuarios, amistades, solicitudes, mensajes y persistencia.
Usuario	Almacena los datos de cada usuario y administra sus álbumes.
SolicitudAmistad	Representa una solicitud entre dos usuarios.
Mensaje	Representa un mensaje, su fecha y estado de lectura.
Album	Agrupa fotografías y establece su privacidad.
Foto	Almacena los datos de una fotografía y administra sus likes.


Además, la clase auxiliar MedidorRecursos apoya el conteo de iteraciones y la estimación de memoria.

La implementación utiliza arreglos dinámicos y punteros, incluida una matriz de adyacencia almacenada en un arreglo unidimensional de bool para representar las amistades. No se utilizan contenedores STL, struct ni herencia.
Archivos de almacenamiento permanente
El programa emplea cinco archivos de texto:
Archivo	Contenido
usuarios.txt	Datos personales y credenciales.
amistades.txt	Relaciones de amistad.
solicitudes.txt	Solicitudes de amistad pendientes.
mensajes.txt	Mensajes enviados y recibidos.
albumes.txt	Álbumes, fotografías, privacidad y likes.


