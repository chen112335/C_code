#pragma once


#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_SZ 3
#define INC_SZ 2
#define MAX 100
#define MAX_NAME 20
#define MAX_SEX 10
#define MAX_TELE 12
#define MAX_ADDR 30

//类型声明

//联系人信息结构体
typedef struct PeoInfo
{
	char name[MAX_NAME];
	int age;
	char sex[MAX_SEX];
	char tele[MAX_TELE];
	char addr[MAX_ADDR];
}PeoInfo;

//静态版本
//通讯录结构体
//typedef struct Contact
//{
//	PeoInfo data[100];//通讯录中存放的联系人信息
//	int count;//记录当前通讯录中有多少人
//}Contact;


//动态版本
typedef struct Contact
{
	PeoInfo* data;//通讯录中存放的联系人信息
	int count;//记录当前通讯录中有多少人
	int capacity;//记录当前通讯录的容量
}Contact;



//初始化通讯录
int InitContact(Contact* pc);

//添加联系人
void AddContact(Contact* pc);

//显示联系人
void ShowContact(const Contact* pc);

//删除联系人
void DelContact(Contact* pc);

//查找联系人
void SearchContact(const Contact* pc);

//修改联系人
void ModifyContact(Contact* pc);

//排序联系人
void SortContact(Contact* pc);

//销毁通讯录
void DestroyContact(Contact* pc);

//保存通讯录
void SaveContact(const Contact* pc);

//加载通讯录
void LoadContact(Contact* pc);