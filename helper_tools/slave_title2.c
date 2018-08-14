#include <stdio.h>

int main(int argc, char* argv[])
{
	FILE* fp;
	char Title[100], *title;
	int i = -1;

	struct SlaveInfo
	{
		unsigned long Security;
		char ID[8];
		unsigned short Version;
		unsigned short Flags;
		unsigned long BaseMemSize;
		unsigned long ExecInstall;
		unsigned short GameLoader;
		unsigned short CurrentDir;
		unsigned short DontCache;
		char keydebug;
		char keyexit;
		unsigned long ExpMem;
		unsigned short name;
		unsigned short copy;
		unsigned short info;
	};

	struct SlaveInfo sl;

	if (argc == 1)
	{
		printf("Usage: %s filename.slave\n", argv[0]);
		exit(0);
	}

	fp = fopen(argv[1], "rb");
	if (fp == NULL)
	{
		printf("Could not open %s\n", argv[1]);
		exit(0);
	}

	//seek to +0x20
	fseek(fp, 32, SEEK_SET);

	fread(&sl, 1, sizeof(sl), fp);


	sl.Version = (sl.Version >> 8) | (sl.Version << 8);
	sl.name = (sl.name >> 8) | (sl.name << 8);

	printf("[%s] [%d]\n", sl.ID, sl.Version);

	//sl.name holds the offset for the slave name
	fseek(fp, sl.name + 32, SEEK_SET);
	//title = calloc (1, 100);
	//fread (title, 1, 100, fp);

	for (i = 0; i <= 99; i++)
	{
		Title[i] = fgetc(fp);
		if (Title[i] == '\n')
		{
			Title[i] = '\0';
			break;
		}
	}

	printf("[%s]\n", Title);

	//fclose(fp);	
}
