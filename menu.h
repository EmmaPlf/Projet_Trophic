#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "grman/grman.h"
#include "grman/widget.h"
class Menu
{ public:
Menu();
~Menu();
    grman::WidgetBox m_box_global;
    grman::WidgetButton m_bouton_image;
    grman::WidgetImage m_image;
    grman::WidgetButton m_bouton_graphe;
    grman::WidgetButton m_bouton_quitter;
    grman::WidgetImage m_image_graphe;
    grman::WidgetImage m_image_quitter;
    void load();
    int display();
};

#endif // MENU_H_INCLUDED
