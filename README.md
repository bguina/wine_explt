Le projet WineExplt est destine a fournir des outils dans le cadre de l'exploitation d'une injection
d'un executable tournant avec Wine et X11 et utilisant le moteur de rendu graphique OpenGL.
L'injection se fait par le LD_PRELOAD d'une bibliotheque dynamique (wineexplt.so) lors du lancement
du programme avec Wine.

WineExplt est donc un outil particulierement adapte dans le developpement de hacks et de bots pour
des jeux video Windows sous un systeme Unix.

WineExplt permet actuellement :
    [1] L'interception et l'injection des evenements X11.
    [2] De generer un rendu OpenGL par-dessus la fenetre.
    [3] De creer une interface graphique facilement.
        Une console quake-like est fournie.
    [4] Executer du code C/C++ au sein du programme (classe User).

Dependances :
    [1] Python 2.7
    [2] CEGUI

Roadmap :
    [1] Charger au runtime, par la console, du code C/C++ pour l'executer au sein du programme.
    [2] Charger au runtime, par la console, du code Python pour l'executer au sein du programme.

Limitations arbitraires :
    
    Les limitations arbitraires sont destinees a s'assurer qu'aucun effort d'abstraction ni
    d'implementation ne soit fait avec ces eventualites en tete afin d'eviter l'over-engineering et
    la perte de temps sur des fonctionalites hypothetiques mais, au final, considerees futiles
    dans l'etat actuel du projet. Des limitations peuvent cependant etre levees si de nouveaux
    objectifs pertinents a la roadmap sont definis, menant donc surement a de la reecriture de code.
    
    Du 2012 03 06:
    [1] Seuls les modules d'interface graphique pour CEGUI sont fournies par WineExplt.
    L'utilisateur peit cependant utiliser une autre biliotheque compatible OpenGL en adaptant les
    evenements X11 afin de pouvoir les injecter manuellement dans son interface graphique.
    [2] Un unique skin CEGUI est fourni ("TaharezLook") et il n'est pas possible de le changer.

    Definitifs:
    [1] WineExplt ne s'injecte que lors du lancement du programme (pas de hijack avec ptrace).
    [2] WineExplt est uniquement destine aux application Windows tournant avec X11/Wine/OpenGL.

