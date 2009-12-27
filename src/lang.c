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
	"Removed all keys for the game",
	"Scrolling", // _STR_SCROLLING
	"Menu type", // _STR_MENUTYPE
	"Slow", // _STR_SLOW
	"Medium", // _STR_MEDIUM
	"Fast", // _STR_FAST
	"Static", // _STR_STATIC
	"Dynamic", // _STR_DYNAMIC
	"Default menu", // _STR_DEF_DEVICE
};

char *czech[] = {
	"               VITEJTE V PROGRAMU OPEN PS2 LOADER. ZALOZENO NA ZDROJ. KODECH PROJEKTU HD PROJECT <http://psx-scene.com> ADAPTACE NA USB ADVANCE FORMAT A PRVOTNI GUI STVORIL IFCARO <http://ps2dev.ifcaro.net> VETSINU KODU LOADERU SEPSAL JIMMIKAELKAEL. PRIDAVNE PRACE NA GUI PROVEDL VOLCA. DEKUJEME ZE POUZIVATE TENTO PROGRAM ^^ ", // gfx.c:85
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
	"Vsechna nastaveni hry odstranena",
	"Posuv", // _STR_SCROLLING
	"Typ menu", // _STR_MENUTYPE
	"Pomalu", // _STR_SLOW
	"Stredne", // _STR_MEDIUM
	"Rychle", // _STR_FAST
	"Staticke", // _STR_STATIC
	"Dynamicke", // _STR_DYNAMIC
	"Vychozi nabidka", // _STR_DEF_DEVICE
};

char *spanish[] = {
	"               BIENVENIDO A OPEN PS2 LOADER. EL CODIGO PRINCIPAL ESTA BASADO EN EL DE HD PROJECT <http://psx-scene.com> ADAPTADO AL FORMATO DE USB ADVANCE Y INTERFAZ GRAFICA INICIAL POR IFCARO <http://ps2dev.ifcaro.net> LA MAYOR PARTE DEL NUCLEO DEL CARGADOR ES REALIZADA POR JIMMIKAELKAEL. TODAS LAS MEJORAS EN LA INTERFAZ SON REALIZADAS POR VOLCA. GRACIAS POR USAR NUESTRO PROGRAMA ^^", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"Configuraci�n de tema", // gfx.c:502
	"Configuraci�n de red", // gfx.c:764
	"Guardar cambios", // gfx.c:586
	"O Atr�s", // gfx.c:736
	"Desplazamiento Lento", // main.c:21 
	"Desplazamiento Medio", 
	"Desplazamiento R�pido",
	"Men� din�mico",  // main.c:22
	"Men� est�tico",
	"Configurar tema",  // main.c:23
	"Configurar red",
	"Seleccionar idioma",  
	"No hay elementos", // main.c:26
	"A�n no disponible", // main.c:158
	"Configuraci�n guardada...", //  main.c:166
	"Error al escribir la configuraci�n!", //  main.c:168
	"Salir", // Menu item strings
	"Configuraci�n",
	"Juegos por USB",
	"Juegos por HDD",
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
	"Preferencias de compatibilidad",
	"Borrar la configuraci�n",
	"Borrar todas las claves del juego",
	"Desplazamiento", // _STR_SCROLLING
	"Tipo de men�", // _STR_MENUTYPE
	"Lento", // _STR_SLOW
	"Medio", // _STR_MEDIUM
	"R�pido", // _STR_FAST
	"Est�tico", // _STR_STATIC
	"Din�mico", // _STR_DYNAMIC
	"Default menu", // _STR_DEF_DEVICE
};

