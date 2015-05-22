/**************************************/
/* Fichier CPP de la librairie reglin */
/* R�alis� par GONNELLA St�phane      */
/**************************************/

#include <math.h>
#include <stdlib.h>

/* D�claratin globale des variables */

double *tab_temp; //D�claration d'un tableau temporaire

/*********************/
/* Fonctions de test */
/*********************/

/* Fonction de test de pr�sence d'un z�ro */

int test_zero(double tab[],int n)
{
        for(int i=0;i<n;i++)
        {
                if(tab[i]==0)
                { return i;}
        }

        return -1;
}

/* Fonction de test de pr�sence d'un n�gatif */

int test_negatif(double tab[],int n)
{
        for(int i=0;i<n;i++)
        {
                if(tab[i]<0)
                { return i;}
        }

        return -1;
}

/*********************************/
/* Fonctions de valeurs absolues */
/*********************************/

/*fonction qui retourne la valeur absolue*/

double val_abs(double x)
{
	if(x<0) {return -x;}
	else {return x;}
}
/********************************/
/* Fonction de recherche du max */
/********************************/

/* Fonction qui retourne celui qui est le max */

int rmax(double r[])
{
        double temp=0;
        int ajust=0;

        for(int i=0;i<5;i++)
        {
                if(r[i]>temp)
                {
                     temp=r[i];
                     ajust = i+1;
                }
        }

        return ajust;
}

/**********************/
/* Fonctions de somme */
/**********************/

/* Fonction de somme d'�l�ments d'un tableau d'entier */

int somme(int tab[],int n)
{
	int somme=0;

	for (int i=0;i<n;i++)
	{
	 somme=((tab[i])+(somme));
   	}

	return(somme);
}

/* Fonction de somme d'�l�ments d'un tableau de r�el*/

double somme(double tab[],int n)
{
	double somme=0;

	for (int i=0;i<n;i++)
	{
	 somme=((tab[i])+(somme));
   	}

	return(somme);
}

/**********************************/
/* Fonctions de calcul de moyenne */
/**********************************/

/* Fonction de calcul de moyenne d'�l�ments d'un tableau d'entier */

double moyenne(int tab[],int n)
{
	double moyenne = double(somme(tab,n))/n;

	return (moyenne);
}

/* Fonction de calcul de moyenne d'�l�ments d'un tableau de r�el */

double moyenne(double tab[],int n)
{
	double moyenne = somme(tab,n)/n;

	return (moyenne);
}

/* Fonction de calcul de moyenne d'elements d'un tableau de r�el(2) */

double moyenne2(double somme,int n)
{
	double moyenne = somme/n;

	return (moyenne);
}

/***********************/
/* Fonction de produit */
/***********************/

/* Fonction de calcul du produit d'�l�ments de deux tableau ligne par ligne */

void produittab(double tab1[],double tab2[],int n)
{
        tab_temp = new double[n];   //R�servation de l'espace m�moire

	for (int i=0;i<n;i++)
	{
		tab_temp[i]=(tab1[i]*tab2[i]);
	}
}

/***************************************/
/* Fonctions de changement de variable */
/***************************************/

/* Fonctions de calcul du ln des �l�ments d'un tableau de r�el */

void lntab(double tab[],double tabTemp[],int n)
{
        tab_temp = new double[n];    //R�servation de l'espace m�moire

	for (int i=0;i<n;i++)
	{
		tabTemp[i]=(log(tab[i]));
	}
}

/* Fonctions de calcul du log base 10 des �l�ments d'un tableau de r�el */

void logtab(double tab[],double tabTemp[],int n)
{
        tab_temp = new double[n];     //R�servation de l'espace m�moire

	for (int i=0;i<n;i++)
	{
		tabTemp[i]=(log10(tab[i]));
	}
}

/* Fonction d'inverstion des �lements d'un tableau de r�el */

void invtab(double tab[],double tabTemp[],int n)
{
        tab_temp = new double[n];     //R�servation de l'espace m�moire

	for (int i=0;i<n;i++)
	{
		tabTemp[i]=(1/tab[i]);
	}
}

/********************/
/* Autres fonctions */
/********************/

/* Fonction de calcul des �carts � la moyenne */

void ecart_a_moyenne(double tab[],double Moyenne,int n)
{
        tab_temp = new double[n];    //R�servation de l'espace m�moire

	for (int i=0;i<n;i++)
	{
		tab_temp[i]=(tab[i] - Moyenne);
	}
}

/****************************/
/* Fonctions de statistique */
/****************************/

/* Fonction de calcul de la covariance */

double covariance(double Xi[],double Yi[],int n)
{
	double cov;

	produittab(Xi,Yi,n);
	cov = moyenne(tab_temp,n) - ( moyenne(Xi,n) * moyenne(Yi,n) );

	return (cov);
}

/* Fonction de calcul de la somme des carr�s des �carts a la moyenne */

double variance(double val[],int n)
{
	double sce;

	produittab(val,val,n);
	sce = moyenne(tab_temp,n) - ( moyenne(val,n) * moyenne(val,n));

  	return (sce);
}

/* Fonction de calcul de l'�cart-type */

double ecarttype(double val[],int n)
{
	double ect= sqrt(variance(val,n));

	return (ect);
}
/******************************************************/
/* Fonctions pour le calcul de la r�gression lin�aire */
/* par la m�thode des moindres carr�                  */
/******************************************************/

/* Fonction de clacul de la pente (a) */

double pente(double Xi[],double Yi[],int n)
{
	double a = covariance(Xi,Yi,n)/variance(Xi,n);

	return (a);
}

/* Fonction de clacul de l'ordonn�e a l'origine (b) */

double ordonnee(double Xi[],double Yi[],int n)
{
	double b = moyenne(Yi,n) - ( pente(Xi,Yi,n) * moyenne(Xi,n) );

	return (b);
}

/* Fonction de calcul du coef de corr�lation (r) */

double corr(double Xi[],double Yi[],int n)
{
	double r = covariance(Xi,Yi,n)/(ecarttype(Xi,n)*ecarttype(Yi,n));
        //double r=pente(Xi,Yi,n)*pente(Xi,Yi,n)*(variance(Xi,n)/variance(Yi,n));
	return (r);
}

/* Fonction de d�termination du meilleur ajustement */

int ajustement(double Xi[],double Yi[],int n)
{
        double r[5],lnXi[100],lnYi[100],logXi[100],logYi[100],invXi[100];

        //corr�lation pour lin�aire

        r[0]=val_abs(corr(Xi,Yi,n));

        //corr�lation pour exponetielle

        lntab(Yi,lnYi,n);
        r[1]=val_abs(corr(Xi,lnYi,n));

        //corr�lation pour puissance

        logtab(Xi,logXi,n);
        logtab(Yi,logYi,n);
        r[2]=val_abs(corr(logXi,logYi,n));

        //corr�lation pour inverse

        invtab(Xi,invXi,n);
        r[3]=val_abs(corr(invXi,Yi,n));

        //corr�lation pour logarithmique

        lntab(Xi,lnXi,n);
        r[4]=val_abs(corr(lnXi,Yi,n));

        //Test du meilleur ajustement

        return rmax(r);
}

/*****************************/
/* Fin du fichier reglin.cpp */
/*****************************/


