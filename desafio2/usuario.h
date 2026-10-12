
#ifndef USUARIO_H
#define USUARIO_H

#include <string>

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
    int cantidadAlbumes;

public:
    // constructores
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

    // getters
    const string& getNombreUsuario() const;
    const string& getFechaNacimiento() const;
    const string& getCorreo() const;
    const string& getFotoPerfil() const;
    const string& getCiudad() const;
    const string& getPais() const;
    const string& getFechaInscripcion() const;

    // setters
    void setNombreUsuario(const string& nombre);

    // validacion de contrasena
    bool verificarContrasena(const string& clave) const;
};

#endif
