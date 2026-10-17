#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include "usuario.h"
#include "solicitudamistad.h"
#include "mensaje.h"
#include <string>

using namespace std;

class Plataforma
{
private:
    Usuario** usuarios;
    bool* matrizAmistades;
    SolicitudAmistad* solicitudes;
    Mensaje* mensajes;

    int cantidadUsuarios;
    int capacidadUsuarios;
    int cantidadSolicitudes;
    int capacidadSolicitudes;
    int cantidadMensajes;
    int capacidadMensajes;

    void ampliarCapacidad();
    void ampliarCapacidadSolicitudes();
    void ampliarCapacidadMensajes();
    void eliminarSolicitud(int posicion);
    int buscarIndiceUsuario(const string& nombre) const;
    int buscarIndiceSolicitud(const string& emisor,
                              const string& receptor) const;
    bool establecerAmistad(const string& usuario1,
                           const string& usuario2);

public:
    Plataforma();
    ~Plataforma();

    Plataforma(const Plataforma& otra) = delete;
    Plataforma& operator=(const Plataforma& otra) = delete;

    bool agregarUsuario(const Usuario& usuario);
    bool autenticarUsuario(const string& nombre, const string& clave) const;
    const Usuario* buscarUsuario(const string& nombre) const;
    const Usuario* obtenerUsuario(int posicion) const;
    int getCantidadUsuarios() const;
    int getCapacidadUsuarios() const;

    bool sonAmigos(const string& usuario1,
                   const string& usuario2) const;
    int contarAmigos(const string& nombre) const;

    bool enviarSolicitud(const string& emisor, const string& receptor);
    int contarSolicitudesRecibidas(const string& receptor) const;
    const SolicitudAmistad* obtenerSolicitudRecibida(
        const string& receptor, int posicion) const;
    bool aceptarSolicitud(const string& receptor, const string& emisor);
    bool rechazarSolicitud(const string& receptor, const string& emisor);

    // calcular hasta k sugerencias ordenadas por amigos en comun
    // indices y coincidencias apuntan a arreglos con al menos k posiciones
    // devuelve -1 si los parametros son incorrectos
    int generarSugerencias(const string& nombre, int k,
                           int* indices, int* coincidencias) const;

    bool enviarMensaje(const string& emisor, const string& receptor,
                       const string& texto);
    int getCantidadMensajes() const;
    const Mensaje* obtenerMensaje(int posicion) const;
    int marcarMensajesLeidos(const string& receptor);

    // albumes y fotos (el usuario es dueno de sus albumes)
    bool agregarAlbumInicial(const string& dueno, const Album& album);
    bool puedeVerAlbum(const string& visitante, const string& dueno,
                       int posicionAlbum) const;
    bool modificarLikeFoto(const string& visitante, const string& dueno,
                           int posicionAlbum, int posicionFoto, bool darLike);
    bool guardarAlbumes(const string& ruta) const;
    bool cargarAlbumes(const string& ruta);

    bool guardarMensajes(const string& ruta) const;
    bool cargarMensajes(const string& ruta);

    bool guardarUsuarios(const string& ruta) const;
    bool cargarUsuarios(const string& ruta);
    bool guardarAmistades(const string& ruta) const;
    bool cargarAmistades(const string& ruta);
    bool guardarSolicitudes(const string& ruta) const;
    bool cargarSolicitudes(const string& ruta);
};

#endif
