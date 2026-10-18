#include "medidorrecursos.h"
#include <iostream>
#include <fstream>
#include <string>
#include "usuario.h"
#include "plataforma.h"

using namespace std;

// convertir a entero pequeno sin excepciones ni librerias adicionales
static bool convertirNumeroHasta20(const string& texto, int& numero)
{
    if (texto.empty())
    {
        return false;
    }

    numero = 0;

    for (
        int i = 0;
        i < static_cast<int>(texto.size());
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        if (texto[i] < '0' || texto[i] > '9')
        {
            return false;
        }

        numero = numero * 10 + (texto[i] - '0');

        if (numero > 20)
        {
            return false;
        }
    }

    return true;
}

// convertir un indice decimal sin contenedores ni algoritmos externos
static bool convertirIndice(const string& texto, int maximo, int& numero)
{
    if (texto.empty() || maximo < 0)
    {
        return false;
    }
    numero = 0;
    for (
        int i = 0;
        i < static_cast<int>(texto.size());
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        if (texto[i] < '0' || texto[i] > '9')
        {
            return false;
        }
        int digito = texto[i] - '0';
        if (digito > maximo || numero > (maximo - digito) / 10)
        {
            return false;
        }
        numero = numero * 10 + digito;
    }
    return true;
}

// albumes ficticios para probar el carrusel sin crear imagenes reales
// esta funcion no forma parte del menu de la red social
static void prepararAlbumesDePrueba(Plataforma& plataforma)
{
    if (plataforma.buscarUsuario("laura25") == nullptr)
    {
        return;
    }

    const string base = "/users/udeabook/laura25/albums/";

    Album cartagena("Cartagena2026", base + "Cartagena2026", "publico");
    cartagena.agregarFoto(Foto("2026-01-10", 125000, base + "Cartagena2026/Malecon2612.png", 4));
    cartagena.agregarFoto(Foto("2026-01-11", 135000, base + "Cartagena2026/Bocagrande1.png", 7));
    cartagena.agregarFoto(Foto("2026-01-12", 145000, base + "Cartagena2026/Murallas.png", 1));
    cartagena.agregarFoto(Foto("2026-01-13", 155000, base + "Cartagena2026/Castillo.png", 2));
    cartagena.agregarFoto(Foto("2026-01-14", 165000, base + "Cartagena2026/Atardecer.png", 9));

    Album amigos("Amigos2026", base + "Amigos2026", "amigos");
    amigos.agregarFoto(Foto("2026-05-01", 102000, base + "Amigos2026/Parque.png", 2));
    amigos.agregarFoto(Foto("2026-05-02", 112000, base + "Amigos2026/Cafe.png", 5));
    amigos.agregarFoto(Foto("2026-05-03", 122000, base + "Amigos2026/Universidad.png", 3));
    amigos.agregarFoto(Foto("2026-05-04", 132000, base + "Amigos2026/Concierto.png", 8));
    amigos.agregarFoto(Foto("2026-05-05", 142000, base + "Amigos2026/Caminata.png", 1));

    Album privado("Personal2026", base + "Personal2026", "privado");
    privado.agregarFoto(Foto("2026-06-01", 180000, base + "Personal2026/Privada.png", 0));

    plataforma.agregarAlbumInicial("laura25", cartagena);
    plataforma.agregarAlbumInicial("laura25", amigos);
    plataforma.agregarAlbumInicial("laura25", privado);
}

