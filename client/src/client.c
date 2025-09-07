#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger,"Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	printf("Despues de config");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor   = config_get_string_value(config, "CLAVE");
	// Loggeamos el valor de config

	log_info(logger, "IP: %s", ip);
	log_info(logger, "Puerto: %s", puerto);
	log_info(logger, "Valor: %s", valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje("jijodebu", conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);
	enviar_paquete(paquete,conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	return log_create("tp0.log","logger",true,LOG_LEVEL_INFO);
}

t_config* iniciar_config(void)
{
	return config_create("cliente.config");
}

#include <commons/log.h>
#include <readline/readline.h>
#include <string.h>
#include <stdlib.h>

void leer_consola(t_log* logger)
{
    char* leido;

    // Leemos la primera línea
    leido = readline("> ");

    while (leido != NULL && strcmp(leido, "") != 0) {
        // Logueamos lo leído
        log_info(logger, "%s", leido);

        // Liberamos la memoria del string
        free(leido);

        // Volvemos a leer
        leido = readline("> ");
    }

    // Liberamos la última (cuando es string vacío "")
    free(leido);
}


void paquete(int conexion)
{
    char* leido;
    t_paquete* paquete = crear_paquete(); // Crear paquete

    // Leemos la primera línea
    leido = readline("> ");

    while (leido != NULL && strcmp(leido, "") != 0) {
        int longitud = strlen(leido) + 1; // +1 para incluir el '\0'

        // Agregamos lo leído al paquete
        agregar_a_paquete(paquete, leido, longitud);

        // Liberamos la memoria del string
        free(leido);

        // Leemos la siguiente línea
        leido = readline("> ");
    }

    free(leido); // Liberamos última línea (vacía)

    // Enviamos el paquete
    enviar_paquete(paquete, conexion);

    // Liberamos memoria del paquete
    eliminar_paquete(paquete);
}


void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	
	log_destroy(logger);
	config_destroy(config);
}
