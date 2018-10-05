#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "myheader.h"
#include "testing.h"

int authentication()
{
	//it will check if the user is present in the blob file and gives dashboard 
	printf("_________________________\n");
	struct user_metadata* user_details=(user_metadata*)malloc(sizeof(user_metadata));
	printf("enter the user name\n");
	scanf("%s", user_details->username);
	printf("enter the password\n");
	scanf("%s", user_details->password);

	FILE* fp = fopen("filestore.bin", "rb+");
	char* ch = (char*)malloc(sizeof(char) * 10);
	for (int i = 0; i < 10; i++)
	{
		fread(&ch[i], 1, 1, fp);
		printf("for index i: %d--->%c\n",i, ch[i]);
	}
	
	for (int i = 0; i < 10; i++)
	{
		if (ch[i] == '1')
		{
			struct user_metadata* user = (user_metadata*)malloc(sizeof(user_metadata));
			fread(user, sizeof(user_metadata), 1, fp);
			printf("%s %s\n", user->username, user->password);
			if (strcmp(user->username, user_details->username) == 0 && strcmp(user->password, user_details->password) == 0)
			{
				free(user);
				fclose(fp);
				return i;
			}
		}
	}
	fclose(fp);
	return -1;
}
void show_users()
{
	FILE* fp = fopen("filestore.bin", "rb+");
	char* ch = (char*)malloc(sizeof(char) * 10);
	printf("\nUSERS_POOL:\n");
	printf("==============================\n");
	for (int i = 0; i < 10; i++)
	{
		fread(&ch[i], 1, 1, fp);
		printf("for index i: %d--->%c\n", i, ch[i]);
	}
	printf("\nUSER_DETAILS\n");
	printf("==============================\n");
	for (int i = 0; i < 10; i++)
	{
		if (ch[i] == '1')
		{
			struct user_metadata* user = (user_metadata*)malloc(sizeof(user_metadata));
			fread(user, sizeof(user_metadata), 1, fp);
			printf("%s \t\t%s\n", user->username, user->password);
			free(user);
		}
	}
	fclose(fp);
}
void login()
{
	printf("LOGIN:\n");
	printf("========================================\n");
	int value = authentication();
	if (value!=-1)
	{
		printf("SUCCESSFULLY LOGGED IN\n");
		dashboard(value);
	}
	else
	{
		printf("YOU HAVE ENTERED A WRONG PASSWORD\N");
	}
}
void signup()
{
	struct user_metadata *user_details = (struct user_metadata*)malloc(sizeof(struct user_metadata));
	

	printf("ENTER THE USER-NAME\n");					
	scanf("%s", user_details->username);
	printf("ENTER THE PASSWORD\n");
	scanf("%s", user_details->password);

	int index=0;
	char ch = '*';
	FILE* fp = fopen(filename, "rb+");
	fseek(fp, 0, SEEK_SET);
	for (index = 0; index < 10; index++)
	{
		fread(&ch, sizeof(char), 1, fp);
		if (ch == '0')
			break;
	}
	fclose(fp);
	printf("free index = %d\n", index);

	fp = fopen(filename, "rb+");							
	fseek(fp, index, SEEK_SET);
	ch = '1';
	fwrite(&ch, sizeof(char), 1, fp);
	fclose(fp);

	fp = fopen(filename, "rb+");							
	fseek(fp, 10 + (index*SIZE_OF_USER_METADATA), SEEK_SET);
	fwrite(user_details, sizeof(struct user_metadata), 1, fp);
	fclose(fp);

	struct user_metadata *user = (struct user_metadata*)malloc(sizeof(struct user_metadata));
	fp = fopen(filename, "rb+");							
	fseek(fp, 10 + (index*SIZE_OF_USER_METADATA), SEEK_SET);
	fread(user, sizeof(struct user_metadata), 1, fp);
	fclose(fp);

	struct post_metadata *post = (struct post_metadata*)malloc(sizeof(struct post_metadata));
	for (int j = 0; j < 10; j++)
		post->bit_map[j] = '0';
	
	fp = fopen(filename, "rb+");									
	fseek(fp, INITIAL_FRAME + index*(FRAME_SIZE), SEEK_SET);
	fwrite(post, SIZE_OF_POST_METADATA, 1, fp);
	fclose(fp);

	fp = fopen(filename, "rb+");									
	fseek(fp, INITIAL_FRAME + index*(FRAME_SIZE), SEEK_SET);
	fread(post, SIZE_OF_POST_METADATA, 1, fp);
	fclose(fp);
	
	printf("\nBITMAP:\n");
	printf("==============================\n");
	for (int j = 0; j < 10; j++)
		printf("%c ", post->bit_map[j]);
	printf("\n");
	free(post);
	return;
}
void driver()
{
	while (1)
	{
		printf("\n\n\nOPERATIONS:\n");
		printf("==============================\n");
		printf("1.login\n2.signup\n0.exit\n");
		int ch;
		scanf("%d", &ch);
		switch (ch)
		{
		case 1: login();
			break;
		case 2: signup();
			break;
		case 3:show_users();
			break;
		case 0:
			return ;
		}
	}
}

int main()
{
//	testing_area_write();
	//testing_area_read();

	driver();
	showing();
	//display_meta();
	system("pause");
	return 0;
}
//void register_user(user_metadata user_details)
//{
//	//register user
//	int empty_user_index = get_empty_user_index();
//	printf("empty index: %d\n", empty_user_index);
//	FILE* fp = fopen("filestore.bin", "rb+");
//	user_details.offset = empty_user_index;
//	
//	fseek(fp, NO_OF_USERS + empty_user_index*(SIZE_OF_USER_METADATA), SEEK_SET);
//	fwrite(&user_details, sizeof(user_metadata), 1, fp);
//	
//	char bit = '1';
//	fseek(fp, empty_user_index, SEEK_SET);
//	fwrite(&bit, 1, 1, fp);
//	fclose(fp);
//
//	fp = fopen("filestore.bin", "rb+");
//	fseek(fp, INITIAL_FRAME+empty_user_index*(SIZE_OF_POST_METADATA + NO_OF_USERS * SIZE_OF_POST), SEEK_SET);
//	
//	struct post_metadata* p_meta=(post_metadata*)malloc(sizeof(post_metadata));
//	p_meta->post_package_size = SIZE_OF_POST;
//	for (int i = 0; i < 10; i++)
//	{
//		p_meta->bit_map[i] = '0';
//	}
//	fwrite(p_meta, sizeof(SIZE_OF_POST_METADATA),1,fp);
//	fclose(fp);
//	return;
//}
//void signup()
//{
//	printf("_________________________\n");
//	user_metadata user_details;
//	printf("enter the user name\n");
//	scanf("%s", user_details.username);
//	printf("enter the password\n");
//	scanf("%s", user_details.password);
//	register_user(user_details);
//}