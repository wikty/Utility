---
title: "Hugo 起步"
author: "Xiao Wenbin"
date: 2017-08-17T10:58:08-04:00
draft: false
tags: ["hugo"]
---

## 简介

[Hugo][1] 是一款静态网站生成工具。如果你之前使用过 [Jekyll][2] ，那么一定不会对这类工具感到陌生。简单来说静态网站生成工具允许你以不同于传统博客系统（比如：[WordPress][3] ）的模式书写/发布内容，你可以用 [Markdown][4] 之类的文本标记语言撰写内容，然后使用静态网站生成工具将内容转换为网站形式。这里之所以说生成的网站是静态的，是由于网站内容在生成那一刻就是固定不变的了，不会像普通网站那样可以通过跟用户交互来动态生成内容。博客、文章以及技术文档等内容本来就是很少变化的，跟用户也没有交互性，因此这类站点很适合用静态网站工具来生成。

静态网站生成工具的巨大优势在于：*书写语言灵活性带来内容创作的自由*。首先，Markdown 等标记语言本来就是通过简单的格式标记符来撰写文档的利器，使得内容创作者将精力集中在内容本身而不是内容的显示格式上；其次，用 Markdown 等标记语言书写的文档不仅仅可以通过静态网站生成工具转换为网站，利用 [Pandoc][5] 这类文档转换工具可以将其转换为 PDF 、Word 等各种格式。使用 Markdown 等标记语言创作的内容数据源，可以使用各种工具将其转换为丰富多样的格式。更进一步来说，使用标记语言创作内容，使得内容创作过程和内容展示发布过程独立开来，给予了内容创作者极大的自由空间。

为什么选择 Hugo 而不是 Jekyll ：Hugo 简单易用、安装方便而且生成网站的速度快。我是一个懒惰而且懒惰的人，以上两点足够成为我拥抱 Hugo 的理由，或者说只是不小心入坑了，懒得再爬出来。

## 安装 Hugo

Hugo 支持各种操作系统而且安装方式也很多，最为简单的方法是[下载][6]一个二进制程序（要下载跟自己操作系统对应的版本），然后将程序移动到合适的位置，就可以使用了 Hugo 。虽然将该程序放在任意位置都可以，不过为了使用起来方便，通常应将其移动到操作体系环境变量 `PATH` 可以搜索到的位置或者将程序所在位置添加到 `PATH` 环境变量中。下载 Hugo 并将程序移动到 `PATH` 可以搜索的位置，就算安装好 Hugo 了，要注意的是 Hugo 是一个基于命令行的工具（并没有图形界面），测试 Hugo 是否安装成功，打开命令行运行以下命令：

```shell
$ hugo version
```

如果成功的返回了 Hugo 的版本信息，说明安装成功。如果提示找不到 hugo 命令，说明没有将程序移动到 `PATH` 可以搜索到的位置。

此外 Hugo 还可以通过操作系统的软件包管理工具来安装，比如 MacOS 可以使用 [Homebrew][7]，Windows 可以使用 [Chocolatey][8] 。如果有需要甚至可以自己编译源码进行安装，更多关于安装介绍参见[官网][]。

## 创建网站

运行以下命令来创建网站

```shell
$ hugo new site your-folder-name
```

`your-folder-name` 替换为自己的网站根目录名称（可以是相对路径或者绝对路径）。命令运行完后，可以看到 Hugo 创建了如下目录结构：

```
your-folder-name
 ├─config.toml
 ├─archetypes
 ├─content
 ├─data
 ├─layouts
 ├─static
 └─themes
```

现在暂时不需要担心这么多目录到底用来干嘛的，只要记住我们平时使用最多的就是目录 `content` ，该目录用来存放我们编辑的文档。

注：接下来大多数命令都需要在网站根目录（`your-folder-name`）中运行，如果没有特殊强调，假定你已经将命令行切换到了网站根目录。

## 添加主题

主题是用来对网站内容进行布局和排版的资源文件，简单来说，主题用来决定网站的外观。Hugo 官网为我们提供了许多开源的主题资源，本文我们以 [Ananke 主题][10]为例，更多主题资源参见[官网][11]。

主题的安装，可直接从主题网站下载，然后解压到网站源码目录中的主题目录下，也可以通过 Git 来安装。通过Git 安装主题的好处在于，以后更新主题时会比较方便。

运行以下命令来添加 Ananke 主题：

```shell
$ cd your-folder-name
$ git init
$ git submodule add https://github.com/budparr/gohugo-theme-ananke.git themes/ananke
$ echo 'theme = "ananke"' >> config.toml
```

如果命令行报错说找不到 git 命令，那是由于系统并未安装版本控制软件 Git，推荐前往 [git-scm.com][12] 下载安装 Git。或者直接前往 https://themes.gohugo.io/gohugo-theme-ananke/ 下载主题的资源文件后，将其解压至目录 `your-folder-name/themes` 中。

最后一行命令 `echo 'theme = "ananke"' >> config.toml ` 用来将网站主题配置信息添加到 `config.toml` 文件中。注：在 Windows 下，应该将这样命令替换为 `echo theme = "ananke" >> config.toml` 。或者也可以直接用文本编辑器打开，添加一行字符串 `theme = "ananke"` 。

## 添加文档

运行以下命令来创建内容文档：

```shell
$ hugo new post/first-article.md
```

