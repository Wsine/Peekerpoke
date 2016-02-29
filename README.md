# Peekerpoke

## 导读

一个基于[Named Data Networking Forwarding Daemon](http://named-data.net/doc/NFD/current/)的项目

首次开发前请先阅读[开发文档规范](https://github.com/Wsine/Peekerpoke/wiki/%E5%BC%80%E5%8F%91%E6%96%87%E6%A1%A3%E8%A7%84%E8%8C%83)

可以查看[备忘录](https://github.com/Wsine/Peekerpoke/wiki/%E5%A4%87%E5%BF%98%E5%BD%95)

注意：使用UTF-8编码方式打开文件

## Workflow

1. 切换到各自的分支

	`git checkout wzy`

2. 编辑与编译

	编辑完之后，使用`make`编译代码，`make help`可以查看编译选项

3. 提交代码

	如果使用了`make`编译，直接`git add .` 然后 `git commit -m "some commit message"` 即可（.gitignore 中已忽略 /build 目录的所有内容）。如果使用其他方式编译，commit 时请注意将编译生成的文件排除在外。

4. 推送到 Github 上的分支
	
	`git push origin wzy`

5. 最后提交 Pull Request 到 master 即可

	所有成员都有写入的权限，如果没有冲突的话也可以直接自动合并。有冲突记得解决冲突。
