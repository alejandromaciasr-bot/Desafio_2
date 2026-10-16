#include "plataforma.h"
#include <fstream>
#include <limits>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// constructor
Plataforma::Plataforma()
{
    cantidadUsuarios = 0;
    capacidadUsuarios = 2;
    cantidadSolicitudes = 0;
    capacidadSolicitudes = 2;
    cantidadMensajes = 0;
    capacidadMensajes = 2;

    usuarios = new Usuario*[capacidadUsuarios];
    matrizAmistades =
        new bool[capacidadUsuarios * capacidadUsuarios]();
    solicitudes = new SolicitudAmistad[capacidadSolicitudes];
    mensajes = new Mensaje[capacidadMensajes];
}

// destructor
Plataforma::~Plataforma()
{
    for (int i = 0; i < cantidadUsuarios; i++)
    {
        delete usuarios[i];
    }

    delete[] usuarios;
    delete[] matrizAmistades;
    delete[] solicitudes;
    delete[] mensajes;
}

// ampliar el arreglo de usuarios y la matriz
void Plataforma::ampliarCapacidad()
{
    int nuevaCapacidad = capacidadUsuarios * 2;
    Usuario** nuevoArreglo = new Usuario*[nuevaCapacidad];
    bool* nuevaMatriz = new bool[nuevaCapacidad * nuevaCapacidad]();

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        nuevoArreglo[i] = usuarios[i];
    }

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        for (int j = 0; j < cantidadUsuarios; j++)
        {
            nuevaMatriz[i * nuevaCapacidad + j] =
                matrizAmistades[i * capacidadUsuarios + j];
        }
    }

    delete[] usuarios;
    delete[] matrizAmistades;

    usuarios = nuevoArreglo;
    matrizAmistades = nuevaMatriz;
    capacidadUsuarios = nuevaCapacidad;
}

// ampliar el arreglo de solicitudes
void Plataforma::ampliarCapacidadSolicitudes()
{
    int nuevaCapacidad = capacidadSolicitudes * 2;
    SolicitudAmistad* nuevoArreglo =
        new SolicitudAmistad[nuevaCapacidad];

    for (int i = 0; i < cantidadSolicitudes; i++)
    {
        nuevoArreglo[i] = solicitudes[i];
    }

    delete[] solicitudes;
    solicitudes = nuevoArreglo;
    capacidadSolicitudes = nuevaCapacidad;
}

// ampliar el arreglo de mensajes
void Plataforma::ampliarCapacidadMensajes()
{
    int nuevaCapacidad = capacidadMensajes * 2;
    Mensaje* nuevoArreglo = new Mensaje[nuevaCapacidad];

    for (int i = 0; i < cantidadMensajes; i++)
    {
        nuevoArreglo[i] = mensajes[i];
    }

    delete[] mensajes;
    mensajes = nuevoArreglo;
    capacidadMensajes = nuevaCapacidad;
}

// eliminar una solicitud desplazando las restantes
void Plataforma::eliminarSolicitud(int posicion)
{
    for (int i = posicion; i < cantidadSolicitudes - 1; i++)
    {
        solicitudes[i] = solicitudes[i + 1];
    }

    cantidadSolicitudes--;
    solicitudes[cantidadSolicitudes] = SolicitudAmistad();
}

// buscar indice de un usuario
int Plataforma::buscarIndiceUsuario(const string& nombre) const
{
    for (int i = 0; i < cantidadUsuarios; i++)
    {
        if (usuarios[i]->getNombreUsuario() == nombre)
        {
            return i;
        }
    }

    return -1;
}

// buscar indice de una solicitud dirigida
int Plataforma::buscarIndiceSolicitud(const string& emisor,
                                      const string& receptor) const
{
    for (int i = 0; i < cantidadSolicitudes; i++)
    {
        if (solicitudes[i].getEmisor() == emisor &&
            solicitudes[i].getReceptor() == receptor)
        {
            return i;
        }
    }

    return -1;
}

