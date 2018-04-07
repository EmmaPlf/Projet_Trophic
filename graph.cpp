#include "graph.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <stack>

using namespace std;
/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
:m_presence(true)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Nombre de gens
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if(pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}

bool VertexInterface::getPresence() const
{
    return m_presence;
}

void VertexInterface::setPresence(bool presence)
{
    m_presence = presence;
}

/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }

    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    m_tool_box.add_child(asterix);
    asterix.set_frame(0,0,85,80);
    asterix.set_bg_color(VERTSOMBRE);

    m_tool_box.add_child(livre);
    livre.set_frame(0,80,85,80);
    livre.set_bg_color(BLANCROSE);

    m_tool_box.add_child(sauvegarder);
    sauvegarder.set_frame(0,160,85,80);
    sauvegarder.set_bg_color(JAUNESOMBRE);

    m_tool_box.add_child(ajouter);
    ajouter.set_frame(0,240,85,80);
    ajouter.set_bg_color(BLEUCLAIR);

    m_tool_box.add_child(supprimer);
    supprimer.set_frame(0,320,85,80);
    supprimer.set_bg_color(VERTCLAIR);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{

 m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
//    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
//    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
//    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
//    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
//    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
//    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
//    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
//    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
//    add_interfaced_edge(0, 1, 2, 50.0);
//    add_interfaced_edge(1, 0, 1, 50.0);
//    add_interfaced_edge(2, 1, 3, 75.0);
//    add_interfaced_edge(3, 4, 1, 25.0);
//    add_interfaced_edge(4, 6, 3, 25.0);
//    add_interfaced_edge(5, 7, 3, 25.0);
//    add_interfaced_edge(6, 3, 4, 0.0);
//    add_interfaced_edge(7, 2, 0, 100.0);
//    add_interfaced_edge(8, 5, 2, 20.0);
//    add_interfaced_edge(9, 3, 7, 80.0);
}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés // value nb individus
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        m_vertices.erase (idx);
        m_vertices.clear();

        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }


    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);

    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);

    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

void Graph::sauvegarder(std::string file_name)
{
    int x = 0, y = 0, id_sommet = 0, num_arete = 0, connexe=0, nb_individus = 0;
    std::string pic_name;

    ///OUVERTURE MODE LECTURE
    ofstream fichier(file_name, ios::out | ios::trunc);

    //OUVERTURE FONCTIONNE
    if(fichier)
    {
        fichier << m_vertices.size() << std::endl;

           std::map<int, Vertex>::iterator it;
           for(it = m_vertices.begin(); it != m_vertices.end(); it ++)
           {
                /// RECUPERE LES DONNEES
                x = it->second.m_interface->m_top_box.get_frame().pos.x;
                y= it->second.m_interface->m_top_box.get_frame().pos.y;
                nb_individus = it->second.m_value;
                pic_name = it->second.m_interface->m_img.get_pic_name();

                /// ECRIT LES DONNEES
                fichier << it->first << " ";
                fichier << x << " ";
                fichier << y << " ";
                fichier << nb_individus << " ";
                fichier << pic_name << std::endl;
           }

           fichier << m_edges.size() << std::endl;

           std::map<int, Edge>::iterator it2;
           for(it2 = m_edges.begin(); it2 != m_edges.end(); it2 ++)
               {
                    /// RECUPERE LES SOMMETS D ARETES
                    fichier << it2->first << " ";
                    fichier << it2->second.m_from << " ";
                    fichier << it2->second.m_to << std::endl;
               }

        fichier.close();
    }

    else
    {
        cerr <<"Impossible d'ouvrir le fichier" << std::endl;
        fichier.close();
        exit (0);
    }


}

