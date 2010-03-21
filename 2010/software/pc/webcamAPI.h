#ifndef WEBCAM_API
#define WEBCAM_API

#include "../common/simul.h"
#include "types.h"
#include <vector>

struct palet_t
{
    position_t pos;
    float surete;// 0:nul->1:super
};

void init_webcam_api();
/*
Renvoie un nombre xyy avec x codant la position des distrib et yy celle des palets
x = 0: distrib côté zones de départ
x = 1: distrib côté zones de dépôt
yy de 0 à 9: configuration des palets, voir réglement
Renvoie -1 en cas d'échec
*/

int wc_reco_config();

bool teste_palet_en_place(bool vert);

std::vector<palet_t> capture_et_detecte_palets_relatif_camera(bool vert);

std::vector<palet_t> capture_et_detecte_palets(bool vert);

#endif
