#include <stdio.h>
#include <stdlib.h>
#define VERTEX_SIZE 10

typedef struct Node{    //���� ����ü
    struct Node* link;
    int key;
}graphnode;

typedef struct graph_p{ //������ ������ ������ ������ �� �ִ� �迭�� ���� ����ü
    int n;
    struct Node* headnode_ptr[VERTEX_SIZE];
}graph;



int visitedNode[VERTEX_SIZE];   //������ �湮 �ߴ��� ����ϴ� �迭
int non_visited[VERTEX_SIZE];   //�湮���� ���� ������ ���� �ϴ� �迭
int stacknode[VERTEX_SIZE];     //���� �迭
int queuenode[VERTEX_SIZE];     //ť �迭
int top=-1;
int rear=-1;
int front=-1;

void Initialize_Graph(graph** g);   //�׷��� ����
void Insert_Vertex(graph* g);       //���� �߰�
void Insert_Edge(graph* g, int key1, int key2); //���� �߰�(����)
void Depth_First_Search(graph* g,int key);  //���� �켱 Ž��
void Breadth_First_Search(graph* g, int key);   //���� �켱 Ž��
void Print_Graph(graph* g);     //�׷����� ������ ������ ���
void Freenode(graph* g);        //�Ҵ�� �޸� ����
void reset_Visitedlist();       //���� �湮 ��� �迭 �ʱ�ȭ
void reset_non_visited();       //�湮���� ���� ���� ��� �迭 �ʱ�ȭ
int isEmpty_S();    //������ ����ִ��� Ȯ��
int isEmpty_Q();    //ť�� ����ִ��� Ȯ��
void push(int key); //���ÿ� push
int pop();  //���ÿ��� pop
void enqueue(int key);  //ť�� enqueue
int dequeue();     //ť���� dequeue
void sortqueue(int n); //�湮���� ���� ���� ������ ������������ ����

int main(){
    char command;
    int key;
    int vertex_key1, vertex_key2;

    graph* g=NULL;

    printf("[----- [����ȭ] [2019038062] -----]\n");
    do{
        printf("----------------------------------------------------------------\n");   //�޴� ���
        printf("                       Graph Searches\n");
        printf("----------------------------------------------------------------\n");
        printf("Initialize Graph = z\n");
        printf("Insert Vertex = v               Insert Edge = e\n");
        printf("Depth First Search = d          Breath First Search = b\n");
        printf("Print Graph = p                 Quit = q\n");
        printf("----------------------------------------------------------------\n");
        
        printf("Command = ");
		scanf(" %c", &command); //�޴� ����

        switch(command) {
		case 'z': case 'Z':
			Initialize_Graph(&g);   //�׷��� ����
			break;
		case 'q': case 'Q':
			Freenode(g);          //�Ҵ�� �޸� ����,���α׷� ����
			break;
		case 'v': case 'V':
			Insert_Vertex(g);       //���� �߰�
			break;
		case 'e': case 'E':
			printf("Your Vertex_key1, Vertex_key2 = ");
            scanf(" %d %d", &vertex_key1, &vertex_key2);
            Insert_Edge(g, vertex_key1, vertex_key2);   //���� �߰�
			break;
		case 'd': case 'D':
            printf("Your Vertex_key = ");
            scanf(" %d", &key);
            reset_Visitedlist();        //�湮�� ���� ��� �迭 �ʱ�ȭ
			Depth_First_Search(g,key);      //���� �켱 Ž��
            printf("\n");
			break;
        case 'b': case 'B':
            printf("your Vertex_key = ");
            scanf(" %d", &key);
            reset_Visitedlist();        //�湮�� ���� ��� �迭 �ʱ�ȭ
            Breadth_First_Search(g,key);    //���� �켱 Ž��
            printf("\n");
            break;
        case 'p': case 'P':
            Print_Graph(g);     //�׷����� ������ ������ ���
            break;
		default:                //�޴� �̿��� �ٸ� �� �Է� �� �������� ���
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}


    }while(command != 'q' && command != 'Q');   //q�� Q�� �Է¹����� �ݺ��� ����


}