void Graph::chargement_fichier(std::string file_name)
{
    int x = 0, y = 0, id_sommet = 0, num_arete = 0, connexe=0, s1=0, s2=0, nb_individus = 0;
    unsigned int ordre = 0, nb_aretes;
    std::string pic_name;

    m_vertices.clear();
    m_edges.clear();

    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    ///OUVERTURE MODE LECTURE
    ifstream fichier(file_name, ios::in);

    ///OUVERTURE FONCTIONNE
    if(fichier)
    {
        fichier >> ordre;

        /// REMPLIR LE VECTEUR DE SOMMETS
        for(int i=0; i<ordre; i++)
        {
            fichier >> id_sommet;
            fichier >> x;
            fichier >> y;
            fichier >> nb_individus;
            fichier >> pic_name;

            add_interfaced_vertex(id_sommet, nb_individus, x, y, pic_name, i);
        }

        fichier >> nb_aretes;
        std::cout << nb_aretes;

        // REMPLIR LE VECTEUR D ARETES
        for(int i=0; i<nb_aretes; i++)
        {
                fichier >> num_arete;
                fichier >> s1;
                fichier >> s2;

                add_interfaced_edge(num_arete, s1, s2, 0.0);
        }

        fichier.close();
    }

    else
    {
        cerr <<"Impossible d'ouvrir le fichier" << std::endl;
        fichier.close();
        exit (0);
    }
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        m_edges.erase (idx);
        m_edges.clear();

        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        m_edges.erase (id_vert1);
        m_edges.erase (id_vert2);

        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(id_vert2);
    m_vertices[id_vert2].m_in.push_back(id_vert1);
}

