#ifndef XML_H
#define XML_H

#include <qdom.h>

#define elementByName(name)  elementsByTagName(name).item(0).toElement()

class world_parser_t
{
  public: 
  QDomDocument doc;  

  static bool get_color(const QDomElement &color, float &R, float &G, float &B);
  static bool get_vector(const QDomElement &vect, float &X, float &Y, float &Z);
  static bool get_value(const QDomElement &value, float &V);  
  static bool get_element(const QDomElement &parent, const char *name, QDomElement &el);   
  
  void parse(const char *path);  
};

#endif
