#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "contact.h"


enum Option
{
	EXIT,
	ADD,
	DEL,
	SEARCH,
	MODIFY,
	SHOW,
	SORT
};


void menu()
{
	printf("*********************************************\n");
	printf("********  1.add       2.del     *************\n");
	printf("********  3.search    4.modify  *************\n");
	printf("********  5.show      6.sort    *************\n");
	printf("********  0.exit                *************\n");
	printf("*********************************************\n");
}



int main()
{
	int input = 0;
	Contact con;//创建通讯录结构体变量
	//初始化通讯录
	InitContact(&con);
	do
	{
		menu();
		printf("请输入你的选择:>");
		scanf("%d", &input);
		switch (input)
		{
		case ADD:
			AddContact(&con);
			break;
		case DEL:
			DelContact(&con);
			break;
		case SEARCH:
			SearchContact(&con);
			break;
		case MODIFY:
			ModifyContact(&con);
			break;
		case SHOW:
			ShowContact(&con);
			break;
		case SORT:
			SortContact(&con);
			break;
		case EXIT:
			SaveContact(&con);
			DestroyContact(&con);
			printf("退出通讯录系统!\n");
			break;
		default:
			printf("选择错误，请重新选择!\n");
			break;
		}
	} while(input);
	return 0;
}