// hello.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <git2.h>

const char* path = R"(C:\Users\ymao\source\repos\libgit2_study)";
const char* remote_url = "git@github.com:waiting4love/libgit2_study.git";
const char* remote_name = "ccs"; // c central server
int main()
{
	git_libgit2_init();
    
	git_repository *repo = NULL;
	// 尝试打开
	int error = git_repository_open(&repo, path);
	if (error == GIT_ENOTFOUND) {
		// 如果打开失败，初始化
		error = git_repository_init(&repo, path, false);
	}

	if (error < 0) {
		const git_error *e = giterr_last();
		printf("Error %d/%d: %s\n", error, e->klass, e->message);
		exit(error);
	}

	// 得到当前的remote
	git_remote *remote = NULL;
	error = git_remote_lookup(&remote, repo, remote_name);
	if (error == GIT_ENOTFOUND) {
		// 如果没有得到，建立
		error = git_remote_create(&remote, repo, remote_name, remote_url);
	}

	if (error < 0) {
		const git_error *e = giterr_last();
		printf("Error %d/%d: %s\n", error, e->klass, e->message);
		exit(error);
	}

	// 怎么得到对比
	error = git_remote_fetch(remote, NULL, NULL, NULL);
	if (error < 0) {
		const git_error *e = giterr_last();
		printf("Error %d/%d: %s\n", error, e->klass, e->message);
		exit(error);
	}

	// diff
	
	git_remote_free(remote);
	git_repository_free(repo);
	git_libgit2_shutdown();
}
