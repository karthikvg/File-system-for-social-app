#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include "myheader.h"
void testing_area_write()
{
	char bit = '0';
	/*struct user_metadata usersMetaData[10] = { 0 };
	struct post postData[10] = { 0 };
	struct post_metadata postMeta[10] = { 0 };*/
	FILE* fp = fopen("filestore.bin", "rb+");
	fseek(fp, 0, SEEK_SET);
	for (int i = 0; i < 10; i++)
	{
		fwrite(&bit, 1, 1, fp);
	}
	fclose(fp);
}
int get_empty_user_index()
{
	char bit;
	FILE* fp = fopen("filestore.bin", "rb+");
	fseek(fp, 0, SEEK_SET);
	for (int i = 0; i < 10; i++)
	{
		fread(&bit, 1, 1, fp);
		if (bit == '0')
		{
			fclose(fp);
			return i;
		}
	}
	fclose(fp);
	return -1;
}
void show_user_of_id(int x)
{
	FILE* fp = fopen("filestore.bin", "rb+");
	printf(">>>%d\n", x);
	fseek(fp, NO_OF_USERS + x*(SIZE_OF_USER_METADATA), SEEK_SET);
	user_metadata temp;
	fread(&temp, SIZE_OF_USER_METADATA, 1, fp);

	printf("userName:%s\n", temp.username);
	printf("password:%s\n", temp.password);
	fseek(fp, INITIAL_FRAME + x*(SIZE_OF_POST_METADATA + NO_OF_USERS * SIZE_OF_POST), SEEK_SET);
	post_metadata temp2;
	fread(&temp2, SIZE_OF_POST_METADATA, 1, fp);
	for (int i = 0; i < 10; i++)
	{
		printf("%d ", temp2.bit_map[i]);
	}
	printf("\n");
	fclose(fp);
}
void testing_area_read()
{
	char bitmap[10];
	FILE* fp = fopen("filestore.bin", "rb+");
	fseek(fp, 0, SEEK_SET);
	for (int i = 0; i < 10; i++)
	{
		fread(&bitmap[i], 1, 1, fp);
		if (bitmap[i] == '1')
			show_user_of_id(i);
		else
			printf("%d<<\n", i);
	}
	fclose(fp);
	//printf("\n\n\n%d\n\n%d\n%d\n", sizeof(user_metadata), sizeof(post), sizeof(post_metadata));
}

void showing()
{
	char bit='*';
	FILE* fp = fopen("filestore.bin", "rb+");
	for (int i = 0; i < 10; i++)
	{
		fread(&bit, 1, 1, fp);
		if (bit == '1')
		{
			fclose(fp);
			show_user_of_id(i);
			fp = fopen("filestore.bin", "rb+");
			fseek(fp, i, SEEK_SET);
		}
		else
		{
			printf("not yet displaying\n");
		}
	}
	fclose(fp);
	return;
}