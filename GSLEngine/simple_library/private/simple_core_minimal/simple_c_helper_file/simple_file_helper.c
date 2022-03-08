// Copyright (C) RenZhai.2022.All Rights Reserved.
#include "../../../public/simple_core_minimal/simple_c_helper_file/simple_file_helper.h"
#include "../../../public/simple_core_minimal/simple_c_core/simple_c_array/simple_c_array_string.h"

void init_def_c_paths(def_c_paths *c_paths)
{
	c_paths->index = 0;
	memset(c_paths->paths,0,sizeof(c_paths->paths) - 1);
}

int copy_file(char *Src, char *Dest)
{
	//当前的缓存 缓存1MB大小，如果超过就会出问题 这个会在std C课程里面继续扩展
	char Buf[1024 * 1024] = { 0 };
	int FileSize = 0;
	FILE *FpSrc = NULL;
	FILE *FpDest = NULL;

	if ((FpSrc = fopen(Src, "rb")) != NULL)
	{
		if ((FpDest = fopen(Dest, "wb")) != NULL)
		{
			while ((FileSize = fread(Buf, 1, 512, FpSrc)) > 0)
			{
				fwrite(Buf, FileSize, sizeof(char), FpDest);
				memset(Buf, 0, sizeof(Buf));
			}

			fclose(FpSrc);
			fclose(FpDest);

			return 0;
		}
	}

	return -1;
}
void find_files(char const *in_path, def_c_paths *str, bool b_recursion)
{
	struct _finddata_t finddata;

	long hfile = 0;
	char tmp_path[8196] = { 0 };
	strcpy(tmp_path, in_path);
	strcat(tmp_path, "\\*");
	if ((hfile = _findfirst(tmp_path, &finddata)) != -1)
	{
		do
		{
			if (finddata.attrib & _A_SUBDIR)
			{
				if (b_recursion)
				{
					if (strcmp(finddata.name, ".") == 0 ||
						strcmp(finddata.name, "..") == 0)
					{
						continue;
					}

					char new_path[8196] = { 0 };
					strcpy(new_path, in_path);
					strcat(new_path, "\\");
					strcat(new_path, finddata.name);

					find_files(new_path, str, b_recursion);
				}
			}
			else
			{
				strcpy(str->paths[str->index], in_path);
				strcat(str->paths[str->index], "\\");
				strcat(str->paths[str->index++], finddata.name);
			}

		} while (_findnext(hfile, &finddata) == 0);
		_findclose(hfile);
	}
}

bool create_file(char const *filename)
{
	FILE *f = NULL;
	if ((f = fopen(filename,"w+")) != NULL)
	{
		fclose(f);

		return true;
	}

	return false;
}

bool create_file_directory(char const *in_path)
{
	simple_c_string c_file;
	if (strstr(in_path, "\\"))
	{
		dismantling_string(in_path, "\\", &c_file);
	}
	else if (strstr(in_path, "/"))
	{
		dismantling_string(in_path, "/", &c_file);
	}

	char path[260] = { 0 };
	for (int i = 0;i < c_file.size;i++)
	{
		char *value = get_string(i,&c_file);
		strcat(value, "\\");
		strcat(path, value);
		if (_access(path,0) == -1)
		{
			_mkdir(path);
		}
	}

	destroy_string(&c_file);

	return _access(path, 0) == 0;
}

bool get_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "r")) != NULL)
	{
		char buf_tmp[2048] = { 0 };
		int file_size = 0;
		while ((file_size = fread(buf_tmp, 1,1024, f)) > 0)
		{
			strcat(buf, buf_tmp);
			memset(buf_tmp, 0, sizeof(buf_tmp));
		}

		fclose(f);

		return buf[0] != '\0';
	}

	return false;
}

bool add_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "a+")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

bool add_new_file_buf(const char *path, char *buf)
{
	FILE *f = NULL;
	if ((f = fopen(path, "w+")) != NULL)
	{
		fprintf(f, "%s", buf);
		fclose(f);

		return true;
	}

	return false;
}

unsigned int get_file_size_by_filename(const char *filename)
{
	unsigned int file_size = 0;

	FILE *f = NULL;
	if ((f = fopen(filename,"r")) != NULL)
	{
		file_size = get_file_size(f);

		fclose(f);
	}

	return file_size;
}

//asdoiajoi ajs aoisjd oaisjd oiasjdoi asodao ijaosijd oaisdja index
unsigned int get_file_size(FILE *file_handle)
{
	unsigned int file_size = 0;

	unsigned int current_read_postion = ftell(file_handle);
	fseek(file_handle, 0, SEEK_END);
	file_size = ftell(file_handle);
	fseek(file_handle, current_read_postion, SEEK_SET);

	return file_size;
}
