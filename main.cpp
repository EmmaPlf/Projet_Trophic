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
    bool foret = false;
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


        /// EVOLUTION DANS LE TEMPS
        if(cmp == 50 && asterix == true)
        {
            g.dynamique("Asterix.txt");
            cmp = 0;
        }

        if(cmp == 50 && livre == true)
        {
            g.dynamique("Livre_jungle.txt");
            cmp = 0;
        }

        if(cmp == 50 && foret == true)
        {
            g.dynamique("Foret.txt");
            cmp = 0;
        }

        /// FORTE CONNEXITE


        g.connexite();

        /// DETECTION DES CLICKS DE BOUTONS
        g.getInterface()->getAsterix().interact_focus();

        if(g.getInterface()->getAsterix().clicked()==true)
        {
            g.chargement_fichier("Asterix.txt");
            asterix = true;
            livre = false;
            foret = false;
        }


        else
        {
            g.getInterface()->getLivre().interact_focus();

            if(g.getInterface()->getLivre().clicked()==true && livre == false)
            {
                g.chargement_fichier("Livre_jungle.txt");
                livre = true;
                asterix = false;
                foret = false;
            }

            else
            {
                g.getInterface()->getAjouter().interact_focus();

                if(g.getInterface()->getAjouter().clicked()==true && asterix == true)
                {
                    g.ajouter_sommet("Asterix.txt");
                    std::cout << std::endl << "AJOUTER DANS ASTERIX" << std::endl;
                }

                g.getInterface()->getAjouter().interact_focus();

                if(g.getInterface()->getAjouter().clicked()==true && livre == true)
                {
                    g.ajouter_sommet_graph2("Livre_jungle.txt");
                    std::cout << std::endl << "AJOUTER DANS LIVRE" << std::endl;
                }

                g.getInterface()->getAjouter().interact_focus();

                if(g.getInterface()->getAjouter().clicked()==true && foret == true)
                {
                    g.ajouter_sommet_foret("Foret.txt");
                    std::cout << std::endl << "AJOUTER DANS FORET" << std::endl;
                }

                else
                {
                    g.getInterface()->getSauvegarder().interact_focus();

                    if(g.getInterface()->getSauvegarder().clicked()==true && asterix == true)
                    {
                        g.sauvegarder("Asterix.txt");
                        std::cout << std::endl << "SAUVEGARDE DE ASTERIX" << std::endl;
                    }

                    g.getInterface()->getSauvegarder().interact_focus();

                    if(g.getInterface()->getSauvegarder().clicked()==true && livre == true)
                    {
                        g.sauvegarder("Livre_jungle.txt");
                        std::cout << std::endl << "SAUVEGARDE DE LIVRE" << std::endl;
                    }

                    g.getInterface()->getSauvegarder().interact_focus();

                    if(g.getInterface()->getSauvegarder().clicked()==true && foret == true)
                    {
                        g.sauvegarder("Foret.txt");
                        std::cout << std::endl << "SAUVEGARDE DE FORET" << std::endl;
                    }

                     else
                    {
                         g.getInterface()->getSupprimer().interact_focus();

                         if(g.getInterface()->getSupprimer().clicked()==true && asterix == true)
                         {
                             std::cout << std::endl  << "Entrez le numero du sommet a supprimer: ";
                             std::cin >> id_sommet;

                             g.supprimer_sommet(id_sommet, "Asterix.txt");
                             std::cout << std::endl << "SUPPRIMER DANS ASTERIX" << std::endl;
                         }

                         g.getInterface()->getSupprimer().interact_focus();

                         if(g.getInterface()->getSupprimer().clicked()==true && livre == true)
                         {
                             std::cout << std::endl  << "Entrez le numero du sommet a supprimer: ";
                             std::cin >> id_sommet;

                             g.supprimer_sommet(id_sommet, "Livre_jungle.txt");
                             std::cout << std::endl << "SUPPRIMER DANS LIVRE" << std::endl;
                         }

                         g.getInterface()->getSupprimer().interact_focus();

                         if(g.getInterface()->getSupprimer().clicked()==true && foret == true)
                         {
                             std::cout << std::endl << "Entrez le numero du sommet a supprimer: "; /// A BLINDER
                             std::cin >> id_sommet;

                             g.supprimer_sommet(id_sommet, "Foret.txt");
                             std::cout << std::endl << "SUPPRIMER DANS FORET" << std::endl;
                         }

                         else
                         {
                            g.getInterface()->getPlay().interact_focus();

                            if(g.getInterface()->getPlay().clicked()==true)
                            {
                                cmp = 49;
                                std::cout << std::endl << "PLAY" << std::endl;
                            }

                            else
                            {
                                g.getInterface()->getPause().interact_focus();

                                if(g.getInterface()->getPause().clicked()==true)
                                {
                                    cmp = 51;
                                    std::cout << std::endl << "PAUSE" << std::endl;
                                }

                                else
                                {
                                    g.getInterface()->getForet().interact_focus();

                                    if(g.getInterface()->getForet().clicked()==true && foret == false)
                                    {
                                        g.chargement_fichier("Foret.txt");
                                        foret = true;
                                        asterix = false;
                                        livre = false;
                                    }
                                }
                            }
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


