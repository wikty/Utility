---
title: "hugo data"
author: "Xiao Wenbin"
date: 2017-10-06T10:58:08-04:00
draft: true
tags: ["hugo"]
---

注：数据以键值对形式保存在模板变量中，键由数据文件所在目录名、文件名以及变量名来决定，比如定义在数据文件 `data/author/en/fiction.toml` 中的变量 `names` ，最终在模板中通过 `.Site.Data.author.en.fiction.names` 来引用。