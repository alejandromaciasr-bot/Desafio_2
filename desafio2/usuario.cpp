#include "usuario.h"

// constructor por defecto
Usuario::Usuario()
{
    nombreUsuario = "";
    fechaNacimiento = "";
    correo = "";
    contrasena = "";
    fotoPerfil = "";
    ciudad = "";
    pais = "";
    fechaInscripcion = "";

    cantidadAmigos = 0;
    cantidadAlbumes = 0;
}

// constructor con nombre
Usuario::Usuario(const string& nombre) : Usuario()
{
    nombreUsuario = nombre;
}

// constructor con todos los datos basicos
Usuario::Usuario(const string& nombre,
                 const string& nacimiento,
                 const string& email,
                 const string& rutaPerfil,
                 const string& clave,
                 const string& ciudadUsuario,
                 const string& paisUsuario,
                 const string& inscripcion) : Usuario()
{
    nombreUsuario = nombre;
    fechaNacimiento = nacimiento;
    correo = email;
    fotoPerfil = rutaPerfil;
    contrasena = clave;
    ciudad = ciudadUsuario;
    pais = paisUsuario;
    fechaInscripcion = inscripcion;
}

// getters
const string& Usuario::getNombreUsuario() const
{
    return nombreUsuario;
}

const string& Usuario::getFechaNacimiento() const
{
    return fechaNacimiento;
}

const string& Usuario::getCorreo() const
{
    return correo;
}

const string& Usuario::getFotoPerfil() const
{
    return fotoPerfil;
}

const string& Usuario::getCiudad() const
{
    return ciudad;
}

const string& Usuario::getPais() const
{
    return pais;
}

const string& Usuario::getFechaInscripcion() const
{
    return fechaInscripcion;
}

// setters
void Usuario::setNombreUsuario(const string& nombre)
{
    nombreUsuario = nombre;
}

// validacion de contrasena
bool Usuario::verificarContrasena(const string& clave) const
{
    return contrasena == clave;
}

// guardar los atributos del usuario
bool Usuario::guardarEnArchivo(ostream& archivo) const
{
    archivo << nombreUsuario << '\n';
    archivo << fechaNacimiento << '\n';
    archivo << correo << '\n';
    archivo << fotoPerfil << '\n';
    archivo << contrasena << '\n';
    archivo << ciudad << '\n';
    archivo << pais << '\n';
    archivo << fechaInscripcion << '\n';

    return archivo.good();
}

// cargar los atributos del usuario
bool Usuario::cargarDesdeArchivo(istream& archivo)
{
    string nombre;
    string nacimiento;
    string email;
    string rutaPerfil;
    string clave;
    string ciudadUsuario;
    string paisUsuario;
    string inscripcion;

    if (!getline(archivo, nombre) ||
        !getline(archivo, nacimiento) ||
        !getline(archivo, email) ||
        !getline(archivo, rutaPerfil) ||
        !getline(archivo, clave) ||
        !getline(archivo, ciudadUsuario) ||
        !getline(archivo, paisUsuario) ||
        !getline(archivo, inscripcion))
    {
        return false;
    }

    if (nombre.empty())
    {
        return false;
    }

    nombreUsuario = nombre;
    fechaNacimiento = nacimiento;
    correo = email;
    fotoPerfil = rutaPerfil;
    contrasena = clave;
    ciudad = ciudadUsuario;
    pais = paisUsuario;
    fechaInscripcion = inscripcion;

    cantidadAmigos = 0;
    cantidadAlbumes = 0;

    return true;
}
