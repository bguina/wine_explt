Projet de 2006 aux oubliettes.

WineExplt est destine a fournir des outils dans le cadre de l'exploitation d'une injection
d'un executable tournant avec Wine et X11 et utilisant le moteur de rendu graphique OpenGL.
L'injection se fait par le LD_PRELOAD d'une bibliotheque dynamique (wineexplt.so) lors du lancement
du programme avec Wine.

Features:
Interception et l'injection des evenements X11.
Overlay OpenGL.
Console in-process.
Executer du code C/C++ au sein du programme (classe User).

Dependances :
    [1] Python 2.7
    [2] CEGUI

Contraintes:
    Du 2012 03 06:
    [1] Seuls les modules d'interface graphique pour CEGUI sont fournies par WineExplt.
    L'utilisateur peit cependant utiliser une autre biliotheque compatible OpenGL en adaptant les
    evenements X11 afin de pouvoir les injecter manuellement dans son interface graphique.
    [2] Un unique skin CEGUI est fourni ("TaharezLook") et il n'est pas possible de le changer.

    Definitifs:
    [1] WineExplt ne s'injecte que lors du lancement du programme (pas de hijack avec ptrace).
    [2] WineExplt est uniquement destine aux application Windows tournant avec X11/Wine/OpenGL.
