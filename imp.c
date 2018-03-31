#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intal.h"

typedef struct big_int bigint;

struct big_int{
    int * arr;
    int len;
    int offset;
};

void * intal_create(const char * str){
    // printf("\ncreating:%s\n",str);
    bigint *intal = (bigint *) malloc(sizeof(bigint)) ;
    intal->len = strlen(str);
    intal->arr = (int *) malloc(sizeof(int) * (intal->len));
    for(int i = 0; i < intal->len; i++){
        if(str[i] < '0' || str[i] > '9')
            return NULL;
        (intal->arr)[i] = str[i] - '0';
    }
    intal->offset = 0;
    return intal;
}

void intal_destroy(void * intal){
    if(intal == NULL)
        return;
    // printf("freeing: %s\n", intal2str(intal));
    int offset=(((bigint *) intal)->offset);
    free((((bigint *) intal)->arr)-offset);
    free((bigint *) intal);
}

char * intal2str(void * intal){
    if(intal == NULL){
        char *print="NaN";
        return print;
    }
    else{
        bigint * temp = (bigint *) intal;
        char * str = (char *) malloc(sizeof(char) * (temp->len + 1));
        for(int i = 0; i < temp->len; i++){
            str[i] = (temp->arr)[i] + '0';
        }
        str[temp->len]='\0';
        return str;
    }
}

void * intal_add(void * intal1, void * intal2){
    if(intal1 == NULL || intal2 ==NULL){
        bigint * intal = NULL;
        return intal;
    }
    bigint * temp1 = (bigint *) intal1;
    bigint * temp2 = (bigint *) intal2;
    int maxl,minl, n1 = temp1->len - 1, n2 = temp2->len - 1, flg;
    if(n1 > n2){
        maxl = n1 + 1;
        minl = n2 + 1;
        flg = 1;
    }
    else{
        maxl = n2 + 1;
        minl = n1 + 1;
        flg = 0;
    }
    bigint * intal = (bigint * ) malloc (sizeof(bigint)) ;
    intal->len = maxl+1;
    intal->arr = (int *) malloc(sizeof(int) * (maxl+1));
    int c = 0, i, s;
    for (i = 0; i < minl; i++){
        s = (temp1->arr)[n1 - i] + (temp2->arr)[n2 - i] + c;
        intal->arr[maxl - i] = s % 10;
        c = s/10;
        // printf("aaa%d,%d,%d\n",intal->arr[maxl - i],(temp1->arr)[n1 - i],(temp2->arr)[n2 - i]);
    }
    while(i < maxl){
        if(flg)
            s = (temp1->arr)[n1 - i] + c;
        else
            s = (temp2->arr)[n2 - i] + c;

        (intal->arr)[maxl - i] = s % 10;
        c = s/10;
        i++;
    }

    if(c){
        (intal->arr)[0]=1;
        intal->offset = 0;
    }
    else{
        (intal->len)--;
        (intal->arr)++;
        intal->offset = 1;
    }

    return intal;
}

void * intal_increment(void* intal){
    if(intal == NULL)
        return NULL;
    char * str1 = "1";
    void * intal1 = intal_create(str1);
    return intal_add( intal, intal1);
}

static int is_smaller(bigint * intal1, bigint * intal2)
{
    int n1 = intal1->len, n2 = intal2->len;
    int i=0,x=0,y=0;
    while((intal1->arr)[i++]==0)
        x++;
    i=0;
    while((intal2->arr)[i++]==0)
        y++;
    // printf("%s,%s,")
    if (n1 - x < n2 - y)
        return 1;
    if (n1 - x> n2 - y)
        return 0;
    for (int i=0; i<n1-x; i++)
    {
        if ((intal1->arr)[i+x] < (intal2->arr)[i+y])
            return 1;
        else if ((intal1->arr)[i+x] > (intal2->arr)[i+y])
            return 0;
    }
    return 0;
}

void * intal_diff(void * intal1, void * intal2){
    if(intal1 == NULL || intal2 == NULL)
        return NULL;
    bigint * temp1 = (bigint *) intal1;
    bigint * temp2 = (bigint *) intal2;
    if(is_smaller(temp1,temp2))
        return intal_diff(temp2,temp1);
    int n1 = temp1->len - 1, n2 = temp2->len - 1, i = 0, d, b = 0;
    bigint * intal = (bigint * ) malloc (sizeof(bigint)) ;
    intal->len = n1 + 1;
    intal->arr = (int *) malloc(sizeof(int) * (n1 + 1));
    intal->offset = 0;
    while(i < n2 + 1){
        d = (temp1->arr)[n1 - i] - (temp2->arr)[n2 - i] - b ;
        // printf("aaa%d,%d,%d\n",d,(temp1->arr)[n1 - i],(temp2->arr)[n2 - i]);
        if(d < 0){
            d = 10 + d;
            b = 1;
        }
        else{
            b = 0;
        }
        (intal->arr)[n1-i]=d;

        i++;

    }
    while(i < n1 + 1){
        d = (temp1->arr)[n1 - i] - b ;
        if(d < 0){
            d = 10 + d;
            b = 1;
        }
        else{
            b = 0;
        }
        (intal->arr)[n1-i]=d;
        i++;
    }
    while((intal->arr)[0]==0 && (intal->len)!=1){
        (intal->len)--;
        (intal->arr)++;
        intal->offset += 1;
    }
    return intal;
}
void * intal_decrement(void* intal){
    if(intal == NULL)
        return NULL;
    char *str1 = "1";
    void * intal1 = intal_create(str1);
    return intal_diff( intal, intal1);
}

