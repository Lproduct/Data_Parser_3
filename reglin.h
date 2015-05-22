/****************************************/
/* Librairie de régression "reglin"     */
/* Fichier d'en-tête                    */
/* Réalisé par GONNELLA Stéphane        */
/****************************************/

/* Fonctions de test */

int test_zero(double tab[],int n);
int test_negatif(double tab[],int n);

/* Fonctions de changement de variables */

void logtab(double tab[],double tabTemp[],int n);
void lntab(double tab[],double tabTemp[],int n);
void invtab(double tab[],double tabTemp[],int n);

/* Fonctions de régression */

double pente(double Xi[],double Yi[],int n);
double ordonnee(double Xi[],double Yi[],int n);
double corr(double Xi[],double Yi[],int n);
int ajustement(double Xi[],double Yi[],int n);

/*******************/
/* Fin de reglin.h */
/*******************/