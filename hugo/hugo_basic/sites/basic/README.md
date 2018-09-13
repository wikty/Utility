## 评论分享

- 根据配置自动切换[多说](http://duoshuo.com/)和[Disqus](https://disqus.com/)评论系统
- 根据配置自动切换国内和国外各种社交媒体分享按钮

## 数学公式

根据配置加载[MathJax](https://www.mathjax.org/)对页面的数学公式进行渲染，markdown文档中插入的数学公式用`$$`包围，如果是行级公式直接写在相应文本段落中即可，如果是块级公式需要独立成段落。并且使用公式的文档需要在front-matter中添加配置项`markup`为`mmark`

## 网站部署

- 部署到[GitHub](https://github.com/)

  首先，在本地使用`hugo`生成静态站点

  如果是第一次部署网站，切换到生成的静态站点目录，输入命令：`git init`，然后在输入命令：`git remote add origin git@github.com:<github-username>/repo-name.git`，然后在输入`git checkout -b gh-pages`

  如果是后续部署，只需要需要输入

  ```
  $ git add --all
  $ git commit -m "<some change message>"
  $ git push -f origin gh-pages
  ```

  ​

- 部署到[GitLab](https://about.gitlab.com/)

- 添加[GitLab Pages](https://pages.gitlab.io/)的部署文件`.gitlab-ci.yml`，只要将该文件添加到项目根目录并提交到自己[GitLab](https://about.gitlab.com/)账户的`your-username.gitlab.io`项目中

- ​

## 网站配置文件

```toml
title = "网站的标题"
# 如果网站主要用英语编写，则改为"en-us"
languageCode = "zh-cn"
# hostname (and path) to the root, e.g. http://example.com/blog/
# if deploy on github pages, maybe is http://your-username.github.io/repo-name
baseURL = ""
# 指定习惯使用的文章头部front matter格式："toml", "yaml", "json"
metaDataFormat = "yaml"
# 项目使用的主题名（千万不要改动此项内容）
theme = "hugo_theme_robust"
# auto-detect Chinese/Japanese/Korean Languages in the content. (.Summary and .WordCount can work properly in CJKLanguage)
hasCJKLanguage = true
# 配置hugo生成静态页面时，忽略哪些文件。下面的正则表达式用来指定忽略vim的临时文件
ignoreFiles = ["*~$"]

[Params]
  Author = "留下作者的大名"
  # 控制页面中日期显示格式
  DateForm = "2006/01/02"
  # 控制是否显示评论框
  comment = true
  # 控制是否显示分享按钮
  share = true
  # 控制是否加载mathjax
  mathjax = true
  # 指定网站面向的用户群所在国家（会根据这一项决定显示什么样的评论框和分享按钮）
  # 可以将其设置为"cn"（表示在国内）或者其它值（表示国外）
  country = "cn"
  # 面向国外的评论框设置
  disqusShortname = "填写你的Disqus Shortname"
  # 面向国内的评论框设置
  duoshuoShortname = "填写你的Duoshuo Shortname"
```

