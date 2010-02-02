#include "path_planning.h"

double normalize(double angle)
{
    if(angle>M_PI)
        return normalize(angle-2*M_PI);
    else if(angle<-M_PI)
        return normalize(angle+2*M_PI);
    else
        return angle;
}
//------------------------------------------------------------------------------
direct_path_t::direct_path_t(position_t _orig, position_t _dest)
{
    orig=_orig;
    dest=_dest;
    angle_to_dest = atan2(dest.y-orig.y,dest.x-orig.x);
    prec_xy = 0.05;
    prec_a = 0.1;
}

direct_path_t::direct_path_t(position_t _orig, position_t _dest,double _prec_xy,double _prec_a)
{
    orig=_orig;
    dest=_dest;
    angle_to_dest = atan2(dest.y-orig.y,dest.x-orig.x);
    prec_xy = _prec_xy;
    prec_a = _prec_a;
}
//------------------------------------------------------------------------------
bool direct_path_t::is_dest_reached(position_t pos)
{
    double dx, dy, da;
    dx = dest.x - pos.x;
    dy = dest.y - pos.y;
    da = dest.a - pos.a;
    da=normalize(da);
    return sqrt(dx*dx+dy*dy)<2*prec_xy && da <= prec_a && da >= -prec_a;
}
//------------------------------------------------------------------------------
void direct_path_t::get_tension_moteurs(double *tensions,position_t pos,
    const std::list<direct_path_t>& plist,std::list<direct_path_t>::const_iterator this_path)
{
    double dx, dy, da, da_to_dest;
    dx = dest.x - pos.x;
    dy = dest.y - pos.y;
    da = dest.a - pos.a;
    //angle_to_dest = acos(dx/sqrt(dx*dx + dy*dy));
    angle_to_dest=atan2(dy,dx);
    da_to_dest = angle_to_dest - pos.a;
    da_to_dest=normalize(da_to_dest);
    if(sqrt(dx*dx + dy*dy) < 2*prec_xy)
    {
        // si |da|>180°, ramène da dans les négatif pour optimiser le sens de rotation
        da=normalize(da);
        
        if(da > prec_a)
        {
            tensions[0] = 0.15;
            tensions[1] = -0.15;
        }
        else if(da < -prec_a)
        {
            tensions[0] = -0.15;
            tensions[1] = 0.15;
        }
        else
        {
            tensions[0] = 0.;
            tensions[1] = 0.;
        }
    }
    else
    {
        const double max_delta_a=2*prec_a;
    
        if(da_to_dest > max_delta_a)
        {
            tensions[0] = 0.4;
            tensions[1] = -0.4;
        }
        else if(da_to_dest < -max_delta_a)
        {
            tensions[0] = -0.4;
            tensions[1] = 0.4;
        }
        else
        {
            const double min_speed=0.5;
            const double max_speed=1.0;
            const double linear_factor=(max_speed-min_speed)/max_delta_a;
            double left_delta_a=0;
            double right_delta_a=0;
            
            if(da_to_dest>0)
                right_delta_a=da_to_dest;
            else
                left_delta_a=-da_to_dest;
            
            tensions[0] = 1.0-left_delta_a*linear_factor;
            tensions[1] = 1.0-right_delta_a*linear_factor;
        }
        
        // ralentit en fin de trajet
        std::list<direct_path_t>::const_iterator next=this_path;
        next++;
        if(next==plist.end() || (fabs(next->orig.x-next->dest.x)<1e-3 && fabs(next->orig.y-next->dest.y)<1e-3))
        {
            const double slow_down_radius=0.15;
            const double min_speed=0.4;
            
            double cur_distance=sqrt(dx*dx+dy*dy);
            if(cur_distance<=slow_down_radius)
            {
                //printf("slow down\n");
                // ralentissement linéaire, probablement pas le meilleur
                double factor=cur_distance/slow_down_radius;
                if(tensions[0]>min_speed && tensions[1]>min_speed)
                {
                    tensions[0]*=factor;
                    tensions[1]*=factor;
                }
            }
        }
    }
    
    tensions[0]/=1.5;
    tensions[1]/=1.5;
}
//------------------------------------------------------------------------------