void Initialize_Graph(graph** g){
    
    if(*g){     //g�� NULL�� �ƴϸ�
        Freenode(*g);   //�Ҵ�� �޸� ����
    }
   *g=(graph*)malloc(sizeof(graph));    //���� �޸� �Ҵ�
   (*g)->n=-1;  //������ ���� -1�� �ʱ�ȭ
   for(int i=0;i<VERTEX_SIZE;i++){
       (*g)->headnode_ptr[i]=NULL;
   }
}

void Insert_Vertex(graph* g){
    if(g==NULL){    //g�� NULL�̸�(�׷����� ������ ���°� �ƴϸ�)
        printf("Graph does not exist.\n");  //�������� ���
        return;     //�Լ� ����
    }
    
    if (g->n + 1 == VERTEX_SIZE) {  //������ ����+1�� 10�̸�
		printf("please input 0~9 number.\n");   //�������� ���
        return;     //�Լ� ����
	}
	g->n=g->n+1;    //������ ���� +1
}
void Insert_Edge(graph* g, int key1, int key2){
    if(g==NULL){    //g�� NULL�̸�(�׷����� ������ ���°� �ƴϸ�)
        printf("Graph does not exist.\n");  //�������� ���
        return;     //�Լ� ����
    }

    if ((key1 > g->n) || (key2 > g->n)) {   //�Է¹��� ���� �� �Ѱ��� ������ ������ ū ���̸�
        printf("vertex does not exist.\n"); //�������� ���
		return; //�Լ� ����
	}

    graphnode* v=(graphnode*)malloc(sizeof(graphnode)); //���� ����(���� �޸� �Ҵ�)
    v->key=key2;
    v->link=NULL;
    graphnode* p=g->headnode_ptr[key1]; //p�� g�� headnode_ptr[key1]�� ����
    graphnode* t;
    if(g->headnode_ptr[key1]==NULL){    //key1�� ������ ���ٸ�
        g->headnode_ptr[key1]=v;    //key1�� v����
        return;     //�Լ� ����
    }else{      //������ ������ �̹� �ִٸ�
        while(p){   //p�� NULL�϶����� p �̵�
            t=p;    //p�� NULL�϶� t�� p�� ���� ����
            p=p->link;
        }
        t->link=v;  //t�� ���� ������ v����
    }
}