int intal_compare(void* intal1, void* intal2){
    if(intal1 == NULL || intal2 == NULL)
        return 0;
    bigint * temp1 = (bigint *) intal1;
    bigint * temp2 = (bigint *) intal2;
    if(is_smaller(temp1,temp2))
        return -1;
    else if (is_smaller(temp2,temp1))
        return 1;
    else
        return 0;
}

void * intal_multiply(void* intal1, void* intal2){
    if(intal1 == NULL || intal2 == NULL)
        return NULL;

    bigint * x = (bigint *) intal1;
    bigint * y = (bigint *) intal2;
    // if(x->len>4||y->len>4)
    // {    if(q==1) printf("1\n");
    //     if(q==2) printf("2\n");
    //     if(q==3) printf("3\n");}
    // if(x->len>4||y->len>4)
    //     printf("enter x:%s y:%s\n",intal2str(intal1),intal2str(intal2));
    int m = x->len, n = y->len;

    if(m > n){
        y->arr = (int *)realloc(y->arr-y->offset,(m + y->offset) * sizeof(int));
        y->arr += y->offset;
        y->len = m;
        for(int i = 0 ; i < n; i++){
            (y->arr)[m - i - 1]=(y->arr)[n - i - 1];
        }
        for(int i = 0; i < m - n; i++){
            (y->arr)[i] = 0;
        }
        n = m;
    }
    else if(m < n){
        x->arr = (int *)realloc(x->arr - x->offset,(n + x->offset) * sizeof(int));
        x->arr += x->offset;
        x->len = n;
        for(int i = 0 ; i < m; i++){
            (x->arr)[n - i - 1]=(x->arr)[m - i - 1];
        }
        for(int i = 0; i < n - m; i++){
            (x->arr)[i] = 0;
        }
        m = n;
    }

    bigint * prod = (bigint *) malloc(sizeof(bigint));
    prod->len = m + n;
    prod->offset=0;
    prod->arr = (int *) malloc(sizeof(int) * (m + n));
    if(x->len == 1 && y->len == 1){
        int p = (x->arr)[0] * (y->arr)[0];
        (prod->arr)[1] = p % 10;
        (prod->arr)[0] = p / 10;
        if((prod->arr)[0] == 0){
            (prod->len)--;
            (prod->arr)++;
            prod->offset += 1;
        }
        // printf("\nprodsing %s\n",intal2str(prod));
        return prod;
    }

    bigint * x_l = (bigint *) malloc(sizeof(bigint));
    bigint * x_r = (bigint *) malloc(sizeof(bigint));
    bigint * y_l = (bigint *) malloc(sizeof(bigint));
    bigint * y_r = (bigint *) malloc(sizeof(bigint));
    x_l->len = m / 2;
    x_r->len = (m + 1)/2;
    y_l->len = n / 2;
    y_r->len = (n + 1)/2;

    x_l->arr = x->arr;
    x_r->arr = x->arr + x_l->len;

    y_l->arr = y->arr;
    y_r->arr = y->arr + y_l->len;

    bigint * c1 = intal_multiply(x_l,y_l);
    bigint * k1 = intal_add(x_l,x_r);
    bigint * k2 = intal_add(y_l,y_r);
    bigint * c3 = intal_multiply(x_r,y_r);
    bigint * c2 = intal_multiply(k1,k2);
    bigint * temp = intal_diff(c2,c1);
    bigint * temp1 = intal_diff(temp,c3);
    // if(x->len>4||y->len>4)
    //     printf("yolo x:%s  y:%s   xl:%s   xr:%s   yl:%s   yr:%s\n",intal2str(x),intal2str(y), intal2str(x_l), intal2str(x_r), intal2str(y_l), intal2str(y_r));
    intal_destroy(c2);
    c2 = temp1;
    // printf("zolo c1:%s c2:%s c3:%s\n\n",intal2str(c1),intal2str(c2),intal2str(c3));
    intal_destroy(k1);
    intal_destroy(k2);
    intal_destroy(temp);
    int k;
    {
        k = c1->len + c1->offset;
        c1->arr = (int *)realloc(c1->arr-c1->offset,(2 * (x_r->len) + k )* sizeof(int));
        k -= c1->offset;
        c1->arr += c1->offset;
        c1->len = 2 * (x_r->len) + k;
        for(int i = k; i < 2 * x_r->len + k; i++){
            (c1->arr)[i] = 0;
        }
    }
    {
        k = c2->len + c2->offset;
        c2->arr = (int *)realloc(c2->arr - c2->offset,(k + x_r->len)* sizeof(int));
        k -= c2->offset;
        c2->arr += c2->offset;
        c2->len = x_r->len + k;
        for(int i = k ; i < x_r->len + k; i++){
            (c2->arr)[i] = 0;
        }
    }
    temp = intal_add(c1,c2);
    prod = intal_add(temp,c3);

    intal_destroy (temp);

    while((prod->arr)[0] == 0 && prod->len!=1){
        (prod->len)--;
        (prod->arr)++;
        prod->offset += 1;
    }
    // if(x->len>4||y->len>4)
    // {
    //     printf("ngolo c1:%s c2:%s c3:%s\n",intal2str(c1),intal2str(c2),intal2str(c3));
    //     printf("\nprod %s\n",intal2str(prod));
    // }
    intal_destroy(c1);
    intal_destroy(c2);
    intal_destroy(c3);
    return  prod;
}