char *french[] = {
	"              BIENVENUE DANS OPEN PS2 LOADER. CODE PRINCIPAL FOND� SUR LE CODE SOIRCE D'HD PROJECT <http://psx-scene.com> ADAPTATION AU FORMAT USB ADVANCE ET GUI INITIAL PAR IFCARO <http://ps2dev.ifcaro.net> LA PLUPART DU CODE DU C�UR DU LOADER EST DE JIMMIKAELKAEL. TOUTES LES AMELIORATIONS DU GUI SONT DE VOLCA. MERCI D'UTILISER NOTRE PROGRAMME ^^", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"R�glage du th�me", // gfx.c:502
	"R�glages IP", // gfx.c:764
	"Sauvegarder les changements", // gfx.c:586
	"O retour", // gfx.c:736
	"D�filement lent", // main.c:21 
	"D�filement moyen", 
	"D�filement rapide",
	"Menu dynamique",  // main.c:22
	"Menu statique",
	"Configurer le th�me",  // main.c:23
	"R�glages r�seau",
	"Choisir la langue",  
	"Vide", // main.c:26
	"Pas encore disponible", // main.c:158
	"R�glages sauvegard�s...", //  main.c:166
	"Erreur � l'�criture des r�glages!", //  main.c:168
	"Sortir", // Menu item strings
	"R�glages",
	"Jeux sur USB",
	"Jeux sur disque dur",
	"Jeux via r�seau",
	"Applications",
	"Theme", // Submenu items
	"Langue",
	"Langue : fran�ais",
	"D�marrer support r�seau",
	"Chargement r�seau: %d",
	"Erreur de d�marrage r�seau",
	"D�marrage r�seau automatique",
	"On",
	"Off",
	"Ok",
	"R�glages de compatibilit�",
	"D�sactiver tous les r�glages",
	"Tous modes d�sactiv�s",
	"D�filement", // _STR_SCROLLING
	"Type de menu", // _STR_MENUTYPE
	"Lent", // _STR_SLOW
	"Moyen", // _STR_MEDIUM
	"Rapide", // _STR_FAST
	"Statique", // _STR_STATIC
	"Dynamique", // _STR_DYNAMIC
	"Default menu", // _STR_DEF_DEVICE	
};

char *german[] = {
	"               WILLKOMMEN BEIM OPEN PS2 LOADER. DAS HAUPTPROGRAMM BASIERT AUF DEM QUELLCODE DES HD PROJEKTS <http://psx-scene.com> PORTIERUNG IN DAS USB ADVANCE FORMAT UND STANDARD GUI BY IFCARO <http://ps2dev.ifcaro.net> DAS MEISTE DES LOADER PROGRAMMS IST VON JIMMIKAELKAEL PROGRAMMIERT. ALLE GUI ERWEITERUNGEN WURDEN VON VOLKA ERSTELLT. DANKE F�R DAS BENUTZEN UNSERES PROGRAMMS ^^", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"Theme Einstellungen", // gfx.c:502
	"IP Einstellungen", // gfx.c:764
	"Anderungen speichern", // gfx.c:586
	"O Zur�ck", // gfx.c:736
	"Langsam scrollen", // main.c:21 
	"Normal scrollen", 
	"Schnell scrollen",
	"Dynamisches Men�",  // main.c:22
	"Statisches Men�",
	"Theme einstellen",  // main.c:23
	"Netzwerk Einstellungen",
	"Sprache ausw�hlen",  
	"Keine Items", // main.c:26
	"Derzeit nicht verf�gbar", // main.c:158
	"Einstellungen gespeichert...", //  main.c:166
	"Fehler beim speichern der Einstellungen!", //  main.c:168
	"Ausgang", // Menu item strings
	"Einstellungen",
	"USB Spiele",
	"HDD Spiele",
	"Netzwerk Spiele",
	"Programme",
	"Theme", // Submenu items
	"Sprache",
	"Sprache: Deutsch",
	"Starte Netzwerk",
	"Lade vom Netzwerk: %d",
	"Fehler beim starten des Netzwerks",
	"Netzwerk automatisch starten",
	"An",
	"Aus",
	"Ok",
	"Kompatibilit�ts Einstellungen",
	"Entferne alle Einstellungen",
	"Alle Einstellungen des Spiels entfernt",
	"Scrolling", // _STR_SCROLLING
	"Men� Typ", // _STR_MENUTYPE
	"Langsam", // _STR_SLOW
	"Normal", // _STR_MEDIUM
	"Schnell", // _STR_FAST
	"Statisch", // _STR_STATIC
	"Dymanisch", // _STR_DYNAMIC
	"Default menu", // _STR_DEF_DEVICE	
};

