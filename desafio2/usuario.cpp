#include "medidorrecursos.h"
#include "usuario.h"
#include <iostream>

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
    albumes = nullptr;
    cantidadAlbumes = 0;
    capacidadAlbumes = 0;
}

Usuario::Usuario(const string& nombre) : Usuario()
{
    nombreUsuario = nombre;
}

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

Usuario::Usuario(const Usuario& otro) : Usuario()
{
    *this = otro;
}

Usuario& Usuario::operator=(const Usuario& otro)
{
    if (this == &otro)
    {
        return *this;
    }

    Album* nuevosAlbumes = nullptr;
    if (otro.capacidadAlbumes > 0)
    {
        nuevosAlbumes = new Album[otro.capacidadAlbumes];
        for (
            int i = 0;
            i < otro.cantidadAlbumes;
            i++
            )
        {
            MedidorRecursos::contarIteracion();
            nuevosAlbumes[i] = otro.albumes[i];
        }
    }

    delete[] albumes;
    albumes = nuevosAlbumes;
    cantidadAlbumes = otro.cantidadAlbumes;
    capacidadAlbumes = otro.capacidadAlbumes;

    nombreUsuario = otro.nombreUsuario;
    fechaNacimiento = otro.fechaNacimiento;
    correo = otro.correo;
    contrasena = otro.contrasena;
    fotoPerfil = otro.fotoPerfil;
    ciudad = otro.ciudad;
    pais = otro.pais;
    fechaInscripcion = otro.fechaInscripcion;
    cantidadAmigos = otro.cantidadAmigos;
    return *this;
}

Usuario::~Usuario()
{
    delete[] albumes;
}

void Usuario::ampliarAlbumes()
{
    int nuevaCapacidad = capacidadAlbumes == 0 ? 2 : capacidadAlbumes * 2;
    Album* nuevoArreglo = new Album[nuevaCapacidad];
    for (
        int i = 0;
        i < cantidadAlbumes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        nuevoArreglo[i] = albumes[i];
    }

    delete[] albumes;
    albumes = nuevoArreglo;
    capacidadAlbumes = nuevaCapacidad;
}

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

void Usuario::setNombreUsuario(const string& nombre)
{
    nombreUsuario = nombre;
}

bool Usuario::verificarContrasena(const string& clave) const
{
    return contrasena == clave;
}

int Usuario::getCantidadAlbumes() const
{
    return cantidadAlbumes;
}

const Album* Usuario::obtenerAlbum(int posicion) const
{
    if (posicion < 0 || posicion >= cantidadAlbumes)
    {
        return nullptr;
    }
    return &albumes[posicion];
}

bool Usuario::agregarAlbum(const Album& album)
{
    if (album.getNombre().empty() ||
        !Album::privacidadValida(album.getPrivacidad()))
    {
        return false;
    }

    for (
        int i = 0;
        i < cantidadAlbumes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        if (albumes[i].getNombre() == album.getNombre())
        {
            return false;
        }
    }

    if (cantidadAlbumes == capacidadAlbumes)
    {
        ampliarAlbumes();
    }

    albumes[cantidadAlbumes] = album;
    cantidadAlbumes++;
    return true;
}

void Usuario::vaciarAlbumes()
{
    delete[] albumes;
    albumes = nullptr;
    cantidadAlbumes = 0;
    capacidadAlbumes = 0;
}

bool Usuario::modificarLikeFoto(int posicionAlbum, int posicionFoto,
                                bool darLike)
{
    if (posicionAlbum < 0 || posicionAlbum >= cantidadAlbumes)
    {
        return false;
    }
    return albumes[posicionAlbum].modificarLike(posicionFoto, darLike);
}

// mantenemos el formato V1 de los datos basicos de usuario
size_t Usuario::memoriaDinamicaEstimada() const
{
    size_t bytes = static_cast<size_t>(capacidadAlbumes) * sizeof(Album);
    bytes = bytes + MedidorRecursos::memoriaCadena(nombreUsuario);
    bytes = bytes + MedidorRecursos::memoriaCadena(fechaNacimiento);
    bytes = bytes + MedidorRecursos::memoriaCadena(correo);
    bytes = bytes + MedidorRecursos::memoriaCadena(contrasena);
    bytes = bytes + MedidorRecursos::memoriaCadena(fotoPerfil);
    bytes = bytes + MedidorRecursos::memoriaCadena(ciudad);
    bytes = bytes + MedidorRecursos::memoriaCadena(pais);
    bytes = bytes + MedidorRecursos::memoriaCadena(fechaInscripcion);

    for (
        int i = 0;
        i < cantidadAlbumes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        bytes = bytes + albumes[i].memoriaDinamicaEstimada();
    }
    return bytes;
}

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
        !getline(archivo, inscripcion) || nombre.empty())
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

    delete[] albumes;
    albumes = nullptr;
    cantidadAlbumes = 0;
    capacidadAlbumes = 0;
    return true;
}