// insertar usuario
bool Plataforma::agregarUsuario(const Usuario& usuario)
{
    if (usuario.getNombreUsuario().empty() ||
        buscarIndiceUsuario(usuario.getNombreUsuario()) != -1)
    {
        return false;
    }

    if (cantidadUsuarios == capacidadUsuarios)
    {
        ampliarCapacidad();
    }

    usuarios[cantidadUsuarios] = new Usuario(usuario);
    cantidadUsuarios++;
    return true;
}

const Usuario* Plataforma::buscarUsuario(const string& nombre) const
{
    int indice = buscarIndiceUsuario(nombre);
    if (indice == -1)
    {
        return nullptr;
    }
    return usuarios[indice];
}

const Usuario* Plataforma::obtenerUsuario(int posicion) const
{
    if (posicion < 0 || posicion >= cantidadUsuarios)
    {
        return nullptr;
    }
    return usuarios[posicion];
}

int Plataforma::getCantidadUsuarios() const
{
    return cantidadUsuarios;
}

int Plataforma::getCapacidadUsuarios() const
{
    return capacidadUsuarios;
}

// crear amistad solo desde la aceptacion de una solicitud
bool Plataforma::establecerAmistad(const string& usuario1,
                                   const string& usuario2)
{
    int i = buscarIndiceUsuario(usuario1);
    int j = buscarIndiceUsuario(usuario2);

    if (i == -1 || j == -1 || i == j ||
        matrizAmistades[i * capacidadUsuarios + j])
    {
        return false;
    }

    matrizAmistades[i * capacidadUsuarios + j] = true;
    matrizAmistades[j * capacidadUsuarios + i] = true;
    return true;
}

bool Plataforma::sonAmigos(const string& usuario1,
                           const string& usuario2) const
{
    int i = buscarIndiceUsuario(usuario1);
    int j = buscarIndiceUsuario(usuario2);

    if (i == -1 || j == -1)
    {
        return false;
    }

    return matrizAmistades[i * capacidadUsuarios + j];
}

int Plataforma::contarAmigos(const string& nombre) const
{
    int indice = buscarIndiceUsuario(nombre);
    if (indice == -1)
    {
        return -1;
    }

    int total = 0;
    for (int j = 0; j < cantidadUsuarios; j++)
    {
        if (matrizAmistades[indice * capacidadUsuarios + j])
        {
            total++;
        }
    }
    return total;
}

// crear solicitud pendiente
bool Plataforma::enviarSolicitud(const string& emisor,
                                 const string& receptor)
{
    if (emisor.empty() || receptor.empty() || emisor == receptor ||
        buscarIndiceUsuario(emisor) == -1 ||
        buscarIndiceUsuario(receptor) == -1 ||
        sonAmigos(emisor, receptor) ||
        buscarIndiceSolicitud(emisor, receptor) != -1 ||
        buscarIndiceSolicitud(receptor, emisor) != -1)
    {
        return false;
    }

    if (cantidadSolicitudes == capacidadSolicitudes)
    {
        ampliarCapacidadSolicitudes();
    }

    solicitudes[cantidadSolicitudes] = SolicitudAmistad(emisor, receptor);
    cantidadSolicitudes++;
    return true;
}

int Plataforma::contarSolicitudesRecibidas(const string& receptor) const
{
    if (buscarIndiceUsuario(receptor) == -1)
    {
        return -1;
    }

    int total = 0;
    for (int i = 0; i < cantidadSolicitudes; i++)
    {
        if (solicitudes[i].getReceptor() == receptor)
        {
            total++;
        }
    }
    return total;
}

const SolicitudAmistad* Plataforma::obtenerSolicitudRecibida(
    const string& receptor, int posicion) const
{
    if (posicion < 0 || buscarIndiceUsuario(receptor) == -1)
    {
        return nullptr;
    }

    int encontrada = 0;
    for (int i = 0; i < cantidadSolicitudes; i++)
    {
        if (solicitudes[i].getReceptor() == receptor)
        {
            if (encontrada == posicion)
            {
                return &solicitudes[i];
            }
            encontrada++;
        }
    }
    return nullptr;
}

