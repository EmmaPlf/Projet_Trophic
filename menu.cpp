#include "menu.h"
#include "grman/grman.h"
#include "grman/widget.h"
Menu::Menu(){}

Menu::~Menu(){}

void Menu::load()
{
    m_box_global.set_frame(0,0,1000,770);
    m_box_global.set_bg_color(GRIS);


    m_box_global.add_child(m_image);
    m_image.set_pic_name("menu.jpg");
    m_image.set_gravity_x(grman::GravityX::Left);


    m_box_global.add_child(m_bouton_quitter);
    m_bouton_quitter.set_frame(400,690,231,85);


    m_box_global.add_child(m_bouton_graphe);
    m_bouton_graphe.set_frame(400,90,231,85);


    m_bouton_quitter.add_child(m_image_quitter);
    m_image_quitter.set_pic_name("quitter.jpg");



    m_bouton_graphe.add_child(m_image_graphe);
    m_image_graphe.set_pic_name("bouton_graphe.jpg");

}
int Menu::display()
{
    m_box_global.update();
    if (m_bouton_quitter.clicked())
        {exit(0);}
    if(m_bouton_graphe.clicked())
    return 1;
}