//------------------------------------------------------------------------------
path_planner_t::path_planner_t()
    :main_circle_pos(3.0/2.0,2.1/2.0,0),main_circle_radius(0.15),two_part_path_tolerance(0.1),
    robot_dim_x(0.25)
{
}
//------------------------------------------------------------------------------
bool path_planner_t::is_dest_reached(position_t pos)
{
  return path.back().is_dest_reached(pos);
}
//------------------------------------------------------------------------------
void path_planner_t::clear_path()
{
  path.clear();
}
//------------------------------------------------------------------------------
position_t path_planner_t::next_dest()
{
  return path.front().dest;
}
//------------------------------------------------------------------------------
bool path_planner_t::is_path_suitable()
{
    // vérifie qu'il n'y a pas d'intersection avec le rond central
    std::list<direct_path_t>::const_iterator it=path.begin();
    for(;it!=path.end();++it)
    {
        // calcule la distance minimale du centre du rond au segment de droite
        double dx=it->orig.x-it->dest.x;
        double dy=it->orig.y-it->dest.y;
        if(sqrt(dx*dy+dy*dy)<2*it->prec_xy)
            continue;
        position_t vec_p2c(main_circle_pos.x-it->orig.x,main_circle_pos.y-it->orig.y,0);
        position_t vec_p2d(it->dest.x-it->orig.x,it->dest.y-it->orig.y,0);
        double dist_p2d_sqr=vec_p2d.x*vec_p2d.x+vec_p2d.y*vec_p2d.y;
        double scalar_prod=(vec_p2c.x*vec_p2d.x+vec_p2c.y*vec_p2d.y)/sqrt(dist_p2d_sqr);
        double temp=scalar_prod*scalar_prod;
        double dist=sqrt(vec_p2c.x*vec_p2c.x+vec_p2c.y*vec_p2c.y-temp);
        /*
        printf("projection: scalar_prod=%f dist_p2d_sqr=%f dist_p2d=%f temp=%f dist=%f\n",
            scalar_prod,dist_p2d_sqr,sqrt(dist_p2d_sqr),temp,dist);
        */
        // deux cas:
        // * la projection du centre du cercle est en dehors du segment
        if(scalar_prod<0)
            continue;// on dit que c'est bon pour éviter les calculs compliqués
        if(scalar_prod>sqrt(dist_p2d_sqr))
            continue;// idem
        // * la projection du centre est dans le segment
        dist-=robot_dim_x/2.0+main_circle_radius+0.03;// 3cm de marge
        if(dist<0)
            return false;
    }
    
    return true;
}

int path_planner_t::find_zone(position_t pos) const
{
    if(pos.x>main_circle_pos.x && pos.y>main_circle_pos.y)
        return 3;
    if(pos.x>main_circle_pos.x && pos.y<=main_circle_pos.y)
        return 0;
    if(pos.x<main_circle_pos.x && pos.y>main_circle_pos.y)
        return 2;
    if(pos.x<main_circle_pos.x && pos.y<=main_circle_pos.y)
        return 1;
    printf("WTF but where is the robot ?\n");
    return 0;
}

