#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

#include <cstddef>
#include <string>

// medidor sencillo: no administra contenedores ni sustituye los algoritmos
class MedidorRecursos
{
private:
    inline static unsigned long long iteraciones = 0;
    inline static bool activo = false;

public:
    // aproximacion superior para la capacidad adicional de una cadena;
    // std::string puede tener optimizacion para cadenas cortas.
    static std::size_t memoriaCadena(const std::string& dato)
    {
        return dato.capacity() + 1;
    }

    static void iniciar()
    {
        iteraciones = 0;
        activo = true;
    }

    static void contarIteracion()
    {
        if (activo)
        {
            iteraciones++;
        }
    }

    static void detener()
    {
        activo = false;
    }

    static unsigned long long getIteraciones()
    {
        return iteraciones;
    }
};

#endif
