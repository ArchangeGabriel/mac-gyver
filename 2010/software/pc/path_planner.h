#ifndef DIST_TRSFRM_H
#define DIST_TRSFRM_H

#include <vector>
#include "types.h"

using namespace std;

//------------------------------------------------------------------------------

typedef float pp_dist;

typedef vector<position_t> pp_path;

//------------------------------------------------------------------------------
 
// Initialise les cartes d'énergie
void pp_init();

// Efface les cartes d'énergie
void pp_clear_maps();

// Remplis les cartes avec les obstacles en prenant en compte la taille du robot  
void pp_draw_config(int configE, int configI);
        
// Calcule la distance en chaque point de la map
void pp_compute_distance_transform();

// Renvoie un chemin 
pp_path pp_find_path(const position_t &from, const position_t &to);

// Charge/Sauve dans un fichier
bool pp_load_from_file(const char* file);
void pp_save_to_file(const char* file);  

// Sauve dans un bmp
void pp_save_to_bmp(const char *file);

#endif
