#ifndef ANSI_H
#define ANSI_H

// ANSI/VT100 Terminal Control Escape Sequences
// taken from http://www.termsys.demon.co.uk/vtansi.htm
//
#define  ANSI_RESET             "\033[2;37;0m"  ///< Alle Einstellungen zurücksetzen

#define  ANSI_CURSOR_OFF        "\033[?25l"     ///< Cursor abschalten
#define  ANSI_CURSOR_ON         "\033[?25h"     ///< Cursor anschalten

#define  ANSI_CLEAR             "\033[2J"       ///< Bildschirm löschen
#define  ANSI_ERASE_EOL         "\033[K"        ///< Bis Zeilenende löschen
#define  ANSI_ERASE_SOL         "\033[1K"       ///< Bis Zeilenanfang löschen
#define  ANSI_ERASE_LINE        "\033[2K"       ///< Zeile löschen
#define  ANSI_ERASE_DOWN        "\033[J"        ///< Bis Seitenende löschen
#define  ANSI_ERASE_UP          "\033[1J"       ///< Bis Seitenanfang löschen

#define  ANSI_DEL               "\033[P"        ///< Zeichen löschen
#define  ANSI_DELn(n)           "\033[" #n "P"  ///< x Zeichen löschen

#define  ANSI_HOME              "\033[H"        ///< Taste Home
#define  ANSI_SCROLL_UP         "\033M"         ///< Hoch scrollen
#define  ANSI_FREEZE(top, btm)  "\033[" #top ";" #btm "r" ///< Bereich einfrieren
#define  ANSI_UNFREEZE          "\033[r"        ///< Eingefrorenen Bereich wieder freigeben
#define  ANSI_GOTO(x, y)        "\033[" #y ";" #x "H" ///< Zu x/y Position springen
#define  ANSI_GOTO_COL(x)       "\033[" #x "G"  ///< Zu x Position springen
#define  ANSI_GOTO_COL1         ANSI_GOTO_COL(1)///< Zum Zeilenanfang springen

#define  ANSI_UP                "\033[A"        ///< Taste hoch
#define  ANSI_DOWN              "\033[B"        ///< Taste runter
#define  ANSI_RIGHT             "\033[C"        ///< Taste rechts
#define  ANSI_LEFT              "\033[D"        ///< Taste links

#define  ANSI_UPn(n)            "\033[" #n "A"  ///< x mal Taste hoch
#define  ANSI_DOWNn(n)          "\033[" #n "B"  ///< x mal Taste runter
#define  ANSI_RIGHTn(n)         "\033[" #n "C"  ///< x mal Taste rechts
#define  ANSI_LEFTn(n)          "\033[" #n "D"  ///< x mal Taste links

#define  ANSI_SAVE_CURSOR       "\033[s"        ///< Cursorposition speichern
#define  ANSI_RESTORE_CURSOR    "\033[u"        ///< Cursorposition wiederherstellen
#define  ANSI_SAVE_ALL          "\033[7"        ///< Alle Einstellungen speichern
#define  ANSI_RESTORE_ALL       "\033[8"        ///< Alle Einstellungen wiederherstellen

#define  ANSI_ENABLE_INSERT     "\033[4h"       ///< Einfügemodus aktivieren
#define  ANSI_DISABLE_INSERT    "\033[4l"       ///< Einfügemodus deaktivieren

#define  ANSI_BOLD              "\033[1m"       ///< Text fett
#define  ANSI_ITALICS           "\033[3m"       ///< Text kursiv
#define  ANSI_UNDERLINE         "\033[4m"       ///< Text unterstrichen
#define  ANSI_BLINK             "\033[5m"       ///< Text blinkend
#define  ANSI_INVERSE           "\033[7m"       ///< Text invers
#define  ANSI_STRIKE            "\033[9m"       ///< Text durchgestrichen
#define  ANSI_BOLD_OFF          "\033[22m"      ///< Text nicht fett
#define  ANSI_ITALICS_OFF       "\033[23m"      ///< Text nicht kursiv
#define  ANSI_UNDERLINE_OFF     "\033[24m"      ///< Text nicht unterstrichen
#define  ANSI_BLINK_OFF         "\033[25m"      ///< Text nicht blinkend
#define  ANSI_INVERSE_OFF       "\033[27m"      ///< Text nicht invers
#define  ANSI_STRIKE_OFF        "\033[29m"      ///< Text nicht durchgestrichen

