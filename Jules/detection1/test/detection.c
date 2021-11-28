#include <stdio.h>
#include <stdlib.h>
#include "math.h"



#define PI 3.1415927

int max_rho(double l[], int t)
{
    double max = -1;
    int m = -1;
    int i = 2;
    int j = 0;
    while (j<t)
    {
        if (l[i] > 1)
        {
            if (max == -1 || l[i-2] >= max)
            {
                max = l[i-2];
                 printf("new max = %lf, theta = %lf\n",max, l[i-1]); 
                m = i-2;
            }
            j +=1 ;
        }
        i += 3;
    }
    return m;
}

int min_rho(double l[], int t)
{
    double max = -1;
    int m = -1;
    int i = 2;
    int j = 0;
    while (j<t)
    {
        if (l[i] > 1)
        {
            if (max == -1 || l[i-2] <= max)
            { 
                max = l[i-2];
                printf("new min = %lf, theta = %lf\n",max,l[i-1]); 
                m = i-2;
            }
            printf("%i\n",j);
            j +=1 ;
        }
        i += 3;
    }
    return m;
}



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
		//printf("    rho:  %lf\n    theta:%lf\n    use:  %lf\n\n", l[i-2],l[i-1],l[i]);
                t += 1;
            }
        }
        f += 0.5;
    }
    /*double new[3*t];
    int j = 2;
    i = 2;
    while ( j <3*t)
    {
        if (l[i] == 2)
        {
            new[j] = 1;
            new[j-1] = l[i-1];
            new[j-2] = l[i-2];
            j +=3;
        }
        i += 3;
    }
    l = new;
    double tmp1,tmp2;
    for (i = 4; i<3*t; i+=2)
    {
        tmp1 = l[i] -1;
        tmp2 = l[i];
        j = i-3;
        while (tmp2 < l[j] && j > 1)
        {
            l[j+3] = l[j];
            l[j+2] = l[j-1];
            j -=3;
        }
        l[j+3] = tmp2;
        l[j+2] = tmp1;

    }*/
    return t;
}


double med (double l[], int len,int t)
{
    printf("len = %i  t = %i \n",len,t);
    int m = t/2;
    double rh1 = 0;
    double th1 = 0;
    double rh2 = 0;
    double th2 = 0;

    int i,j;
    j = 0;
    i = 2;
    while (i < len*3 && j < t)
    {
        printf("j = %i  med = %i  theta = %lf  use = %lf\n",j,m,l[i-1],l[i]);
        if (l[i] == 2)
        {
            if (j >= m)
            {
                rh2 += l[i-2];
                th2 += l[i-1];
                l[i] = 3;
            }
            else
            {
                rh1 += l[i-2];
                th1 += l[i-1];
            }
            j+=1;
            
        }
        i += 3;
    }
    rh1 = rh1 /m;
    th1 = th1 /m;
    rh2 = rh2 /(t-m);
    th2 = th2 /(t-m);
    return (rh2-rh1)/(th2-th1);
}

void selection (double l[], double coef, int t)
{
    printf("test1\n");
    int i = 2;
    int j = 0;
    int k = t;
    double eca = 0;
    while (j<t)
    {
        if (l[i] > 1)
        {
            if (abss(l[i-2] - coef * l[i-1])> eca)
                eca = abss(l[i-2] - coef * l[i-1]);
            j+=1;
        }
        i +=3;
    }
    printf("test2\n");
    double marge = eca/1000;

    while (k>10 && t >0)
    {
        j = 0;
        i = 2;
        eca = eca - marge;
        while (j<t)
        {
            if (l[i] > 1)
            {
                if (abss(l[i-2] - coef * l[i-1])> eca)
                {
                    l[i] = 0;
                    k -= 1;
                }
                
                j += 1;
            }
            i += 3;
        }
        t -= 1;
    }
    printf("test3\n");
}


void search (double l[], int len)
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
    double m1,m2;
    if (t1 > 0 && t2 > 0)
    {
        m1 = med(l1,nb,t1);
        m2 = med(l2,nb2,t2);
        selection(l1,m1,t1);
        selection(l2,m2,t2);
    }

    i = min_rho(l1,t1);
    l[0] = l1[i];
    l[1] = l1[i+1];
    i = max_rho (l1,t1);
    l[2] = l1[i];
    l[3] = l1[i+1];
    i = min_rho(l2,t2);
    l[4] = l2[i];
    l[5] = l2[i+1];
    i = max_rho (l2,t2);
    l[6] = l2[i];
    l[7] = l2[i+1];

    printf(" rho = %lf  theta = %lf\n",l[0],l[1]);
    printf(" rho = %lf  theta = %lf\n",l[2],l[3]);
    printf(" rho = %lf  theta = %lf\n",l[4],l[5]);
    printf(" rho = %lf  theta = %lf\n",l[6],l[7]);


    for (i = 0; i<nb *3; i+=3)
        
    {
        printf("    rho:  %lf\n    theta:%lf\n    use:  %lf\n\n", l1[i],l1[i+1],l1[i+2]);
    }
    printf("CHANGEMENT DE LISTE\n\n");
    for (i = 0; i< nb2 *3; i += 3)
    {
        printf("    rho:  %lf\n    theta:%lf\n    use:  %lf\n\n", l2[i],l2[i+1],l2[i+2]);
    }
    printf("fin de la liste 2\n\n");
}   


/*int main()
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
}*/
