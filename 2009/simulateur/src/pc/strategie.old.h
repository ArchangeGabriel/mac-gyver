#ifndef STRATEGIEH
#define STRATEGIEH


#include <pthread.h>
#include <vector>
#include "path_planning.h"
#include "../common/simul.h"

#ifdef SIMULATION
void change_dest(position_t* pos, int);
#endif

// Liste des états de l'automate  (à compléter)
/*
enum {RECOLTE_PALETS, 
      RECOLTE_PALET_TERRAIN,
      RECOLTE_PALETS_DISTRIB,
      DEPOSE_PALETS,
      DEPOSE_PALETS_NIV1,
      DEPOSE_PALETS_NIV2,
      DEPOSE_PALETS_NIV3,
      RECALIBRAGE
     }; 
*/

enum etat_t
{
    CHERCHE_QUOI_FAIRE,
    CHERCHE_PALET,
    RAMASSE_PALET_TERRAIN,// ramasse un palet sur le terrain(état de transition)
    RAMASSE_PALET_TERRAIN_BOUGE,// ramasse un palet sur le terrain(bouge)
    RAMASSE_PALET_TERRAIN_RAMASSE,// ramasse un palet sur le terrain(ramasse)
    RAMASSE_PALET_DISTRIB,// ramasse un palet dans un distributeur(état de transition)
    RAMASSE_PALET_DISTRIB_BOUGE,// ramasse un palet dans un distributeur(bouge)
    RAMASSE_PALET_DISTRIB_RAMASSE,// ramasse un palet dans un distributeur(ramasse)
    POSE_COLONNE,// pose une colonne(état de transition)
    POSE_COLONNE_BOUGE,// pose une colonne(bouge)
    POSE_COLONNE_POSE // pose effectivement la colonne
};

enum couleur_t
{
    ROUGE,
    VERT
};

struct palet_t
{
    position_t pos;// position absolue
    couleur_t couleur;// la couleur du palet
};

struct etat_camera_devant_t
{
    std::vector<palet_t> palets;
};

class strategie_t
{
    private:
    pthread_mutex_t mutex;
    position_t pos;
    etat_t etat;
    couleur_t ma_couleur;
    int nb_palets_charges;
    etat_camera_devant_t etat_cam_devant;
    position_t pos_distributeur[2];
    position_t palet_a_ramasser;
    int distributeur_cible;
   
    public:
    double tension_moteurs[2];
    path_planner_t path_planner;
  
    void pense();
    void cherche_quoi_faire();
    void cherche_palet();
    void ramasse_palet_distrib();
    void ramasse_palet_terrain();
    void ramasse_palet_terrain_bouge();
    void ramasse_palet_terrain_ramasse();
    void ramasse_palet_distrib_bouge();
    void ramasse_palet_distrib_ramasse();
    void regarde_camera_devant();
    void pose_colonne();
    void pose_colonne_bouge();
    void pose_colonne_pose();
    int trouve_distributeur_plus_proche();
    position_t trouve_position_pour_poser_colonne();
  
    strategie_t();
    void start();
    void reconnect();                    // action à faire après une reconnection (intempestive) avec le pic
  
    bool calc_tension_moteurs();         // renvoie vrai ssi la tension a changé et met à jour la tension des moteurs
    void set_position(position_t _pos);  // règle la position
    position_t get_position();           // renvoie la position
};

#endif
