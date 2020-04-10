#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int Status;              //返回值状态

/*日志
3.29:完成快递员登录,采用单链表,快递格口采用预定义长度的带头双链表,
头结点存放快递柜信息(总数,存数,现指向地址)
3.30:null
3.31:未完成取件码存储,未完成格口的取出
4.2:手机号的取出,快递员的取出,手机号子表的插入
4.6:大改,chaos tap mad,未完成:取件码生成,存快递
*/

//******************node*****type*************************此处定义所有节点类型
typedef struct locker_node* locker_pointer; //指向柜口节点的指针

//柜口

struct locker_node {					//定义柜口节点
	int num;							//柜口号码(头结点存放总结点数)
	int state;							//柜子状态(0/1)(头结点存放已使用数)
	locker_pointer prior,next;				//next,prior域
};
//取件码
typedef struct code_addr_node* code_addr_pointer;
typedef struct code_node* code_pointer;
typedef struct record_node* record_pointer;
typedef struct phonenum_node* phonenum_pointer;
typedef struct deliverman_node* deliverman_pointer;
typedef struct record_node* record_pointer;
struct stuffs {
	char num[6];						//对应物品5位单号
	int iftimeout;						//是否超时(1/0/-1)-1为取出,0正常状态,1超时
	char phonenum[12];					//号码
};
struct code_node {
	struct stuffs stuff;
	char code[7];						//取件码
	locker_pointer locker_p;					//指向柜口节点指针
	phonenum_pointer  phonenum_p;				//指向手机号码节点指针(待定)
	deliverman_pointer deliverman_p;				//指向快递员节点指针
	code_pointer prior,next;					
};
struct record_node {//快递员子表(快递记录)
	struct stuffs stuff;
	code_pointer code_p;//物品对应取件码节点位置
	record_pointer next;
};
//电话号码
struct phonenum_node {
	char phonenum[12];					//定义号码11位类型
	int amount;							//定义该号码存储的快递总数
	code_addr_pointer sublist_front,sublist_rear,sublist_prior;			//定义子表头指针
	phonenum_pointer prior,next;				//定义next域
};
struct code_addr_node {//包含取件码地址的节点,是手机号广义表的子表元素
	code_pointer code_P;
	code_addr_pointer  next;
};
//快递员信息
struct deliverman_node
{
	char Dname[10];						//快递员姓名
	char Dphonenum[12];					//快递员电话号码
	char Dcode[7];						//快递员工号
	char psw[7];					//快递员密码
	record_pointer sublist;		//快递员所派送物品信息(副本)
	deliverman_pointer next;		//定义next域
};




//定义各类链表
typedef struct locker_node* locker_list;//定义柜口双链表类型 格间
typedef struct deliverman_node* deliverman_list;//定义快递员广义表
typedef struct code_node* code_list;//定义取件码链表
typedef struct phonenum_node* phonenum_list;//定义手机号广义表


