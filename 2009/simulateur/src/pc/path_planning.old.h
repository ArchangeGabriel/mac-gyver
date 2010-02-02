#ifndef PATH_PLANNINGH
#define PATH_PLANNINGH

#include <math.h>
#include <list>
#include <cstdio>
#include <stdlib.h>

using namespace std;

typedef struct position_t
{
    position_t(){}
    position_t(double x,double y,double a):x(x),y(y),a(a){}
    double x,y,a;
}position_t;

class direct_path_t
{
    public:
    position_t orig;                                           // origine de l'étape  (inutile? nonon)
    position_t dest;                                           // destination de l'étape
    double angle_to_dest;                                      // l'angle pour aller à la destination
    double prec_xy;                                            // précision en x et en y
    double prec_a;                                             // précision d'angle

    direct_path_t(position_t _orig, position_t _dest);         // constructeur de classe
    direct_path_t(position_t _orig, position_t _dest,double _prec_xy,double _prec_a);
    bool is_dest_reached(position_t pos);                      // renvoie vrai si la destination est atteinte (à peu près)
    void get_tension_moteurs(double *tensions,position_t pos,
        const std::list<direct_path_t>& plist,
        std::list<direct_path_t>::const_iterator this_path); // renvoie la tension à appliquer aux moteurs pour atteindre l'objectif
};

class path_planner_t 
{
    private:
    list<direct_path_t> path;                                  // liste des points de passage à suivre pour atteindre l'objectif
    
    // quelques constantes, j'aime pas trop les mettre ici mais c'est mieux que rien
    const position_t main_circle_pos;
    const double main_circle_radius;
    const double two_part_path_tolerance;
    const double robot_dim_x;
    
    bool is_path_suitable();

    public:
    path_planner_t();
    void clear_path();                                         // efface le chemin en cours
    bool is_dest_reached(position_t pos);                      // renvoie vrai si la destination est atteinte (à peu près)
    position_t next_dest();                                    // prochaine étape
    void plan_path_to(position_t orig, position_t dest,
                double prec_xy=0.02,double prec_a=0.1);       // calcul un chemin jusqu'à la destination et le rajoute à la fin de la liste
    void optimize_path();                                      // optimise le chemin calculé
    int find_zone(position_t pos) const;

    void get_tension_moteurs(double *tensions,position_t pos);  // renvoie la tension à appliquer aux moteurs pour atteindre l'objectif
};

#endif
