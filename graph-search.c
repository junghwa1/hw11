#include <stdio.h>
#include <stdlib.h>
#define VERTEX_SIZE 10

typedef struct Node{
    struct Node* link;
    int key;
}graphnode;

typedef struct graph_p{
    int n;
    struct Node* headnode_ptr[VERTEX_SIZE];
}graph;



int visitedNode[VERTEX_SIZE];
int non_visited[VERTEX_SIZE];
int stacknode[VERTEX_SIZE];
int queuenode[VERTEX_SIZE];
int top=-1;
int rear=-1;
int front=-1;

void Initialize_Graph(graph** g);
void Insert_Vertex(graph* g);
void Insert_Edge(graph* g, int key1, int key2);
void Depth_First_Search(graph* g,int key);
void Breadth_First_Search(graph* g, int key);
void Print_Graph(graph* g);
void Freenode(graph* g);
void reset_Visitedlist();
void reset_non_visited();
int isEmpty_S();
int isEmpty_Q();
void push(int key);
int pop();
void enqueue(int key);
int dequeue();
void sortqueue(int n);

int main(){
    char command;
    int key;
    int vertex_key1, vertex_key2;

    graph* g=NULL;

    printf("[----- [염중화] [2019038062] -----]\n");
    do{
        printf("----------------------------------------------------------------\n");
        printf("                       Graph Searches\n");
        printf("----------------------------------------------------------------\n");
        printf("Initialize Graph = z\n");
        printf("Insert Vertex = v               Insert Edge = e\n");
        printf("Depth First Search = d          Breath First Search = b\n");
        printf("Print Graph = p                 Quit = q\n");
        printf("----------------------------------------------------------------\n");
        
        printf("Command = ");
		scanf(" %c", &command);

        switch(command) {
		case 'z': case 'Z':
			Initialize_Graph(&g);
			break;
		case 'q': case 'Q':
			Freenode(g);          //할당된 메모리 해제,프로그램 종료
			break;
		case 'v': case 'V':
			Insert_Vertex(g);
			break;
		case 'e': case 'E':
			printf("Your Vertex_key1, Vertex_key2 = ");
            scanf(" %d %d", &vertex_key1, &vertex_key2);
            Insert_Edge(g, vertex_key1, vertex_key2);
			break;
		case 'd': case 'D':
            printf("Your Vertex_key = ");
            scanf(" %d", &key);
            reset_Visitedlist();
			Depth_First_Search(g,key);
            printf("\n");
			break;
        case 'b': case 'B':
            printf("your Vertex_key = ");
            scanf(" %d", &key);
            reset_Visitedlist();
            Breadth_First_Search(g,key);
            printf("\n");
            break;
        case 'p': case 'P':
            Print_Graph(g);
            break;
		default:                //메뉴 이외의 다른 값 입력 시 에러문구 출력
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}


    }while(command != 'q' && command != 'Q');


}

void Initialize_Graph(graph** g){
    
    if(*g){
        Freenode(*g);
    }
   *g=(graph*)malloc(sizeof(graph));
   (*g)->n=-1;
   for(int i=0;i<VERTEX_SIZE;i++){
       (*g)->headnode_ptr[i]=NULL;
   }
}

void Insert_Vertex(graph* g){
    if (g->n + 1 == VERTEX_SIZE) {
		return;
	}
	g->n=g->n+1;
}
void Insert_Edge(graph* g, int key1, int key2){
    if ((key1 > g->n) || (key2 > g->n)) {
		return;
	}

    graphnode* v=(graphnode*)malloc(sizeof(graphnode));
    v->key=key2;
    v->link=NULL;
    graphnode* p=g->headnode_ptr[key1];
    graphnode* t;
    if(g->headnode_ptr[key1]==NULL){
        g->headnode_ptr[key1]=v;
        return;
    }else{
        while(p){
            t=p;
            p=p->link;
        }
        t->link=v;
    }
}

/*void Depth_First_Search(graph* g, int key){
    graphnode* w;
    visitedNode[key]=1;
    printf("%d ",key);
    for(w=g->headnode_ptr[key];w;w=w->link){
        if(!visitedNode[w->key]){
            Depth_First_Search(g,w->key);
        }
    }
}*/
void Depth_First_Search(graph* g, int key){
    graphnode* w;
    graphnode* p;
    int min_key;
    int n=0;
    top=-1;
    push(key);
    visitedNode[key]=1;
    printf("%d ", key);

    while(!isEmpty_S()){
        key=pop();
        w=g->headnode_ptr[key];
        p=g->headnode_ptr[key];
        reset_non_visited();
        n=0;
        while(p){
            if(!visitedNode[p->key]){
                non_visited[n]=p->key;
                n++;
            }
            p=p->link;
        }
        if(n){
            min_key=non_visited[0];
            for(int i=0;i<n;i++){
                if(min_key>non_visited[i]){
                    min_key=non_visited[i];
                }
            }
        }

        if(!visitedNode[min_key]){
                push(key);
                push(min_key);
                printf("%d ",min_key);
                visitedNode[min_key]=1;
                key=min_key;
        }
    }

}

void Breadth_First_Search(graph* g, int key) {
	graphnode* w;
    int n=0;
    int min_key=0;
	visitedNode[key] = 1;
    printf("%d ",key);
	enqueue(key);

	while (!isEmpty_Q()) {
        n=0;
		key = dequeue();
        reset_non_visited();

		for (w = g->headnode_ptr[key] ; w; w = w->link) {
            if (!visitedNode[w->key]) {
                non_visited[n++]=w->key;
			}
        }
        if(n) sortqueue(n);
        for(int i=0;i<n;i++){
            min_key=non_visited[i];
		    visitedNode[min_key] = 1;
            printf("%d ",min_key);
            enqueue(min_key);
        }
    }
}

void Print_Graph(graph* g){
    graphnode* p;
    for(int i=0;i<=g->n;i++){
        p=g->headnode_ptr[i];
        printf("graphlist[%d] : ",i);
        while(p){
            printf("-> %d ", p->key);
            p=p->link;
        }
        printf("\n");
    }
}

void Freenode(graph* g){
    graphnode* p;
    graphnode* pre;
    for(int i=0;i<=g->n;i++){
        p=g->headnode_ptr[i];
        while(p){
            pre=p;
            p=p->link;
            free(pre);
        }
    }
    free(g);
}

void reset_Visitedlist(){
    for(int i=0; i<VERTEX_SIZE;i++){
        visitedNode[i]=0;
        non_visited[i]=0;
    }
}
void reset_non_visited(){
    for(int i=0; i<VERTEX_SIZE;i++){
        non_visited[i]=0;
    }
}

void push(int key){
    top++;
    stacknode[top]=key;
}
int pop(){
    return stacknode[top--];
}

int isEmpty_S(){
    if(top==-1){
        return 1;
    }
    return 0;
}

int isEmpty_Q(){
    if(rear==front){
        return 1;
    }
    return 0;
}

void enqueue(int key){
    rear++;
    queuenode[rear]=key;
}

int dequeue(){
    front++;
    return queuenode[front];
}

void sortqueue(int n){
    int min;
    int temp, index;
    for(int a=0;a<n;a++){
        min=non_visited[a];
        index=a;
        for(int b=a+1;b<n;b++){
            if(min>non_visited[b]){
                min=non_visited[b];
                index=b;
            }
        }
        temp=non_visited[a];
        non_visited[a]=min;
        non_visited[index]=temp;
    }
}