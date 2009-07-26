#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <typeinfo>
#include "utils.hpp"

const size_t WINDOW_W=800;
const size_t WINDOW_H=600;

const float FIELD_OF_VISION_Y=40.;// 40°
const float COLUMN_ELEMENT_H=0.03;// 3cm
const float COLUMN_ELEMENT_R=0.035;// 3.5cm
const float TABLE_WIDTH=3.;// 3m
const float TABLE_HEIGHT=2.1;// 2.1m
const float CAMERA_HEIGHT=0.1;// 10cm

const float TABLE_R=23.f/255.f;
const float TABLE_G=97.f/255.f;
const float TABLE_B=171.f/255.f;

const float COLUMN_RED_R=199.f/255.f;
const float COLUMN_RED_G=23.f/255.f;
const float COLUMN_RED_B=18.f/255.f;

const float BORDER_HEIGHT=0.07;// 7cm

const float BORDER_R=1.f;
const float BORDER_G=1.f;
const float BORDER_B=1.f;

class object
{
    public:
    object(float x,float y,float z):x(x),y(y),z(z),rx(0),ry(0),rz(0),ro1(2),ro2(0),ro3(1){}
    virtual ~object(){}

    virtual void draw() const = 0;
    virtual std::string dump() const = 0;

    void set_r(float _r){r=_r;}
    float get_r() const {return r;}
    void set_g(float _g){g=_g;}
    float get_g() const {return g;}
    void set_b(float _b){b=_b;}
    float get_b() const {return b;}
    void set_color(float _r,float _g,float _b){r=_r;g=_g;b=_b;}

    void set_x(float _x){x=_x;}
    float get_x() const{ return x;}
    void set_y(float _y){y=_y;}
    float get_y() const {return y;}
    void set_z(float _z){z=_z;}
    float get_z() const {return z;}
    void set_pos(float _x,float _y,float _z){x=_x;y=_y;z=_z;}

    // rotation order: 0 is X,1 is Y,2 is Z
    // default: Z X Y
    void set_rot_order(int first,int second,int third)
    {
        assert(x!=y && x!=z && y!=z && x>=0 && x<=2 && y>=0 && y<=2 && z>=0 && z<=2);
        ro1=first;ro2=second;ro3=third;
    }
    int get_rot_order(int idx) const
    {
        switch(idx)
        {
            case 0:return ro1;
            case 1:return ro2;
            case 2:return ro3;
            default:assert(0);return 0;
        }
    }
    void set_rot_x(float _rx){rx=_rx;}
    float get_rot_x() const {return rx;}
    void set_rot_y(float _ry){ry=_ry;}
    float get_rot_y() const {return ry;}
    void set_rot_z(float _rz){rz=_rz;}
    float get_rot_z() const {return rz;}
    void set_rot(float _rx,float _ry,float _rz){rx=_rx;ry=_ry;rz=_rz;}
    int get_rot_idx(int i)
    {
        switch(i)
        {
            case 0:return get_rot_x();
            case 1:return get_rot_y();
            case 2:return get_rot_z();
            default:assert(0);return 0;
        }
    }


    protected:
    float x,y,z;
    float r,g,b;
    float rx,ry,rz;
    float ro1,ro2,ro3;
};

class cylinder : public object
{
    public:
    cylinder(float x,float y,float z,float h,float r):object(x,y,z),h(h),r(r){}
    virtual ~cylinder(){}

    virtual void draw() const
    {
        // bottom
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.f,0.f,-h/2.);
        for(unsigned int i=0;i<=tesselation;i++)
        {
            glNormal3f(cos(2*M_PI*i/tesselation),sin(2*M_PI*i/tesselation),0.0);
            glVertex3f(r*cos(2*M_PI*i/tesselation),r*sin(2*M_PI*i/tesselation),-h/2.);
        }
        glEnd();
        // top
        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(0.f,0.f,h/2.);
        for(unsigned int i=0;i<=tesselation;i++)
        {
            glNormal3f(cos(2*M_PI*i/tesselation),sin(2*M_PI*i/tesselation),0.0);
            glVertex3f(r*cos(2*M_PI*i/tesselation),r*sin(2*M_PI*i/tesselation),h/2.);
        }
        glEnd();
        // side
        glBegin(GL_TRIANGLE_STRIP);
        for(unsigned int i=0;i<=tesselation;i++)
        {
            glNormal3f(cos(2*M_PI*i/tesselation),sin(2*M_PI*i/tesselation),0.0);
            glVertex3f(r*cos(2*M_PI*i/tesselation),r*sin(2*M_PI*i/tesselation),-h/2.);
            glVertex3f(r*cos(2*M_PI*i/tesselation),r*sin(2*M_PI*i/tesselation),h/2.);
        }
        glEnd();
    }

    virtual std::string dump() const
    {
        std::ostringstream oss;
        oss<<"+-cylinder:\n";
        oss<<"  +-pos(center)=("<<x<<","<<y<<","<<z<<")\n";
        oss<<"  +-radius="<<r<<"\n";
        oss<<"  +-height="<<h<<"\n";
        return oss.str();
    }

    protected:
    float h,r;
    static const unsigned int tesselation=40;
};

