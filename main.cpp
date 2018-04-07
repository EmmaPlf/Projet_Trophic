#include "grman/grman.h"
#include <iostream>
#include "menu.h"
#include "graph.h"

//BITMAP* charger = load_bitmap("Charger.bmp", NULL);

int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// CHARGEMENT DU GRAPHE
    Graph g;
    g.make_example();

    /// CHARGEMENT DU MENU
    Menu m;
    m.load();

    bool asterix = false;
    bool livre = false;
    bool play = false;
    int id_sommet = 0;
    int compteur_emeu=0;
    int cmp = 0;

    while(m.display()!=1)
    {
        m.display();
        grman::mettre_a_jour();
    }


    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();


//        if(cmp == 50 && livre == false)
//        {
//            g.dynamique("Asterix.txt");
//            cmp = 0;
//        }
//
//        if(cmp == 50 && asterix == false)
//        {
//            g.dynamique("Livre_jungle.txt");
//            cmp = 0;
//        }



        //g.connexite();

        //grman::show_picture(grman::page,"Asterix.bmp", 23,5,0);
        //grman::show_picture(grman::page,"Asterix.bmp", 23,5,0);

        /// DETECTION DES CLICKS DE BOUTONS
        g.getInterface()->getAsterix().interact_focus();

        if(g.getInterface()->getAsterix().clicked()==true)
        {
            g.chargement_fichier("Asterix.txt");
            asterix = true;
            livre = false;
        }


        else
        {

            g.getInterface()->getLivre().interact_focus();

            if(g.getInterface()->getLivre().clicked()==true && livre == false)
            {
                g.chargement_fichier("Livre_jungle.txt");
                livre = true;
                asterix = false;
            }

            else
            {
                g.getInterface()->getAjouter().interact_focus();

                if(g.getInterface()->getAjouter().clicked()==true && livre == false)
                {
                    g.ajouter_sommet("Asterix.txt");
                }

                g.getInterface()->getAjouter().interact_focus();

                if(g.getInterface()->getAjouter().clicked()==true && asterix == false)
                {
                    g.ajouter_sommet("Livre_jungle.txt");
                }

                else
                {
                    g.getInterface()->getSauvegarder().interact_focus();

                    if(g.getInterface()->getSauvegarder().clicked()==true && livre == false)
                    {
                        g.sauvegarder("Asterix.txt");
                    }

                    g.getInterface()->getSauvegarder().interact_focus();

                    if(g.getInterface()->getSauvegarder().clicked()==true && asterix == false)
                    {
                        g.sauvegarder("Livre_jungle.txt");
                    }

                     else
                    {
                         g.getInterface()->getSupprimer().interact_focus();

                         if(g.getInterface()->getSupprimer().clicked()==true && livre == false)
                         {
                             std::cout << "Entrez le numero du sommet a supprimer: "; /// A BLINDER
                             std::cin >> id_sommet;

                             g.supprimer_sommet(id_sommet, "Asterix.txt");
                         }

                         g.getInterface()->getSupprimer().interact_focus();

                         if(g.getInterface()->getSupprimer().clicked()==true && asterix == false)
                         {
                             std::cout << "Entrez le numero du sommet a supprimer: "; /// A BLINDER
                             std::cin >> id_sommet;

                             g.supprimer_sommet(id_sommet, "Livre_jungle.txt");
                         }
                    }
                }
            }
        }

        cmp ++;

        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
   }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


