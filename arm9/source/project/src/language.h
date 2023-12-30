/**********************************************************/
/*   File added in the 2004 Release                       */
/*   Here are stored all literal strings so it is easy    */
/*   to do a translation of the game to other languages   */
/*                                                        */
/*                                                        */
/**********************************************************/

#ifndef LANGUAGE_H
#define LANGUAGE_H

#ifdef SPANISHLANG
static    char str_energy[]		= "energia:";
static    char str_paused[]		= "<p>ausado";
static    char str_grabbed[]	= "en<g>anchado";

/**********************************************/
static    char str_score[]	= "puntos";
static    char str_highscores[]	= "Mejores Puntuaciones";
static    char str_none[]	= "ninguno";

/***********************************************/
static    char str_views[]	= "Vistas:";
static    char str_views1[]	= "-Monstruo";
static    char str_views2[]	= "-Aerea";
static    char str_views3[]	= "-Ejercito";

static    char onstring[]		= " (ON)";
static    char offstring[]		= " (OFF)";
static    char linstring[]		= " (VEC)";
static    char lowstring[]		= " (BAJO)";
static    char medstring[]		= " (MEDIO)";
static    char highstring[]		= " (ALTO)";
static    char unavailablestring[] 	= " ( - )";

static    char str_mouse_keys[]		= "raton/flechas";
static    char str_shoot[]		= "boton/x";
static    char str_az[]		    = "a z";

static    char str_grab[]		    = "-PtGrab";
/* static    char videoEnglish[]	= "-3D"; */

static    char str_rotate[]		= "- Rotar Vista";
static    char str_ijkl[]		= "ijkl";
static    char str_showcase[]	= "-Ayuda";

static    char str_pause[]		= "-Pausa";
static    char str_detail[]		= "-Grafic";
static    char str_sound[]		= "-Sonido";
static    char str_music[]		= "-Musica";


static    char str_controls[]		= "-Control:";
static    char str_options[]		= "-Opciones:";
static    char str_move[]		    = "-Mover";
static    char str_attack[]		    = "-Ataque";
static    char str_tilthead[]		= "-Inclinacion";

static    char str_space[]		= "Pulsa espacio para empezar";

static    char str_googelon[]	= "Googelon-6";    
static    char str_techs[]		= "Techs-7";    
static    char str_vapour[]		= "El Vapor-8";    
static    char str_flutter[]		= "Flutter-9";

static    char str_andy[]		= "andy johnson's";
static    char str_version[]		= "2004b";

static char str_choosemonster[]     = "Elige tu monstruo:";
static char str_hero[]     = "Heroe";

static char str_tank[]     = "Tanque";
static char str_helo[]     = "Helo";
static char str_maser[]     = "Maser";
static char str_hunter[]     = "Caza";
static char str_launcher[]     = "Lazadera";
static char str_fighter[]     = "Fragata";
static char str_bomber[]     = "Bomber";
static char str_mechag[]     = "MechaG";

static char str_googelondest[] = "Googelon destruyo";
static char str_vapourdest[] = "El Vapor destruyo";
static char str_techsdest[] = "Techs destruyo";
static char str_flutterdest[] = "Flutter destruyo";


static char str_nothing[] = "...nada";
static char str_monstersdestroyed[] = "los monstruos destruyeron"; 

#else

/* Default language is english */

static    char str_energy[]		= "energy:";
static    char str_paused[]		= "<p>aused";
static    char str_grabbed[]	= "<g>rabbed";
/**********************************************/
static    char str_score[]	= "score";
static    char str_highscores[]	= "Highscores";
static    char str_none[]	= "none";

/***********************************************/
static    char str_views[]	= "Views:";
static    char str_views1[]	= "-Monster";
static    char str_views2[]	= "-Overview";
static    char str_views3[]	= "-Army";

static    char onstring[]		= " (ON)";
static    char offstring[]		= " (OFF)";
static    char linstring[]		= " (VEC)";
static    char lowstring[]		= " (LOW)";
static    char medstring[]		= " (MED)";
static    char highstring[]		= " (HIGH)";
static    char unavailablestring[] 	= " ( - )";

static    char str_mouse_keys[]		= "mouse/arrows";
static    char str_shoot[]		= "left btn/x";
static    char str_az[]		    = "a z";

static    char str_grab[]		= "-PtGrab";
/* static    char videoEnglish[]	= "-3D"; */

static    char str_rotate[]		= "- Rotate Overview";
static    char str_ijkl[]		= "ijkl";
static    char str_showcase[]	= "-Help";

static    char str_pause[]		= "-Pause";
static    char str_detail[]		= "-Detail";
static    char str_sound[]		= "-Sound";
static    char str_music[]		= "-Music";

static    char str_controls[]		= "-Controls:";
static    char str_options[]		= "-Options:";
static    char str_move[]		    = "-Move";
static    char str_attack[]		    = "-Attack";
static    char str_tilthead[]		= "-Tilt Head";

static    char str_space[]		= "Press the spacebar to begin";

static    char str_googelon[]	= "Googelon-6";    
static    char str_techs[]		= "Techs-7";    
static    char str_vapour[]		= "The Vapour-8";    
static    char str_flutter[]		= "Flutter-9";

static    char str_andy[]		= "andy johnson's";
static    char str_version[]		= "2004b";

static char str_choosemonster[]     = "Choose your monster:";
static char str_hero[]     = "Hero";

static char str_tank[]     = "Tank";
static char str_helo[]     = "Helo";
static char str_maser[]     = "Maser";
static char str_hunter[]     = "Hunter";
static char str_launcher[]     = "Launcher";
static char str_fighter[]     = "Fighter";
static char str_bomber[]     = "Bomber";
static char str_mechag[]     = "MechaG";

static char str_googelondest[] = "Googelon destroyed";
static char str_vapourdest[] = "The Vapour destroyed";
static char str_techsdest[] = "Techs destroyed";
static char str_flutterdest[] = "Flutter destroyed";

static char str_nothing[] = "...nothing";
static char str_monstersdestroyed[] = "Monsters destroyed";
#endif


#endif
