#include "medidorrecursos.h"
#include "plataforma.h"
#include <fstream>
#include <limits>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <random>

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
    for (
        int i = 0;
        i < cantidadUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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

    for (
        int i = 0;
        i < cantidadUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        nuevoArreglo[i] = usuarios[i];
    }

    for (
        int i = 0;
        i < cantidadUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        for (
            int j = 0;
            j < cantidadUsuarios;
            j++
            )
        {
            MedidorRecursos::contarIteracion();
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

    for (
        int i = 0;
        i < cantidadSolicitudes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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

    for (
        int i = 0;
        i < cantidadMensajes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        nuevoArreglo[i] = mensajes[i];
    }

    delete[] mensajes;
    mensajes = nuevoArreglo;
    capacidadMensajes = nuevaCapacidad;
}

// eliminar una solicitud desplazando las restantes
void Plataforma::eliminarSolicitud(int posicion)
{
    for (
        int i = posicion;
        i < cantidadSolicitudes - 1;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        solicitudes[i] = solicitudes[i + 1];
    }

    cantidadSolicitudes--;
    solicitudes[cantidadSolicitudes] = SolicitudAmistad();
}

// buscar indice de un usuario
int Plataforma::buscarIndiceUsuario(const string& nombre) const
{
    for (
        int i = 0;
        i < cantidadUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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
    for (
        int i = 0;
        i < cantidadSolicitudes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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

// comprobar credenciales usando los datos cargados desde usuarios.txt
bool Plataforma::autenticarUsuario(const string& nombre,
                                   const string& clave) const
{
    if (nombre.empty() || clave.empty())
    {
        return false;
    }

    int indice = buscarIndiceUsuario(nombre);
    if (indice == -1)
    {
        return false;
    }

    return usuarios[indice]->verificarContrasena(clave);
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

// estimacion de las estructuras y objetos propios que permanecen en ram
size_t Plataforma::memoriaTotalEstimada() const
{
    size_t bytes = sizeof(Plataforma);
    bytes = bytes + static_cast<size_t>(capacidadUsuarios) * sizeof(Usuario*);
    bytes = bytes + static_cast<size_t>(capacidadUsuarios)
                        * static_cast<size_t>(capacidadUsuarios) * sizeof(bool);
    bytes = bytes + static_cast<size_t>(capacidadSolicitudes)
                        * sizeof(SolicitudAmistad);
    bytes = bytes + static_cast<size_t>(capacidadMensajes) * sizeof(Mensaje);

    for (
        int i = 0;
        i < cantidadUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        bytes = bytes + sizeof(Usuario);
        bytes = bytes + usuarios[i]->memoriaDinamicaEstimada();
    }
    for (
        int i = 0;
        i < cantidadSolicitudes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        bytes = bytes + MedidorRecursos::memoriaCadena(solicitudes[i].getEmisor());
        bytes = bytes + MedidorRecursos::memoriaCadena(solicitudes[i].getReceptor());
    }
    for (
        int i = 0;
        i < cantidadMensajes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        bytes = bytes + MedidorRecursos::memoriaCadena(mensajes[i].getEmisor());
        bytes = bytes + MedidorRecursos::memoriaCadena(mensajes[i].getReceptor());
        bytes = bytes + MedidorRecursos::memoriaCadena(mensajes[i].getTexto());
        bytes = bytes + MedidorRecursos::memoriaCadena(mensajes[i].getFechaEnvio());
    }
    return bytes;
}

// seleccionar fotos de amigos con prioridad segun amigos en comun
// los pesos 2, 5 y 7 equivalen a 1, 2.5 y 3.5
int Plataforma::seleccionarFotosAmigos(const string& nombre, int* resultado,
                                       int maximo, const int* anteriores,
                                       int cantidadAnteriores) const
{
    int indiceUsuario = buscarIndiceUsuario(nombre);
    if (indiceUsuario == -1 || resultado == nullptr || maximo < 1 ||
        cantidadAnteriores < 0 || (cantidadAnteriores > 0 && anteriores == nullptr))
    {
        return -1;
    }

    // arreglos dinamicos propios, sin contenedores stl
    int* candidatos = new int[cantidadUsuarios];
    int* pesos = new int[cantidadUsuarios];
    int totalElegidos = 0;

    // generador de numeros aleatorios de la biblioteca estandar
    mt19937 generador(random_device{}());

    // fase 0: seleccionar primero amigos que no salieron antes
    // fase 1: completar con amigos anteriores si no hay otros suficientes
    for (
        int fase = 0;
        fase < 2 && totalElegidos < maximo;
        fase++
        )
    {
        MedidorRecursos::contarIteracion();
        int totalCandidatos = 0;

        for (
            int j = 0;
            j < cantidadUsuarios;
            j++
            )
        {
            MedidorRecursos::contarIteracion();
            if (!matrizAmistades[indiceUsuario * capacidadUsuarios + j])
            {
                continue;
            }

            bool yaElegido = false;
            for (
                int k = 0;
                k < totalElegidos;
                k++
                )
            {
                MedidorRecursos::contarIteracion();
                if (resultado[k] == j)
                {
                    yaElegido = true;
                    break;
                }
            }
            if (yaElegido)
            {
                continue;
            }

            if (fase == 0)
            {
                bool aparecioAntes = false;
                for (
                    int k = 0;
                    k < cantidadAnteriores;
                    k++
                    )
                {
                    MedidorRecursos::contarIteracion();
                    if (anteriores[k] == j)
                    {
                        aparecioAntes = true;
                        break;
                    }
                }
                if (aparecioAntes)
                {
                    continue;
                }
            }

            int enComun = 0;
            for (
                int k = 0;
                k < cantidadUsuarios;
                k++
                )
            {
                MedidorRecursos::contarIteracion();
                if (matrizAmistades[indiceUsuario * capacidadUsuarios + k] &&
                    matrizAmistades[j * capacidadUsuarios + k])
                {
                    enComun++;
                }
            }

            candidatos[totalCandidatos] = j;
            if (enComun <= 10)
            {
                pesos[totalCandidatos] = 2;
            }
            else if (enComun <= 20)
            {
                pesos[totalCandidatos] = 5;
            }
            else
            {
                pesos[totalCandidatos] = 7;
            }
            totalCandidatos++;
        }

        // sorteo ponderado sin reemplazo: quitar al elegido de los candidatos
        while (totalElegidos < maximo && totalCandidatos > 0)
        {
            MedidorRecursos::contarIteracion();
            int sumaPesos = 0;
            for (
                int i = 0;
                i < totalCandidatos;
                i++
                )
            {
                MedidorRecursos::contarIteracion();
                sumaPesos = sumaPesos + pesos[i];
            }

            uniform_int_distribution<int> sorteo(1, sumaPesos);
            int boleto = sorteo(generador);
            int posicionElegida = 0;

            for (
                int i = 0;
                i < totalCandidatos;
                i++
                )
            {
                MedidorRecursos::contarIteracion();
                boleto = boleto - pesos[i];
                if (boleto <= 0)
                {
                    posicionElegida = i;
                    break;
                }
            }

            resultado[totalElegidos] = candidatos[posicionElegida];
            totalElegidos++;

            // eliminacion manual del candidato, sin repetirlo en el grupo
            for (
                int i = posicionElegida;
                i < totalCandidatos - 1;
                i++
                )
            {
                MedidorRecursos::contarIteracion();
                candidatos[i] = candidatos[i + 1];
                pesos[i] = pesos[i + 1];
            }
            totalCandidatos--;
        }
    }

    // mezclamos el resultado para que las fotos nuevas no aparezcan
    // siempre al principio del segundo grupo
    for (
        int i = totalElegidos - 1;
        i > 0;
        i--
        )
    {
        MedidorRecursos::contarIteracion();
        uniform_int_distribution<int> sorteo(0, i);
        int j = sorteo(generador);
        int auxiliar = resultado[i];
        resultado[i] = resultado[j];
        resultado[j] = auxiliar;
    }

    delete[] candidatos;
    delete[] pesos;
    return totalElegidos;
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
    for (
        int j = 0;
        j < cantidadUsuarios;
        j++
        )
    {
        MedidorRecursos::contarIteracion();
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
    for (
        int i = 0;
        i < cantidadSolicitudes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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
    for (
        int i = 0;
        i < cantidadSolicitudes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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
    for (
        int i = 0;
        i < cantidadUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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
    for (
        int i = 0;
        i < totalUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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
    for (
        int i = 0;
        i < cantidadUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        for (
            int j = i + 1;
            j < cantidadUsuarios;
            j++
            )
        {
            MedidorRecursos::contarIteracion();
            if (matrizAmistades[i * capacidadUsuarios + j])
            {
                totalRelaciones++;
            }
        }
    }

    archivo << "UDEABOOK_AMISTADES_V1" << '\n';
    archivo << totalRelaciones << '\n';

    for (
        int i = 0;
        i < cantidadUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        for (
            int j = i + 1;
            j < cantidadUsuarios;
            j++
            )
        {
            MedidorRecursos::contarIteracion();
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

    for (
        int k = 0;
        k < totalRelaciones;
        k++
        )
    {
        MedidorRecursos::contarIteracion();
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
    for (
        int i = 0;
        i < cantidadSolicitudes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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

    for (
        int i = 0;
        i < totalSolicitudes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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

        for (
            int j = 0;
            j < i;
            j++
            )
        {
            MedidorRecursos::contarIteracion();
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
    chrono::system_clock::time_point ahora = chrono::system_clock::now();
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
        MedidorRecursos::contarIteracion();
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
    for (
        int i = 0;
        i < cantidadMensajes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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
    for (
        int i = 0;
        i < cantidadMensajes;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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

    for (
        int i = 0;
        i < total;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
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

// sugerir amigos de amigos, sin contenedores ni ordenamiento stl
int Plataforma::generarSugerencias(const string& nombre, int k,
                                   int* indices, int* coincidencias) const
{
    int usuario = buscarIndiceUsuario(nombre);

    if (usuario == -1 || k < 1 || k > 20 ||
        indices == nullptr || coincidencias == nullptr)
    {
        return -1;
    }

    // contar amigos en comun mediante la matriz de adyacencia
    int* comunes = new int[cantidadUsuarios]();

    for (
        int amigo = 0;
        amigo < cantidadUsuarios;
        amigo++
        )
    {
        MedidorRecursos::contarIteracion();
        if (matrizAmistades[usuario * capacidadUsuarios + amigo])
        {
            for (
                int candidato = 0;
                candidato < cantidadUsuarios;
                candidato++
                )
            {
                MedidorRecursos::contarIteracion();
                if (matrizAmistades[amigo * capacidadUsuarios + candidato])
                {
                    comunes[candidato]++;
                }
            }
        }
    }

    // guardar solo los mejores k en arreglos propios
    int total = 0;

    for (
        int candidato = 0;
        candidato < cantidadUsuarios;
        candidato++
        )
    {
        MedidorRecursos::contarIteracion();
        if (candidato == usuario || comunes[candidato] == 0 ||
            matrizAmistades[usuario * capacidadUsuarios + candidato])
        {
            continue;
        }

        const string& nombreCandidato =
            usuarios[candidato]->getNombreUsuario();

        // no sugerir personas con una solicitud pendiente entre ambos
        if (buscarIndiceSolicitud(nombre, nombreCandidato) != -1 ||
            buscarIndiceSolicitud(nombreCandidato, nombre) != -1)
        {
            continue;
        }

        // insercion manual: mas coincidencias primero; empate alfabetico
        int posicion = 0;

        while (posicion < total)
        {
            MedidorRecursos::contarIteracion();
            if (comunes[candidato] > coincidencias[posicion] ||
                (comunes[candidato] == coincidencias[posicion] &&
                 nombreCandidato <
                     usuarios[indices[posicion]]->getNombreUsuario()))
            {
                break;
            }

            posicion++;
        }

        if (posicion < k)
        {
            if (total < k)
            {
                total++;
            }

            for (
                int i = total - 1;
                i > posicion;
                i--
                )
            {
                MedidorRecursos::contarIteracion();
                indices[i] = indices[i - 1];
                coincidencias[i] = coincidencias[i - 1];
            }

            indices[posicion] = candidato;
            coincidencias[posicion] = comunes[candidato];
        }
    }

    delete[] comunes;
    return total;
}

// anadir datos iniciales de prueba, nunca desde el menu del usuario
bool Plataforma::agregarAlbumInicial(const string& dueno, const Album& album)
{
    int indice = buscarIndiceUsuario(dueno);
    if (indice == -1)
    {
        return false;
    }
    return usuarios[indice]->agregarAlbum(album);
}

// comprobar las reglas de privacidad del album
bool Plataforma::puedeVerAlbum(const string& visitante,
                               const string& dueno,
                               int posicionAlbum) const
{
    int indiceVisitante = buscarIndiceUsuario(visitante);
    int indiceDueno = buscarIndiceUsuario(dueno);

    if (indiceVisitante == -1 || indiceDueno == -1)
    {
        return false;
    }

    const Album* album = usuarios[indiceDueno]->obtenerAlbum(posicionAlbum);
    if (album == nullptr)
    {
        return false;
    }
    if (visitante == dueno)
    {
        return true;
    }
    if (album->getPrivacidad() == "publico")
    {
        return true;
    }
    if (album->getPrivacidad() == "amigos")
    {
        return matrizAmistades[indiceVisitante * capacidadUsuarios + indiceDueno];
    }
    return false;
}

bool Plataforma::modificarLikeFoto(const string& visitante,
                                   const string& dueno,
                                   int posicionAlbum, int posicionFoto,
                                   bool darLike)
{
    if (!puedeVerAlbum(visitante, dueno, posicionAlbum))
    {
        return false;
    }
    int indiceDueno = buscarIndiceUsuario(dueno);
    return usuarios[indiceDueno]->modificarLikeFoto(
        posicionAlbum, posicionFoto, darLike);
}

// guardar albumes y fotos, vinculados por nombre del usuario propietario
bool Plataforma::guardarAlbumes(const string& ruta) const
{
    ofstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }

    archivo << "UDEABOOK_ALBUMES_V1" << '\n';
    archivo << cantidadUsuarios << '\n';
    for (
        int i = 0;
        i < cantidadUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        archivo << usuarios[i]->getNombreUsuario() << '\n';
        archivo << usuarios[i]->getCantidadAlbumes() << '\n';

        for (
            int j = 0;
            j < usuarios[i]->getCantidadAlbumes();
            j++
            )
        {
            MedidorRecursos::contarIteracion();
            if (!usuarios[i]->obtenerAlbum(j)->guardarEnArchivo(archivo))
            {
                return false;
            }
        }
    }
    archivo.close();
    return archivo.good();
}

// reconstruir los albumes en copias; solo reemplazar al validar todo el archivo
bool Plataforma::cargarAlbumes(const string& ruta)
{
    ifstream archivo(ruta);
    if (!archivo.is_open())
    {
        return false;
    }

    string encabezado;
    int totalUsuarios;
    if (!getline(archivo, encabezado) ||
        encabezado != "UDEABOOK_ALBUMES_V1" ||
        !(archivo >> totalUsuarios) || totalUsuarios != cantidadUsuarios)
    {
        return false;
    }
    archivo.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!archivo)
    {
        return false;
    }

    Usuario** copias = new Usuario*[cantidadUsuarios]();
    bool correcto = true;

    for (
        int i = 0;
        i < cantidadUsuarios && correcto;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        copias[i] = new Usuario(*usuarios[i]);
        copias[i]->vaciarAlbumes();

        string dueno;
        int cantidad;
        if (!getline(archivo, dueno) ||
            !(archivo >> cantidad) || cantidad < 0 ||
            dueno != usuarios[i]->getNombreUsuario())
        {
            correcto = false;
            break;
        }
        archivo.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!archivo)
        {
            correcto = false;
            break;
        }

        for (
            int j = 0;
            j < cantidad && correcto;
            j++
            )
        {
            MedidorRecursos::contarIteracion();
            Album nuevoAlbum;
            if (!nuevoAlbum.cargarDesdeArchivo(archivo) ||
                !copias[i]->agregarAlbum(nuevoAlbum))
            {
                correcto = false;
            }
        }
    }

    if (correcto)
    {
        for (
            int i = 0;
            i < cantidadUsuarios;
            i++
            )
        {
            MedidorRecursos::contarIteracion();
            delete usuarios[i];
            usuarios[i] = copias[i];
            copias[i] = nullptr;
        }
    }

    for (
        int i = 0;
        i < cantidadUsuarios;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        delete copias[i];
    }
    delete[] copias;
    return correcto;
}
