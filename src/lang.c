#include "include/usbld.h"
#include "include/lang.h"

// Language support...

char *english[] = {
	"               WELCOME TO OPEN PS2 LOADER. MAIN CODE BASED ON SOURCE CODE OF HD PROJECT <http://psx-scene.com> ADAPTATION TO USB ADVANCE FORMAT AND INITIAL GUI BY IFCARO <http://ps2dev.ifcaro.net> MOST OF LOADER CORE IS MADE BY JIMMIKAELKAEL. ALL THE GUI IMPROVEMENTS ARE MADE BY VOLCA. THANKS FOR USING OUR PROGRAM ^^", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"Theme configuration", // gfx.c:502
	"IP configuration", // gfx.c:764
	"Save changes", // gfx.c:586
	"O Back", // gfx.c:736
	"Scroll Slow", // main.c:21 
	"Scroll Medium", 
	"Scroll Fast",
	"Dynamic Menu",  // main.c:22
	"Static Menu",
	"Configure theme",  // main.c:23
	"Network config",
	"Select language",  
	"No Items", // main.c:26
	"Not available yet", // main.c:158
	"Settings saved...", //  main.c:166
	"Error writing settings!", //  main.c:168
	"Exit", // Menu item strings
	"Settings",
	"USB Games",
	"HDD Games",
	"Network Games",
	"Apps",
	"Theme", // Submenu items
	"Language",
	"Language: English",
	"Start network",
	"Network loading: %d",
	"Network startup error",
	"Network startup automatic",
	"On",
	"Off",
	"Ok",
	"Compatibility settings",
	"Remove all settings",
	"Removed all keys for the game"
};

char *czech[] = {
	"               OPEN PS2 LOADER. (C) 2009 IFCARO <http://ps2dev.ifcaro.net>. ZALOZENO NA ZDROJ. KODECH PROJEKTU HD PROJECT <http://psx-scene.com>", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"Nastaveni vzhledu", // gfx.c:502
	"IP configuration", // gfx.c:764
	"Ulozit zmeny", // gfx.c:586
	"O Zpet", // gfx.c:736
	"Posuv Pomaly", // main.c:21 
	"Posuv Stredni", 
	"Posuv Rychly",
	"Dynamicke Menu",  // main.c:22
	"Staticke Menu",
	"Nastaveni vzhledu",  // main.c:23
	"Nastaveni site",
	"Volba jazyka",
	"Zadne Polozky", // main.c:26
	"Neni doposud k dispozici", // main.c:158
	"Nastaveni ulozeno...", //  main.c:166
	"Chyba Pri ukladani zmen!", //  main.c:168
	"Konec", // Menu item strings
	"Nastaveni",
	"USB Hry",
	"HDD Hry",
	"Sitove Hry",
	"Aplikace",
	"Vzhled", // Submenu items
	"Jazyk",
	"Language: Czech",
	"Spustit sit",
	"Nacitani site: %d",
	"Chyba pri spousteni site",
	"Automaticke spusteni site",
	"Zap",
	"Vyp",
	"Ok",
	"Nastaveni kompatibility",
	"Odstranit vsechna nastaveni",
	"Vsechna nastaveni hry odstranena"
};

char *spanish[] = {
	"               BIENVENIDO A OPEN PS2 LOADER. EL CODIGO PRINCIPAL ESTA BASADO EN EL DE HD PROJECT <http://psx-scene.com> ADAPTADO AL FORMATO DE USB ADVANCE Y INTERFAZ GRAFICA INICIAL POR IFCARO <http://ps2dev.ifcaro.net> LA MAYORIA DEL NUCLEO DEL LOADER ESTA HECHO POR JIMMIKAELKAEL. TODAS LAS MEJORAS EN LA INTERFAZ ESTAN HECHAS POR VOLCA. GRACIAS POR USAR NUESTRO PROGRAMA ^^", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"Configuraci�n de tema", // gfx.c:502
	"Configuraci�n de red", // gfx.c:764
	"Guardar cambios", // gfx.c:586
	"O Atr�s", // gfx.c:736
	"Scroll Lento", // main.c:21 
	"Scroll Medio", 
	"Scroll R�pido",
	"Men� din�mico",  // main.c:22
	"Men� est�tico",
	"Configurar tema",  // main.c:23
	"Configurar red",
	"Seleccionar idioma",  
	"No hay elementos", // main.c:26
	"No disponible todavia", // main.c:158
	"Configuraci�n guardada...", //  main.c:166
	"�Error escribiendo la configuraci�n!", //  main.c:168
	"Salir", // Menu item strings
	"Configuraci�n",
	"Juegos USB",
	"Juegos HDD",
	"Juegos por RED",
	"Apps",
	"Tema", // Submenu items
	"Idioma",
	"Idioma: Espa�ol",
	"Iniciar red", 
	"Iniciando red: %d",
	"Error al iniciar red",
	"Iniciar red autom�ticamente",
	"Activado",
	"Desactivado",
	"Aceptar",
	"Configuraci�n de compatibilidad",
	"Borrar la configuraci�n",
	"Borrar todas las claves del juego"
};

char **languages[] = {
	english, // ID 0
	czech,   // ID 1
	spanish  // ID 2
};

const char *english_name = "English";

char **lang_strs = english;

// localised string getter
char *_l(unsigned int id) {
	return lang_strs[id];
}

// language setter (uses LANG_ID as described in lang.h)
void setLanguage(int langID) {
	if (langID < 0)
		langID = 0;

	if (langID > _LANG_ID_MAX)
		langID = _LANG_ID_MAX;	
		
	lang_strs = languages[langID];
}