char *portuguese[] = {
	"               BEM-VINDO AO OPEN PS2 LOADER. C�DIGO PRINCIPAL BASEADO NO C�DIGO FONTE DO HD PROJECT <http://psx-scene.com> ADAPTA��O PARA O FORMATO USB ADVANCE E INTERFACE GR�FICA INICIAL POR IFCARO <http://ps2dev.ifcaro.net> GRANDE PARTE DO N�CLEO DO PROGRAMA � FEITO POR JIMMIKAELKAEL. TODOS INCREMENTOS DA INTERFACE GR�FICA S�O FEITOS POR VOLCA. OBRIGADO POR UTILIZAR NOSSO PROGRAMA ^^", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"Configura��o do tema", // gfx.c:502
	"Configura��o de IP", // gfx.c:764
	"Salvar altera��es", // gfx.c:586
	"O Voltar", // gfx.c:736
	"Rolagem lenta", // main.c:21 
	"Rolagem  m�dia", 
	"Rolagem r�pida",
	"Menu din�mico",  // main.c:22
	"Menu est�tico",
	"Configurar tema",  // main.c:23
	"Configurar rede",
	"Sele��o de idioma",  
	"Sem Items", // main.c:26
	"Ainda n�o dispon�vel", // main.c:158
	"Op��es salvas...", //  main.c:166
	"Erro guardando op��es!", //  main.c:168
	"Sair", // Menu item strings
	"Op��es",
	"Jogos via USB",
	"Jogos via HDD",
	"Jogos via rede",
	"Aplicativos",
	"Tema", // Submenu items
	"Idioma",
	"Idioma: portugu�s",
	"Iniciar rede",
	"Carregando rede: %d",
	"Erro ao iniciar rede",
	"In�cio de rede autom�tico",
	"Ligado",
	"Desligado",
	"Ok",
	"Op��es de compatibilidade",
	"Remover todas configura��es",
	"Todas entradas do jogo removidas",
	"Rolagem", // _STR_SCROLLING
	"Tipo de menu", // _STR_MENUTYPE
	"Lento", // _STR_SLOW
	"M�dio", // _STR_MEDIUM
	"R�pido", // _STR_FAST
	"Est�tico", // _STR_STATIC
	"Din�mico", // _STR_DYNAMIC
	"Default menu", // _STR_DEF_DEVICE	
};

char *norwegian[] = {
	"               VELKOMMEN TIL OPEN PS2 LOADER. HOVEDPROGRAMMET ER BASERT P� KILDEKODE FRA \"HD PROJECT\" <http://psx-scene.com> TILPASSING TIL USB ADVANCE FORMAT OG F�RSTE BRUKERGRENSESNITT AV IFCARO <http://ps2dev.ifcaro.net> DET MESTE AV OPEN PS2 LOADERS KJERNE ER LAGET AV JIMMIKAELKAEL. ALLE BRUKERGRENSESNITTFORBEDRINGENE ER LAGET AV VOLCA. TAKK FOR AT DU BRUKER V�RT PROGRAM ^^", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"Tema oppsett", // gfx.c:502
	"IP oppsett", // gfx.c:764
	"Lagre endringer", // gfx.c:586
	"O Tilbake", // gfx.c:736
	"Rull sakte", // main.c:21 
	"Rull middels", 
	"Rull raskt",
	"Dynamisk meny",  // main.c:22
	"Statisk meny",
	"Velg tema",  // main.c:23
	"Nettverksinstillinger",
	"Velg spr�k",  
	"No Items", // main.c:26
	"Ikke tilgjengelig enda", // main.c:158
	"Oppsett lagret...", //  main.c:166
	"Feil ved lagring av oppsett!", //  main.c:168
	"Avslutt", // Menu item strings
	"Oppsett",
	"USB Spill",
	"HDD Spill",
	"Nettverksspill",
	"Programmer",
	"Temaer", // Submenu items
	"Spr�k",
	"Spr�k: Norsk",
	"Start nettverk",
	"Nettverk lastes: %d",
	"Nettverk lastings feilet",
	"Nettverk starter automatisk",
	"P�",
	"Av",
	"Ok",
	"Kompatibilitetsoppsett",
	"Fjern alle instillinge",
	"Fjernet alle instillinger fra spillet",
	"Rulling", // _STR_SCROLLING
	"Meny type", // _STR_MENUTYPE
	"Sakte", // _STR_SLOW
	"Middels", // _STR_MEDIUM
	"Raskt", // _STR_FAST
	"Statisk", // _STR_STATIC
	"Dynamisk", // _STR_DYNAMIC
	"Default menu", // _STR_DEF_DEVICE	
};