void Graph::ajouter_sommet(std::string file_name)
{
    std::string test_string="";
    int choix = 0;
    int cmp_emeu = 0, cmp_idefix = 0,cmp_gaulois=0, cmp_romain=0,cmp_sanglier=0,cmp_souris=0,cmp_gland=0,cmp_crocodile=0,cmp_chevre=0;
    int cmp_plante=0, cmp_organique=0, cmp_vers=0, cmp_potion=0, cmp_serpent=0, cmp_chat=0;

        std::cout << std::endl << "Choisir l'animal a ajouter: " << std::endl << std::endl;
            std::cout << "1) Poule" << std::endl << "2) Idefix " << std::endl<<"3) Gaulois "<<std::endl<<"4) Romain "<<std::endl<<"5) Sanglier"<<std::endl<<"6) Souris"<<std::endl<<"7) Gland"<<std::endl<<"😎 Crocodiles"<<std::endl;
            std::cout<<"9) Chevre"<<std::endl<<"10) Plante"<<std::endl<<"11) Organique"<<std::endl<<"12) Vers"<<std::endl<<"13) Potion"<<std::endl<<"14) Serpent"<<std::endl<<"15)Chat"<<std::endl;
        std::cin >> choix;

        std::map< int, Vertex >::iterator it;
        for (it = m_vertices.begin(); it != m_vertices.end(); ++it)
        {
            switch(choix)
            {
                case 1:
                        if( "Emeu.jpg" != it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_emeu++;
                        }
                        break;

                case 2:
                        if( "Idefix.jpg" != it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_idefix++;
                        }
                        break;
                case 3:
                        if("Gaulois.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_gaulois++;
                        }
                        break;
                case 4:
                        if("Romain.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_romain++;
                        }
                        break;
                 case 5:
                        if("Sanglier.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_sanglier++;
                        }
                        break;
                case 6:
                        if("Souris.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_souris++;
                        }
                        break;
                 case 7:
                        if("Gland.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_gland++;
                        }
                        break;
                case 8:
                        if("Crocodile.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_crocodile++;
                        }
                        break;
                case 9:
                        if("Chevre.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_chevre++;
                        }
                        break;
                case 10:
                        if("Plante.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_plante++;
                        }
                        break;
                case 11:
                        if("Organique.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_organique++;
                        }
                        break;
                case 12:
                        if("Vers.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_vers++;
                        }
                        break;
                case 13:
                        if("Potion.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_potion++;
                        }
                        break;
                case 14:
                        if("Potion.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_potion++;
                        }
                        break;
                case 15:
                        if("Potion.jpg"!= it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_potion++;
                        }
                        break;

            }
        }

            if(cmp_emeu == m_vertices.size())
            {
                  add_interfaced_vertex(12, 90, 200, 100, "Emeu.jpg",12);

                 add_interfaced_edge(14,12,10,20.0);
                // nb_morts--;
                // nb_aretes_mortes--;
            }

            else if(cmp_idefix == m_vertices.size())
            {
                 //add_interfaced_vertex(m_vertices.size(), 90, 200, 100, "Idefix.jpg",m_vertices.size());
                 add_interfaced_vertex(3, 90, 200, 100, "Idefix.jpg",3);

                 //add_interfaced_edge(m_edges.size(),m_vertices.size()-1,1,20.0);
                 add_interfaced_edge(15,3,2,20.0);
                // nb_morts--;
                // nb_aretes_mortes--;
            }
             else if(cmp_gaulois == m_vertices.size())
            {
               // std::cout << m_vertices.end()->first+nb_morts;
                //std::cout << nb_morts;
                 add_interfaced_vertex(0, 90, 200, 100, "Gaulois.jpg",0);

                 add_interfaced_edge(1,0,1,20.0);
                 add_interfaced_edge(2,0,2,20.0);
                 add_interfaced_edge(13,0,11,20.0);
                 /*add_interfaced_edge(m_edges.end()->first+nb_aretes_mortes+1,m_vertices.end()->first+nb_morts,2,20.0);
                 add_interfaced_edge(m_edges.end()->first+nb_aretes_mortes+1,m_vertices.end()->first+nb_morts,11,20.0);*/

                 // nb_morts--;
                  //nb_aretes_mortes--;
            }

            else if(cmp_romain == m_vertices.size())
            {
                add_interfaced_vertex(1, 90, 200, 100, "Romain.jpg",1);

                 add_interfaced_edge(1,0,1,20.0);
                 add_interfaced_edge(4,1,2,20.0);
                 add_interfaced_edge(3,1,6,20.0);
                 add_interfaced_edge(9,
                                     6,1,20.0);
                 add_interfaced_edge(14,12,1,20.0);
                 // add_interfaced_edge(m_edges.end()->first+nb_aretes_mortes+1,m_vertices.end()->first+nb_morts,6,20.0);///arete vers croco
                 // add_interfaced_edge(m_edges.end()->first+nb_aretes_mortes+1,0,m_vertices.end()->first+nb_morts,20.0);
                 // nb_morts--;
                 // nb_aretes_mortes--;
            }
             else if(cmp_sanglier == m_vertices.size())
            {
                add_interfaced_vertex(2, 90, 200, 100, "Sanglier.jpg",2);

                 //add_interfaced_edge(m_edges.end()->first+nb_aretes_mortes+1,m_vertices.end()->first+nb_morts,7,20.0);
                  add_interfaced_edge(6,2,5,20.0);
                   add_interfaced_edge(5,2,4,20.0);
                    add_interfaced_edge(4,1,2,20.0);
                    add_interfaced_edge(2,0,2,20.0);
                 // nb_morts--;
                 // nb_aretes_mortes--;
            }
             else if(cmp_souris == m_vertices.size())
            {
                add_interfaced_vertex(4, 90, 200, 100, "Souris.jpg",4);

                 add_interfaced_edge(5,2,4,20.0);

            }
            else if(cmp_gland == m_vertices.size())
            {
                add_interfaced_vertex(5, 90, 200, 100, "Gland.jpg",5);

                 add_interfaced_edge(6,2,5,20.0);

            }
             else if(cmp_crocodile == m_vertices.size())
            {
                add_interfaced_vertex(6, 90, 200, 100, "Crocodile.jpg",6);

                 add_interfaced_edge(9,6,1,20.0);
                 add_interfaced_edge(3,1,6,20.0);
                 add_interfaced_edge(8,6,7,20.0);

            }
            else if(cmp_chevre == m_vertices.size())
            {
                add_interfaced_vertex(7, 90, 200, 100, "Chevre.jpg",7);

                 add_interfaced_edge(0,7,10,20.0);
                 add_interfaced_edge(10,7,8,20.0);
                 add_interfaced_edge(8,6,7,20.0);

            }
            else if(cmp_plante == m_vertices.size())
            {
                add_interfaced_vertex(8, 90, 200, 100, "Plante.jpg",8);

                 add_interfaced_edge(10,7,8,20.0);

            }
            else if(cmp_organique == m_vertices.size())
            {
                add_interfaced_vertex(9, 90, 200, 100, "Organique.jpg",9);

                 add_interfaced_edge(12,10,9,20.0);
                 add_interfaced_edge(11,8,9,20.0);

            }
             else if(cmp_vers == m_vertices.size())
            {
                add_interfaced_vertex(10, 90, 200, 100, "Vers.jpg",10);

                 add_interfaced_edge(0,7,10,20.0);
                 add_interfaced_edge(12,10,9,20.0);
                 add_interfaced_edge(14,12,10,20.0);

            }
             else if(cmp_potion == m_vertices.size())
            {
                add_interfaced_vertex(11, 90, 200, 100, "Potion.jpg",11);

                 add_interfaced_edge(13,0,11,20.0);

            }


            else std::cout << "L'espece selectionnee est deja presente" << std::endl;




        sauvegarder(file_name);
        chargement_fichier(file_name);
        update();

}

