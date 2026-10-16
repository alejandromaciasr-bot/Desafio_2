#ifndef USUARIO_H
#define USUARIO_H

#include "album.h"
#include <string>
#include <iosfwd>

using namespace std;

class Usuario
{
private:
    string nombreUsuario;
    string fechaNacimiento;
    string correo;
    string contrasena;
    string fotoPerfil;
    string ciudad;
    string pais;
    string fechaInscripcion;

    int cantidadAmigos;
    Album* albumes;
    int cantidadAlbumes;
    int capacidadAlbumes;

    void ampliarAlbumes();

public:
    Usuario();
    Usuario(const string& nombre);
    Usuario(const string& nombre,
            const string& nacimiento,
            const string& email,
            const string& rutaPerfil,
            const string& clave,
            const string& ciudadUsuario,
            const string& paisUsuario,
            const string& inscripcion);

    // regla de tres: el usuario administra sus propios albumes
    Usuario(const Usuario& otro);
    Usuario& operator=(const Usuario& otro);
    ~Usuario();

    const string& getNombreUsuario() const;
    const string& getFechaNacimiento() const;
    const string& getCorreo() const;
    const string& getFotoPerfil() const;
    const string& getCiudad() const;
    const string& getPais() const;
    const string& getFechaInscripcion() const;

    void setNombreUsuario(const string& nombre);
    bool verificarContrasena(const string& clave) const;

    // albumes: cambios solo al preparar datos iniciales y dar likes
    int getCantidadAlbumes() const;
    const Album* obtenerAlbum(int posicion) const;
    bool agregarAlbum(const Album& album);
    void vaciarAlbumes();
    bool modificarLikeFoto(int posicionAlbum, int posicionFoto, bool darLike);

    bool guardarEnArchivo(ostream& archivo) const;
    bool cargarDesdeArchivo(istream& archivo);
};

#endif