bool Plataforma::aceptarSolicitud(const string& receptor,
                                  const string& emisor)
{
    int posicion = buscarIndiceSolicitud(emisor, receptor);
    if (posicion == -1 || !establecerAmistad(emisor, receptor))
    {
        return false;
    }

    eliminarSolicitud(posicion);
    return true;
}

bool Plataforma::rechazarSolicitud(const string& receptor,
                                   const string& emisor)
{
    int posicion = buscarIndiceSolicitud(emisor, receptor);
    if (posicion == -1)
    {
        return false;
    }

    eliminarSolicitud(posicion);
    return true;
}

// guardar datos basicos de los usuarios
bool Plataforma::guardarUsuarios(const string& ruta) const
{
    ofstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }

    archivo << "UDEABOOK_USUARIOS_V1" << '\n';
    archivo << cantidadUsuarios << '\n';
    for (int i = 0; i < cantidadUsuarios; i++)
    {
        if (!usuarios[i]->guardarEnArchivo(archivo))
        {
            return false;
        }
    }

    archivo.close();
    return archivo.good();
}

bool Plataforma::cargarUsuarios(const string& ruta)
{
    ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }

    string encabezado;
    if (!getline(archivo, encabezado) ||
        encabezado != "UDEABOOK_USUARIOS_V1")
    {
        return false;
    }

    int totalUsuarios;
    if (!(archivo >> totalUsuarios) || totalUsuarios < 0)
    {
        return false;
    }
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!archivo)
    {
        return false;
    }

    Plataforma temporal;
    for (int i = 0; i < totalUsuarios; i++)
    {
        Usuario nuevoUsuario;
        if (!nuevoUsuario.cargarDesdeArchivo(archivo) ||
            !temporal.agregarUsuario(nuevoUsuario))
        {
            return false;
        }
    }

    Usuario** usuariosAnteriores = usuarios;
    bool* matrizAnterior = matrizAmistades;
    SolicitudAmistad* solicitudesAnteriores = solicitudes;
    Mensaje* mensajesAnteriores = mensajes;
    int cantidadAnterior = cantidadUsuarios;
    int capacidadAnterior = capacidadUsuarios;
    int cantidadSolicitudesAnterior = cantidadSolicitudes;
    int capacidadSolicitudesAnterior = capacidadSolicitudes;
    int cantidadMensajesAnterior = cantidadMensajes;
    int capacidadMensajesAnterior = capacidadMensajes;

    usuarios = temporal.usuarios;
    matrizAmistades = temporal.matrizAmistades;
    solicitudes = temporal.solicitudes;
    mensajes = temporal.mensajes;
    cantidadUsuarios = temporal.cantidadUsuarios;
    capacidadUsuarios = temporal.capacidadUsuarios;
    cantidadSolicitudes = temporal.cantidadSolicitudes;
    capacidadSolicitudes = temporal.capacidadSolicitudes;
    cantidadMensajes = temporal.cantidadMensajes;
    capacidadMensajes = temporal.capacidadMensajes;

    temporal.usuarios = usuariosAnteriores;
    temporal.matrizAmistades = matrizAnterior;
    temporal.solicitudes = solicitudesAnteriores;
    temporal.mensajes = mensajesAnteriores;
    temporal.cantidadUsuarios = cantidadAnterior;
    temporal.capacidadUsuarios = capacidadAnterior;
    temporal.cantidadSolicitudes = cantidadSolicitudesAnterior;
    temporal.capacidadSolicitudes = capacidadSolicitudesAnterior;
    temporal.cantidadMensajes = cantidadMensajesAnterior;
    temporal.capacidadMensajes = capacidadMensajesAnterior;
    return true;
}

