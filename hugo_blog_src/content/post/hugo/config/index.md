---
title: "Hugo 配置"
author: "Xiao Wenbin"
date: 2017-08-17T10:58:08-04:00
draft: true
tags: ["hugo"]
---


既可以通过操作系统中以`HUGO_`为前缀的环境变量名来配置，也可以通过位于工作目录中的文件`config.toml`、`config.yaml`或者`config.json`来配置，Hugo会依次查找这三个配置文件，将找到的第一个文件作为配置文件（这三种文件在配置Hugo的功效上是等价的，仅仅只是文件内容格式不同而已）。

下面是一个`config.yaml`配置文件样例

```yaml
baseURL: "http://yoursite.example.com/"
title: "My Blog"
contentDir: "content"
layoutDir: "layouts"
publishDir: "public"
buildDrafts: false
params:
  AuthorName: "Your Name"
  Hobby:
    - "foo"
    - "bar"
  SidebarRecentLimit: 5
```

配置文件中主要含Hugo内置配置项以及用户自定义配置项两类。像上文配置文件中`baseURL`、`contentDir`、`layoutDir `、`publishDir`、`buildDrafts`等这些都是Hugo内置的配置项，这些配置项Hugo都是给它们添加默认值，除非你要修改这些默认值，一般情况下这些配置项保持Hugo的默认值即可，是不需要出现在你的配置文件中的。此外某些时候可能想要添加自定义配置项，比如上文中`AuthorName`、`Hobby`、`SidebarRecentLimit`都是用户自定义的全局参数，这些参数可以在主题模板文件中访问，以此来控制网站的生成过程。