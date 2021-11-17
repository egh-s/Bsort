#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sfen_quicksort(void);

struct hash					    // 40 bytes
  { unsigned long long key1;	// 8 bytes
    unsigned long long key2;	// 8 bytes
    unsigned long long key3;	// 8 bytes
    unsigned long long key4;	// 8 bytes
    unsigned long long key5; };	// 8 bytes
struct hash *sfen;

unsigned int ms;
char *p_memory,*bgn;
unsigned int limit;
FILE *fpc;
char w[200],fn[200];
int count=0;
int doubles=0;
int x,y,i,j,a,z;

unsigned long long k1,k2,k3,k4;

int st1[3000],st2[3000];	// quicksort stack

int main(int argc, char *argv[]) {

        for (ms = (size_t)(3ull*1024*1048576); ms >= 2000; ms -= 100000)     // allocate 3 Gb
         { p_memory = (char*)malloc(ms); if (p_memory) break; }
           printf("Free memory = %d Mb\n",ms>>20); 

		free(p_memory);
		p_memory=(char*)malloc(ms);
		sfen=(struct hash *)p_memory;
		bgn=p_memory;
		p_memory=p_memory+ms;
		limit=p_memory-bgn;                        // calulate maximum number entries
        limit/=40;                                 // length structure *sfen
        limit-=100;                                // create some slack space
		printf("\nAllocated space for %d million sfen entries\n\n",limit>>20);
 		
x10:	printf("Enter Souce File Name     : " ); gets(fn); 
	    fpc=fopen(fn,"rb");	          
		if (fpc==0) { printf("\nFile %s not present, try again",fn); goto x10; }
	  	
        printf("\nReading file into memory...");

lees:   x=fread(&sfen[count].key1,8,1,fpc); if (x<1) { fclose(fpc); goto done; }
	      fread(&sfen[count].key2,8,1,fpc);
	      fread(&sfen[count].key3,8,1,fpc);
	      fread(&sfen[count].key4,8,1,fpc);
	      fread(&sfen[count].key5,8,1,fpc);
        count++; if (count > limit) { printf("Insufficient memory... \a"); gets(w); exit(1); }
        goto lees;

done:   printf("\n\nDone... counted %d records ",count);
    
        fclose(fpc);
        
        printf("\n\nSorting records : %d ",count);

        sfen_quicksort();

        printf("\n\nSorting done... ");

		printf("\n\nChecking for doubles ");
		
        k1=0; k2=0; k3=0; k4=0;

        for(x=0; x<count; x++)
         { if (sfen[x].key1==k1 && sfen[x].key2==k2 && sfen[x].key3==k3 && sfen[x].key4==k4) { doubles++; continue; }
           k1=sfen[x].key1; k2=sfen[x].key2; k3=sfen[x].key3; k4=sfen[x].key4; }

		printf("\n\nDoubles found : %d ",doubles);

        gets(w);

		return 0;
}

void sfen_quicksort()

{	    unsigned long long zz; char H[500],I[500],J[500];
    	int t1,l,i,j,r;

        t1=1; st1[1]=0; st2[1]=count-1;		        

lab8:	l=st1[t1]; r=st2[t1]; t1=t1-1;

lab9:   i=l; j=r;
        sprintf(H,"%d%d%d%d",
        sfen[(l+r)>>1].key1,
        sfen[(l+r)>>1].key2,
        sfen[(l+r)>>1].key3,
        sfen[(l+r)>>1].key4);
        
lab10:  sprintf(I,"%d%d%d%d",
        sfen[i].key1,
        sfen[i].key2,
        sfen[i].key3,
        sfen[i].key4);
        
        if (strcmp(I,H) >=0  ||  i >= r) goto lab20;
        i=i+1; goto lab10;

lab20:  sprintf(J,"%d%d%d%d",
        sfen[j].key1,
        sfen[j].key2,
        sfen[j].key3,
        sfen[j].key4);
        
		if (strcmp(J,H) <=0  ||  j <= l) goto lab30;
        j=j-1; goto lab20;

lab30:  if (i > j)  goto lab40;
    	zz=sfen[i].key1;   sfen[i].key1=sfen[j].key1;   sfen[j].key1=zz;
    	zz=sfen[i].key2;   sfen[i].key2=sfen[j].key2;   sfen[j].key2=zz;
    	zz=sfen[i].key3;   sfen[i].key3=sfen[j].key3;   sfen[j].key3=zz;
    	zz=sfen[i].key4;   sfen[i].key4=sfen[j].key4;   sfen[j].key4=zz;
    	zz=sfen[i].key5;   sfen[i].key5=sfen[j].key5;   sfen[j].key5=zz;
    		
        i=i+1; j=j-1;

lab40:  if (i <= j) goto lab10;

        if (r-i <= j-l) goto lab60;

        if (l >= j) goto lab50;
        t1=t1+1; st1[t1]=l; st2[t1]=j;

lab50:  l=i; goto lab80;

lab60:  if (i >= r) goto lab70;

        t1=t1+1; st1[t1]=i; st2[t1]=r;

lab70:  r=j;

lab80:  if (r > l)  goto lab9;

        if (t1) goto lab8;
}