void Graph::ajouter_sommet_graph2(std::string file_name)
{
     std::string test_string="";
    int choix = 0;
    int cmp_kaa = 0, cmp_mowgli = 0,cmp_poisson=0, cmp_banane=0,cmp_junior=0;
    int cmp_singe=0, cmp_emeu=0;
    std::cout << std::endl << "Choisir l'animal a ajouter: " << std::endl << std::endl;
            std::cout << "1) Kaa" << std::endl << "2) Mowgli " << std::endl<<"3) POisson "<<std::endl<<"4) Banane "<<std::endl<<"5) Junior"<<std::endl<<"6) SInge"<<std::endl<<"7) Emeu"<<std::endl;
        std::cin >> choix;
     std::map< int, Vertex >::iterator it;
        for (it = m_vertices.begin(); it != m_vertices.end(); ++it)
        {
            switch(choix)
            {
                case 1:
                        if( "Kaa.jpg" != it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_kaa++;
                        }
                        break;
                 case 2:
                        if( "Mowgli.jpg" != it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_mowgli++;
                        }
                        break;
                 case 3:
                        if( "Poisson.jpg" != it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_poisson++;
                        }
                        break;
                 case 4:
                        if( "Banane.jpg" != it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_banane++;
                        }
                        break;
                 case 5:
                        if( "Junior.jpg" != it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_junior++;
                        }
                        break;
                 case 6:
                        if( "Singe.jpg" != it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_singe++;
                        }
                        break;
                 case 7:
                        if( "Emeu.jpg" != it->second.m_interface->m_img.get_pic_name())
                        {
                           cmp_emeu++;
                        }
                        break;

            }
        }
    if(cmp_kaa == m_vertices.size())
            {
                  add_interfaced_vertex(0, 90, 200, 100, "Kaa.jpg",0);

                 add_interfaced_edge(1,0,3,20.0);
                 add_interfaced_edge(0,0,1,20.0);
                 add_interfaced_edge(2,0,6,20.0);
                 add_interfaced_edge(7,0,4,20.0);
                   add_interfaced_edge(10,7,0,20.0);

                // nb_morts--;
                // nb_aretes_mortes--;
            }
    else if(cmp_mowgli == m_vertices.size())
            {
                add_interfaced_vertex(1, 90, 200, 100, "Mowgli.jpg",1);

                 add_interfaced_edge(4,1,4,20.0);
                 add_interfaced_edge(3,1,3,20.0);
                 add_interfaced_edge(0,0,1,20.0);


            }
    else if(cmp_poisson == m_vertices.size())
            {
                add_interfaced_vertex(3, 90, 200, 100, "Poisson.jpg",3);


                 add_interfaced_edge(3,1,3,20.0);
                 add_interfaced_edge(1,0,3,20.0);


            }
    else if(cmp_banane == m_vertices.size())
            {
                add_interfaced_vertex(4, 90, 200, 100, "Banane.jpg",4);

                 add_interfaced_edge(4,1,4,20.0);
                 add_interfaced_edge(8,5,4,20.0);
                 add_interfaced_edge(7,0,4,20.0);
                 add_interfaced_edge(9,6,4,20.0);


            }
     else if(cmp_junior == m_vertices.size())
            {
                add_interfaced_vertex(5, 90, 200, 100, "Junior.jpg",5);

                 add_interfaced_edge(8,5,4,20.0);
            }
    else if(cmp_singe == m_vertices.size())
            {
                add_interfaced_vertex(6, 90, 200, 100, "Singe.jpg",6);

                 add_interfaced_edge(2,0,6,20.0);
                 add_interfaced_edge(9,6,4,20.0);
            }
    else if(cmp_emeu == m_vertices.size())
            {
                add_interfaced_vertex(7, 90, 200, 100, "Emeu.jpg",7);
                add_interfaced_edge(10,7,0,20.0);
            }
}