void path_planner_t::plan_path_to(position_t orig, position_t dest,double prec_xy,double prec_a)
{
    // A faire: doit calculer une succession d'étape de orig à dest
    // Les étapes doivent être libres d'obstacles
    // path.push_back(etape) rajoute une étape à la liste
    path.clear();
    
    // Amaury: si jamais la destination est trop proche de la source(exemple: on ne fait que tourner)
    // ces calculs sont faux car l'angle intermédiaire calculé est chaotique
    
    double dx=orig.x-dest.x;
    double dy=orig.y-dest.y;
    
    if(sqrt(dx*dx+dy*dy)>(2*prec_xy))
    {
        // 1) Essaye la ligne droite
        position_t inter1; //d'abord on s'anglifie vers la destination
        inter1.x = orig.x;
        inter1.y = orig.y;
        //inter1.a = (dest.x - orig.x)/sqrt((dest.x - orig.x)*(dest.x - orig.x) + (dest.y - orig.y)*(dest.y - orig.y));
        //inter1.a = acos(inter1.a);
        inter1.a=atan2(dest.y-orig.y,dest.x-orig.x);
        direct_path_t mid1(orig, inter1);

        position_t inter2; //puis on trace  
        inter2.x = dest.x;
        inter2.y = dest.y;
        inter2.a = inter1.a;
        direct_path_t mid2(inter1, inter2);

        direct_path_t etape(inter2,dest); //et enfin on s'anglifie bien
        etape.prec_xy=prec_xy;// ajuste éventuellement la précision
        etape.prec_a=prec_a;
        path.push_back(mid1);
        path.push_back(mid2);
        path.push_back(etape);
        if(is_path_suitable())
        {
            printf("path planner: straight line path computed\n");
            goto Lend;
        }
        // 2) Essaye un trajet en deux parties
        // tente de calculer le trajet otpimal avec une petite marge de manoeuvre
        double displacement=main_circle_radius+robot_dim_x/2.0+two_part_path_tolerance;
        double normal_x=-sin(inter1.a);
        double normal_y=cos(inter1.a);
        
        position_t interm_pos;
        interm_pos.x=main_circle_pos.x+normal_x*displacement;
        interm_pos.y=main_circle_pos.y+normal_y*displacement;
        interm_pos.a=atan2(interm_pos.y-orig.y,interm_pos.x-orig.x);
        
        position_t interm_pos2;
        interm_pos2.x=interm_pos.x;
        interm_pos2.y=interm_pos.y;
        interm_pos2.a=atan2(dest.y-interm_pos2.y,dest.x-interm_pos2.x);
        
        path.clear();
        // change l'angle vers le point intermédiaire
        inter1.x=orig.x;
        inter1.y=orig.y;
        inter1.a=interm_pos.a;
        path.push_back(direct_path_t(orig,inter1));
        // va vers le point intermédiaire
        direct_path_t p(inter1,interm_pos);
        path.push_back(p);
        // va vers la destination
        inter2.x=dest.x;
        inter2.y=dest.y;
        inter2.a=interm_pos2.a;
        direct_path_t p2(interm_pos,inter2);
        #if 0
        p2.prec_a=normalize(fabs(interm_pos.a-interm_pos2.a));// lissage automatique
        #endif
        path.push_back(p2);

        // change l'angle final
        direct_path_t p3(inter2,dest);
        p3.prec_xy=prec_xy;
        p3.prec_a=prec_a;
        path.push_back(p3);
        
        if(is_path_suitable())
        {
            printf("path planner: two part path computed\n");
            goto Lend;
        }
        
        // 3) Essaye en 2 parties mais moins efficace
        path.clear();
        
        position_t interm;
        interm.x=orig.x;
        interm.y=dest.y;
        interm.a=atan2(dest.y-interm.y,dest.x-interm.x);
        
        path.push_back(direct_path_t(orig,interm));
        path.push_back(direct_path_t(interm,dest,prec_xy,prec_a));
        
        if(is_path_suitable())
        {
            printf("path planner: two part defensive path computed\n");
            goto Lend;
        }
        
        // 4) Bourrin mais marche à tout les coups(normalement :))
        // délimite le terrain en quatre zones:
        /*
          +--------+--------+
          |  1     |    0   |
          |      /---\      |
          +-----+     +-----+
          |  2   \---/  3   |
          |        |        |
          +--------+--------+
        */
        position_t safe_pos[4];
        safe_pos[0].x=main_circle_pos.x+main_circle_radius+robot_dim_x;
        safe_pos[0].y=main_circle_pos.y-main_circle_radius-robot_dim_x;
        safe_pos[0].a=-M_PI;
        safe_pos[3].x=main_circle_pos.x+main_circle_radius+robot_dim_x;
        safe_pos[3].y=main_circle_pos.y+main_circle_radius+robot_dim_x;
        safe_pos[3].a=-M_PI/2;
        safe_pos[1].x=main_circle_pos.x-main_circle_radius-robot_dim_x;
        safe_pos[1].y=main_circle_pos.y+main_circle_radius+robot_dim_x;
        safe_pos[1].a=M_PI;
        safe_pos[2].x=main_circle_pos.x-main_circle_radius-robot_dim_x;
        safe_pos[2].y=main_circle_pos.y+main_circle_radius+robot_dim_x;
        safe_pos[2].a=0;
        
        int from_zone=find_zone(orig);
        int to_zone=find_zone(dest);
        
        path.clear();
        // va vers le point sûr de la zone de départ
        path.push_back(direct_path_t(orig,safe_pos[from_zone]));
        // rejoint les deux points sûrs
        // * cas 1: les deux zones se touchent: ligne droite
        if(abs(from_zone-to_zone)==1 || abs(from_zone-to_zone)==3)
            path.push_back(direct_path_t(safe_pos[from_zone],safe_pos[to_zone]));
        // * case 2: passe par une zone intermédiaire
        else
        {
            int interm_zone=(from_zone+1)%4;
            path.push_back(direct_path_t(safe_pos[from_zone],safe_pos[interm_zone]));
            path.push_back(direct_path_t(safe_pos[interm_zone],safe_pos[to_zone]));
        }
        // va du point sûr de la zone d'arrivée vers le point d'arrivée
        path.push_back(direct_path_t(safe_pos[to_zone],dest,prec_xy,prec_a));
        
        
        if(is_path_suitable())
        {
            printf("path planner: three or four part secure path computed\n");
            goto Lend;
        }
        else
        {
            printf("path planner: oops, eating dandelion's roots\n");
            goto Lend;
        }
    }
    else
    {
        direct_path_t p(orig,dest);
        p.prec_xy=prec_xy;
        path.push_back(p);
    }
        
    Lend:
    optimize_path();
}
//------------------------------------------------------------------------------
void path_planner_t::optimize_path()
{
    // A faire: doit lisser le chemin calculé précédemment 
    const double max_smoothing_angle=M_PI*50.0/180.0;
    //const double max_smoothing_displacement=0.02;
    
    // cherche deux parties de chemins consécutives
    std::list<direct_path_t>::iterator it=path.begin();
    
    for(;it!=path.end();it++)
    {
        std::list<direct_path_t>::iterator it2=it;
        ++it2;
        if(it2==path.end())
            break;
        double dx1=-it->orig.x+it->dest.x;
        double dy1=-it->orig.y+it->dest.y;
        double dx2=-it2->orig.x+it2->dest.x;
        double dy2=-it2->orig.y+it2->dest.y;
        double len1=sqrt(dx1*dx1+dy1*dy1);
        double len2=sqrt(dx2*dx2+dy2*dy2);
        if(len1<2*it->prec_xy)
            continue;
        if(len2<2*it2->prec_xy)
            continue;
        //printf("path optimize: found one potential optimization...\n");
        // calcule l'angle entre les deux
        double a1=atan2(dy1,dx1);
        double a2=atan2(dy2,dx2);
        double da=normalize(a1-a2);
        double cda=fabs(da);
        //printf("da=%f cda=%f max_smoothing_angle=%f\n",da,cda,max_smoothing_angle);
        // ne fait rien si l'angle est trop grand
        if(cda>max_smoothing_angle)
            continue;
        it2->prec_a=max_smoothing_angle/2.0;
        /*
        // utilise une chemin intermédiaire pour diviser par deux l'angle à chaque virage
        double tan_alpha_2=tan(cda/2.0);
        double delta=max_smoothing_displacement*tan_alpha_2;
        // si le déplacement est trop grand par rapport au deux parties du chemins, ne fait rien
        if(delta>len1/2.0 || delta>len2/2.0)
            continue;
        // met à jour
        position_t interm1;
        double factor1=(1-delta)/len1;
        interm1.x=it->orig.x+factor1*dx1;
        interm1.y=it->orig.y+factor1*dy1;
        interm1.a=it->dest.a;
        
        position_t interm2;
        double factor2=delta/len2;
        interm2.x=it2->orig.x+factor2*dx2;
        interm2.y=it2->orig.y+factor2*dy2;
        interm2.a=atan2(interm2.y-interm1.y,interm2.x-interm1.x);
        
        it->dest=interm1;
        it->prec_a=max_smoothing_angle;
        it->prec_xy*=2;
        
        direct_path_t new_p(interm1,interm2);
        new_p.prec_a=max_smoothing_angle;
        new_p.prec_xy=it->prec_xy;
        std::list<direct_path_t>::iterator it3=path.insert(it2,new_p);
        
        it2->orig=interm2;
        // saute un chemin pour éviter de sous-sous-diviser la courbe
        it=it3;
        printf("path optimize: one optimization done\n");
        */
    }
}
//------------------------------------------------------------------------------
void path_planner_t::get_tension_moteurs(double *tensions,position_t pos)
{
    if(path.empty())
    {
        tensions[0]=0.;
        tensions[1]=0.;    
    }
    else if(path.front().is_dest_reached(pos))
    {
        path.pop_front();
        get_tension_moteurs(tensions,pos);
    }
    else
        path.front().get_tension_moteurs(tensions,pos,path,path.begin());
}
//------------------------------------------------------------------------------
