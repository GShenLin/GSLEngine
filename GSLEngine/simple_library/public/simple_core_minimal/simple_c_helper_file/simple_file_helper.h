#pragma once
//Copyright (C) RenZhai.2019.All Rights Reserved.
//作者 人宅
//该c库的详细源码讲解在AboutCG 《手把手入门硬核c语言》视频工程里面：
//https://www.aboutcg.org/courseDetails/902/introduce
//希望学习其他方面技术 比如做UE4游戏, 可以访问下列网址：
//https://zhuanlan.zhihu.com/p/60117613
//
//bibi可以看到各个人宅系列教程介绍：
//https://space.bilibili.com/29544409
//
//个人博客网站
//http://renzhai.net
//
//关于本套案例的详细操作 :
//文字版本(详细)：
//https://zhuanlan.zhihu.com/p/144558934
//视频版本：
//https://www.bilibili.com/video/BV1x5411s7s3
#include "../simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
typedef struct
{
	int index;
	char paths[8196][1024];//MAX_PATH
}def_c_paths;

void init_def_c_paths(def_c_paths *c_paths);

int copy_file(char *Src, char *Dest);

void find_files(char const *in_path, def_c_paths *str,bool b_recursion);

bool create_file(char const *filename);
 
bool create_file_directory(char const *in_path);

//使用该接口 一定要初始化buf
bool get_file_buf(const char *path,char *buf);

bool add_file_buf(const char *path, char *buf);

bool add_new_file_buf(const char *path, char *buf);

unsigned int get_file_size_by_filename(const char *filename);

unsigned int get_file_size(FILE *file_handle);
_CRT_END_C_HEADER