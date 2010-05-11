#ifndef WEBCAM_API
#define WEBCAM_API

void* wc_MainLoop(void *);

/*
les différentes configuration possible pour les maïs: xyz : 
x = 0 ou 1 : tomates activées
y de 0 à 9 : les maïs externes
z de 0 à 4 : les maïs internes
xy = 00 -> pas de mais
*/
int wc_reco_config();

#endif