char *turkish[] = {
	"               OPEN PS2 LOADER'A HOS GELDINIZ. ANA KOD HD PROJECT <http://psx-scene.com> PROJESI UZERINE IFCARO <http://ps2dev.ifcaro.net> TARAFINDAN GELISTIRILMISTIR. COGU YUKLEME KODLARI JIMMIKAELKAEL TARAFINDAN YAZILMISTIR. TUM ARAYUZ IYILESTIRMELERI VOLCA TARAFINDAN HAZIRLANMISTIR. PROGRAMIMIZ KULLANDIGINIZ ICIN TESEKKURLER ^^", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"Arayuz ayarlari", // gfx.c:502
	"IP ayarlari", // gfx.c:764
	"Ayarlari kaydet", // gfx.c:586
	"O Geri", // gfx.c:736
	"Kaydirma: Yavas", // main.c:21 
	"Kaydirma: Normal", 
	"Kaydirma: Hizli",
	"Hareketli Menu",  // main.c:22
	"Sabit Menu",
	"Arayuzu ayarla",  // main.c:23
	"Ag ayarlari",
	"Dil secin",  
	"Bos", // main.c:26
	"Henuz mevcut degil", // main.c:158
	"Ayarlar kaydedildi...", //  main.c:166
	"Hata: Ayarlar kaydedilemedi!", //  main.c:168
	"Cikis", // Menu item strings
	"Ayarlar",
	"USB Oyunlari",
	"HDD Oyunlari",
	"Ag Oyunlari",
	"Uygulamalar",
	"Arayuz", // Submenu items
	"Dil",
	"Dil: Turkce",
	"Agi baslat",
	"Ag yukleniyor: %d",
	"Ag baslatma hatasi",
	"Agi otomatik baslatma",
	"Acik",
	"Kapali",
	"Tamam",
	"Uyumululuk ayarlari",
	"Tum ayarlari sifirla",
	"Oyun icin tum anahatarlari kaldir",
	"Kaydirma", // _STR_SCROLLING
	"Menu tipi", // _STR_MENUTYPE
	"Yavas", // _STR_SLOW
	"Normal", // _STR_MEDIUM
	"Hizli", // _STR_FAST
	"Sabit", // _STR_STATIC
	"Hareketli", // _STR_DYNAMIC
	"Default menu", // _STR_DEF_DEVICE	
};

