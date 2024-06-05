#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Error: bad arguments\nUsage: ./ircserver <port> <password>" << std::endl;
        exit(1);
    }
    //checkear que argv[1] sea numero
    //checkear que argv[1] sea un puerto disponible, de entre 1024 al 65535, ya que estos no estan reservados por el sistema

    //Rellenar la clase Server con los argumentos
    //Iniciar el servidor -> socket(), socketaddr_in, bind(), listen(), etc.
    //Conectar clientes -> poll() y el contenedor donde se almacenan los fds
        //Aqui se haria la escucha "infinita" para ver si se ha añadido algun cliente nuevo o si hay que procesar algun mensaje
            //quizas es aqui donde se tendria que mandar bien la refe del server para tener la lista de usuarios o la lsita de fds
 
}