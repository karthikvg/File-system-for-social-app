#define filename "filestore.bin"
#define NO_OF_USERS 10
#define SIZE_OF_USER_METADATA 12448
#define SIZE_OF_POST_METADATA 116
#define SIZE_OF_POST 510012
#define INITIAL_FRAME 124490
#define FRAME_SIZE 5100236

struct user_metadata//12448
{
	char username[24];
	char password[20];
	char sender[100][24];
	char messages[100][100];
	int offset;//unusing
};

struct post//510012
{
	int size;
	int likes;
	int no_of_comments;
	char image[500000];
	char comments[100][100];
};
struct post_metadata//116
{
	char bit_map[10];
	char post_title[10][10];
	int post_package_size;//510012
};
void dashboard(int user_id);