#include <stdio.h>
#include <stdlib.h>
#define VERTEX_SIZE 10

typedef struct Node{    //정점 구조체
    struct Node* link;
    int key;
}graphnode;

typedef struct graph_p{ //정점의 개수와 정점을 저장할 수 있는 배열을 가진 구조체
    int n;
    struct Node* headnode_ptr[VERTEX_SIZE];
}graph;



int visitedNode[VERTEX_SIZE];   //정점을 방문 했는지 기록하는 배열
int non_visited[VERTEX_SIZE];   //방문하지 않은 정점을 저장 하는 배열
int stacknode[VERTEX_SIZE];     //스택 배열
int queuenode[VERTEX_SIZE];     //큐 배열
int top=-1;
int rear=-1;
int front=-1;

void Initialize_Graph(graph** g);   //그래프 생성
void Insert_Vertex(graph* g);       //정점 추가
void Insert_Edge(graph* g, int key1, int key2); //간선 추가(방향)
void Depth_First_Search(graph* g,int key);  //깊이 우선 탐색
void Breadth_First_Search(graph* g, int key);   //넓이 우선 탐색
void Print_Graph(graph* g);     //그래프의 정점과 간선을 출력
void Freenode(graph* g);        //할당된 메모리 해제
void reset_Visitedlist();       //정점 방문 기록 배열 초기화
void reset_non_visited();       //방문하지 않은 정점 기록 배열 초기화
int isEmpty_S();    //스택이 비어있는지 확인
int isEmpty_Q();    //큐가 비어있는지 확인
void push(int key); //스택에 push
int pop();  //스택에서 pop
void enqueue(int key);  //큐에 enqueue
int dequeue();     //큐에서 dequeue
void sortqueue(int n); //방문하지 않은 인접 정점을 오름차순으로 정렬

int main(){
    char command;
    int key;
    int vertex_key1, vertex_key2;

    graph* g=NULL;

    printf("[----- [염중화] [2019038062] -----]\n");
    do{
        printf("----------------------------------------------------------------\n");   //메뉴 출력
        printf("                       Graph Searches\n");
        printf("----------------------------------------------------------------\n");
        printf("Initialize Graph = z\n");
        printf("Insert Vertex = v               Insert Edge = e\n");
        printf("Depth First Search = d          Breath First Search = b\n");
        printf("Print Graph = p                 Quit = q\n");
        printf("----------------------------------------------------------------\n");
        
        printf("Command = ");
		scanf(" %c", &command); //메뉴 선택

        switch(command) {
		case 'z': case 'Z':
			Initialize_Graph(&g);   //그래프 생성
			break;
		case 'q': case 'Q':
			Freenode(g);          //할당된 메모리 해제,프로그램 종료
			break;
		case 'v': case 'V':
			Insert_Vertex(g);       //정점 추가
			break;
		case 'e': case 'E':
			printf("Your Vertex_key1, Vertex_key2 = ");
            scanf(" %d %d", &vertex_key1, &vertex_key2);
            Insert_Edge(g, vertex_key1, vertex_key2);   //간선 추가
			break;
		case 'd': case 'D':
            printf("Your Vertex_key = ");
            scanf(" %d", &key);
            reset_Visitedlist();        //방문한 정점 기록 배열 초기화
			Depth_First_Search(g,key);      //깊이 우선 탐색
            printf("\n");
			break;
        case 'b': case 'B':
            printf("your Vertex_key = ");
            scanf(" %d", &key);
            reset_Visitedlist();        //방문한 정점 기록 배열 초기화
            Breadth_First_Search(g,key);    //넓이 우선 탐색
            printf("\n");
            break;
        case 'p': case 'P':
            Print_Graph(g);     //그래프의 정점과 간선을 출력
            break;
		default:                //메뉴 이외의 다른 값 입력 시 에러문구 출력
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}


    }while(command != 'q' && command != 'Q');   //q나 Q를 입력받으면 반복문 종료


}

void Initialize_Graph(graph** g){
    
    if(*g){     //g가 NULL이 아니면
        Freenode(*g);   //할당된 메모리 해제
    }
   *g=(graph*)malloc(sizeof(graph));    //동적 메모리 할당
   (*g)->n=-1;  //정점의 개수 -1로 초기화
   for(int i=0;i<VERTEX_SIZE;i++){
       (*g)->headnode_ptr[i]=NULL;
   }
}

void Insert_Vertex(graph* g){
    if(g==NULL){    //g가 NULL이면(그래프가 생성된 상태가 아니면)
        printf("Graph does not exist.\n");  //에러문구 출력
        return;     //함수 종료
    }
    
    if (g->n + 1 == VERTEX_SIZE) {  //정점의 개수+1이 10이면
		printf("please input 0~9 number.\n");   //에러문구 출력
        return;     //함수 종료
	}
	g->n=g->n+1;    //정점의 개수 +1
}
void Insert_Edge(graph* g, int key1, int key2){
    if(g==NULL){    //g가 NULL이면(그래프가 생성된 상태가 아니면)
        printf("Graph does not exist.\n");  //에러문구 출력
        return;     //함수 종료
    }

    if ((key1 > g->n) || (key2 > g->n)) {   //입력받은 정점 중 한개라도 정점의 수보다 큰 값이면
        printf("vertex does not exist.\n"); //에러문구 출력
		return; //함수 종료
	}

    graphnode* v=(graphnode*)malloc(sizeof(graphnode)); //정점 생성(동적 메모리 할당)
    v->key=key2;
    v->link=NULL;
    graphnode* p=g->headnode_ptr[key1]; //p에 g의 headnode_ptr[key1]를 저장
    graphnode* t;
    if(g->headnode_ptr[key1]==NULL){    //key1에 간선이 없다면
        g->headnode_ptr[key1]=v;    //key1에 v저장
        return;     //함수 종료
    }else{      //생성된 간선이 이미 있다면
        while(p){   //p가 NULL일때까지 p 이동
            t=p;    //p가 NULL일때 t는 p의 이전 정점
            p=p->link;
        }
        t->link=v;  //t의 다음 정점에 v저장
    }
}

