#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include "LLC_liste.h"

typedef LLC_liste ListeMots;
typedef LLC_liste ListeFreq;
typedef LLC_liste DictContext;
 
typedef struct{
  char* m;
  int freq;
} M_freq;

typedef struct{
  char* m;
  ListeFreq Contxs;
} Dict;
 

/* helper methods */
void affCH(void* n){
  printf("[%s]", *(char**)n);
}
void affFreq(void* n){
  printf("{%s,%d},",((M_freq*)n)->m,((M_freq*)n)->freq );
}

/*
   fonction qui découpe une chaîne de caractère 
   par un caractère choisi
   a un tableau des chaînes de caractère 
*/

char** deCoupe(char* f,char x){
  f=strdup(f);
	int n=0;
	char **c, **d, *cc;
	for(cc=f;*cc;n=n+(*cc==x),cc++);
	cc=f;
	c=d=(char**)malloc((n+2)*sizeof(char*));
  for(*d=f,d++ ; *f ; *d=f,d++,f++){	for(;*f !=x && *f;f++); for(;*f==x && *f;f++);}
  for(;*cc;cc++) if (*cc == x) *cc='\0';
  *(d-1)=NULL;
	return c;
}

int DICT_equals(void* d1,void* d2){ return !strcmp(((Dict*)d1)->m,((Dict*)d2)->m);}
int FREQ_equals(void* d1,void* d2){ return !strcmp(((M_freq*)d1)->m,((M_freq*)d2)->m);}
int ComparatorFreqCR (void* fq1,void* fq2){ return ((M_freq*)fq1)->freq > ((M_freq*)fq2)->freq;}
int ComparatorFreqDec(void* fq1,void* fq2){ return ((M_freq*)fq1)->freq < ((M_freq*)fq2)->freq;}
int DecLen(char** ch){ return *ch ? DecLen(++ch)+1:0;}
int frqMot(char** ch,char* c) { return *ch ? (strcmp(*ch,c)==0)+frqMot(ch+1,c):0;}




ListeFreq Contexte(char* t,char* m){
  ListeFreq l=NULL,lq=NULL;
  char **i,**ii;
  for ( i = ii = deCoupe(t,' '); *i && *(i+1) ; i++) {
    if (! strcmp(*i,m)){
    M_freq tmp={.m=*(i+1) , .freq =frqMot(ii,*(i+1))};
      if (!l) l=lq=LLC_add(lq,&tmp,sizeof(M_freq));
           else lq=LLC_add(lq,&tmp,sizeof(M_freq));
    }
  }  
  LLC_sort(&l,ComparatorFreqDec);
  return l;
}

/* 
   créer un dictionnaire a partire d'une 
   chaine de caractère donner 
*/

DictContext Cr_DictContexte(char* t){
  DictContext l=NULL,lq=NULL;
  for (char** i = deCoupe(t,' '); *i ; i++) {
    Dict lf={.m=*i , .Contxs=Contexte(t,*i)};
      if (!l) l=lq=LLC_add(lq,&lf,sizeof(Dict));
           else lq=LLC_add(lq,&lf,sizeof(Dict));
  }
  return l ;
}
char* maxContexte(DictContext d, char* m){
  LLC_liste l= LLC_Recherche(d,&m,DICT_equals);
  return l?((M_freq*)((((Dict*)(l->info))->Contxs)->info))->m : NULL ;
}

/* 
   maxContexteRAND fonction qui choisit
   un des contextes de [mot] les plus
   fréquents
*/
char* maxContexteRAND(DictContext d, char* m){
  LLC_liste l= LLC_Recherche(d,&m,DICT_equals);
  if (!l) return NULL;
  else {
    ListeFreq  freqs = (ListeFreq)((((Dict*)(l->info))->Contxs));
    int pos = (rand()+1)%(LLC_Size(freqs)/2+1);
    for (; freqs && pos--; freqs=freqs->svt);
    return ((M_freq*)(freqs->info))->m;
  }
}


void genererText(DictContext d,int n){
  srand((time(NULL))); 
  int pos = (rand())%LLC_Size(d);
  char* mot =((Dict*)LLC_Get(d,pos))->m;
  for (int i = 0; i < n; i++) {
    printf("%s ",mot);
    mot=maxContexteRAND(d,mot) ;
  }
  
}

char* getFromDataSet(char* filename){
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);

    string[fsize] = 0;

    return string;
}

int main(){
 
  // charger la dataset
  char* s=getFromDataSet("ds.txt");

  // Fonction d'affichage d'un elmt d'un DICT
  void affDict(void* d){
    printf("\n    ( mot=\"%s\", context=",((Dict*)d)->m);
    LLC_Affiche(((Dict*)d)->Contxs,affFreq)   ;
    printf(" ),\n");
  }

  // cree le DICT   
  // DICT Liste des liste  :
  //    -> chaque mot a un liste des mots trie par frequences
  DictContext dict=Cr_DictContexte(s);

  // afficher le DICT [OPTIONEL]
  //LLC_Affiche2(dict,affDict,"DictContext:");

  // GENERER UN TEXTE DE TAILLE (N) utilisons le (DICT) 
  genererText(dict,100);

  printf("\n");
  return 0;
}