void Graph::supprimer_sommet(int id_sommet, std::string file_name)
{

    std::vector<int> ind_aretes;

    m_interface->m_main_box.remove_child(m_vertices[id_sommet].m_interface->m_top_box);
    m_vertices.erase(id_sommet);

    std::map<int, Edge>:: iterator it;
    for(it=m_edges.begin(); it!=m_edges.end(); it++)
    {
        if(it->second.m_to == id_sommet || it->second.m_from == id_sommet)
        {
            ind_aretes.push_back(it->first); // Pas le choix sinon il parcours des artes qui ont été supprimées auparavant
            m_interface->m_main_box.remove_child(it->second.m_interface->m_top_edge); // on supprime de l'interface
        }
    }

    if(ind_aretes.size()!=0)
    {
        for(int i=0; i<ind_aretes.size(); i++)
        {
            m_edges.erase(ind_aretes[i]);
        }
    }

    sauvegarder(file_name);
    chargement_fichier(file_name);
    update();
}

void Graph::dynamique(std::string file_name)
{
    int var_temp=0;

    std::map<int, Vertex>::iterator it;
    for(it = m_vertices.begin(); it != m_vertices.end(); it++)
    {
        if(it->second.m_value > 2)
        {
            std::map<int, Edge>::iterator it2;
            for(it2 = m_edges.begin(); it2 != m_edges.end(); it2++)
            {
                if(it2->second.m_from == it->first)
                {
                    var_temp = it->second.m_value + 0.1*it->second.m_value*(1-(it->second.m_value/m_vertices[it2->second.m_to].m_value));

                    if(var_temp < 0)
                    var_temp = 0;
                    else if (var_temp >= 100)
                    var_temp = 100;
                    else
                    it->second.m_value = var_temp;
                }

                if(it2->second.m_to == it->first)
                {
                    var_temp = var_temp - (0.01*m_vertices[it2->second.m_from].m_value);

                    if(var_temp < 0)
                    var_temp = 0;
                    else if (var_temp >= 100)
                    var_temp = 100;
                    else
                    it->second.m_value = var_temp;
                }
            }
         }
     }

    sauvegarder(file_name);
    chargement_fichier(file_name);
    update();
}

void Graph::connexite()
{
    int taille_sic = 0;
    std::vector<int> sic;

    // REINITIALISER LES MARQUAGES
         std::map<int, Vertex>::iterator it2;
         for(it2 = m_vertices.begin(); it2 != m_vertices.end(); it2++)
         {
             it2->second.setMarque(false);
         }

   /// ON PARCOURS CHAQUE SOMMETS
   std::map<int, Vertex>::iterator it;
   for(it = m_vertices.begin(); it != m_vertices.end(); it++)
    {
        DFS(it->first);
    }
}

