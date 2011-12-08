typedef struct{

	int current_idx;
	char img_list[255][255];
	int length;
}Com_box;
typedef struct{

	int current_idx;
	char mp3_list[255][255];
	int length;
}Mp3_box;

Com_box  *img_search(char *dirname , Com_box * list);
Mp3_box  *mp3_search(char *dirname , Mp3_box * list);
