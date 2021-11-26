#include <stdio.h>
#include <stdlib.h>
#include "math.h"



#define PI 3.1415927

double moyenne (double l[], int len, int s, int pas)
{
    double res = 0;
    double count;
    while (s<len)
    {
        res += l[s];
        count += 1;
        s += pas;
    }
    return res / count;
}




double ecart_type (double l[], int len, int s, double moy)
{
    double var = 0;
    for (int n = 0;  n < len; n += 3)
    {
        var += (l[n] - moy) * (l[n] - moy);
    }
    var = var / (len / 3);
    return sqrt(var);
}

int gauss (double l[], int len, double eca, double moy)
{
    int k = 0;
    double f = 0.5;
    while ( k < 10)
    {
        for (int i = 2; i < len * 3; i += 3)
        {
            if (fabsf(moy - l[i -1]) < f * eca)
            {
                l[i] = 2;
                k += 1;
            }
        }
        f += 0.5;
    }
    return k;
}

/*void mediane (double l[], int t, double r_m, double t_m)
{
    int med = t/2;
    double new[3*t];
    int i = 2;
    int j = 0;
    while (i<t*3)
    {
        if(l[j] == 2)
        {
            new[i] = l[j - 2];
            new[i + 1] = l[j - 1];
            if (i<med)
            {
                new[i + 2] = 1;
            }
            else
            {
                new[i +2] = 2;
            }
            i += 3;
        }
        j += 3;
    }
    l = new;
}*/


void search (double l[], int len)
{
    double moy = moyenne (l , len , 1, 2);
    int nb = 0;
    int i,j,k;
    for (i = 1; i < len; i += 2)
    {
        if ( l[i] < moy)
            nb += 1;
    }
    double l1[nb * 3];
    double l2[(len - nb)*3];
    j = 0;
    k = 0;
    for (i = 0; i < len; i += 2)
    {
        if (l[i] < moy)
        {
            l1[j] = l[i];
            l1[j+1] = l[i+1];
            l1[j+2] = 1;
            j += 3;
        }
        else
        {
            l2[k] = l[i];
            l2[k+1] = l[i+1];
            l2[k+2] = 1;
            k += 3;
        }
    }
    double s = 1;

    double moy1 = moyenne(l1,nb * 3, 1, 3);
    double moy2 = moyenne(l2, (len - nb) *3, 1, 3);

    double eca1 = ecart_type (l1, nb * 3, 1,moy1);
    double eca2 = ecart_type (l2, (len - nb) * 3, 1,moy2);
      
    int t1 = gauss(l1,nb,eca1, moy1);
    int t2 = gauss(l2, (len - nb) ,eca2,moy2);

    mediane (l1,t1);
    meidane (l2,t2);
    
    /*for (i = 0; i<nb *3; i+=3)
    {
        printf("    rho:  %lf\n    theta:%lf\n    use:  %lf\n\n", l1[i],l1[i+1],l1[i+2]);
    }
    printf("CHANGEMENT DE LISTE\n\n");
    for (i = 0; i< (len - nb) *3; i += 3)
    {
        printf("    rho:  %lf\n    theta:%lf\n    use:  %lf\n\n", l2[i],l2[i+1],l2[i+2]);
    }*/
}   


int main()
{
    double k = 0;
    double l[60];
    for (int i = 0; i < 34;i+= 2)
    {
        l[i] = k;
        k += 100;
        l[i+1] = 0;
    }
    k = 0;
    for (int i = 34; i< 60; i += 2)
    {
        l[i] = k;
        l[i + 1] = PI/2;
        k += 100;
    }
    search(l,60);
    return 1;
}