// mostrar en consola las rutas de las fotos seleccionadas
static void mostrarFotosPerfilAmigos(const Plataforma& plataforma,
                                     const int* indices, int total)
{
    cout << "\nFotos de perfil de amigos" << endl;

    if (total <= 0)
    {
        cout << "Este usuario no tiene amigos para mostrar." << endl;
        return;
    }

    for (
        int i = 0;
        i < total;
        i++
        )
    {
        MedidorRecursos::contarIteracion();
        const Usuario* amigo = plataforma.obtenerUsuario(indices[i]);
        if (amigo != nullptr)
        {
            cout << "Amigo " << i + 1 << ": " << amigo->getNombreUsuario()
            << endl;
            if (amigo->getFotoPerfil().empty())
            {
                cout << "Foto: ruta no registrada en usuarios.txt" << endl;
            }
            else
            {
                cout << "Foto: " << amigo->getFotoPerfil() << endl;
            }
        }
    }

    if (total < 3)
    {
        cout << "Solo hay " << total
             << " amigo(s); no es posible mostrar tres fotos distintas."
             << endl;
    }
}

// memoria de las variables de main que permanecen vivas durante la sesion
// no incluye memoria de plataforma porque ya se suma por separado
static size_t estimarLocalesBase(bool duranteSesion)
{
    size_t bytes = 5 * sizeof(string) + 5 * sizeof(ifstream)
    + 5 * sizeof(bool);
    if (duranteSesion)
    {
        bytes = bytes + 4 * sizeof(string) + 3 * sizeof(bool)
        + 4 * sizeof(int);
    }
    return bytes;
}

// este reporte se toma despues de la operacion y antes del proximo menu
static void mostrarRecursos(const string& funcion, const Plataforma& plataforma,
                            size_t memoriaLocal = 0, bool duranteSesion = true)
{
    MedidorRecursos::detener();
    size_t ram = plataforma.memoriaTotalEstimada()
                 + estimarLocalesBase(duranteSesion) + memoriaLocal
                 + 2 * sizeof(void*) + 2 * sizeof(size_t);
    cout << "\nConsumo de recursos: " << funcion << endl;
    cout << "Iteraciones propias directas e indirectas: "
         << MedidorRecursos::getIteraciones() << endl;
    cout << "Memoria estimada: " << ram << " bytes" << endl;

}