char *polish[] = {
	"               WITAM W OPEN PS2 LOADER. GL�WNY KOD WZOROWANY NA KODZIE ZR�DLOWYM HD PROJEKTU <http://psx-scene.com> IMPLEMENTACJE FORMATU USB ADVANCE I GUI PRZEZ IFCARO <http://ps2dev.ifcaro.net> WIEKSZOSC JADRA LOADER'A WYKONANA PRZEZ JIMMIKAELKAEL. WSZYSTKIE ULEPSZENIA GUI WYKONANE PRZEZ VOLCA. DZIEKUJE ZA SKORZYSTANIE Z NASZEJ APLIKACJI. ^^", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"Ustawienia theme'u", // gfx.c:502
	"Ustawienia IP", // gfx.c:764
	"Zapisz Zmiany", // gfx.c:586
	"O Powr�t", // gfx.c:736
	"Wolne przewijanie", // main.c:21 
	"Normalne przewijanie", 
	"Szybkie przewijanie",
	"Dynamiczne Menu",  // main.c:22
	"Statyczne Menu",
	"stawienia Theme'�w",  // main.c:23
	"Ustawienia Sieci",
	"Wybierz Jezyk",  
	"Brak Rzeczy", // main.c:26
	"Jeszcze Niedostepne", // main.c:158
	"Ustawienia zostaly zapisane...", //  main.c:166
	"Blad przy zapisie ustawien!", //  main.c:168
	"Wyjscie", // Menu item strings
	"Ustawienia",
	"Gry z USB",
	"Gry z HDD",
	"Gry z Sieci",
	"Programy",
	"Theme", // Submenu items
	"Jezyk",
	"Jezyk : Polski",
	"Uruchom Siec",
	"Ladowanie z sieci: %d",
	"Blad podczas uruchamiania sieci",
	"Automatyczne uruchamianie sieci",
	"Wlaczone",
	"Wylaczone",
	"Ok",
	"Ustawienia Kompatybilnosci",
	"Usun wszystkie ustawienia",
	"Usunieto wszystkie klucze dla gry",
	"Przewijanie", // _STR_SCROLLING
	"Typ Menu", // _STR_MENUTYPE
	"Powoli", // _STR_SLOW
	"Normalnie", // _STR_MEDIUM
	"Szybko", // _STR_FAST
	"Statycznie", // _STR_STATIC
	"Dynamicznie", // _STR_DYNAMIC
	"Default menu", // _STR_DEF_DEVICE	
};

char *russian[] = {
	"               ����� ���������� � ��������� OPEN PS2 LOADER. �������� ��� ��������� �� �������� ���� ��������� HD Project <http://psx-scene.com> ��������� � ������� USB ADVANCE � ��������� ����������� ��������� ��������� IFCARO <http://ps2dev.ifcaro.net> ����������� ���� ��������� JIMMIKAELKAEL. ��� ����������� ��������� ��������� VOLCA. ������� ��� �� ������������� ����� ���������!", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"��������� ����", // gfx.c:502
	"��������� ��������� IP", // gfx.c:764
	"Save changes", // gfx.c:586
	"O �����", // gfx.c:736
	"��������� ���������", // main.c:21 
	"���������� ���������", 
	"������� ���������",
	"������������ ����",  // main.c:22
	"����������� ����",
	"��������� ����",  // main.c:23
	"��������� ����",
	"������� ����",  
	"������� �����������", // main.c:26
	"���� ����������", // main.c:158
	"��������� ��������� �������...", //  main.c:166
	"������ ������ ��������!", //  main.c:168
	"�����", // Menu item strings
	"���������",
	"���� � USB - ����������",
	"���� � HDD",
	"���� � �������� ���������",
	"����������",
	"����", // Submenu items
	"����",
	"����: �������",
	"������ ����",
	"�������� ����: %d",
	"������ ������� ����",
	"�������������� ������ ����",
	"��������",
	"���������",
	"OK",
	"��������� �������������",
	"�������� ��� ���������",
	"������ ��� ��������� ��� ����",
	"���������", // _STR_SCROLLING
	"��� ����", // _STR_MENUTYPE
	"���������", // _STR_SLOW
	"�������", // _STR_MEDIUM
	"�������", // _STR_FAST
	"�����������", // _STR_STATIC
	"������������", // _STR_DYNAMIC
	"Default menu", // _STR_DEF_DEVICE	
};

