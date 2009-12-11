#include "parse_params.h"


//------------------------------------------------------------------------------
val_param_t::val_param_t(char* name_deb, int name_len, char* val_deb, char** val_fin)
{
  while(**val_fin==' ' || **val_fin==';') (*val_fin)--;
  (*val_fin)++;
  if(name_len>0 && *val_fin-val_deb>0)
  {
    name = new char[name_len+1];
    memcpy(name,name_deb,name_len);
    name[name_len]='\0';
    value = new char[*val_fin-val_deb+1];  
    memcpy(value,val_deb,*val_fin-val_deb);
    value[*val_fin-val_deb]='\0';      
  }
  else
  {
    name=NULL;
    value=NULL;
  }
}
//------------------------------------------------------------------------------ 
val_param_t::val_param_t(const val_param_t &s)
{
  if(s.name)
  {
    name = new char[strlen(s.name)+1];
    memcpy(name,s.name,strlen(s.name)+1);
  }
  if(s.value)
  {
    value = new char[strlen(s.value)+1];
    memcpy(value,s.value,strlen(s.value)+1);
  } 
}
//------------------------------------------------------------------------------  
val_param_t::~val_param_t()
{
  if(name) delete name;
  if(value) delete value;
}
//------------------------------------------------------------------------------
void val_param_t::operator = (const val_param_t &s)
{
  if(name) {delete name; name=NULL;}
  if(value) {delete value; value=NULL;}
  if(s.name)
  {
    name = new char[strlen(s.name)+1];
    memcpy(name,s.name,strlen(s.name)+1);
  }
  if(s.value)
  {
    value = new char[strlen(s.value)+1];
    memcpy(value,s.value,strlen(s.value)+1);
  }  
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
params_t::params_t()
{
  FilePath=NULL;
  param_list.clear();
}
//------------------------------------------------------------------------------
params_t::~params_t()
{
  if(FilePath) delete FilePath;
  param_list.clear();
}
//------------------------------------------------------------------------------
void params_t::init_params()
{
}
//------------------------------------------------------------------------------
int params_t::int_param_value(const char *name)
{
  list<val_param_t> :: iterator iter;
  for(iter = param_list.begin(); iter != param_list.end(); iter++)
    if(strcmp(name,iter->name)==0)
    {
      char *deb=iter->value;
      char *fin=&iter->value[strlen(iter->value)];
      return strtol(deb,&fin,10);
    }
  if(iter == param_list.end())
  {
    fprintf(stderr, "Paramètre manquant dans %s: %s\n",FilePath,name);
    exit(EXIT_FAILURE);
  }
  return 0;  
}
//------------------------------------------------------------------------------
float params_t::float_param_value(const char *name)
{
  list<val_param_t> :: iterator iter;
  for(iter = param_list.begin(); iter != param_list.end(); iter++)
    if(strcmp(name,iter->name)==0)
    {
      char *deb=iter->value;
      char *fin=&iter->value[strlen(iter->value)];
      return strtof(deb,&fin);
    }
  if(iter == param_list.end())
  {
    fprintf(stderr, "Paramètre manquant dans %s: %s\n",FilePath,name);
    exit(EXIT_FAILURE);
  }
  return 0.;
}
//------------------------------------------------------------------------------
double params_t::double_param_value(const char *name)
{
  list<val_param_t> :: iterator iter;
  for(iter = param_list.begin(); iter != param_list.end(); iter++)
    if(strcmp(name,iter->name)==0)
    {
      char *deb=iter->value;
      char *fin=&iter->value[strlen(iter->value)];
      return strtod(deb,&fin);
    }
  if(iter == param_list.end())
  {
    fprintf(stderr, "Paramètre manquant dans %s: %s\n",FilePath,name);
    exit(EXIT_FAILURE);
  }
  return 0.;
}
//------------------------------------------------------------------------------
char* params_t::string_param_value(const char *name)
{
  char* ptr;
  list<val_param_t> :: iterator iter;
  for(iter = param_list.begin(); iter != param_list.end(); iter++)
    if(strcmp(name,iter->name)==0)
    {
      char *deb=iter->value;
      char *fin=&iter->value[strlen(iter->value)];
      
      while(*fin==' ') fin--;
      fin++;
      ptr = new char[fin-deb+1];
      memcpy(ptr,deb,fin-deb);
      ptr[fin-deb]='\0';
      return ptr;
    }
  if(iter == param_list.end())
  {
    fprintf(stderr, "Paramètre manquant dans %s: %s\n",FilePath,name);
    exit(EXIT_FAILURE);
  }    
  return NULL;  
}
//------------------------------------------------------------------------------
void params_t::load_params(const char *FP)
{
  FilePath=new char[strlen(FP)+1];
  memcpy(FilePath,FP,strlen(FP)+1);
  FILE *File=fopen(FilePath,"rt+");
  if(File!=NULL)
  {
    list<val_param_t> :: iterator iter;
    int Len,Line;
    char Buff[1001],*PtrDeb,*Ptr;
    Ptr=Buff;
    Len=0;
    Line=1;
    while(!feof(File))
    {
      PtrDeb=Buff;
      Len+=fread(Ptr,1,1000-Len,File);
      if(feof(File)) Buff[Len]='\n';
      while((Ptr=(char*)memchr(PtrDeb,'\n',Len-(PtrDeb-Buff))))
      {
        while(*PtrDeb==' ') PtrDeb++;
        if(*PtrDeb!=';')
        {
          while(*PtrDeb==' ' || *PtrDeb=='\r' || *PtrDeb=='\t') PtrDeb++;
          if(PtrDeb!=Ptr)
          {
            char *PtrEqual=(char*)memchr(PtrDeb,'=',Ptr-PtrDeb);
            char *PtrFin;
            if(!PtrEqual)
            {
              fprintf(stderr, "Erreur de syntaxe dans %s, ligne %d\n",FilePath,Line);
              exit(EXIT_FAILURE);
            }
            PtrEqual++;
            PtrFin=PtrEqual;
            while(*PtrFin!=';' && PtrFin<Ptr) PtrFin++;
            if(*PtrFin=='\r') PtrFin--;
            
            for(iter = param_list.begin(); iter != param_list.end(); iter++)
              if(((unsigned int)(PtrEqual-PtrDeb-1))>=strlen(iter->name) && 
                memcmp(PtrDeb,iter->name,((unsigned int)(PtrEqual-PtrDeb-1)))==0)
                break;              
            // On n'a pas trouvé dans la liste des paramètres inattendus                  
            // On le rajoute
            if(iter == param_list.end())
            {
              val_param_t p(PtrDeb,PtrEqual-PtrDeb-1,PtrEqual,&PtrFin);
              if(PtrFin==PtrEqual)
              {
                fprintf(stderr, "Valeur de paramètre incorrecte dans %s, ligne %d\n",FilePath,Line);
                exit(EXIT_FAILURE);
              }
              param_list.push_front(p);
            }
            else
            {
              fprintf(stderr, "Paramètre déjà initialisé dans %s, ligne %d\n",FilePath,Line);
              exit(EXIT_FAILURE);              
            }
          }
        }
        Line++;
        PtrDeb=Ptr+1;
      }
      Len-=(PtrDeb-Buff);
      memcpy(Buff,PtrDeb,Len);
      Ptr=&Buff[Len];
    }
    fclose(File);
    init_params();
  }
  else
  {
    fprintf(stderr, "Impossible de charger le fichier %s\n",FilePath);
    exit(EXIT_FAILURE);
  }
}
//------------------------------------------------------------------------------
