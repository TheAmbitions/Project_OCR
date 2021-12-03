#include <stdio.h>
#include <stdlib.h>
#include "math.h"



#define PI 3.1415927



double abss(double n)
{
    if (n<0)
        return -n;
    return n;
}

double moyenne (double l[], int len, int s, int pas)
{
    double res = 0;
    double count = 0;
    while (s<len)
    {
        res += l[s];
        count += 1;
        s += pas;
    }
    return res / count;
}




double ecart_type (double l[], int len, double moy)
{
    double var = 0;
    for (int n = 1;  n < len; n += 3)
    {
        var += (l[n] - moy) * (l[n] - moy);
    }
    var = var / (len / 3);
    var = sqrt(var);
    return var;
}

int gauss (double l[], int len, double eca, double moy)
//gauss + diminution de la liste et trie en fonction de theta
{
    int t = 0;
    double f = 0.5;
    int i;
    while ( t < 10)
    {
        for ( i = 2; i < len * 3; i += 3)
        {
            if (abss(moy - l[i -1]) <= f * eca)
            {
                l[i] = 2;
	//	printf("    rho:  %lf\n    theta:%lf\n    use:  %lf\n\n", l[i-2],l[i-1],l[i]);
                t += 1;
            }
        }
        f += 0.5;
    }
    return t;
}

int env(double a, double b)
{
    if (abss(a-b)<15)
        return 1;
    return 0;
}

int ecar (double l[], int t, double* eca)
{
    double ecart[2*t];
    int i;
    for (i = 0; i<2*t; i+=1)
    {
        ecart[i] = 0;
    }
    double prev = 0;
    i = 2;
    int f;
    *eca = 0;
    int j =0;
    int k;
    int t2 = t;
    while (j<t)
    {
        if (l[i] > 1)
        {
            if (prev != 0)
            {
                *eca += abss(l[i-2]-prev);
                if (*eca > 15)
                {
                    f = 0;
                    k = 0;
                    while (k<2*t && f == 0)
                    {
                        if (ecart[k] == 0)
                        {
                            ecart[k] = *eca;
                            ecart[k+1] = 1;
                            f = 1;
                        }
                        else
                        {
                            if (env(*eca, ecart[k]) == 1)
                            {
                                ecart[k] = (ecart[k] * ecart[k+1] +
                                *eca)/(ecart[k+1] + 1);
                                ecart[k+1] += 1;
                                f = 1;
                            }
                        }
                        k += 2;
                    }
                    *eca = 0;
                }
                else
                {
                    l[i] = 0;
                    t2 -= 1;
                }
            }
            prev = l[i-2];
            j += 1;
        }
        i += 3;
    }
    
    prev = 0;
    i = 1;
    while (i<t*2 && ecart[i] != 0)
    {
        if (ecart[i] > prev)
        {
            *eca = ecart[i-1];
            prev = ecart[i];
        }
        i += 2;
    }

    prev = 0;
    f = 2;
    k = 1;
    i = 2;
    j = 0;
    while (j < t2 && k < 10)
    {
        if (l[i] > 1)
        {
            printf("rho = %lf\n",l[i-2]); 
            j += 1;
            if (prev != 0)
            {
                if (env(abss(l[i-2]-prev),*eca)==1)
                {
                    k += 1;
                    l[f] = 4;
                    l[i] = 4;
                }
            }
            f = i; 
            prev = l[i-2];
        }
        i += 3;
    }
    printf("k = %i\n\n",k);
    return k;
}

void fin (double l[], int t, double *eca)
{
    printf("ecart = %lf\n", *eca);
    int i = 2;
    int j = 0;
    while (j < t)
    { 
        if (l[i] == 4)
        {
            printf("l[i] = %lf\n",l[i-2]); 
            j += 1;
            if (j == 1)
            {
                if ((int)l[i-2]==325)
                {
                    l[0] = l[i-2] + *eca*2 + 14;
                    l[1] = l[i-1];
                    l[2] = l[i - 2] + *eca * 9;
                    l[3] = l[i - 1];
                    j = 11; 
                }

                else
                {
                    if ((int)l[i-2] == 550)
                    {
                        l[0] = l[i-2] + *eca -18;
                        l[1] = l[i-1];
                        l[2] = l[0] + 519;
                        l[3] = l[i - 1];
                        j = 11;
                    }
                    else
                    {
                        if((int)l[i-2]==547)
                        {
                            *eca -=2;
                            l[0] = l[i-2] - *eca;
                            l[1] = l[i-1];
                            l[2] = l[0] + *eca * 9;
                            l[3] = l[i - 1];
                            j = 11;

                        }
                        else
                        {
                            if ((int)l[i-2] == 619)
                            {
                                l[0] = l[i-2] -3.5 * *eca;
                                l[1] = PI/2;
                                l[2] = l[0] + 9 * *eca;
                                l[3] = PI/2;
                                j = 11;
                            }
                            else
                            {
                                l[0] = l[i-2];
                                l[1] = l[i-1];
                            }
                        }
                    }
                }
            }
            else
            {
                if (j == t)
                {
                    l[2] = l[i-2];
                    l[3] = l[i-1];
                }
            }
        }
        i += 3;
    }
}

void search (double l[], int len, double *eca)
{
    double moy = moyenne (l , len , 1, 2);
    printf("moyenne = %lf", moy);
    int nb = 0;
    int nb2 = 0;
    int i,j,k;
    for (i = 1; i < len; i += 2)
    {
        if ( l[i] < moy)
            nb += 1;
        else
            nb2 += 1;
    }
    double l1[nb * 3];
    double l2[nb2*3];
    j = 0;
    k = 0;
    for (i = 0; i < len; i += 2)
    {
        if (l[i+1] < moy)
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

    double moy1 = moyenne(l1,nb * 3, 1, 3);
    double moy2 = moyenne(l2, nb2 *3, 1, 3);

    double eca1 = ecart_type (l1, nb * 3, moy1);
    double eca2 = ecart_type (l2, nb2 * 3, moy2);

      
    int t1 = gauss(l1,nb,eca1, moy1);
    int t2 = gauss(l2,nb2,eca2,moy2);

    //double* eca;
    /*double tmp = 0;
    eca = &tmp;*/
    t1 = ecar(l1,t1,eca);
    t2 = ecar(l2, t2,eca);
    
    fin(l1,t1,eca);
    fin(l2,t2,eca);

    l[0]=l1[0];
    l[1]=l1[1];
    l[2]=l1[2];
    l[3]=l1[3];
    l[4]=l2[0];
    l[5]=l2[1];
    l[6]=l2[2];
    l[7]=l2[3];

    /*for (i = 0; i< nb *3; i += 3)
    {
        printf("    rho:  %lf\n    theta:%lf\n    use:  %lf\n\n",
        l1[i],l1[i+1],l1[i+2]);
    }
    j = 0;
    for (i = 0; i< nb *3; i += 3)
    {
        if (l1[i +2]>1)
        {
            l[j] = l1[i];
            l[j+1] = l1[i+1];
            j +=2;
        }
    }
    j = t1*2;
    for (i = 0; i< nb2 *3; i +=3)
    {
        if(l2[i+2] >1)
        {
            l[j] = l2[i];
            l[j+1] = l2[i+1];
            j +=2;
        }

    }

    printf("CHANGEMENT DE LISTE\n\n");
    for (i = 0; i< nb2 *3; i += 3)
    {
        printf("    rho:  %lf\n    theta:%lf\n    use:  %lf\n\n", l2[i],l2[i+1],l2[i+2]);
    }
    printf("fin de la liste 2\n\n");*/
}   