//函数声明
deliverman_list init_deliverman_list();
code_list  init_code_list();
phonenum_list init_phonenum_list();
locker_list init_locker_list(int start, int end);
Status insert_deliverman_list(deliverman_list head, struct deliverman_node* node);
Status insert_locker_list(locker_list head);
Status insert_phonenum_list(phonenum_list head, struct phonenum_node* node);
Status insert_code_list(code_list list, struct code_node* node);
deliverman_pointer locate_deliverman_list(deliverman_list head, char Dcode[7]);
phonenum_pointer locate_phonenum_list(phonenum_list head, char phonenum[12]);
code_pointer locate_code_list(code_list list, char code[7]);
Status de_locker_list(locker_list head, locker_pointer node);
Status de_code_list(code_list list, code_pointer f);
Status put_stuff(code_list cL, deliverman_list dL, locker_list lL, phonenum_list pL,struct stuffs stuff);
Status get_stuff(code_list cL, deliverman_list dL, locker_list lL, phonenum_list pL,struct stuffs stuff);
Status sign_up(deliverman_list list);
Status sign_in(deliverman_list list,deliverman_pointer p);
int main()
{
	/*先预定义两个快递员并添加到内存*/
	deliverman_list DL = init_deliverman_list();//初始化快递链表
	locker_list lockers_L= init_locker_list(1,5);//各个码数不同的快递柜双链
	locker_list lockers_M= init_locker_list(6,14);
	locker_list lockers_S= init_locker_list(15,24);
	code_list cL = init_code_list();
	phonenum_list pL = init_phonenum_list();
	
	deliverman_pointer D1 = (deliverman_pointer)malloc(sizeof(struct deliverman_node));
	deliverman_pointer D2 = (deliverman_pointer)malloc(sizeof(struct deliverman_node));


	strcpy(D1->Dname, "张三");
	strcpy(D1->Dcode, "000001");
	strcpy(D1->Dphonenum, "13927367681");
	strcpy(D1->psw, "000001");


	strcpy(D2->Dname, "李四");
	strcpy(D2->Dcode, "000002");
	strcpy(D2->Dphonenum, "13565741386");
	strcpy(D2->psw, "000002");
	/*将快递员插入链表*/
	insert_deliverman_list(DL, D1);
	insert_deliverman_list(DL, D2);
	//printf("%s\n", DL->next->next->Dname);
	//************************************************************************以上均为测试数据
	while (1)
	{
		char i;
		printf("欢迎使用格口递柜\n");
		printf("请选择功能:输入功能序号\n");
		printf("1.取快递\n");
		printf("2.管理员模式\n");
		scanf("%c", &i);//读取输入字符
		getchar();
		if (i == '1')//用户接口
		{
			printf("1.取快递\n");
			printf("2.返回上一层\n");
			scanf("%c",&i);
			getchar();
		}
		else if (i == '2')//管理员接口
		{
			while (1)
			{
				printf("1.登录\n");
				printf("2.注册\n");
				printf("3.返回上一层\n");
				scanf("%c", &i);
				if (i == '1')
				{
					struct deliverman_node d;
					deliverman_pointer dP = &d;
					if (sign_in(DL,dP) == TRUE)//登录成功
					{
						getchar();
						printf("登录成功\n");
						while (1) 
						{
							/*快递员界面*/
							printf("1.投递快递\n");
							printf("2.取出快递\n");
							printf("3.查询快递柜状态\n");
							printf("4.查询已投递快递状态\n");
							printf("5.返回主页\n");
							scanf("%c", &i);
							getchar();
							if (i == '1')
							{
								struct stuffs stuff;
								stuff.iftimeout = -1;
								printf("读取快递信息\n");
								printf("输入快递单号\n");
								scanf("%s",stuff.num);
								printf("输入快递号码\n");
								scanf("%s", stuff.phonenum);
								printf("请选择格口大小(S/M/L)\n");
								getchar();
								scanf("%c", &i);
								getchar();
								switch (i)
								{
								case 'S':put_stuff(cL,dP,lockers_S,pL,stuff); break;
								case 'M':put_stuff(cL, dP, lockers_M, pL, stuff); break;
								case 'L':put_stuff(cL, dP, lockers_L, pL, stuff); break;
								default:
									printf("输入错误,按任意键返回上一层\n");
									getchar();
									continue;
								}
								printf("投递成功!\n");
								printf("发送的信息\n");
							}
							else if (i == '2')
							{
								printf("2尚在完善中\n");
								printf("按任意键返回上一层\n");
								getchar();
								continue;
							}
							else if (i == '3')
							{

								printf("当前快递柜状态\n");
								printf("当前大号柜已用%d/%d\n",lockers_L->state,lockers_L->num);
								printf("当前中号柜已用%d/%d\n",lockers_M->state, lockers_M->num);
								printf("当前小号柜已用%d/%d\n",lockers_S->state, lockers_S->num);
								printf("按任意键返回上一层\n");
								getchar();
								continue;
							}
							else if (i == '4')
							{
								printf("4尚在完善中\n");
								printf("按任意键返回上一层\n");
								getchar();
								continue;
							}
							else if (i == '5')
							{
								break;
							}
							else
							{
								printf("输入错误,按任意键重新选择\n");
								getchar();
								continue;
							}
						}						
					}
					else//输入密码错误
					{
						printf("密码错误或者账号不存在!按任意键返回上一层\n");
						getchar();
						continue;
					}
					break;
				}
				else if (i == '2')
				{
					if (sign_up(DL)==TRUE)
					{
						printf("注册成功!按任意键返回上一层重新登录\n");
						getchar();
						continue;
					}
					else
					{
						printf("密码验证错误!按任意键返回上一层\n");
						getchar();
						continue;
					}
				}
				else if (i == '3')
				{
					break;
				}
				else
				{
					printf("选项错误请重新选择!\n");
					continue;
				}
			}
		}
		else
		{
			printf("输入错误！！ 输入任意键重新输入\n");
			continue;
		}
	}
	return 0;
}
//************************************************************************程序数据预设
void preinstall()
{


}
//************************************************************************初始化
deliverman_list init_deliverman_list() {							//初始化快递员链表
	deliverman_pointer head;
	head = (deliverman_list)malloc(sizeof(struct deliverman_node));
	head->next = NULL;
	return head;
}
code_list  init_code_list() {	//初始化取件码链表
	code_pointer  head;
	head = (struct code_node*)malloc(sizeof(struct code_node));
	head->next = NULL;
	head->prior = head;//头结点prior存储rear
	return head;
}
phonenum_list init_phonenum_list() 
{
	phonenum_list  head;
	head = (phonenum_list)malloc(sizeof(struct phonenum_node));
	head->next =NULL;
	head->sublist_front = head->sublist_rear = NULL;
	head->prior = head;//头结点指向rear
	return head;
}
locker_list init_locker_list(int start,int end)								//初始化快递柜,传入快递柜长度
{
	int i;
	if (start > end)
	{
		printf("数值输入错误");
		exit(0);
	}
	locker_pointer head,p,s;//p为当前新节点地址,s为该链最后节点分配地址
	head =s= (locker_pointer)malloc(sizeof(struct locker_node));//生成柜口链表头结点
	head->next =  NULL;
	head->prior = head;//头结点前驱地址存放当前指针所指位置
	head->num = end-start+1;//存储柜口链表的节点总数
	head->state = 0;//柜口已存储数
	for (i = start; i <= end; i++)
	{
		p = (locker_pointer)malloc(sizeof(struct locker_node));
		p->next = NULL;
		p->num = i ;//初始快递格口号码
		p->state = 0;//默认状态为关闭状态
		p->prior = s;
		s->next = p;	
		s = p;
	}
	return head;
}
//************************************************************************插入数据
Status insert_deliverman_list(deliverman_list head, struct deliverman_node* node) {//关于快递员链表的插入/注册方法
	deliverman_pointer p = node;			//p指针指向node	
	p->sublist = NULL;
	p->next = head->next;					//头插法入链表
	head->next = p;
	return OK;
}
Status insert_locker_list(locker_list head) {//关于快递柜方面的物品存入,addr返回分配柜子地址
	locker_pointer p = head->prior;

	if (p->next==NULL) {//若p指向链表最后一位,则说明存柜已满
		return ERROR;
	}
	p = p->next;//向后移动
	p->state = 1;//改变柜口状态
	head->state++;//存数加一
	head->prior = p;//存放最新的地址
	return OK;
}
Status insert_phonenum_list(phonenum_list head, struct phonenum_node* node)//关于取件顾客的号码节点的存储
{
	node->next = NULL;
	code_addr_pointer head1;
	head1 = (code_addr_pointer)malloc(sizeof(struct code_addr_node));
	head1->next = NULL;
	head1->code_P = NULL;
	node->sublist_front = head1;//子表头节点
	phonenum_pointer p = node;			//p指针指向node	
	phonenum_pointer rear = head->prior;
	rear->next=node;//后插入链表
	node->prior = rear;
	head->prior = node;
	return OK;
}
Status insert_code_list(code_list head, struct code_node* node)//关于取件码的存储
{
	code_pointer p = node;
	node->next = NULL;
	code_pointer rear = head->prior;
	rear->next = node;//后插入链表
	node->prior = rear;
	head->prior = node;
	return OK;
}
Status insert_code_addr_list(phonenum_pointer p, struct code_addr_node* node)
{
	p->sublist_rear->next=node;//尾插法
	p->sublist_rear = node;
	p->amount++;
	return OK;
}
Status insert_record_list(deliverman_pointer head,struct code_node* node) //快递员子表的插入
{
	record_pointer p;
	p = (record_pointer)malloc(sizeof(struct record_node));
	p->code_p=node;
	p->stuff = node->stuff;
	p->next=head->sublist;
	head->sublist = p;
	return OK;
}
//*************************************************************************查找数据
deliverman_pointer locate_deliverman_list(deliverman_list head, char Dcode[7]) {//登录查找工号,返回员工节点前驱地址
	deliverman_pointer p;
	p = head;
	while (p->next != NULL)
	{
		if (strcmp(p->next->Dcode, Dcode) == 0)
			break;
		else
			p = p->next;
	}
	return p;
}
phonenum_pointer locate_phonenum_list(phonenum_list head,char phonenum[12])//传入号码,链表,返回号码所在地址
{
	phonenum_pointer p;
	p = head->next;
	while (p->next != NULL)
	{
		if (strcmp(p->phonenum, phonenum) == 0)
			break;
		else
			p = p->next;
	}
	return p;
}
code_pointer locate_code_list(code_list head, char code[7]) {//传入取件码,链表,查找对应取件码的取件码节点地址
	code_pointer p=head->next;
	while (p != NULL)
	{
		if (strcmp(p->code, code) == 0)
			break;
		else
			p = p->next;
	}
	return p;//返回对应节点地址
}
code_pointer locate_code_addr_list(phonenum_pointer node, char code[7]) {//子链索引表查询
	code_addr_pointer p = node->sublist_front;
	while (p->next != NULL)
	{
		if (strcmp(p->next->code_P->code, code))
			break;
		else
			p = p->next;
	}
	node->sublist_prior = p;//对应节点的前驱节点,用于删除
	return p->next->code_P;//返回对应节点
}

