
#ifndef PLATAFORMA_H
#define PLATAFORMA_H
#include "usuario.h"
#include <string>

using namespace std;

class Plataforma
{
private:
    Usuario** usuarios;

    int cantidadUsuarios;
    int capacidadUsuarios;

    void ampliarCapacidad();

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

    bool guardarUsuarios(const string& ruta) const;
    bool cargarUsuarios(const string& ruta);
};

#endif