class xyplane : public object
{
    public:
    xyplane(float x,float y,float z,float w,float h):object(x,y,z),w(w),h(h){}
    virtual ~xyplane(){}

    virtual void draw() const
    {
        glBegin(GL_QUADS);
        glNormal3f(0.0,0.0,1.0f);
        glVertex3f( w/2., h/2.,0);
        glVertex3f(-w/2., h/2.,0);
        glVertex3f(-w/2.,-h/2.,0);
        glVertex3f( w/2.,-h/2.,0);
        glEnd();
    }

    virtual std::string dump() const
    {
        std::ostringstream oss;
        oss<<"+-xyplane:\n";
        oss<<"  +-pos(center)=("<<x<<","<<y<<","<<z<<")\n";
        oss<<"  +-width="<<w<<"\n";
        oss<<"  +-height="<<h<<"\n";
        return oss.str();
    }

    protected:
    float w,h;
};

struct light_t
{
    float pos[3];
    float ambiant[4];
    float diffuse[4];
};

std::vector<object *> objects;
std::vector<light_t> lights;
float cam_x,cam_y,cam_z;
float cam_angle;

void create_scene()
{
    object *obj=
        new cylinder(
            TABLE_WIDTH/4.f,TABLE_HEIGHT/4.f,COLUMN_ELEMENT_H/2.0f,
            COLUMN_ELEMENT_H,COLUMN_ELEMENT_R);
    obj->set_color(COLUMN_RED_R,COLUMN_RED_G,COLUMN_RED_B);
    objects.push_back(obj);

    obj=
        new cylinder(
            TABLE_WIDTH/4.f,TABLE_HEIGHT/4.f,3.1*COLUMN_ELEMENT_H/2.0f,
            COLUMN_ELEMENT_H,COLUMN_ELEMENT_R);
    obj->set_color(COLUMN_RED_R,COLUMN_RED_G,COLUMN_RED_B);
    objects.push_back(obj);

    obj=
        new xyplane(
            0.f,0.f,0.f,
            TABLE_WIDTH,TABLE_HEIGHT);
    obj->set_color(TABLE_R,TABLE_G,TABLE_B);
    objects.push_back(obj);

    obj=
        new xyplane(
            0.f,TABLE_HEIGHT/2.,BORDER_HEIGHT/2,
            TABLE_WIDTH,BORDER_HEIGHT);
    obj->set_rot_x(90.f);
    obj->set_color(BORDER_R,BORDER_G,BORDER_B);
    objects.push_back(obj);

    obj=
        new xyplane(
            0.f,-TABLE_HEIGHT/2.,BORDER_HEIGHT/2,
            TABLE_WIDTH,BORDER_HEIGHT);
    obj->set_rot_x(-90.f);
    obj->set_color(BORDER_R,BORDER_G,BORDER_B);
    objects.push_back(obj);

    obj=
        new xyplane(
            TABLE_WIDTH/2.f,0,BORDER_HEIGHT/2,
            BORDER_HEIGHT,TABLE_HEIGHT);
    obj->set_rot_y(-90.f);
    obj->set_color(BORDER_R,BORDER_G,BORDER_B);
    objects.push_back(obj);

    obj=
        new xyplane(
            -TABLE_WIDTH/2.f,0,BORDER_HEIGHT/2,
            BORDER_HEIGHT,TABLE_HEIGHT);
    obj->set_rot_y(90.f);
    obj->set_color(BORDER_R,BORDER_G,BORDER_B);
    objects.push_back(obj);

    cam_x=0.;
    cam_y=0;
    cam_z=CAMERA_HEIGHT;
    cam_angle=45;

    light_t l0=
    {
        {TABLE_WIDTH/2,TABLE_HEIGHT/2,3.0},// position
        {0.5,0.5,0.5,1.0},// ambiant
        {0.7,0.7,0.7,1.0} // diffuse
    };
    light_t l1=
    {
        {-TABLE_WIDTH/2,TABLE_HEIGHT/2,3.0},// position
        {0.5,0.5,0.5,1.0},// ambiant
        {0.7,0.7,0.7,1.0} // diffuse
    };
    light_t l2=
    {
        {-TABLE_WIDTH/2,-TABLE_HEIGHT/2,3.0},// position
        {0.5,0.5,0.5,1.0},// ambiant
        {0.7,0.7,0.7,1.0} // diffuse
    };
    light_t l3=
    {
        {TABLE_WIDTH/2,-TABLE_HEIGHT/2,3.0},// position
        {0.5,0.5,0.5,1.0},// ambiant
        {0.7,0.7,0.7,1.0} // diffuse
    };

    lights.push_back(l0);
    lights.push_back(l1);
    lights.push_back(l2);
    lights.push_back(l3);
}

