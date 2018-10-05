#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "myheader.h"
void update_user_name(int user_id)
{
	/*updates the username*/
	
	char* str = (char*)malloc(sizeof(char)*24);

	FILE* fp = fopen(filename, "rb+");
	fseek(fp, 10 + user_id*SIZE_OF_USER_METADATA, SEEK_SET);

	user_metadata* user_details = (user_metadata*)malloc(sizeof(user_metadata));
	fread(user_details, sizeof(user_metadata), 1, fp);
	printf("ENTER THE NEW USER NAME\n");
	scanf("%s", str);
	strcpy(user_details->username, str);
	fseek(fp, 10 + user_id*SIZE_OF_USER_METADATA, SEEK_SET);

	fwrite(user_details, sizeof(user_metadata), 1, fp);
	fclose(fp);

	free(str);
}
void update_user_password(int user_id)
{
	/*updates the user password*/
	char* str = (char*)malloc(sizeof(char) * 20);

	FILE* fp = fopen(filename, "rb+");
	fseek(fp, 10 + user_id*SIZE_OF_USER_METADATA, SEEK_SET);

	user_metadata* user_details = (user_metadata*)malloc(sizeof(user_metadata));
	fread(user_details, sizeof(user_metadata), 1, fp);
	printf("ENTER THE NEW password\n");
	scanf("%s", str);
	strcpy(user_details->password, str);
	fseek(fp, 10 + user_id*SIZE_OF_USER_METADATA, SEEK_SET);

	fwrite(user_details, sizeof(user_metadata), 1, fp);
	fclose(fp);

	free(str);
}
void remove_user(int user_id)
{
	/*removes the user from bit map*/
	FILE* fp = fopen(filename, "rb+");
	fseek(fp, user_id, SEEK_SET);
	char ch = '0';
	fwrite(&ch, 1, 1, fp);
	fclose(fp);
}
int get_empty_slot_for_writing(int id,char* path)
{
	/*return the empty post slot address*/
	int location = INITIAL_FRAME + id*FRAME_SIZE;
	FILE* fp = fopen(filename, "rb+");

	fseek(fp, location, SEEK_SET);
	post_metadata* postMeta = (post_metadata*)malloc(sizeof(post_metadata));
	fread(postMeta, sizeof(post_metadata), 1, fp);
	fclose(fp);
	int index = 0;
	for (index=0; index < 10; index++)
	{
		if (postMeta->bit_map[index] == '0')
		{
			strcpy(postMeta->post_title[index], path);
			postMeta->bit_map[index] = '1';
			break;
		}
	}

	fp = fopen(filename, "rb+");

	fseek(fp, location, SEEK_SET);
	fwrite(postMeta, sizeof(post_metadata), 1, fp);
	location += index*SIZE_OF_POST + SIZE_OF_POST_METADATA;
	fclose(fp);
	return location;
}
int get_title_location(int user_id,char* path)
{
	int location = INITIAL_FRAME + user_id*FRAME_SIZE;
	FILE* fp = fopen(filename, "rb+");

	fseek(fp, location, SEEK_SET);
	post_metadata* postMeta = (post_metadata*)malloc(sizeof(post_metadata));
	fread(postMeta, sizeof(post_metadata), 1, fp);
	int index = 0;
	for (; index < 10; index++)
	{
		if (strcmp(postMeta->post_title[index],path) == 0)
		{

			break;
		}
	}
	location += index*SIZE_OF_POST+SIZE_OF_POST_METADATA;
	fclose(fp);
	return location;
}
void download_post(int user_id, char* path)
{
	//testers
	printf("ENTER THE FILE LOCATION TO DOWNLOAD TO EXAMPLE: 171.JPG:\n");
	char str[100];
	scanf("%s", str);
	
	post* new_post = (post*)malloc(sizeof(post));
	FILE* fp_s = fopen(filename, "rb+");
	int location = get_title_location(user_id,path);
	fseek(fp_s, location, SEEK_SET);
	char* c = (char *)malloc(sizeof(char));
	fread(new_post, sizeof(post), 1, fp_s);
	fclose(fp_s);
	
	//printf("%d-location\n", location);
	//printf("%d %d %d\n", new_post->likes, new_post->no_of_comments, new_post->size);
	
	FILE* fp_d = fopen(str, "wb");
	for (int i = 0; i < new_post->size; i++)
	{

		fwrite(&(new_post->image[i]), 1, 1, fp_d);
	}
	
	fclose(fp_d);
	printf("YOUR FILE HAS BEEN DOWNLOADED TO %s\n", str);
	//system("download1.jpg");
}
void upload_post(int user_id)
{
	printf("\nUPLOAD-POST:\n");
	printf("===========================\n");
	printf("enter the path of the file from your disk\n");
	
	//--->
	char* path = (char*)malloc(sizeof(char) * 100);
	scanf("%s", path);
	printf("%s\n", path);
	int location = get_empty_slot_for_writing(user_id,path);
	
	FILE* fp_s = fopen(path, "rb+");
	char *c = (char *)malloc(sizeof(char));
	struct post* new_post = (post*)malloc(sizeof(post));
	new_post->no_of_comments = new_post->likes = new_post->size=0;
	while (fread(c, sizeof(char), 1, fp_s) >0)
	{
		new_post->image[new_post->size] = *c;
		new_post->size+=1;
	}
	fclose(fp_s);

	FILE* fp_d = fopen(filename, "rb+");
	fseek(fp_d, location, SEEK_SET);
	fwrite(new_post, sizeof(post), 1, fp_d);
	fclose(fp_d);

}
void view_post(int user_id,char* str)
{
	post* new_post = (post*)malloc(sizeof(post));
	FILE* fp_s = fopen(filename, "rb+");
	int location = get_title_location(user_id, str);
	fseek(fp_s, location, SEEK_SET);
	char* c = (char *)malloc(sizeof(char));
	fread(new_post, sizeof(post), 1, fp_s);
	fclose(fp_s);
	//printf("%d-location\n", location);
	FILE* fp_d = fopen(str, "wb");
	for (int i = 0; i < new_post->size; i++)
	{
		fwrite(&(new_post->image[i]), 1, 1, fp_d);
	}
	fclose(fp_d);
	printf("YOUR FILE HAS BEEN DOWNLOADED TO %s\n", str);
	system(str);
	printf("%d %d %d\n", new_post->likes, new_post->no_of_comments, new_post->size);
	while (true)
	{
		printf("ENTER YOUR CHOICE\n");
		printf("----------------------------\n");
		printf("1.like\t2.comment\t3.view_comments\t0.back\n");
		int ch;
		scanf("%d", &ch);
		if (ch == 1)
		{
			new_post->likes++;
		}
		else if (ch == 2)
		{
			char temp[100];
			scanf("%s", temp);
			strcpy(new_post->comments[new_post->no_of_comments++], temp);
		}
		else if (ch == 3)
		{
			for (int i = 0; i < new_post->no_of_comments; i++)
			{
				printf("--->%s\n", new_post->comments[i]);
			}
		}
		else
		{
			break;
		}
	}
	fp_s = fopen(filename, "rb+");
	fseek(fp_s, location, SEEK_SET);
	fwrite(new_post, sizeof(post), 1, fp_s);
	fclose(fp_s);
	free(new_post);
}
void dashboard(int user_id)
{
	/*on logging the user gets the dashboard*/
	while (1)
	{
		printf("\n\nDashboard:\n");
		printf("============================\n");
		printf("1.UPDATE_USER_ID\n2.UPDATE_PASSWORD\n3.REMOVE_USER\n4.LOGOUT\n5.UPLOAD_POST\n6.DOWNLOAD\n7.view_post\n");
		int ch;
		scanf("%d", &ch);
		switch (ch)
		{
		case 1:
			update_user_name(user_id);
			break;
		case 2:
			update_user_password(user_id);
			break;
		case 3:
			remove_user(user_id);
			return;
		case 4:
			return;
		case 5:
			upload_post(user_id);
			break;
		case 6:
			char str[10];
			printf("enter the title name:\n");
			scanf("%s", str);
			download_post(user_id, str);
			break;
		case 7:
			printf("enter the title name:\n");
			scanf("%s", str);
			view_post(user_id, str);
		}
	}

}