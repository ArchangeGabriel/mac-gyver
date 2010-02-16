#include <map>
#include <QFile>

#include "xml.h"



/*----------------------------------------------------------------------------*/
bool world_parser_t::get_color(const QDomElement &color, float &R, float &G, float &B)
{
  if(color.hasAttribute("name"))
  {
    if(color.attribute("name") == "black")
    {
      R = 0; G = 0; B = 0;
    }
    else if(color.attribute("name") == "blue")
    {
      R = 0; G = 0; B = 1;
    }
    else if(color.attribute("name") == "cyan")
    {
      R = 0; G = 1; B = 1;
    }
    else if(color.attribute("name") == "darkgray")
    {
      R = 0.25; G = 0.25; B = 0.25;
    }
    else if(color.attribute("name") == "gray")
    {
      R = 0.5; G = 0.5; B = 0.5;
    }
    else if(color.attribute("name") == "green")
    {
      R = 0; G = 1; B = 0;
    }
    else if(color.attribute("name") == "lightgray")
    {
      R = 0.75; G = 0.75; B = 0.75;
    }  
    else if(color.attribute("name") == "magenta")
    {
      R = 1; G = 0; B = 1;
    }
    else if(color.attribute("name") == "orange")
    {
      R = 1; G = 0.5; B = 0;
    }
    else if(color.attribute("name") == "pink")
    {
      R = 1; G = 0.5; B = 1;
    }
    else if(color.attribute("name") == "red")
    {
      R = 1; G = 0; B = 0;
    }
    else if(color.attribute("name") == "white")
    {
      R = 1; G = 1; B = 1;
    }    
    else if(color.attribute("name") == "yellow")
    {
      R = 1; G = 1; B = 0;
    }                                        
    else
      return false;
  }
  else
  {
    if(color.hasAttribute("r") && color.hasAttribute("g") && color.hasAttribute("b"))
    {
      R = color.attribute("r").toFloat();
      G = color.attribute("g").toFloat();
      B = color.attribute("b").toFloat();  
    }
    else
      return false;
  }  
  return true;  
}
/*----------------------------------------------------------------------------*/
bool world_parser_t::get_vector(const QDomElement &vect, float &X, float &Y, float &Z)
{
  if(vect.hasAttribute("x") && vect.hasAttribute("y") && vect.hasAttribute("z"))
  {
    X = vect.attribute("x").toFloat();
    Y = vect.attribute("y").toFloat();
    Z = vect.attribute("z").toFloat();  
    return true;
  }
  else
    return false;
}
/*----------------------------------------------------------------------------*/
bool world_parser_t::get_value(const QDomElement &value, float &V)
{
  if(value.hasAttribute("value"))
  {
    V = value.attribute("value").toFloat();
    return true;
  }
  else
    return false;
}
/*----------------------------------------------------------------------------*/
bool world_parser_t::get_element(const QDomElement &parent, const char *name, QDomElement &el)
{
  QDomNodeList nodes = parent.elementsByTagName(name);
  if(nodes.count() == 0)
    return false;
  else
  {
    el = nodes.item(0).toElement();
    return true;
  }
}
/*----------------------------------------------------------------------------*/
void world_parser_t::parse(const char *path)
{
  QFile file(path);
  if(!file.open(QIODevice::ReadOnly))
    return;
  if (!doc.setContent(&file))
  {
     file.close();
     return;
  }
  file.close();
}
/*----------------------------------------------------------------------------*/