void Graph::DFS(int id_depart)
{
    std::stack<int> pile;
    bool connexe_externe = false, connexe_interne = false;
    int id_sommet = 0, cmp_connexite=0;
    std::vector<int> vec_connexite, vec_externe, vec_interne;
    BITMAP* fond = create_bitmap(908,720);

         /// REINITIALISER LES MARQUAGES
         std::map<int, Vertex>::iterator it;
         for(it = m_vertices.begin(); it != m_vertices.end(); it++)
         {
             it->second.setMarque(false);
         }

         /// ENFILE ET MARQUE LE SOMMET DE DEPART
         pile.push(id_depart);
         m_vertices[id_depart].setMarque(true);
         id_sommet = id_depart;
         vec_externe.push_back(id_depart);

            /// TANT QUE CONNEXITE PAS COMPLETE
            while(connexe_externe != true)
            {

                std::map<int, Edge>::iterator it;
                for(it = m_edges.begin(); it != m_edges.end(); it++)
                {
                   // SI SOMMET ADJACENT NON MARQUE EXTERNE
                   if(it->second.m_from == id_sommet && m_vertices[it->second.m_to].getMarque() == false)
                   {
                      // ON ENFILE L INDICE DE L ADJACENT
                        pile.push(it->second.m_to);
                        //std::cout << m_sommets[id_sommet].m_adjacents[i];

                        // ON MARQUE L ADJACENT
                        m_vertices[it->second.m_to].setMarque(true);
                   }
                 }

                // ON RECUPE LE SOMMET EN TETE DE PILE
                id_sommet = pile.top();
                vec_externe.push_back(pile.top());

                // ON DEPILE LE PREMIER SOMMET
                pile.pop();

                 // SI FILE VIDE ALORS CONNEXITE
                if(pile.size() == 0)
                {
                    connexe_externe = true;
                }
            }


         /// REINITIALISER LES MARQUAGES
         for(std::map<int, Vertex>::iterator it = m_vertices.begin(); it != m_vertices.end(); it++)
         {
             it->second.setMarque(false);
         }

         /// ENFILE ET MARQUE LE SOMMET DE DEPART
         pile.push(id_depart);
         m_vertices[id_depart].setMarque(true);
         id_sommet = id_depart;
         vec_interne.push_back(id_depart);

            /// TANT QUE CONNEXITE PAS COMPLETE
            while(connexe_interne != true)
            {
                std::map<int, Edge>::iterator it;
                for(it = m_edges.begin(); it != m_edges.end(); it++)
                {
                   // SI SOMMET ADJACENT NON MARQUE EXTERNE
                   if(it->second.m_to == id_sommet && m_vertices[it->second.m_from].getMarque() == false)
                    {
                         // ON ENFILE L INDICE DE L ADJACENT
                        pile.push(it->second.m_from);
                        //std::cout << m_sommets[id_sommet].m_adjacents[i];

                        // ON MARQUE L ADJACENT
                        m_vertices[it->second.m_from].setMarque(true);
                    }
                 }

                // ON RECUPE LE SOMMET EN TETE DE PILE
                id_sommet = pile.top();
                vec_interne.push_back(pile.top());

                // ON DEPILE LE PREMIER SOMMET
                pile.pop();

                 // SI FILE VIDE ALORS CONNEXITE
                if(pile.size() == 0)
                {
                    connexe_interne = true;
                }
            }

            /// REMPLISSAGE DES SOMMETS CONNEXES
            for(int i=0; i<vec_externe.size(); i++)
            {
                for(int j=0; j<vec_interne.size(); j++)
                {
                    if(vec_externe[i] == vec_interne[j])
                    {
                        for(int k=0; k<vec_connexite.size(); k++)
                        {
                            if(vec_externe[i] != vec_connexite[k])
                                cmp_connexite++;
                        }

                        if(cmp_connexite == vec_connexite.size())
                        {
                            vec_connexite.push_back(vec_externe[i]);
                            cmp_connexite=0;
                        }
                    }
                }
            }

            /// COLORATION
            for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); it++)
            {
                for(int j=0; j<vec_connexite.size(); j++)
                {
                    for(int k=0; k<vec_connexite.size(); k++)
                     {
                         if(it->second.m_to == vec_connexite[j] && it->second.m_from == vec_connexite[k])
                         {
                             it->second.m_interface->m_top_edge.setColor(ROUGESOMBRE);
                         }
                      }
                 }
             }

//             blit(fond,grman::page,0,0,0,0,908,720);
//             for(int i=0; i<vec_connexite.size(); i++)
//             {
//                 for(int j=0; j<vec_connexite.size(); j++)
//                 {
//                     textprintf(fond, font, m_vertices[vec_connexite[i]].m_interface->m_top_box.get_frame().pos.x, m_vertices[vec_connexite[i]].m_interface->m_top_box.get_frame().pos.y, makecol(0,0,0),"%d",vec_connexite[i]);
//
//                     for(std::map<int, Edge>::iterator it = m_edges.begin(); it != m_edges.end(); it++)
//                     {
//                        if(it->second.m_to == i && it->second.m_from == j);
//                        line(fond, m_vertices[vec_connexite[i]].m_interface->m_top_box.get_frame().pos.x, m_vertices[vec_connexite[i]].m_interface->m_top_box.get_frame().pos.y,  m_vertices[vec_connexite[j]].m_interface->m_top_box.get_frame().pos.x, m_vertices[vec_connexite[j]].m_interface->m_top_box.get_frame().pos.y, makecol(255,0,0));
//                     }
//                  }
//             }
}

 std::shared_ptr<GraphInterface>& Graph::getInterface()
 {
     return m_interface;
 }

grman::WidgetButton& GraphInterface::getAsterix()
{
    return asterix;
}

grman::WidgetButton& GraphInterface::getLivre()
{
    return livre;
}
