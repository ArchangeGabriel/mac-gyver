#ifndef PARSE_PARAMSH
#define PARSE_PARAMSH

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<list>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

using std::list;

//------------------------------------------------------------------------------
class val_param_t
{
  public:
  char *name;
  char *value;
  
  val_param_t(char* name_deb, int name_len, char* val_deb, char** val_fin);
  val_param_t(const val_param_t &s);
  ~val_param_t();
  
  void operator = (const val_param_t &s);
};
//------------------------------------------------------------------------------
class params_t
{
  private:
  char *FilePath;
    
  virtual void init_params();

  public:
  params_t();
  virtual ~params_t();
  list<val_param_t> param_list;
  int    int_param_value(const char *name);
  float  float_param_value(const char *name);
  double double_param_value(const char *name);    
  char*  string_param_value(const char *name);      
  void load_params(const char *FP);
};
//------------------------------------------------------------------------------
#endif