bool Plataforma::guardarAmistades(const string& ruta) const
{
    ofstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }

    int totalRelaciones = 0;
    for (int i = 0; i < cantidadUsuarios; i++)
    {
        for (int j = i + 1; j < cantidadUsuarios; j++)
        {
            if (matrizAmistades[i * capacidadUsuarios + j])
            {
                totalRelaciones++;
            }
        }
    }

    archivo << "UDEABOOK_AMISTADES_V1" << '\n';
    archivo << totalRelaciones << '\n';

    for (int i = 0; i < cantidadUsuarios; i++)
    {
        for (int j = i + 1; j < cantidadUsuarios; j++)
        {
            if (matrizAmistades[i * capacidadUsuarios + j])
            {
                archivo << usuarios[i]->getNombreUsuario() << '\n';
                archivo << usuarios[j]->getNombreUsuario() << '\n';
            }
        }
    }

    archivo.close();
    return archivo.good();
}

bool Plataforma::cargarAmistades(const string& ruta)
{
    ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }

    string encabezado;
    if (!getline(archivo, encabezado) ||
        encabezado != "UDEABOOK_AMISTADES_V1")
    {
        return false;
    }

    int totalRelaciones;
    if (!(archivo >> totalRelaciones) || totalRelaciones < 0)
    {
        return false;
    }
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!archivo)
    {
        return false;
    }

    bool* nuevaMatriz =
        new bool[capacidadUsuarios * capacidadUsuarios]();

    for (int k = 0; k < totalRelaciones; k++)
    {
        string nombre1;
        string nombre2;
        if (!getline(archivo, nombre1) || !getline(archivo, nombre2))
        {
            delete[] nuevaMatriz;
            return false;
        }

        int i = buscarIndiceUsuario(nombre1);
        int j = buscarIndiceUsuario(nombre2);
        if (i == -1 || j == -1 || i == j ||
            nuevaMatriz[i * capacidadUsuarios + j])
        {
            delete[] nuevaMatriz;
            return false;
        }

        nuevaMatriz[i * capacidadUsuarios + j] = true;
        nuevaMatriz[j * capacidadUsuarios + i] = true;
    }

    delete[] matrizAmistades;
    matrizAmistades = nuevaMatriz;
    return true;
}

bool Plataforma::guardarSolicitudes(const string& ruta) const
{
    ofstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }

    archivo << "UDEABOOK_SOLICITUDES_V1" << '\n';
    archivo << cantidadSolicitudes << '\n';
    for (int i = 0; i < cantidadSolicitudes; i++)
    {
        archivo << solicitudes[i].getEmisor() << '\n';
        archivo << solicitudes[i].getReceptor() << '\n';
    }

    archivo.close();
    return archivo.good();
}

bool Plataforma::cargarSolicitudes(const string& ruta)
{
    ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }

    string encabezado;
    if (!getline(archivo, encabezado) ||
        encabezado != "UDEABOOK_SOLICITUDES_V1")
    {
        return false;
    }

    int totalSolicitudes;
    if (!(archivo >> totalSolicitudes) || totalSolicitudes < 0)
    {
        return false;
    }
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!archivo)
    {
        return false;
    }

    int nuevaCapacidad = (totalSolicitudes > 2) ? totalSolicitudes : 2;
    SolicitudAmistad* nuevasSolicitudes =
        new SolicitudAmistad[nuevaCapacidad];

    for (int i = 0; i < totalSolicitudes; i++)
    {
        string emisor;
        string receptor;
        if (!getline(archivo, emisor) || !getline(archivo, receptor))
        {
            delete[] nuevasSolicitudes;
            return false;
        }

        if (emisor.empty() || receptor.empty() || emisor == receptor ||
            buscarIndiceUsuario(emisor) == -1 ||
            buscarIndiceUsuario(receptor) == -1 ||
            sonAmigos(emisor, receptor))
        {
            delete[] nuevasSolicitudes;
            return false;
        }

        for (int j = 0; j < i; j++)
        {
            if ((nuevasSolicitudes[j].getEmisor() == emisor &&
                 nuevasSolicitudes[j].getReceptor() == receptor) ||
                (nuevasSolicitudes[j].getEmisor() == receptor &&
                 nuevasSolicitudes[j].getReceptor() == emisor))
            {
                delete[] nuevasSolicitudes;
                return false;
            }
        }

        nuevasSolicitudes[i] = SolicitudAmistad(emisor, receptor);
    }

    delete[] solicitudes;
    solicitudes = nuevasSolicitudes;
    cantidadSolicitudes = totalSolicitudes;
    capacidadSolicitudes = nuevaCapacidad;
    return true;
}


