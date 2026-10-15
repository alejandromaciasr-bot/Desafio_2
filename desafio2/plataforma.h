#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include "usuario.h"
#include <string>

using namespace std;

class Plataforma
{
private:
    Usuario** usuarios;
    bool* matrizAmistades;

    int cantidadUsuarios;
    int capacidadUsuarios;

    void ampliarCapacidad();
    int buscarIndiceUsuario(const string& nombre) const;

public:
    Plataforma();
    ~Plataforma();

    Plataforma(const Plataforma& otra) = delete;
    Plataforma& operator=(const Plataforma& otra) = delete;

    bool agregarUsuario(const Usuario& usuario);

    const Usuario* buscarUsuario(const string& nombre) const;
    const Usuario* obtenerUsuario(int posicion) const;

    int getCantidadUsuarios() const;
    int getCapacidadUsuarios() const;

    // operaciones de amistad
    bool establecerAmistad(const string& usuario1,
                           const string& usuario2);

    bool sonAmigos(const string& usuario1,
                   const string& usuario2) const;

    int contarAmigos(const string& nombre) const;

    // almacenamiento permanente
    bool guardarUsuarios(const string& ruta) const;
    bool cargarUsuarios(const string& ruta);

    bool guardarAmistades(const string& ruta) const;
    bool cargarAmistades(const string& ruta);
};

#endif
