#include <stdio.h>
#include <stdlib.h>
typedef struct Elt{
	void *info;
	struct Elt *svt;
} node ;

typedef node* LLC_liste ;

int LLC_Vide(LLC_liste l){
	return l == NULL;
}

int LLC_Size(LLC_liste l){
  return (!l)?0:LLC_Size(l->svt)+1;
}

void* LLC_neGet(LLC_liste l,int index){
  if (index > LLC_Size(l))
    return NULL;
  return index != 0 ?LLC_neGet(l->svt,index-1): l->info; 
}

void* LLC_Get(LLC_liste l,int index){
  LLC_neGet(l,LLC_Size(l)-index-1);
}

LLC_liste LLC_CrNode(void *val,size_t taille){
  LLC_liste l=(LLC_liste)malloc(sizeof(node));
  l->info = malloc(taille);
  //on copie les info byte par byte
  for (int i = 0; i < taille; i++) {
    *(char *)(l->info +i) = *(char *)(val + i);
  }
  l->svt = NULL;
  return l;
}

//return le dernier elt ajouter
LLC_liste LLC_add(LLC_liste last,void* val,size_t taille){
  LLC_liste l = LLC_CrNode(val,taille);
  if(last != NULL) last->svt = l;
  return l;
}

LLC_liste getLast(LLC_liste lst){
   return lst->svt?getLast(lst->svt):lst ;
}

void LLC_addLast(LLC_liste *l ,void* val , size_t taille){
  if (!(*l)) *l=LLC_CrNode(val,taille);  
  else LLC_add(getLast(*l),val,taille);
}

void push(LLC_liste* p,void* val,size_t taille){
  if (!*p)
    *p = LLC_CrNode(val,taille);
  else {
    LLC_liste tmp = *p;
    *p = LLC_CrNode(val,taille);
    (*p)->svt = tmp;
  }
}

void* pop(LLC_liste* p){
  LLC_liste l;
  void* tmp = NULL;
  if (*p){
    l = *p ;
    *p = (*p)->svt ;
    l->svt = NULL;
    tmp = l->info;
    free(l);
    l=NULL;
  }
  return tmp;
}

void LLC_addFirst(LLC_liste *l,void* val, size_t taille){
  push(l,val,taille);
}

void LLC_Affiche(LLC_liste t,void (*ptrF)(void *)){
  printf("[");
  for (LLC_liste w = t; w ; w=w->svt)
    (*ptrF)(w->info);
  printf("]");

}
void LLC_Affiche2(LLC_liste t,void (*ptrF)(void*),char* Listname){
  printf("\n%s", Listname);
  LLC_Affiche(t,ptrF);
}
//return l'adr de 1er occurence d Elt qui verifier le prediact ptrF(fonction->>boolean)
LLC_liste LLC_Recherche(LLC_liste l,void* val,int (*ptrF)(void *,void *)){
  for (; l ; l=l->svt)
    if ((*ptrF)(l->info,val))
      return l;
  return NULL;
}

int LLC_Count(LLC_liste l , void* val ,int (*ptrF)(void *,void *)){
  return l?LLC_Count(l->svt,val,ptrF)+(*ptrF)(l->info,val):0;
}

void __Swap(LLC_liste *lst1,LLC_liste *lst2){
  void* tmp    =(*lst1)->info;
  (*lst1)->info=(*lst2)->info;
  (*lst2)->info=tmp;
}

void LLC_sort(LLC_liste *l ,int (*CmpPtrF)(void*,void*)){
  if (*l) 
  for (LLC_liste q = *l; q->svt ; q=q->svt) 
    for (LLC_liste w = q->svt ; w ; w=w->svt) 
      if ((*CmpPtrF)(q->info,w->info))__Swap(&q,&w);
}
//fonction qui faire l affichage au  sense inverse
//Comme Pile (pour afficher les Erreur)
void LLC2Pile_Affiche(LLC_liste t,void (*ptrF)(void *)){
  if(t->svt)
    LLC2Pile_Affiche(t->svt,ptrF);
  (*ptrF)(t->info);
}