// crear una fecha con la hora local del computador
static string obtenerFechaHoraActual()
{
    auto ahora = chrono::system_clock::now();
    time_t instante = chrono::system_clock::to_time_t(ahora);
    tm* fecha = std::localtime(&instante);

    if (fecha == nullptr)
    {
        return "";
    }

    ostringstream salida;
    salida << put_time(fecha, "%Y-%m-%d %H:%M:%S");
    return salida.str();
}

// enviar solo a amigos e insertar por fecha de envio
bool Plataforma::enviarMensaje(const string& emisor,
                               const string& receptor,
                               const string& texto)
{
    if (emisor == receptor || !sonAmigos(emisor, receptor) ||
        !Mensaje::textoValido(texto))
    {
        return false;
    }

    string fecha = obtenerFechaHoraActual();
    if (!Mensaje::fechaHoraValida(fecha))
    {
        return false;
    }

    if (cantidadMensajes == capacidadMensajes)
    {
        ampliarCapacidadMensajes();
    }

    int posicion = cantidadMensajes;
    while (posicion > 0 && mensajes[posicion - 1].getFechaEnvio() > fecha)
    {
        mensajes[posicion] = mensajes[posicion - 1];
        posicion--;
    }

    mensajes[posicion] = Mensaje(emisor, receptor, texto, fecha);
    cantidadMensajes++;
    return true;
}

int Plataforma::getCantidadMensajes() const
{
    return cantidadMensajes;
}

const Mensaje* Plataforma::obtenerMensaje(int posicion) const
{
    if (posicion < 0 || posicion >= cantidadMensajes)
    {
        return nullptr;
    }
    return &mensajes[posicion];
}

// todos los mensajes nuevos se marcan como leidos tras mostrarlos
int Plataforma::marcarMensajesLeidos(const string& receptor)
{
    if (buscarIndiceUsuario(receptor) == -1)
    {
        return -1;
    }

    int cantidad = 0;
    for (int i = 0; i < cantidadMensajes; i++)
    {
        if (mensajes[i].getReceptor() == receptor &&
            !mensajes[i].estaLeido())
        {
            mensajes[i].marcarLeido();
            cantidad++;
        }
    }
    return cantidad;
}

bool Plataforma::guardarMensajes(const string& ruta) const
{
    ofstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }

    archivo << "UDEABOOK_MENSAJES_V1" << '\n';
    archivo << cantidadMensajes << '\n';
    for (int i = 0; i < cantidadMensajes; i++)
    {
        if (!mensajes[i].guardarEnArchivo(archivo))
        {
            return false;
        }
    }

    archivo.close();
    return archivo.good();
}

bool Plataforma::cargarMensajes(const string& ruta)
{
    ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }

    string encabezado;
    if (!getline(archivo, encabezado) ||
        encabezado != "UDEABOOK_MENSAJES_V1")
    {
        return false;
    }

    int total;
    if (!(archivo >> total) || total < 0)
    {
        return false;
    }
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!archivo)
    {
        return false;
    }

    int nuevaCapacidad = (total > 2) ? total : 2;
    Mensaje* nuevosMensajes = new Mensaje[nuevaCapacidad];

    for (int i = 0; i < total; i++)
    {
        if (!nuevosMensajes[i].cargarDesdeArchivo(archivo))
        {
            delete[] nuevosMensajes;
            return false;
        }

        if (buscarIndiceUsuario(nuevosMensajes[i].getEmisor()) == -1 ||
            buscarIndiceUsuario(nuevosMensajes[i].getReceptor()) == -1 ||
            (i > 0 && nuevosMensajes[i - 1].getFechaEnvio() >
                          nuevosMensajes[i].getFechaEnvio()))
        {
            delete[] nuevosMensajes;
            return false;
        }
    }

    delete[] mensajes;
    mensajes = nuevosMensajes;
    cantidadMensajes = total;
    capacidadMensajes = nuevaCapacidad;
    return true;
}
