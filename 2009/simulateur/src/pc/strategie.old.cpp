#include "strategie.h"
#include <cmath>


//------------------------------------------------------------------------------
strategie_t::strategie_t()
{
  pthread_mutex_init(&mutex,NULL);
  tension_moteurs[0]=0.;
  tension_moteurs[1]=0.;
}
//------------------------------------------------------------------------------
void strategie_t::reconnect()
{
  path_planner.clear_path();
}
//------------------------------------------------------------------------------
void strategie_t::set_position(position_t _pos)
{
  pthread_mutex_lock(&mutex);
  pos=_pos;
  pthread_mutex_unlock(&mutex);
}
//------------------------------------------------------------------------------
position_t strategie_t::get_position()
{
  position_t _pos;
  pthread_mutex_lock(&mutex);
  _pos=pos;
  pthread_mutex_unlock(&mutex);
  return _pos;
}
//------------------------------------------------------------------------------
#ifdef SIMULATION
void change_dest(position_t*, int)
{
}
#endif
//------------------------------------------------------------------------------
bool strategie_t::calc_tension_moteurs()
{
  double _tension_moteurs[2];
  path_planner.get_tension_moteurs(_tension_moteurs,pos);
  if(_tension_moteurs[0]!=tension_moteurs[0] || _tension_moteurs[1]!=tension_moteurs[1])
  {
    tension_moteurs[0]=_tension_moteurs[0];
    tension_moteurs[1]=_tension_moteurs[1];
    return true;
  }
  else return false;
}
//------------------------------------------------------------------------------
void strategie_t::start()
{
}
//------------------------------------------------------------------------------
void strategie_t::pense()
{
    switch(etat)
    {
        case CHERCHE_QUOI_FAIRE:cherche_quoi_faire();break;
        case CHERCHE_PALET:cherche_palet();break;
        case RAMASSE_PALET_TERRAIN:ramasse_palet_terrain();break;
        case RAMASSE_PALET_DISTRIB:ramasse_palet_distrib();break;
        case POSE_COLONNE:pose_colonne();break;
        case POSE_COLONNE_BOUGE:pose_colonne_bouge();break;
        case POSE_COLONNE_POSE:pose_colonne_pose();break;
        default:etat=CHERCHE_QUOI_FAIRE;
    }
}

void strategie_t::cherche_quoi_faire()
{
    // FIXME: TODO
    if(nb_palets_charges==4)
        etat=POSE_COLONNE;
    else
        etat=CHERCHE_PALET;
}

void strategie_t::pose_colonne()
{
    // il faut trouver un endroits où poser la colonne
    // puis le donner au path planner
    position_t pos=trouve_position_pour_poser_colonne();
    path_planner.plan_path_to(pos,pos_distributeur[distributeur_cible]);
    //
    etat=POSE_COLONNE_BOUGE;
}

void strategie_t::pose_colonne_bouge()
{
    // regarde si on a atteint la destination
    if(path_planner.is_dest_reached(pos))
    {
        etat=POSE_COLONNE_POSE;
        return;
    }
}

void strategie_t::pose_colonne_pose()
{
    // FIXME: TODO
    // active des trucs qui pose la colonne au sommet du monde
    // une fois que c'est fait, on repasse dans l'état CHERCHE_QUOI_FAIRE
    // remettre nb_palets_charges à 0
    /*
    if(fait)
    {
        nb_palets_charges=0;
        etat=CHERCHE_QUOI_FAIRE;
    }
    else
    {
        ...le faire...
    }
    */
}

void strategie_t::cherche_palet()
{
    // regarde devant soit
    regarde_camera_devant();
    // est-ce qu'on voit un palet de la bonne couleur ?
    size_t idx=0;
    for(;idx<etat_cam_devant.palets.size();idx++)
        if(etat_cam_devant.palets[idx].couleur==ma_couleur)
            break;
    // FIXME: privilégie un palet au sol à un palet dans un distributeur
    // si on voit un palet de la bonne couleur, on le prend
    if(idx!=etat_cam_devant.palets.size())
    {
        palet_a_ramasser=etat_cam_devant.palets[idx].pos;
        etat=RAMASSE_PALET_TERRAIN;
    }
    else
    {
        // va au distributeur le plus proche
        distributeur_cible=trouve_distributeur_plus_proche();
        etat=RAMASSE_PALET_DISTRIB;
    }
}

void strategie_t::ramasse_palet_distrib()
{
    // planifie le chemin
    path_planner.plan_path_to(pos,pos_distributeur[distributeur_cible]);
    //
    etat=RAMASSE_PALET_DISTRIB_BOUGE;
}

void strategie_t::ramasse_palet_distrib_bouge()
{
    // regarde si on a atteint la destination
    if(path_planner.is_dest_reached(pos))
    {
        etat=RAMASSE_PALET_DISTRIB_RAMASSE;
        return;
    }
}

void strategie_t::ramasse_palet_terrain()
{
    // planifie le chemin
    path_planner.plan_path_to(pos,palet_a_ramasser);
    //
    etat=RAMASSE_PALET_TERRAIN_BOUGE;
}

void strategie_t::ramasse_palet_terrain_bouge()
{
    // regarde si on a atteint la destination
    if(path_planner.is_dest_reached(pos))
    {
        etat=RAMASSE_PALET_TERRAIN_RAMASSE;
        return;
    }
}

void strategie_t::ramasse_palet_terrain_ramasse()
{
    // FIXME: TODO
    // cf ramasse_palet_distrib_ramasse
}

void strategie_t::ramasse_palet_distrib_ramasse()
{
    // FIXME: TODO
    // active probablement des trucs qui vont faire bouger le palet et entraîner le monde dans un trou noir
    // une fois que c'est fait, on repasse dans l'état CHERCHE_QUOI_FAIRE
    // incrémenter aussi nb_palets_charges
    /*
    if(fait)
    {
        nb_palets_charges++;
        etat=CHERCHE_QUOI_FAIRE;
    }
    else
    {
        ...le faire...
    }
    */
}

void strategie_t::regarde_camera_devant()
{
    // FIXME: TODO
    // utilise la caméra pour regarder devant et trouver les palets visibles
}

int strategie_t::trouve_distributeur_plus_proche()
{
    double min_dist2=0;
    double min_dist_index=-1;
    
    for(int i=0;i<2;i++)
    {
        double dist2=(pos.x-pos_distributeur[i].x)*(pos.x-pos_distributeur[i].x)+
                (pos.y-pos_distributeur[i].y)*(pos.y-pos_distributeur[i].y);
        if(min_dist_index==-1 || (min_dist_index>=0 && min_dist2>dist2))
        {
            min_dist_index=i;
            min_dist2=dist2;
        }
    }
    
    return min_dist_index;
}

position_t strategie_t::trouve_position_pour_poser_colonne()
{
    // FIXME: TODO
    position_t pos;
    pos.x=-1.0;
    pos.y=-1.0;
    return pos;
}