int main()
{
    const string rutaUsuarios = "usuarios.txt";
    const string rutaAmistades = "amistades.txt";
    const string rutaSolicitudes = "solicitudes.txt";
    const string rutaMensajes = "mensajes.txt";
    const string rutaAlbumes = "albumes.txt";

    MedidorRecursos::iniciar();
    Plataforma plataforma;
    ifstream archivoUsuarios(rutaUsuarios);
    bool existenUsuarios = archivoUsuarios.is_open();
    archivoUsuarios.close();

    if (existenUsuarios)
    {
        if (!plataforma.cargarUsuarios(rutaUsuarios))
        {
            cout << "Error al cargar usuarios." << endl;
            return 1;
        }
    }
    else
    {
        // estas cuentas demo solo se crean si no existe usuarios.txt
        plataforma.agregarUsuario(Usuario(
            "juanito76", "2001-01-01", "juanito@ejemplo.com",
            "/users/udeabook/juanito76/albums/profile.png",
            "demo123", "Medellin", "Colombia", "2026-10-05"));
        plataforma.agregarUsuario(Usuario(
            "pedrito89", "2001-02-01", "pedrito@ejemplo.com",
            "/users/udeabook/pedrito89/albums/profile.png",
            "demo123", "Medellin", "Colombia", "2026-10-05"));

        Usuario laura(
            "laura25",
            "2002-06-18",
            "laura@ejemplo.com",
            "/users/udeabook/laura25/albums/profile.png",
            "clave123",
            "Medellin",
            "Colombia",
            "2026-10-05"
            );
        plataforma.agregarUsuario(laura);

        if (!plataforma.guardarUsuarios(rutaUsuarios))
        {
            cout << "Error al crear usuarios.txt." << endl;
            return 1;
        }
    }

    ifstream archivoAmistades(rutaAmistades);
    bool existenAmistades = archivoAmistades.is_open();
    archivoAmistades.close();

    if (existenAmistades)
    {
        if (!plataforma.cargarAmistades(rutaAmistades))
        {
            cout << "Error al cargar amistades." << endl;
            return 1;
        }
    }
    else if (!plataforma.guardarAmistades(rutaAmistades))
    {
        cout << "Error al crear amistades.txt." << endl;
        return 1;
    }

    ifstream archivoSolicitudes(rutaSolicitudes);
    bool existenSolicitudes = archivoSolicitudes.is_open();
    archivoSolicitudes.close();

    if (existenSolicitudes)
    {
        if (!plataforma.cargarSolicitudes(rutaSolicitudes))
        {
            cout << "Error al cargar solicitudes." << endl;
            return 1;
        }
    }
    else if (!plataforma.guardarSolicitudes(rutaSolicitudes))
    {
        cout << "Error al crear solicitudes.txt." << endl;
        return 1;
    }

    // recuperar mensajes guardados, o crear archivo vacio
    ifstream archivoMensajes(rutaMensajes);
    bool existenMensajes = archivoMensajes.is_open();
    archivoMensajes.close();

    if (existenMensajes)
    {
        if (!plataforma.cargarMensajes(rutaMensajes))
        {
            cout << "Error al cargar mensajes." << endl;
            return 1;
        }
    }
    else if (!plataforma.guardarMensajes(rutaMensajes))
    {
        cout << "Error al crear mensajes.txt." << endl;
        return 1;
    }

    // cargar albumes si ya existen; si no, crear datos de prueba una sola vez
    ifstream archivoAlbumes(rutaAlbumes);
    bool existenAlbumes = archivoAlbumes.is_open();
    archivoAlbumes.close();

    if (existenAlbumes)
    {
        if (!plataforma.cargarAlbumes(rutaAlbumes))
        {
            cout << "Error al cargar albumes.txt." << endl;
            return 1;
        }
    }
    else
    {
        prepararAlbumesDePrueba(plataforma);
        if (!plataforma.guardarAlbumes(rutaAlbumes))
        {
            cout << "Error al crear albumes.txt." << endl;
            return 1;
        }
    }

    mostrarRecursos("carga inicial desde archivos", plataforma, 0, false);

    cout << "Ude@Book - Inicio de sesion" << endl;
    cout << "Usuarios cargados: " << plataforma.getCantidadUsuarios() << endl;

    bool terminarPrograma = false;
    while (cin && !terminarPrograma)
    {
        MedidorRecursos::contarIteracion();
        string opcionInicio;
        cout << "\n1. Iniciar sesion" << endl;
        cout << "0. Salir del programa" << endl;
        cout << "Opcion: ";
        if (!getline(cin, opcionInicio))
        {
            break;
        }

        if (opcionInicio == "0")
        {
            break;
        }
        if (opcionInicio != "1")
        {
            cout << "Opcion no valida." << endl;
            continue;
        }

        string usuarioActivo;
        string clave;
        cout << "Nombre de usuario: ";
        if (!getline(cin, usuarioActivo))
        {
            break;
        }
        cout << "Contrasena: ";
        if (!getline(cin, clave))
        {
            break;
        }

        MedidorRecursos::iniciar();
        bool credencialesValidas =
            plataforma.autenticarUsuario(usuarioActivo, clave);
        mostrarRecursos("autenticacion", plataforma);
        if (!credencialesValidas)
        {
            cout << "Credenciales incorrectas o cuenta sin contrasena."
                 << endl;
            continue;
        }
        cout << "\nBienvenido a Ude@Book, " << usuarioActivo << "!" << endl;

        // un grupo inicial y como maximo una actualizacion por sesion
        MedidorRecursos::iniciar();
        int fotosMostradas[3] = {-1, -1, -1};
        int totalFotosMostradas = plataforma.seleccionarFotosAmigos(
            usuarioActivo, fotosMostradas, 3);
        bool fotosActualizadas = false;
        mostrarFotosPerfilAmigos(plataforma, fotosMostradas, totalFotosMostradas);
        mostrarRecursos("seleccion inicial de fotos", plataforma,
                        sizeof(fotosMostradas) + sizeof(totalFotosMostradas)
                            + sizeof(fotosActualizadas));

        string opcion;
        do
        {
            MedidorRecursos::contarIteracion();
            cout << "\nMenu de " << usuarioActivo << endl;
            cout << "1. Ver mi perfil" << endl;
            cout << "2. Enviar solicitud" << endl;
            cout << "3. Ver mis solicitudes recibidas" << endl;
            cout << "4. Aceptar solicitud" << endl;
            cout << "5. Rechazar solicitud" << endl;
            cout << "6. Consultar amistad" << endl;
            cout << "7. Contar mis amigos" << endl;
            cout << "8. Listar usuarios" << endl;
            cout << "9. Enviar mensaje" << endl;
            cout << "10. Leer mis mensajes nuevos" << endl;
            cout << "11. Buscar mis mensajes recibidos por fechas" << endl;
            cout << "12. Ver mis mensajes enviados" << endl;
            cout << "13. Ver mis sugerencias de amistad" << endl;
            cout << "14. Visitar albumes de un amigo" << endl;
            cout << "15. Cerrar sesion" << endl;
            cout << "16. Cambiar fotos de perfil (una vez)" << endl;
            cout << "0. Salir del programa" << endl;
            cout << "Opcion: ";
            if (!getline(cin, opcion))
            {
                terminarPrograma = true;
                break;
            }

            MedidorRecursos::iniciar();
            if (opcion == "1")
            {
                const Usuario* perfil = plataforma.buscarUsuario(usuarioActivo);
                if (perfil != nullptr)
                {
                    cout << "Nombre: " << perfil->getNombreUsuario() << endl;
                    cout << "Correo: " << perfil->getCorreo() << endl;
                    cout << "Ciudad: " << perfil->getCiudad() << endl;
                    cout << "Pais: " << perfil->getPais() << endl;
                    cout << "Foto de perfil: " << perfil->getFotoPerfil() << endl;
                    cout << "Amigos: " << plataforma.contarAmigos(usuarioActivo)
                         << endl;
                }
            }
            else if (opcion == "2")
            {
                string receptor;
                cout << "Quien recibe: ";
                getline(cin, receptor);

                if (plataforma.enviarSolicitud(usuarioActivo, receptor))
                {
                    if (plataforma.guardarSolicitudes(rutaSolicitudes))
                    {
                        cout << "Solicitud enviada y guardada." << endl;
                    }
                    else
                    {
                        cout << "Solicitud creada en RAM, pero no se guardo." << endl;
                    }
                }
                else
                {
                    cout << "No se puede enviar: revisa usuarios, amistad o solicitudes pendientes." << endl;
                }
            }
            else if (opcion == "3")
            {
                int total = plataforma.contarSolicitudesRecibidas(usuarioActivo);

                if (total == -1)
                {
                    cout << "Ese usuario no existe." << endl;
                }
                else if (total == 0)
                {
                    cout << "No tiene solicitudes pendientes." << endl;
                }
                else
                {
                    cout << "Solicitudes pendientes: " << total << endl;
                    for (
                        int i = 0;
                        i < total;
                        i++
                        )
                    {
                        MedidorRecursos::contarIteracion();
                        const SolicitudAmistad* solicitud =
                            plataforma.obtenerSolicitudRecibida(usuarioActivo, i);
                        if (solicitud != nullptr)
                        {
                            cout << i << ". De " << solicitud->getEmisor() << endl;
                        }
                    }
                }
            }
            else if (opcion == "4" || opcion == "5")
            {
                string emisor;
                cout << "Usuario que la envio: ";
                getline(cin, emisor);

                bool realizada = false;
                if (opcion == "4")
                {
                    realizada = plataforma.aceptarSolicitud(usuarioActivo, emisor);
                }
                else
                {
                    realizada = plataforma.rechazarSolicitud(usuarioActivo, emisor);
                }

                if (!realizada)
                {
                    cout << "No existe esa solicitud pendiente." << endl;
                }
                else
                {
                    bool guardada = plataforma.guardarSolicitudes(rutaSolicitudes);
                    if (opcion == "4")
                    {
                        guardada = plataforma.guardarAmistades(rutaAmistades) && guardada;
                    }

                    if (guardada)
                    {
                        cout << (opcion == "4" ? "Solicitud aceptada. Ahora son amigos."
                                               : "Solicitud rechazada.") << endl;
                    }
                    else
                    {
                        cout << "Cambio realizado en RAM, pero fallo al guardar." << endl;
                    }
                }
            }
            else if (opcion == "6")
            {
                string segundo;
                cout << "Consultar amistad con: ";
                getline(cin, segundo);

                if (plataforma.buscarUsuario(usuarioActivo) == nullptr ||
                    plataforma.buscarUsuario(segundo) == nullptr)
                {
                    cout << "Uno de los usuarios no existe." << endl;
                }
                else
                {
                    cout << (plataforma.sonAmigos(usuarioActivo, segundo) ?
                                 "Si son amigos." : "No son amigos.") << endl;
                }
            }
            else if (opcion == "7")
            {
                int total = plataforma.contarAmigos(usuarioActivo);
                if (total == -1)
                {
                    cout << "Ese usuario no existe." << endl;
                }
                else
                {
                    cout << "Cantidad de amigos: " << total << endl;
                }
            }
            else if (opcion == "8")
            {
                cout << "Usuarios registrados:" << endl;
                for (
                    int i = 0;
                    i < plataforma.getCantidadUsuarios();
                    i++
                    )
                {
                    MedidorRecursos::contarIteracion();
                    const Usuario* usuario = plataforma.obtenerUsuario(i);
                    cout << i << ". " << usuario->getNombreUsuario() << endl;
                }
            }
            else if (opcion == "9")
            {
                string receptor;
                string texto;
                cout << "Quien recibe: ";
                getline(cin, receptor);
                cout << "Mensaje (hasta 512 caracteres): ";
                getline(cin, texto);

                if (plataforma.enviarMensaje(usuarioActivo, receptor, texto))
                {
                    if (plataforma.guardarMensajes(rutaMensajes))
                    {
                        cout << "Mensaje enviado y guardado." << endl;
                    }
                    else
                    {
                        cout << "Mensaje creado en RAM, pero fallo al guardar." << endl;
                    }
                }
                else
                {
                    cout << "No se pudo enviar: deben ser amigos y el texto no puede"
                         << " estar vacio ni superar 512 caracteres." << endl;
                }
            }
            else if (opcion == "10")
            {
                if (plataforma.buscarUsuario(usuarioActivo) == nullptr)
                {
                    cout << "Ese usuario no existe." << endl;
                }
                else
                {
                    int total = 0;
                    for (
                        int i = 0;
                        i < plataforma.getCantidadMensajes();
                        i++
                        )
                    {
                        MedidorRecursos::contarIteracion();
                        const Mensaje* mensaje = plataforma.obtenerMensaje(i);
                        if (mensaje->getReceptor() == usuarioActivo &&
                            !mensaje->estaLeido())
                        {
                            cout << "De: " << mensaje->getEmisor() << endl;
                            cout << "Fecha: " << mensaje->getFechaEnvio() << endl;
                            cout << "Texto: " << mensaje->getTexto() << endl;
                            cout << "--------------------" << endl;
                            total++;
                        }
                    }

                    if (total == 0)
                    {
                        cout << "No hay mensajes nuevos." << endl;
                    }
                    else
                    {
                        plataforma.marcarMensajesLeidos(usuarioActivo);
                        if (plataforma.guardarMensajes(rutaMensajes))
                        {
                            cout << "Mensajes leidos y guardados: " << total << endl;
                        }
                        else
                        {
                            cout << "Se marcaron como leidos en RAM, pero fallo"
                                 << " el guardado." << endl;
                        }
                    }
                }
            }
            else if (opcion == "11")
            {
                string inicio;
                string fin;
                cout << "Fecha inicio (AAAA-MM-DD): ";
                getline(cin, inicio);
                cout << "Fecha fin (AAAA-MM-DD): ";
                getline(cin, fin);

                if (plataforma.buscarUsuario(usuarioActivo) == nullptr)
                {
                    cout << "Ese usuario no existe." << endl;
                }
                else if (!Mensaje::fechaValida(inicio) ||
                         !Mensaje::fechaValida(fin) || inicio > fin)
                {
                    cout << "Rango de fechas no valido." << endl;
                }
                else
                {
                    int total = 0;
                    for (
                        int i = 0;
                        i < plataforma.getCantidadMensajes();
                        i++
                        )
                    {
                        MedidorRecursos::contarIteracion();
                        const Mensaje* mensaje = plataforma.obtenerMensaje(i);
                        string fecha = mensaje->getFechaEnvio().substr(0, 10);

                        if (mensaje->getReceptor() == usuarioActivo &&
                            fecha >= inicio && fecha <= fin)
                        {
                            cout << "De: " << mensaje->getEmisor() << endl;
                            cout << "Fecha: " << mensaje->getFechaEnvio() << endl;
                            cout << "Texto: " << mensaje->getTexto() << endl;
                            cout << "Estado: "
                                 << (mensaje->estaLeido() ? "Leido" : "No leido")
                                 << endl;
                            cout << "--------------------" << endl;
                            total++;
                        }
                    }
                    cout << "Mensajes encontrados: " << total << endl;
                }
            }
            else if (opcion == "12")
            {
                if (plataforma.buscarUsuario(usuarioActivo) == nullptr)
                {
                    cout << "Ese usuario no existe." << endl;
                }
                else
                {
                    int total = 0;
                    for (
                        int i = 0;
                        i < plataforma.getCantidadMensajes();
                        i++
                        )
                    {
                        MedidorRecursos::contarIteracion();
                        const Mensaje* mensaje = plataforma.obtenerMensaje(i);
                        if (mensaje->getEmisor() == usuarioActivo)
                        {
                            cout << "Para: " << mensaje->getReceptor() << endl;
                            cout << "Fecha: " << mensaje->getFechaEnvio() << endl;
                            cout << "Texto: " << mensaje->getTexto() << endl;
                            cout << "Estado: "
                                 << (mensaje->estaLeido() ? "Leido" : "No leido")
                                 << endl;
                            cout << "--------------------" << endl;
                            total++;
                        }
                    }
                    cout << "Mensajes enviados: " << total << endl;
                }
            }
            else if (opcion == "13")
            {
                string entradaK;
                int k = 0;

                cout << "Cuantas sugerencias desea (1 a 20): ";
                getline(cin, entradaK);

                if (!convertirNumeroHasta20(entradaK, k) || k < 1)
                {
                    cout << "K debe ser un entero de 1 a 20." << endl;
                }
                else if (plataforma.buscarUsuario(usuarioActivo) == nullptr)
                {
                    cout << "Ese usuario no existe." << endl;
                }
                else
                {
                    // dos arreglos dinamicos para resultados y coincidencias
                    int* indices = new int[k];
                    int* coincidencias = new int[k];

                    int total = plataforma.generarSugerencias(
                        usuarioActivo, k, indices, coincidencias);

                    if (total <= 0)
                    {
                        cout << "No hay sugerencias disponibles." << endl;
                    }
                    else
                    {
                        cout << "Sugerencias encontradas: " << total << endl;

                        for (
                            int i = 0;
                            i < total;
                            i++
                            )
                        {
                            MedidorRecursos::contarIteracion();
                            const Usuario* sugerido =
                                plataforma.obtenerUsuario(indices[i]);

                            cout << i << ". " << sugerido->getNombreUsuario()
                                 << " - amigos en comun: "
                                 << coincidencias[i] << endl;
                        }

                        string respuesta;
                        cout << "Desea enviar solicitud a un sugerido (s/n): ";
                        getline(cin, respuesta);

                        if (respuesta == "s" || respuesta == "S")
                        {
                            string entradaPosicion;
                            int posicion = 0;

                            cout << "Numero del sugerido: ";
                            getline(cin, entradaPosicion);

                            if (!convertirNumeroHasta20(entradaPosicion, posicion) ||
                                posicion >= total)
                            {
                                cout << "Posicion no valida." << endl;
                            }
                            else
                            {
                                const Usuario* sugerido =
                                    plataforma.obtenerUsuario(indices[posicion]);

                                if (plataforma.enviarSolicitud(
                                        usuarioActivo, sugerido->getNombreUsuario()))
                                {
                                    if (plataforma.guardarSolicitudes(rutaSolicitudes))
                                    {
                                        cout << "Solicitud enviada y guardada." << endl;
                                    }
                                    else
                                    {
                                        cout << "Solicitud creada en RAM, pero no se guardo."
                                             << endl;
                                    }
                                }
                                else
                                {
                                    cout << "No se pudo enviar la solicitud." << endl;
                                }
                            }
                        }
                    }

                    delete[] indices;
                    delete[] coincidencias;
                }
            }
            else if (opcion == "14")
            {
                string dueno;
                cout << "Nombre del amigo: ";
                getline(cin, dueno);

                // esta opcion de menu es especificamente para visitar a un amigo
                if (plataforma.buscarUsuario(usuarioActivo) == nullptr ||
                    plataforma.buscarUsuario(dueno) == nullptr ||
                    (usuarioActivo != dueno && !plataforma.sonAmigos(usuarioActivo, dueno)))
                {
                    cout << "Solo puede visitar su perfil o el de un amigo registrado."
                         << endl;
                }
                else
                {
                    const Usuario* amigo = plataforma.buscarUsuario(dueno);
                    int visibles = 0;

                    cout << "Albumes visibles de " << dueno << ":" << endl;
                    for (
                        int i = 0;
                        i < amigo->getCantidadAlbumes();
                        i++
                        )
                    {
                        MedidorRecursos::contarIteracion();
                        if (plataforma.puedeVerAlbum(usuarioActivo, dueno, i))
                        {
                            const Album* album = amigo->obtenerAlbum(i);
                            cout << i << ". " << album->getNombre()
                                 << " [" << album->getPrivacidad() << "]"
                                 << " - Fotos: " << album->getCantidadFotos() << endl;
                            visibles++;
                        }
                    }

                    if (visibles == 0)
                    {
                        cout << "No hay albumes visibles." << endl;
                    }
                    else
                    {
                        string seleccion;
                        cout << "Numero de album (o x para regresar): ";
                        getline(cin, seleccion);

                        if (seleccion != "x" && seleccion != "X")
                        {
                            int indiceAlbum;
                            if (!convertirIndice(seleccion,
                                                 amigo->getCantidadAlbumes() - 1, indiceAlbum) ||
                                !plataforma.puedeVerAlbum(usuarioActivo, dueno, indiceAlbum))
                            {
                                cout << "Album no valido o privado." << endl;
                            }
                            else
                            {
                                const Album* album = amigo->obtenerAlbum(indiceAlbum);
                                int totalFotos = album->getCantidadFotos();
                                if (totalFotos == 0)
                                {
                                    cout << "Este album no tiene fotos." << endl;
                                }
                                else
                                {
                                    int posicion = 0;
                                    string accion;
                                    do
                                    {
                                        MedidorRecursos::contarIteracion();
                                        // consultar nuevamente por indice tras cada like
                                        album = amigo->obtenerAlbum(indiceAlbum);
                                        const Foto* foto = album->obtenerFoto(posicion);

                                        cout << "\nAlbum: " << album->getNombre()
                                             << " | Foto " << posicion + 1
                                             << " de " << totalFotos << endl;
                                        cout << "Ruta: " << foto->getRuta() << endl;
                                        cout << "Captura: " << foto->getFechaCaptura()
                                             << endl;
                                        cout << "Tamano: " << foto->getTamanoBytes()
                                             << " bytes" << endl;
                                        cout << "Likes: " << foto->getLikes() << endl;

                                        cout << "1. Siguiente  2. Anterior  "
                                             << "3. Dar like  4. Quitar like  "
                                             << "0. Salir del album" << endl;
                                        cout << "Opcion: ";
                                        getline(cin, accion);

                                        if (accion == "1")
                                        {
                                            posicion = (posicion + 1) % totalFotos;
                                        }
                                        else if (accion == "2")
                                        {
                                            posicion = (posicion + totalFotos - 1)
                                            % totalFotos;
                                        }
                                        else if (accion == "3" || accion == "4")
                                        {
                                            bool darLike = accion == "3";
                                            if (plataforma.modificarLikeFoto(
                                                    usuarioActivo, dueno, indiceAlbum,
                                                    posicion, darLike))
                                            {
                                                if (plataforma.guardarAlbumes(rutaAlbumes))
                                                {
                                                    cout << "Likes actualizados y guardados."
                                                         << endl;
                                                }
                                                else
                                                {
                                                    cout << "Actualizados en RAM, error al guardar."
                                                         << endl;
                                                }
                                            }
                                            else
                                            {
                                                cout << "No se pudo modificar el like."
                                                     << endl;
                                            }
                                        }
                                        else if (accion != "0")
                                        {
                                            cout << "Opcion no valida." << endl;
                                        }
                                    }
                                    while (accion != "0" && cin);
                                }
                            }
                        }
                    }
                }
            }
            else if (opcion == "16")
            {
                if (fotosActualizadas)
                {
                    cout << "Ya uso la actualizacion de fotos en esta sesion."
                         << endl;
                }
                else if (plataforma.contarAmigos(usuarioActivo) <= totalFotosMostradas)
                {
                    cout << "No hay amigos adicionales para formar otro grupo."
                         << endl;
                    cout << "Se necesita al menos un amigo distinto a los ya mostrados."
                         << endl;
                }
                else
                {
                    int nuevasFotos[3] = {-1, -1, -1};
                    int totalNuevas = plataforma.seleccionarFotosAmigos(
                        usuarioActivo, nuevasFotos, 3,
                        fotosMostradas, totalFotosMostradas);

                    if (totalNuevas > 0)
                    {
                        for (
                            int i = 0;
                            i < totalNuevas;
                            i++
                            )
                        {
                            MedidorRecursos::contarIteracion();
                            fotosMostradas[i] = nuevasFotos[i];
                        }
                        totalFotosMostradas = totalNuevas;
                        fotosActualizadas = true;
                        mostrarFotosPerfilAmigos(plataforma,
                                                 fotosMostradas, totalFotosMostradas);
                        cout << "Actualizacion usada; no se puede repetir en esta sesion."
                             << endl;
                    }
                    else
                    {
                        cout << "No fue posible actualizar las fotos." << endl;
                    }
                }
            }
            else if (opcion != "0" && opcion != "15")
            {
                cout << "Opcion no valida." << endl;
            }

            if (opcion != "0" && opcion != "15")
            {
                mostrarRecursos("opcion " + opcion, plataforma);
            }
        }
        while (cin && opcion != "0" && opcion != "15");

        if (opcion == "0")
        {
            terminarPrograma = true;
        }
        else if (opcion == "15")
        {
            cout << "Sesion cerrada." << endl;
        }
    }

    cout << "Ude@Book finalizado." << endl;
    return 0;
}
