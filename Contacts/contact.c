#define _CRT_SECURE_NO_WARNINGS
#include "Contact.h"

//检测通讯录是否需要增容
void CheckCapacity(Contact* pc)
{
	assert(pc);
	if (pc->count == pc->capacity)
	{
		PeoInfo* ptr = (PeoInfo*)realloc(pc->data, (pc->capacity + INC_SZ) * sizeof(PeoInfo));
		if (ptr == NULL)
		{
			printf("CheckCapacity::%s\n", strerror(errno));
			return;
		}
		pc->data = ptr;
		pc->capacity += INC_SZ;
		printf("增容成功！\n");
	}
}


//静态版本
//void InitContact(Contact* pc)
//{
//	assert(pc);
//	pc->count = 0;
//	memset(pc->data, 0, sizeof(pc->data));
//}

void LoadContact(Contact* pc)
{
	assert(pc);
	FILE* pfRead = fopen("contact.txt", "rb");
	if (pfRead == NULL)
	{
		printf("LoadContact::%s\n", strerror(errno));
		return;
	}
	PeoInfo tmp = { 0 };

	while(fread(&tmp, sizeof(PeoInfo), 1, pfRead) == 1)
	{
		CheckCapacity(pc);
		pc->data[pc->count] = tmp;
		pc->count++;
	}

	fclose(pfRead);
	pfRead = NULL;
}

//动态版本
int InitContact(Contact* pc)
{
	assert(pc);
	pc->count = 0;
	pc->data = (PeoInfo*)calloc(DEFAULT_SZ,sizeof(PeoInfo));
	if (pc->data == NULL)
	{
		printf("InitContact::%s\n", strerror(errno));
		return 1;
	}
	pc->capacity = DEFAULT_SZ;
	//加载文件的信息到通讯录中
	LoadContact(pc);
	return 0;
}

//静态版本
//void AddContact(Contact* pc)
//{
//	assert(pc);
//	if(pc->count == MAX)
//	{
//		printf("通讯录已满，无法添加新联系人!\n");
//		return;
//	}
//	//输入联系人信息
//	printf("请输入姓名:>");
//	scanf("%s", pc->data[pc->count].name);
//	printf("请输入年龄:>");
//	scanf("%d", &pc->data[pc->count].age);
//	printf("请输入性别:>");
//	scanf("%s", pc->data[pc->count].sex);
//	printf("请输入电话:>");
//	scanf("%s", pc->data[pc->count].tele);
//	printf("请输入地址:>");
//	scanf("%s", pc->data[pc->count].addr);
//
//	pc->count++;
//	printf("添加成功!\n");
//}



//动态版本
void AddContact(Contact* pc)
{
	assert(pc);
	//检测增容
	CheckCapacity(pc);
	//输入联系人信息
	printf("请输入姓名:>");
	scanf("%s", pc->data[pc->count].name);
	printf("请输入年龄:>");
	scanf("%d", &pc->data[pc->count].age);
	printf("请输入性别:>");
	scanf("%s", pc->data[pc->count].sex);
	printf("请输入电话:>");
	scanf("%s", pc->data[pc->count].tele);
	printf("请输入地址:>");
	scanf("%s", pc->data[pc->count].addr);

	pc->count++;
	printf("添加成功!\n");
}


void ShowContact(Contact* pc)
{
	assert(pc);
	int i = 0;
	for(i = 0; i < pc->count; i++)
	{
		printf("姓名: %s\t年龄: %d\t性别: %s\t电话: %s\t地址: %s\n",
			pc->data[i].name,
			pc->data[i].age,
			pc->data[i].sex,
			pc->data[i].tele,
			pc->data[i].addr);
	}
}


static int FindByName(Contact* pc, char name[])
{
	assert(pc);
	int i = 0;
	for(i = 0; i < pc->count; i++)
	{
		if(0 == strcmp(pc->data[i].name, name))
		{
			return i;
		}
	}
	return -1;
}

void DelContact(Contact* pc)
{
	assert(pc);
	char name[MAX_NAME];
	if(pc->count == 0)
	{
		printf("通讯录为空，无法删除联系人!\n");
		return;
	}
	printf("请输入要删除的联系人姓名:>");
	scanf("%s", name);
	//删除
	//1.查找要删除的联系人
	int pos = FindByName(pc, name);
	if(pos == -1)
	{
		printf("要删除的联系人不存在!\n");
		return;
	}
	//2.删除联系人
	for(int i = pos; i < pc->count - 1; i++)
	{
		pc->data[i] = pc->data[i + 1];
	}
	pc->count--;
	printf("删除成功!\n");
}

//查找联系人
void SearchContact(const Contact* pc)
{
	assert(pc);
	char name[MAX_NAME];
	if(0 == pc->count)
	{
		printf("通讯录为空，无法查找联系人!\n");
		return;
	}
	printf("请输入要查找的联系人姓名:>");
	scanf("%s", name);
	//查找
	int pos = FindByName((Contact*)pc, name);
	if(pos == -1)
	{
		printf("要查找的联系人不存在!\n");
		return;
	}
	printf("姓名: %s\t年龄: %d\t性别: %s\t电话: %s\t地址: %s\n",
		pc->data[pos].name,
		pc->data[pos].age,
		pc->data[pos].sex,
		pc->data[pos].tele,
		pc->data[pos].addr);
}

void ModifyContact(Contact* pc)
{
	assert(pc);
	char name[MAX_NAME];
	if(0 == pc->count)
	{
		printf("通讯录为空，无法修改联系人!\n");
		return;
	}
	printf("请输入要修改的联系人姓名:>");
	scanf("%s", name);
	//查找
	int pos = FindByName(pc, name);
	if(pos == -1)
	{
		printf("要修改的联系人不存在!\n");
		return;
	}
	//修改
	printf("请输入新的姓名:>");
	scanf("%s", pc->data[pos].name);
	printf("请输入新的年龄:>");
	scanf("%d", &pc->data[pos].age);
	printf("请输入新的性别:>");
	scanf("%s", pc->data[pos].sex);
	printf("请输入新的电话:>");
	scanf("%s", pc->data[pos].tele);
	printf("请输入新的地址:>");
	scanf("%s", pc->data[pos].addr);
	printf("修改成功!\n");
}


int CompareByName(const void* e1, const void* e2)
{
	return strcmp(((PeoInfo*)e1)->name, ((PeoInfo*)e2)->name);
}

//按名字排序联系人
void SortContact(Contact* pc)
{
	assert(pc);
	if(0 == pc->count)
	{
		printf("通讯录为空，无法排序联系人!\n");
		return;
	}
	qsort(pc->data, pc->count, sizeof(PeoInfo), CompareByName);
	printf("排序成功!\n");
}

//销毁通讯录
void DestroyContact(Contact* pc)
{
	assert(pc);
	free(pc->data);
	pc->data = NULL;
	pc->count = 0;
	pc->capacity = 0;
}

//保存通讯录
void SaveContact(const Contact* pc)
{
	assert(pc);
	FILE* pfWrite = fopen("contact.txt", "wb");
	if(pfWrite == NULL)
	{
		printf("SaveContact::%s\n", strerror(errno));
		return;
	}
	int i = 0;
	for (i = 0; i < pc->count; i++)
	{
		fwrite(&pc->data[i], sizeof(PeoInfo), 1, pfWrite);
	}
	fclose(pfWrite);
	pfWrite = NULL;
}