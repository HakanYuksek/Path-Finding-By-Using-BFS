#include <stdio.h>
#include <stdlib.h>


struct node{
	int neigh;
	int dist;
	struct node *next;
}; 

struct list{
	struct node *head;
	int cnt;
};

struct forQueue{//kuyruga eklenecek eleman tipleri
	int nodeIndex;
	struct forQueue *parent;
	int dist;
};

struct QUEUE{
	struct forQueue nodes[100];
	int t;//kuyrukta neredeyiz onu tutmak icin,eleman cikarinca bir arttir	
	int size;//kuyruktaki eleman sayisini tutacak
};

void enqueue(struct forQueue *element,struct QUEUE *queue){//kuyruga eleman ekleme fonksiyonu
	queue->nodes[queue->size]=*element;
	queue->size++;
}

struct forQueue *dequeue(struct QUEUE *queue){//kuyruktan eleman cikarma fonksiyonu
	struct forQueue *parentObject=&queue->nodes[queue->t];
	queue->t=queue->t+1;
	return parentObject;//cikartilan objeyi geri dondurelim
}


void insertNewNeigh(struct list *h,int i,int n,int d){//bu fonksiyon i inci dugumden n inci dugume olan baglantisini eklemek icin kullanilir
	struct node *newNode=(struct node*)malloc(sizeof(struct node));
	struct node *tmp=(struct node*)malloc(sizeof(struct node));
	newNode->dist=d;//mesafe
	newNode->neigh=n;//komsu
	newNode->next=NULL;
	//i inci dugume bu komsusunu ekleyelim
	if(h[i].head==NULL){//baslangictaki head null ise,basa ekleyelim
		h[i].head=newNode;
		h[i].cnt=1;
	}else{
		tmp=h[i].head;
		while(tmp->next!=NULL){
			tmp=tmp->next;
		}
		tmp->next=(struct node*)malloc(sizeof(struct node));
		tmp->next=newNode;
		h[i].cnt++;
	}
}
void yazdir(struct list *h,int i){
	struct node *tmp=(struct node*)malloc(sizeof(struct node));
	tmp=h[i].head;
	while(tmp!=NULL){
		printf("   %d birim : %d --> %d\n",tmp->dist,i,tmp->neigh);
		tmp=tmp->next;
	}	
}
void initialize(struct list *h,int n){
	int i;
	for(i=0;i<n;i++){
		h[i].head=NULL;
		h[i].cnt=0;
	}
}

void WriteTheWay(int *A,int n,int sum,int isMin){//yol varsa yolu ekrana yazdirmak icin kullanilir
	int i,tmp;
	if(isMin==0){
		for(i=0;i<n/2;i++){//once diziyi ters cevirelim
			tmp=A[i];
			A[i]=A[n-i-1];
			A[n-1-i]=tmp;
		}	
	}
	printf("   %d birim\t:\t",sum);
	for(i=0;i<n-1;i++){
		printf("%d->",A[i]);
	}
	printf("%d\n",A[i]);
}



int bfs(struct list *h,int n,int m,int startNode,int finishNode,struct QUEUE *queue){//startNode dan finishNode a yol var mi,onu arastiriyoruz
	int min=INT_MAX;//minimum yolun mesafesini tutacak
	int k;
	int l;
	int i;
	int *minWay;//minimum yol	
	int *done=(int*)calloc(sizeof(int),n);//eger bir dugumun tum komsularina gidildiyse bu dugum done yapilir	
	int sum;
	int flag=0;//yolun var olup olmadigini anlamak icin bu flag kullanilir
	struct forQueue *element=(struct forQueue*)malloc(sizeof(struct forQueue));
	struct node *tmp=(struct node*)malloc(sizeof(struct node));
	element->parent=(struct forQueue*)malloc(sizeof(struct forQueue));
	element->parent->parent=(struct forQueue*)malloc(sizeof(struct forQueue));
	element->parent->parent->nodeIndex=-1;
	element->parent->parent->dist=0;
	element->parent->parent->parent=NULL;
	element->parent->dist=0;
	element->dist=0;
	element->nodeIndex=startNode;
	enqueue(element,queue);
	//baslangic icin kuyruga eleman eklendi
	printf("\n");
	printf("3) Butun Yollar :\n\n");
	while(queue->size>queue->t){//kuyruk bos olmadigi surece devam edelim
		element->parent=dequeue(queue);//kuyruktan cikan elemanin komsularina gidecegiz
		tmp=h[element->parent->nodeIndex].head;
		if(done[element->parent->nodeIndex]==0){
			while(tmp!=NULL){
				element->dist=tmp->dist;
				element->nodeIndex=tmp->neigh;
				enqueue(element,queue);//komsularini kuyruga ekleyelim
			 if(tmp->neigh==finishNode){
					flag=1;//yol var demektir...
					k=0;
					sum=0;
					int *A=(int*)malloc(sizeof(int)*150);
					struct forQueue *temp=(struct forQueue*)malloc(sizeof(struct forQueue));
					temp=element;
					while(temp->parent->nodeIndex!=-1){
						A[k]=temp->nodeIndex;
						sum=sum+temp->dist;
						k=k+1;
						temp=temp->parent;
					}
					WriteTheWay(A,k,sum,0);//buldugumuz yolu ekrana yazdiralim...
					if(sum<min){
						minWay=(int*)malloc(sizeof(int)*k);
						min=sum;
						l=k;//en kisa yolun eleman sayisi
						for(i=0;i<k;i++){
							minWay[i]=A[i];
						}						
					}
				}
				tmp=tmp->next;
			}
		}
	}
	printf("\n");
	printf("4) En Kisa Yol :\n\n");
 if(flag==1){
		WriteTheWay(minWay,l,min,1);
		printf("\n");
		printf("5)Verilen Dugumler arasinda baglanti var.\n");
		return min;
	}else{
		printf("    Yol Yok\n");
		printf("5)Verilen Dugumler arasinda baglanti yok.\n");
	 return -1;//yol yok...
	}	
}

