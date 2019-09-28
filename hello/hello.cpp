// hello.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <git2.h>

const char* path = R"(C:\Users\ymao\source\repos\libgit2_study)";
const char* remote_url = "https://github.com/waiting4love/libgit2_study.git";
const char* remote_name = "ccs"; // c central server

void check_error(int error)
{
	if (error < 0) {
		const git_error *e = giterr_last();
		printf("Error %d/%d: %s\n", error, e->klass, e->message);
		exit(error);
	}
}

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

	check_error(error);

	// 得到当前的remote
	git_remote *remote = NULL;
	error = git_remote_lookup(&remote, repo, remote_name);
	if (error == GIT_ENOTFOUND) {
		// 如果没有得到，建立
		error = git_remote_create(&remote, repo, remote_name, remote_url);
	}

	check_error(error);

	// 与对比远程，先取远程数据
	error = git_remote_fetch(remote, NULL, NULL, NULL);
	check_error(error);

	// 单独显示指定的提交信息
	git_commit *head_commit = NULL;
	error = git_revparse_single((git_object**)&head_commit, repo, "HEAD");	
	check_error(error);
	printf("%s", git_commit_message(head_commit));
	git_commit_free(head_commit);

	git_commit *remote_header = NULL;
	error = git_revparse_single((git_object**)&remote_header, repo, "ccs/master");
	check_error(error);
	printf("%s", git_commit_message(remote_header));
	git_commit_free(remote_header);

	// git log HEAD..ccs/master --oneline 显示版本变化
	git_revwalk *walker;
	error = git_revwalk_new(&walker, repo);
	check_error(error);
	error = git_revwalk_push_range(walker, "HEAD..ccs/master");
	check_error(error);
	git_oid oid;
	while (!git_revwalk_next(&oid, walker)) {
		git_commit *c = NULL;
		error = git_commit_lookup(&c, repo, &oid);
		check_error(error);
		printf("%s", git_commit_message(c));
		git_commit_free(c);
	}
	git_revwalk_free(walker);

	// git diff --stat master ccs/master  显示修改
	

	git_remote_free(remote);
	git_repository_free(repo);
	git_libgit2_shutdown();
}
