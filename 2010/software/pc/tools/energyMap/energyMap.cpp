#include <stdio.h>
#include <string.h>

#define PC_INCLUDE
#include "../../path_planner.h"
#include "../../../common/const.h"


//------------------------------------------------------------------------------
int main()
{
  char answer[100];
  printf("This program will generate the energy maps. It may take some time...\n");
  printf("Do you want to continue (yes/no)? ");
  if(scanf("%s", answer) != EOF && strcmp(answer, "yes") == 0)
  {
    char file[200];
    
    for(int configE = 1; configE <= 9; configE++)
      for(int configI = 1; configI <= 4; configI++)    
      {
        pp_map *map = new pp_map(_LONGUEUR_TER, _LARGEUR_TER, _LARGEUR_ROBOT, _LONGUEUR_ROBOT);
        printf("Generating configuration (%d/9,%d/4)...\n", configE, configI);
        
        map->draw_config(configE, configI);
        map->compute_distance_transform();
        
        sprintf(file,"config%d.dat",configE*10+configI);
        map->save_to_file(file);

        sprintf(file,"config%d.bmp",configE*10+configI);
        map->save_to_bmp(file);
        
        delete map;
      }
  }
  return 0;
}