void* intal_divide(void* intal1, void* intal2){

    if (intal1 == NULL || intal2 == NULL|| intal2 == 0)
        return NULL;
    bigint * dividend = (bigint *)intal1;
    bigint * divisor = (bigint *)intal2;
    while((divisor->arr)[0]==0 && (divisor->len)!=1){
        (divisor->len)--;
        (divisor->arr)++;
        divisor->offset += 1;
    }
    int n = divisor->len;
    int m = dividend->len;
    bigint * q = (bigint*)malloc(sizeof(bigint));
    q->len = m - n + 1;
    q->arr = (int*)malloc(sizeof(int) * (q->len));
    q->offset = 0;
    bigint * rem = (bigint*)malloc(sizeof(bigint));
    rem->len = n;
    rem->arr = (int*)malloc(sizeof(int) * (n+1));
    for(int i = 0; i < n; i++){
        (rem->arr)[i] = (dividend->arr)[i];
    }
    int i=0;
    bigint * chk=intal_create("1");
    while(n+i<=m){
        bigint * temp = rem;
        // printf("\n%d temp %s",i,intal2str(temp));
        if((intal_compare(temp,divisor) == -1||is_smaller(temp,chk))&&i+n!=m){
            rem->len+=1;
            // printf("\nxolo%d,%d",i,(dividend->arr)[i+n]);
            (rem->arr)[rem->len-1]=(dividend->arr)[i+n];
            (q->arr)[i]=0;
        }
        else{
            int k = 0;
            bigint * tempq = (bigint *)malloc(sizeof(bigint));
            tempq = intal_create("0");
            while(intal_compare(tempq,temp) == -1){
                bigint * sum = intal_add(tempq,divisor);
                k += 1;
                intal_destroy(tempq);
                tempq=sum;
                // printf("\ntemp%s, tempq%s, i%d, k%d\n",intal2str(temp),intal2str(tempq),i,k);
            }
            if(intal_compare(tempq,temp) == 0){
                // printf("nohds\n");
                rem->len=n;
                for(int j = 0; j < n; j++)
                    (rem->arr)[j]=0;
                (q->arr)[i] = k;
            }
            else{
                bigint* tempdiff = intal_diff(tempq,divisor);
                intal_destroy(tempq);
                tempq=tempdiff;
                // printf("\n%dtempq%s",temp->len,intal2str(tempq));
                int b = 0,d,z = 0;
                int n1 = temp->len - 1, n2 = tempq->len - 1, flg;
                if(intal_compare(temp,tempq)==1){
                    while(z < n2 + 1){
                        // printf("\ntemp%s,tempq%s",intal2str(temp),intal2str(tempq));
                        d = (temp->arr)[n1 - z] - (tempq->arr)[n2 - z] - b ;

                        if(d < 0){
                            d = 10 + d;
                            b = 1;
                        }
                        else{
                            b = 0;
                        }
                        // printf("aaa%d,%d,%d\n",d,(temp->arr)[n1 - z ],(tempq->arr)[n2 - z]);
                        (rem->arr)[n1 - z ]=d;
                        z++;
                    }
                    while(z < n1 + 1){
                        d = (temp->arr)[n1 - z] - b ;
                        if(d < 0){
                            d = 10 + d;
                            b = 1;
                        }
                        else{
                            b = 0;
                        }
                        (rem->arr)[n1 - z] = d;
                        z++;
                    }
                    while((rem->arr)[0]==0 && (rem->len)!=1){
                        z = 0;
                        (rem->len)--;
                        while(z < rem->len){
                            (rem->arr)[z] = (rem->arr)[z + 1];
                            z++;
                        }
                    }
                    (rem->len)++;
                    (rem->arr)[rem->len-1]=(dividend->arr)[i+n];
                }
                (q->arr)[i] = k - 1;
            }
        }
        i++;
    }
    while((q->arr)[0]==0 && (q->len)!=1){
        (q->len)--;
        (q->arr)++;
        q->offset += 1;
    }
    return q;
}