//*********************************************************************链表的取出
Status de_locker_list(locker_list head, locker_pointer node) {//柜口取出物品后格口后移当今指定位置,不做销毁处理
	if (head->next == NULL)
	{
		printf("链空;");//处理安全问题,防止传入野指针
		return ERROR;
	}
	locker_pointer p = head->prior;
	node->prior->next = node->next;
	node->next->prior = node->prior;//取出node节点
	node->next = p->next;//node节点指向当前节点的next
	node->prior = p;//node节点前驱指向当前节点
	p->next = node;
	node->next->prior = p;
	return OK;
}
Status de_code_list(code_list head, code_pointer p)//取出取件码,p为取出节点地址
{
	code_pointer rear = head->prior;
	if (rear == head)
	{
		printf("链空;");
		return ERROR;
	}
	else if (p->next == NULL)
	{
		head->prior = p->prior;//当删除节点为尾结点,将尾结点指针指向头结点
	}
	else
	{
		p->prior->next=p->next;//移除前驱的指向
		p->next->prior = p->prior;
	}
	return OK;
}
Status de_phonenum_list(phonenum_list head, phonenum_pointer p)//取出手机号节点
{
	phonenum_pointer rear = head->prior;
	if (rear == head)
	{
		printf("链空;");
		return ERROR;
	}
	else if (p->next == NULL)
	{
		head->prior = p->prior;//当删除节点为尾结点,将尾结点指针指向头结点
	}
	else
	{
		p->prior->next = p->next;//移除前驱的指向
		p->next->prior = p->prior;
	}
	return OK;
}
Status de_deliverman_list(deliverman_list head, deliverman_pointer f) {//取出快递员节点,f为快递员前驱结点
	if (head->next==NULL) 
	{
		printf("链空;");
		return ERROR;
	}
	deliverman_pointer p = f->next;
	f->next = p->next;
	return OK;
}
Status de_code_addr_list(phonenum_pointer node, code_addr_pointer f) {//取出号码节点的子表中节点()
	if (node->sublist_front->next== NULL)
	{
		printf("子链空;");
		return ERROR;
	}
	code_addr_pointer p = f->next;
	f->next = p->next;
	return OK;
}
//**********************************************************************************其他操作
void create_code(char *Scode)//返回随机的6位取件码
{
	int code;
	srand((unsigned)time(NULL));
	code = rand() % (999999 - 100000 + 1) + 100000;
	_itoa(code, Scode, 10);
}
Status sign_in(deliverman_list list,deliverman_pointer p) {//登录
	char Dcode[7], psw[7];
	printf("账户(工号):");
	scanf("%s", Dcode);
	printf("密码:");
	scanf("%s", psw);
	p = locate_deliverman_list(list, Dcode);//查找链表中快递员节点,返回前驱节点
	if (strcmp(p->next->psw, psw) == 0)//登录成功
		return TRUE;
	else
		return FALSE;

}
Status sign_up(deliverman_list list) {//注册
	char  psw2[7];
	struct deliverman_node n;
	printf("账户(工号):\n");
	scanf("%s", n.Dcode);
	printf("手机号:\n");
	scanf("%s", n.Dphonenum);
	printf("姓名:\n");
	scanf("%s", n.Dname);
	printf("密码:\n");
	scanf("%s", n.psw);
	printf("再次确认密码:\n");
	scanf("%s", psw2);
	if (strcmp(n.psw, psw2) == 0)
	{
		insert_deliverman_list(list, &n);
		return TRUE;
	}
	else
		return FALSE;
}
//******************************************************************************************复合操作(存取)
Status get_stuff(code_list cL,deliverman_pointer dP,locker_list lL,phonenum_list pL, struct stuffs stuff) {//取件(dL为当前快递员的节点)

	return OK;
}
Status put_stuff(code_list cL, deliverman_pointer dP, locker_list lL, phonenum_list pL, struct stuffs stuff) {//投件
	code_pointer cP;
	phonenum_pointer pP;
	code_addr_pointer caP;
	cP = (code_pointer)malloc(sizeof(struct code_node));//新建取件码节点
	caP = (code_addr_pointer)malloc(sizeof(struct code_addr_node));//索引节点
	char Scode[7];
	/*关于格口的操作*/
	if(insert_locker_list(lL)==ERROR);//自动分配格口,传入(返回)所需的格口地址
		return FALSE;//柜满则入柜失败
   /*关于号码的操作*/
	pP = locate_phonenum_list(pL, stuff.phonenum);
	if (pP == NULL)//号码链表不存在该号码,为新号码
	{
		pP = (phonenum_pointer)malloc(sizeof(struct phonenum_node));
		strcpy(pP->phonenum, stuff.phonenum);
		insert_phonenum_list(pL, pP);//插入手机号
	}
	/*关于取件码的操作*/
	create_code(Scode);
	strcpy(cP->code,Scode );//生成取件码并写入取件码节点
	cP->stuff = stuff;
	cP->locker_p = lL->prior;//存入分配的格口地址
	cP->phonenum_p = pP;//存入电话号地址
	cP->deliverman_p = dP;//存入快递员地址
	/*关于取件码的索引表操作*/
	caP->code_P = cP;
	/*节点插入操作*/
	insert_code_list(cL, cP);//插入取件码
	insert_code_addr_list(pP,caP);//插入索引
	insert_record_list(dP,cP);//插入记录
}