char *indonesian[] = {
	"               SELAMAT DATANG DI OPEN PS2 LOADER. KODE UTAMA BERDASARKAN SUMBER KODE DARI HD PROJECT <http://psx-scene.com> DI ADAPTASI UNTUK FORMAT USB ADVANCE DAN GUI UTAMA DIBUAT OLEH IFCARO <http://ps2dev.ifcaro.net> SEBAGIAN BESAR CORE DIBUAT OLEH JIMMIKAELKAEL. SEMUA PERBAIKAN GUI DIBUAT OLEH VOLCA. TERIMA KASIH TELAH MENGGUNAKAN PROGRAM KAMI ^^", // gfx.c:85
	"Open PS2 Loader %s", // gfx.c:320
	"Konfigurasi tema", // gfx.c:502
	"Konfigurasi IP", // gfx.c:764
	"Simpan perubahan", // gfx.c:586
	"O Kembali", // gfx.c:736
	"Gulung Lambat", // main.c:21 
	"Gulung Normal", 
	"Gulung Cepat",
	"Menu Dinamis",  // main.c:22
	"Menu Statis",
	"Mengatur tema",  // main.c:23
	"Konfigurasi jaringan",
	"Pilih bahasa",  
	"Tidak ada data", // main.c:26
	"Belum tersedia", // main.c:158
	"Pengaturan disimpan...", //  main.c:166
	"Gagal menulis pengaturan!", //  main.c:168
	"Keluar", // Menu item strings
	"Pengaturan",
	"Game-Game USB",
	"Game-Game HDD",
	"Game-Game Jaringan",
	"Aplikasi",
	"Tema", // Submenu items
	"Bahasa",
	"Bahasa: Bahasa Indonesia",
	"Mulai Jaringan",
	"Memuat jaringan: %d",
	"Gagal memulai jaringan",
	"jaringan memulai otomatis",
	"On",
	"Off",
	"Oke",
	"Pengaturan kompatibilitas",
	"Hapus semua pengaturan",
	"Hapus semua pengaturan gamee",
	"Perputaran", // _STR_SCROLLING
	"Tipe menu", // _STR_MENUTYPE
	"Lambat", // _STR_SLOW
	"Normal", // _STR_MEDIUM
	"Cepat", // _STR_FAST
	"Statis", // _STR_STATIC
	"Dinamis", // _STR_DYNAMIC
	"Default menu", // _STR_DEF_DEVICE	
};

char *bulgarian[] = {
	"               ����� ����� � OPEN PS2 LOADER. ����� � ������� ��  HD PROJECT <http://psx-scene.com> ��������� �� USB ADVANCE ������� � ����������� �� IFCARO <http://ps2dev.ifcaro.net> ������ �� ���������� � �� JIMMIKAELKAEL. ��������� �� VOLCA. ���������� ��, �� �������� ���������� ��",
	"Open PS2 Loader %s",
	"������������ �� ������",
	"IP ������������",
	"������ �������",
	"O �����",
	"����� ����", 
	"����� ����", 
	"����� ����� ����",
	"��������� ����",
	"�������� ����",
	"��������� �� ������",
	"��������� �� �������",
	"����� �� ����",
	"���� ����",
	"�� � �������",
	"����������� �� ��������...",
	"������ ��� �����!",
	"�����", 
	"���������",
	"���� �� USB",
	"���� �� HDD",
	"���� �� �������",
	"��������",
	"����",
	"����",
	"����: ���������",
	"��������� �����",
	"��������� �� �����:", 
	"������ ��� ��������� �� �����",
	"����������� ���������� �� �����",
	"��������",
	"���������",
	"��",
	"��������� �� ������������",
	"��������� �� ������ ���������",
	"��������� �� ������ ���������",
	"Scrolling",
	"��� �� ������",
	"�����",
	"�����",
	"����� �����",
	"��������",
	"���������",
	"Default menu", // _STR_DEF_DEVICE	
};


char **languages[] = {
	english, // ID 0
	czech,   // ID 1
	spanish,  // ID 2
	french,   // ID 3
	german,   // ID 4
	portuguese, // ID 5
	norwegian,  // ID 6
	turkish,   // ID 7
	polish,   // ID 8
	russian,   // ID 9
	indonesian,  // ID 10
	bulgarian  // ID 11
};

const char *language_names[] = {
	"English",
	"Cesky (Czech)",
	"Espa�ol (Spanish)",
	"Fran�ais (French)",
	"Deutsch  (German)",
	"Portugu�s  (Portuguese)",
	"Norsk  (Norwegian)",
	"Turkce  (Turkish)",
	"Polski  (Polish)",
	"������� (Russian)",
	"Indonesia (Indonesian)",
	"��������� (Bulgarian)",
	NULL  // Null termination for UI enumeration to have padding
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

const char **getLanguageList() {
	return language_names;
}