int main(int argc,char **argv)
{
    if(!init_utils())
    {
        std::cerr<<"Couldn't initialize utils library"<<std::endl;
        return EXIT_FAILURE;
    }
    atexit(deinit_utils);

    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        std::cerr<<"SDL_Init failed: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }
    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,2);
    SDL_Surface* screen=SDL_SetVideoMode(WINDOW_W,WINDOW_H,16,SDL_OPENGL);
    if(!screen)
    {
        std::cerr<<"SDL_SetVideoMode failed: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    create_scene();

    glViewport(0,0,WINDOW_W,WINDOW_H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VISION_Y,float(WINDOW_W)/float(WINDOW_H),0.01,10.);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE_ARB);

    for(size_t i=0;i<lights.size();i++)
    {
        glLightfv(GL_LIGHT0+i,GL_AMBIENT,&lights[i].ambiant[0]);
        glLightfv(GL_LIGHT0+i,GL_DIFFUSE,&lights[i].diffuse[0]);
        glLightfv(GL_LIGHT0+i,GL_POSITION,&lights[i].pos[0]);
        glEnable(GL_LIGHT0+i);
    }
    if(lights.size()!=0)
        glEnable(GL_LIGHTING);

    bool quit=false;
    while(!quit)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    quit=true;
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym==SDLK_ESCAPE)
                        quit=true;
                    break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        gluLookAt(
            cam_x,cam_y,cam_z,
            cam_x+cos(cam_angle/180.*M_PI),cam_y+sin(cam_angle/180.*M_PI),cam_z,
            0.,0.,1.);

        for(size_t i=0;i<objects.size();i++)
        {
            glPushMatrix();
            glTranslatef(objects[i]->get_x(),objects[i]->get_y(),objects[i]->get_z());
            for(int r=2;r>=0;r--)
                glRotatef(
                    objects[i]->get_rot_idx(objects[i]->get_rot_order(r)),
                    objects[i]->get_rot_order(r) == 0 ? 1.f : 0.f,
                    objects[i]->get_rot_order(r) == 1 ? 1.f : 0.f,
                    objects[i]->get_rot_order(r) == 2 ? 1.f : 0.f);

            GLfloat col[4]={objects[i]->get_r(),objects[i]->get_g(),objects[i]->get_b(),1.0f};
            GLfloat amb[4]={objects[i]->get_r()/4,objects[i]->get_g()/4,objects[i]->get_b()/4,1.0f};
            glColor4fv(col);
            glMaterialfv(GL_FRONT,GL_AMBIENT,amb);
            glMaterialfv(GL_FRONT,GL_DIFFUSE,col);
            objects[i]->draw();
            glPopMatrix();
        }

        glPopMatrix();
        SDL_GL_SwapBuffers();
    }

    image_t img(image_t::rgb_format,WINDOW_W,WINDOW_H);
    glReadPixels(0,0,WINDOW_W,WINDOW_H,GL_RGB,
        sizeof(image_t::pixel_type_t)==1 ? GL_UNSIGNED_BYTE :
        sizeof(image_t::pixel_type_t)==2 ? GL_UNSIGNED_SHORT :
        GL_UNSIGNED_INT,img.data);

    save_image(img,"generated.png");
    for(size_t i=0;i<objects.size();i++)
    {
        // horrible !
        if(typeid(*objects[i])==typeid(cylinder))
            std::cout<<objects[i]->dump();
    }
    std::cout<<"+-camera\n";
    std::cout<<"  +-pos=("<<cam_x<<","<<cam_y<<","<<cam_z<<")\n";
    std::cout<<"  +-fovx="<<(FIELD_OF_VISION_Y/WINDOW_H*WINDOW_W)<<"\n";
    std::cout<<"  +-fovy="<<FIELD_OF_VISION_Y<<"\n";
    std::cout<<"  +-w="<<WINDOW_W<<"\n";
    std::cout<<"  +-h="<<WINDOW_H<<"\n";
    std::cout<<"  +-angle="<<cam_angle<<"\n";

    for(size_t i=0;i<objects.size();i++)
        delete objects[i];

    return EXIT_SUCCESS;
}