#define  ANSI_FG_BLACK          "\033[0;30m"    ///< Textfarbe schwarz
#define  ANSI_FG_RED            "\033[0;31m"    ///< Textfarbe rot
#define  ANSI_FG_GREEN          "\033[0;32m"    ///< Textfarbe grün
#define  ANSI_FG_ORANGE         "\033[0;33m"    ///< Textfarbe orange
#define  ANSI_FG_BLUE           "\033[0;34m"    ///< Textfarbe blau
#define  ANSI_FG_MAGENTA        "\033[0;35m"    ///< Textfarbe magenta
#define  ANSI_FG_CYAN           "\033[0;36m"    ///< Textfarbe cyan
#define  ANSI_FG_GRAY           "\033[0;37m"    ///< Textfarbe grau
#define  ANSI_FG_DEFAULT        "\033[0;39m"    ///< Textfarbe standard

#define  ANSI_FG_DKGRAY         "\033[1;30m"    ///< Textfarbe dunkelgrau
#define  ANSI_FG_LTRED          "\033[1;31m"    ///< Textfarbe hellrot
#define  ANSI_FG_LTGREEN        "\033[1;32m"    ///< Textfarbe gellgrün
#define  ANSI_FG_YELLOW         "\033[1;33m"    ///< Textfarbe gelb
#define  ANSI_FG_LTBLUE         "\033[1;34m"    ///< Textfarbe hellblau
#define  ANSI_FG_LTMAGENTA      "\033[1;35m"    ///< Textfarbe hellmagenta
#define  ANSI_FG_LTCYAN         "\033[1;36m"    ///< Textfarbe hellcyan
#define  ANSI_FG_WHITE          "\033[1;37m"    ///< Textfarbe weiss

#define  ANSI_BG_BLACK          "\033[0;40m"    ///< Hintergrund schwarz
#define  ANSI_BG_RED            "\033[0;41m"    ///< Hintergrund rot
#define  ANSI_BG_GREEN          "\033[0;42m"    ///< Hintergrund grün
#define  ANSI_BG_ORANGE         "\033[0;43m"    ///< Hintergrund orange
#define  ANSI_BG_BLUE           "\033[0;44m"    ///< Hintergrund blau
#define  ANSI_BG_MAGENTA        "\033[0;45m"    ///< Hintergrund magenta
#define  ANSI_BG_CYAN           "\033[0;46m"    ///< Hintergrund cyan
#define  ANSI_BG_GRAY           "\033[0;47m"    ///< Hintergrund grau
#define  ANSI_BG_DEFAULT        "\033[0;49m"    ///< Hintergrund standard

#define  ANSI_BG_DKGRAY         "\033[1;40m"    ///< Hintergrund dunkelgrau
#define  ANSI_BG_LTRED          "\033[1;41m"    ///< Hintergrund hellrot
#define  ANSI_BG_LTGREEN        "\033[1;42m"    ///< Hintergrund gellgrün
#define  ANSI_BG_YELLOW         "\033[1;43m"    ///< Hintergrund gelb
#define  ANSI_BG_LTBLUE         "\033[1;44m"    ///< Hintergrund hellblau
#define  ANSI_BG_LTMAGENTA      "\033[1;45m"    ///< Hintergrund hellmagenta
#define  ANSI_BG_LTCYAN         "\033[1;46m"    ///< Hintergrund hellcyan
#define  ANSI_BG_WHITE          "\033[1;47m"    ///< Hintergrund weiss


#endif