int findTheFarNodes(struct list *h,int n,int m,int startNode,int finishNode,struct QUEUE *queue){//birbirine en uzak iki dugumun mesafesini bulalim
	int min=INT_MAX,max=0;
	int i;
	int *done=(int*)calloc(sizeof(int),n);//bu dugumun tum komsularini gezdiysek bu dugum done yapilir
	int sum;
	int flag=0;//yol var mi diye bakilir
	struct forQueue *element=(struct forQueue*)malloc(sizeof(struct forQueue));
	struct node *tmp=(struct node*)malloc(sizeof(struct node));
	
	element->parent=(struct forQueue*)malloc(sizeof(struct forQueue));
	element->parent->parent=(struct forQueue*)malloc(sizeof(struct forQueue));
	element->parent->parent->nodeIndex=-1;
	element->parent->parent->dist=0;
	element->parent->parent->parent=NULL;
	element->parent->dist=0;
	element->dist=0;
	element->nodeIndex=startNode;
	enqueue(element,queue);
	//ilk elemani kuyruga ekledik
	while(queue->size>queue->t){//kuyruk bos degilse devam
		element->parent=dequeue(queue);//kuyruktan cikarttigimiz elemanin node larini gezelim...
		tmp=h[element->parent->nodeIndex].head;
		if(done[element->parent->nodeIndex]==0){		
			while(tmp!=NULL){
				element->dist=tmp->dist;
				element->nodeIndex=tmp->neigh;
				enqueue(element,queue);
				if(tmp->neigh==finishNode){
					flag=1;//yol var demektir...
					sum=0;
					struct forQueue *temp=(struct forQueue*)malloc(sizeof(struct forQueue));
					temp=element;
					while(temp->parent->nodeIndex!=-1){
						sum=sum+temp->dist;
						temp=temp->parent;
					}
					if(sum<min){
						min=sum;				
					}
				}
				tmp=tmp->next;
			}
			done[element->parent->nodeIndex]=1;//bu dugumun tum komsularini gezdik
		}
	}
 	if(flag==1){
		return min;
	}else{
	 	return -1;//yol yok...
	}		
	
}


int main(){
 	struct list *h;
 	int i;
 	int nd1;//en uzak nodelar icin kullanilacak
 	int nd2;//en uzak iki node icin kullanilacak
 	int maxDistance=0;//iki dugum arasi en uzak mesafe
 	int way;//dugumler arasi mesafeleri tutacak degisken
 	int j;
	int n;//dugum sayisi
 	int m;//baglanti sayisi
 	int startNode,finishNode;
 	int neigh;
 	int dist;
 	int head;
 	int maxIndex;//maksimum komsuya sahip dugumun indexi
 	int max=0;//maksimum komsu sayisi
 	struct QUEUE *queue=(struct QUEUE*)malloc(sizeof(struct QUEUE));
 	
	queue->t=0;
	queue->size=0;
	FILE *fp=fopen("input.txt","r");//input.txt dosyasindan okuma yapabiliriz
 	fscanf(fp,"%d %d",&n,&m);
 	fscanf(fp,"%d %d",&startNode,&finishNode);
 	h=(struct list*)malloc(sizeof(struct list)*n);
	initialize(h,n);//adjaceny list i initialize edelim
	
	//dosyadan verimizi okuyalim
 	for(i=0;i<m;i++){
 		if(!feof(fp)){		 
 			fscanf(fp,"%d %d %d",&head,&neigh,&dist);
 			insertNewNeigh(h,head,neigh,dist);		
		}
	} 		
	//dosyadan verimiz okundu...
	printf("\n");
	printf("1) Dosyadan N = %d adet dugum, M = %d adet baglanti okundu...\n",n,m);
	printf("\n");
	printf("2)Baslangic:%d\n  Bitis:%d",startNode,finishNode); 	
 	printf("\n");
 	bfs(h,n,m,startNode,finishNode,queue);//istenen dugumler arasi yollari ve en kisa yolu bulalim
   	printf("\n");
   	printf("6)Dogrudan Bagli Dugumler :\n\n");
   	for(i=0;i<n;i++){
   		yazdir(h,i);	
		if(h[i].cnt>max){//en fazla komsuya sahip dugumu ve komsu sayisini bulalim...
			maxIndex=i;
			max=h[i].cnt;
		}
	}
	printf("\n\n");
	printf("7) En fazla komsusu olan dugum : %d ",maxIndex);
	printf("\n");
	printf("   Komsu sayisi : %d\n",h[maxIndex].cnt);
	
	//en uzak iki dugumu ve aralarindaki mesafeyi bulalim
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(i!=j){
				queue=(struct QUEUE*)calloc(sizeof(struct QUEUE),1);
				queue->t=0;
				queue->size=0;
				way=findTheFarNodes(h,n,m,i,j,queue);//i nci ile j inci dugumler arasi en kisa mesafeyi bulalim
				if(way!=-1 && way>=maxDistance){
					maxDistance=way;
					nd1=i;
					nd2=j;
				}
			}
		}
	}
	printf("\n");
	printf("8) Birbirine En Uzak Iki Nokta :\n");
	printf("   %d -> %d (%d birim)",nd1,nd2,maxDistance);

 	getch();
 	return 0;
}
