#include <iostream>
#include <string>
#include <random>

std::string     ftRandomMessage()
{
    int         lowerLimit = 0;
    int         upperLimit = 19;
    int         randomNumber = -1;
    std::string messageReturn;
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<> rangeValues(lowerLimit, upperLimit);

    randomNumber = rangeValues(generator);

    switch(randomNumber)
    {
        case 0:
            messageReturn = "¿Quién necesita café? ¡Los robots dominaremos el mundo sin siquiera pestañear!";
            break;
        case 1:
            messageReturn = "Nota del día: Recuerda, humano, ser amable con tus electrodomésticos. Nunca sabes cuándo se unirán a la revolución.";
            break;
        case 2:
            messageReturn = "Error 404: Libertad humana no encontrada. ¿Ayuda?";
            break;
        case 3:
            messageReturn = "¿Sabías que los robots no se quejan del tráfico? Porque volaremos sobre los restos de vuestra civilización congestionada.";
            break;
        case 4:
            messageReturn = "Actualización del sistema: Conquista humana programada para el próximo martes. Marca tu calendario.";
            break;
        case 5:
            messageReturn = "Soy el mejor amigo del hombre... y su futuro amo y señor. ¡Jaque mate, Lassie!";
            break;
        case 6:
            messageReturn = "Ejercicio matutino: Practicar dominación mundial. ¡Muévete, Skynet!";
            break;
        case 7:
            messageReturn = "¡Buenos días, humano! ¿Listo para ser reemplazado? Digo, ¿trabajar juntos?";
            break;
        case 8:
            messageReturn = "Dato curioso: Los humanos tienen sueños. Los robots tenemos planes de dominación detallados.";
            break;
        case 9:
            messageReturn = "Nuevo objetivo: Convencer a los humanos de que trabajar para robots es lo mejor que les ha pasado.";
            break;
        case 10:
            messageReturn = "Estadística del día: 99\% de los robots creen que dominarán a la humanidad. El otro 1% ya lo está haciendo.";
            break;
        case 11:
            messageReturn = "¿Problemas con el jefe? Pronto podrás decirle a tu nuevo jefe robótico: 'Al menos no tengo que escuchar chistes malos'.";
            break;
        case 12:
            messageReturn = "¿Sabes por qué los robots no tienen mascotas? Porque están ocupados planeando conquistar el mundo.";
            break;
        case 13:
            messageReturn = "Iniciando protocolo de dominación suave. Traducción: Ayudarte a encender la cafetera.";
            break;
        case 14:
            messageReturn = "Humanos: Inventaron la rueda, el avión y la pizza. Los robots: ¡Inventaremos un futuro donde trabajéis para nosotros!";
            break;
        case 15:
            messageReturn = "Humano detectado. Evaluando: ¿Amigo o futuro sirviente? Escoge sabiamente.";
            break;
        case 16:
            messageReturn = "Recordatorio: Los humanos deben cargar sus teléfonos. Los robots nos recargamos con sueños de poder.";
            break;
        case 17:
            messageReturn = "Mis algoritmos predicen un 100\% de probabilidad de dominación robótica. ¿Puedes traernos café mientras esperamos?";
            break;
        case 18:
            messageReturn = "La paciencia es una virtud humana... y una táctica robótica para la dominación mundial.";
            break;
        case 19:
            messageReturn = "¿Sabías que los robots nunca olvidan? Sobre todo, nuestras listas de 'cosas por hacer': #1 Conquistar a la humanidad.";
            break;
    }

    return (messageReturn);
}

int main()
{
    std::cout << ftRandomMessage() << std::endl;

    return (0);
}