/*void Depth_First_Search(graph* g, int key){       //��� ȣ�� ���(vertex ��ȣ�� ���� ������� Ž���� �ȵ� �� ����)
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
    if(g==NULL){    //g�� NULL�̸�(�׷����� ������ ���°� �ƴϸ�)
        printf("Graph does not exist.\n");  //�������� ���
        return;     //�Լ� ����
    }
    if(key>g->n){   //Ž���� ���� �� key�� ������ ������ �������� Ŭ ���
        printf("vertex does not exist.\n"); //�������� ���
        return; //�Լ� ����
    }
    graphnode* w;
    graphnode* p;
    int min_key;
    int n=0;
    top=-1;
    push(key);  //ó�� key�� push
    visitedNode[key]=1; //�湮 ���
    printf("%d ", key);

    while(!isEmpty_S()){    //������ ����ִٸ� �ݺ��� ����
        key=pop();  //���ÿ��� pop�Ͽ� key���� ����
        w=g->headnode_ptr[key]; //w�� key�� ����� ù ����
        p=g->headnode_ptr[key]; //p�� key�� ����� ù ����
        reset_non_visited();    //�湮 ���� ���� ������ �����ϴ� �迭 �ʱ�ȭ
        n=0;
        while(p){   //p�� NULL�̸� �ݺ� ����
            if(!visitedNode[p->key]){   //�湮���� ���� �����̸�
                non_visited[n]=p->key;  //�迭�� key�� ����
                n++;
            }
            p=p->link;  //p�� ����� ���� ������ ����Ŵ
        }
        if(n){  //n�� 1�̻��̸� �湮���� ���� ������ �����Ѵٴ� ��
            min_key=non_visited[0];
            for(int i=0;i<n;i++){   //�湮���� ���� ������ �� �ּڰ��� min_key�� ����
                if(min_key>non_visited[i]){
                    min_key=non_visited[i];
                }
            }
        }

        if(!visitedNode[min_key]){  //min_key�� �ش��ϴ� ������ �湮���� �ʾ�����
                push(key);      //������ pop�ߴ� key�� �ٽ� push
                push(min_key);  //���� ���� �� ���� ���� ������ push
                printf("%d ",min_key);  //���
                visitedNode[min_key]=1; //�湮 ���=1
                key=min_key;
        }
    }

}

void Breadth_First_Search(graph* g, int key) {
    if(g==NULL){    //g�� NULL�̸�(�׷����� ������ ���°� �ƴϸ�)
        printf("Graph does not exist.\n");  //�������� ���
        return;     //�Լ� ����
    }
    if(key>g->n){   //Ž���� ���� �� key�� ������ ������ �������� Ŭ ���
        printf("vertex does not exist.\n"); //�������� ���
        return; //�Լ� ����
    }
	graphnode* w;
    int n=0;
    int min_key=0;
	visitedNode[key] = 1;   //�湮 ��� = 1
    printf("%d ",key);
	enqueue(key);   //ó�� key�� enqueue

	while (!isEmpty_Q()) {  //ť�� ����ִٸ� �ݺ��� ����
        n=0;
		key = dequeue();    //key�� dequeue�� ���� ����
        reset_non_visited();    //�湮���� ���� ������ ������ �迭 �ʱ�ȭ

		for (w = g->headnode_ptr[key] ; w; w = w->link) {
            if (!visitedNode[w->key]) { //w�� ����� ��� �����߿��� �湮���� ���� ������ �迭�� ����
                non_visited[n++]=w->key;
			}
        }
        if(n) sortqueue(n); //������������ �迭 ����
        for(int i=0;i<n;i++){   //�湮 ���� ���� ������ ������ŭ �ݺ�
            min_key=non_visited[i]; //key���� min_key�� ����
		    visitedNode[min_key] = 1;   //�湮 ���
            printf("%d ",min_key);  //���
            enqueue(min_key);   //enqueue
        }
    }
}

void Print_Graph(graph* g){
    if(g==NULL){    //g�� NULL�̸�(�׷����� ������ ���°� �ƴϸ�)
        printf("Graph does not exist.\n");  //�������� ���
        return;     //�Լ� ����
    }
    graphnode* p;
    for(int i=0;i<=g->n;i++){   //������ ������ ������ŭ �ݺ�
        p=g->headnode_ptr[i];
        printf("graphlist[%d] : ",i);   //���� ���
        while(p){
            printf("-> %d ", p->key);   //������ ����� ���� ���� ���
            p=p->link;
        }
        printf("\n");
    }
}

void Freenode(graph* g){
    if(g==NULL){    //g�� NULL�̸�(�׷����� ������ ���°� �ƴϸ�)
        printf("Graph does not exist.\n");  //�������� ���
        return;     //�Լ� ����
    }
    graphnode* p;
    graphnode* pre;
    for(int i=0;i<=g->n;i++){   //������ ������ ������ŭ �ݺ�
        p=g->headnode_ptr[i];
        while(p){       //����i�� ���� ������ �Ҵ�� �޸� ����
            pre=p;
            p=p->link;
            free(pre);
        }
    }
    free(g);    //�׷����� �Ҵ�� �޸� ����
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

void sortqueue(int n){  //���� ���� ����
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