/*void Depth_First_Search(graph* g, int key){       //재귀 호출 방법(vertex 번호가 적은 순서대로 탐색이 안될 수 있음)
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
    if(g==NULL){    //g가 NULL이면(그래프가 생성된 상태가 아니면)
        printf("Graph does not exist.\n");  //에러문구 출력
        return;     //함수 종료
    }
    if(key>g->n){   //탐색을 시작 할 key가 생성된 정점의 개수보다 클 경우
        printf("vertex does not exist.\n"); //에러문구 출력
        return; //함수 종료
    }
    graphnode* w;
    graphnode* p;
    int min_key;
    int n=0;
    top=-1;
    push(key);  //처음 key를 push
    visitedNode[key]=1; //방문 기록
    printf("%d ", key);

    while(!isEmpty_S()){    //스택이 비어있다면 반복문 종료
        key=pop();  //스택에서 pop하여 key값에 저장
        w=g->headnode_ptr[key]; //w는 key에 연결된 첫 정점
        p=g->headnode_ptr[key]; //p는 key에 연결된 첫 정점
        reset_non_visited();    //방문 하지 않은 정점을 저장하는 배열 초기화
        n=0;
        while(p){   //p가 NULL이면 반복 종료
            if(!visitedNode[p->key]){   //방문하지 않은 정점이면
                non_visited[n]=p->key;  //배열에 key를 저장
                n++;
            }
            p=p->link;  //p는 연결된 다음 정점을 가리킴
        }
        if(n){  //n이 1이상이면 방문하지 않은 정점이 존재한다는 뜻
            min_key=non_visited[0];
            for(int i=0;i<n;i++){   //방문하지 않은 정점들 중 최솟값을 min_key에 저장
                if(min_key>non_visited[i]){
                    min_key=non_visited[i];
                }
            }
        }

        if(!visitedNode[min_key]){  //min_key에 해당하는 정점에 방문하지 않았으면
                push(key);      //이전에 pop했던 key를 다시 push(인접 정점이 있기 때문에)
                push(min_key);  //인접 정점 중 가장 작은 정점을 push
                printf("%d ",min_key);  //출력
                visitedNode[min_key]=1; //방문 기록=1
                key=min_key;
        }
    }

}

void Breadth_First_Search(graph* g, int key) {
    if(g==NULL){    //g가 NULL이면(그래프가 생성된 상태가 아니면)
        printf("Graph does not exist.\n");  //에러문구 출력
        return;     //함수 종료
    }
    if(key>g->n){   //탐색을 시작 할 key가 생성된 정점의 개수보다 클 경우
        printf("vertex does not exist.\n"); //에러문구 출력
        return; //함수 종료
    }
	graphnode* w;
    int n=0;
    int min_key=0;
	visitedNode[key] = 1;   //방문 기록 = 1
    printf("%d ",key);
	enqueue(key);   //처음 key를 enqueue

	while (!isEmpty_Q()) {  //큐가 비어있다면 반복문 종료
        n=0;
		key = dequeue();    //key에 dequeue한 값을 저장
        reset_non_visited();    //방문하지 않은 정점을 저장할 배열 초기화

		for (w = g->headnode_ptr[key] ; w; w = w->link) {
            if (!visitedNode[w->key]) { //w에 연결된 모든 정점중에서 방문하지 않은 정점을 배열에 저장
                non_visited[n++]=w->key;
			}
        }
        if(n) sortqueue(n); //오름차순으로 배열 정렬
        for(int i=0;i<n;i++){   //방문 하지 않은 정점의 개수만큼 반복
            min_key=non_visited[i]; //key값을 min_key에 저장
		    visitedNode[min_key] = 1;   //방문 기록
            printf("%d ",min_key);  //출력
            enqueue(min_key);   //enqueue
        }
    }
}

void Print_Graph(graph* g){
    if(g==NULL){    //g가 NULL이면(그래프가 생성된 상태가 아니면)
        printf("Graph does not exist.\n");  //에러문구 출력
        return;     //함수 종료
    }
    graphnode* p;
    for(int i=0;i<=g->n;i++){   //생성된 정점의 개수만큼 반복
        p=g->headnode_ptr[i];
        printf("graphlist[%d] : ",i);   //정점 출력
        while(p){
            printf("-> %d ", p->key);   //정점에 연결된 인접 정점 출력
            p=p->link;
        }
        printf("\n");
    }
}

void Freenode(graph* g){
    if(g==NULL){    //g가 NULL이면(그래프가 생성된 상태가 아니면)
        printf("Graph does not exist.\n");  //에러문구 출력
        return;     //함수 종료
    }
    graphnode* p;
    graphnode* pre;
    for(int i=0;i<=g->n;i++){   //생성된 정점의 개수만큼 반복
        p=g->headnode_ptr[i];
        while(p){       //정점i의 인접 정점에 할당된 메모리 해제
            pre=p;
            p=p->link;
            free(pre);
        }
    }
    free(g);    //그래프에 할당된 메모리 해제
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

void sortqueue(int n){  //오름 차순 정렬
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