此时可以在目录 `your-folder-name/content/post` 中发现新建文件 `first-article.md`，这是一个普通的 Markdown 文档，是我们写作的起点。这里之所以使用 Hugo 命令而不是自己创建 Markdown 文档，是因为通过 Hugo 命令创建的文档会包含一些初始内容，打开该文档可以看到如下内容：

```toml
+++
date = "2016-02-14T16:11:58+05:30"
draft = true
title = "First Article"

+++
```

这些内容主要用来声明文档创建时间、标题等信息，我们可以修改这些这些内容，也可以添加其它内容到这里。上面内容中有一项 `draft=true` 表示当前文档是草稿，新创建的文档默认是草稿文档，在生成静态网站时是不会被发布出去的。现在在文档尾部另起一行，然后随便写一些内容，以便后续生成静态网站时可以看到文档的内容。

## 配置网站

之前添加主题时，向文件 `config.toml` 添加过配置内容，其实该文件是网站的配置文件。现在该文件的内容应该是这样的：

```toml
languageCode = "en-us"
title = "My New Hugo Site"
baseURL = "http://example.org/"
theme = "ananke"
```

其中`languageCode` 是网站语言信息； `title` 是网站标题；`baseURL` 是网站 URL 地址，在网站发布时需要设置此项；现在可以试着自己设置一个网站标题 `title` ，其它配置项暂时不改动。

## 预览网站

运行以下命令来预览网站：

```shell
$ hugo server -D
```

命令运行完成后，可以通过浏览器来预览网站，在地址栏输入 http://localhost:1313/ 就可以看到生成的静态网站。

以上命令中的 `-D` 用来表明草稿文档也同时发布，如果没有该选项草稿文档是不会被发布出去的。当然也可以将草稿文档变为非草稿状态：将文档头部的 `draft=true` 改为 `draft=false` ，或者使用 Hugo 命令来完成：

```shell
$ hugo undraft content/post/first-article.md
```

## 发布网站

经过上面折腾后，网站已经可以在自己的电脑上浏览了，不过只有将网站发布到互联网后，别人才能浏览。可以将网站发布到 GitHub ，也可以发布到自己的服务器上。当然对于大多数人而言发布到 GitHub 是首选，因为它是免费的而且操作也很简单。利用 Github 搭建网站，参见[官网介绍][13]。简单来说，就是要在 Github 上注册账号，并创建一个代码仓库。

### 发布到GitHub

在发布网站之前，还需要做些配置工作，假设你 Github 的用户名是 `github-username` ，Github 上创建的仓库名是 `your-repo-name`，那么在最终发布网站之后，所有人都可以通过网址 ：http://github-username.github.io/your-repo-name 来访问你的网站了，不过在发布之前，我们需要把这个信息写到网站的配置文件中，将配置文件中的 `baseURL = "http://example.org/"` 改为 `baseURL = "http://github-username.github.io/your-repo-name"` ，然后就可以继续发布网站了。

首次发布到 GitHub，运行以下命令：

```shell
# 首先你需要在 GitHub 上创建一个仓库，具体方法参见 GitHub 官网
# 在网站根目录中运行以下命令
$ hugo
$ cd public
$ git init
$ git remote add origin git@github.com:<github-username>/your-repo-name.git
$ git checkout -b gh-pages
$ git add --all
$ git commit -m "message about this commit"
$ git push -f origin gh-pages
```

然后就可以通过网址： 来访问发布的网站了，注意要将 `github-username` 替换为自己的 Github 用户名，`your-repo-name` 替换为在 Github 上创建的仓库名。

以后网站内容要更新时，需要再次进行发布，后续发布运行以下命令：

```shell
# 网站根目录下运行以下命令
$ hugo
$ cd public
$ git add --all
$ git commit -m "message about this commit"
$ git push -f origin gh-pages
```

### 发布到自己的服务器

Hugo 程序使用 Go 语言编写，天生具有多线程的优势，可以在自己的服务器上使用 Hugo 来作为静态网站的 Web Server，当然也可以选择其它 Web Server 来托管自己的静态网站。要使 Hugo 作为 Web Server，需要运行以下命令：

```shell
$ hugo server --baseURL=http://yoursite.org/ \
              --port=80 \
              --appendPort=false \
              --bind=87.245.198.50
```

其中 `--bind` 指定监听服务器上哪块网卡，`0.0.0.0` 表示监听所有网卡；运行上述命令后Hugo 会实时监测文档内容改变并根据文档内容来响应请求，为了优化性能，可以通过传递参数 `--disableLiveReload=true` 来关闭该功能。

## 更新网站

后续对网站的更新流程大概是这样的：

- 编辑内容文档
- 本地预览效果
- 生成并发布网站



[1]: https://www.gohugo.io/	"Hugo Site"
[2]: http://jekyllrb.com/	"Jekyll Site"
[3]: https://wordpress.org/	"WordPress Site"
[4]: http://daringfireball.net/projects/markdown/syntax	"Markdown "
[5]: http://pandoc.org/	"Pandoc Site"
[6]: https://github.com/spf13/hugo/releases	"Hugo Download"
[7]: https://brew.sh/	"MacOS Homebrew"
[8]: https://chocolatey.org/	"Windows Chocolatey"
[9]: https://gohugo.io/getting-started/installing	"Hugo Installing"
[10]: https://themes.gohugo.io/gohugo-theme-ananke/	"Hugo Ananke Theme"
[11]: https://themes.gohugo.io/	"Hugo Theme"
[12]: https://git-scm.com/	"Git"
[13]: https://pages.github.com/	"Github Pages"