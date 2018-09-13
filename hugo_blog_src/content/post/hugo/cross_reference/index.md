---
title: "Hugo 文档引用"
author: "Xiao Wenbin"
date: 2017-10-06T10:58:08-04:00
draft: false
tags: ["hugo"]
---

## 简介

撰写文档时，经常需要引用该文档内特定部分或者其它文档，以方便读者阅读时可以通过超链接直接跳转到相关的内容。一个文档往往会包含多个知识点，通过超链接交叉引用各个文档，使得文档之间的知识联系更加紧密，同时也提供了更好的阅读体验。

虽然 Markdown 支持超链接的插入，但一个未发布的文档其链接地址是未知的，所以直接在 Markdown 中插入被引用文档的链接地址是不可行的，链接地址的插入应该由文档发布工具来实现。Hugo 提供了两种 shortcode 用来在文档中插入引用链接地址，基本思路是文档创作者在撰写文档时，通过本地文件系统路径来引用文档，然后在生成网站时，Hugo 会自动将这些文件系统路径转换为相应的超链接地址。

## 用法

Hugo 提供了两种 shortcode 用来在文档中插入引用链接地址： `ref` 和 `relref` ，语法如下：

```
{{</* ref "path/to/document.md#achor" */>}}
{{</* relref "path/to/document.md#achor" */>}}
```

其中 `relref` 插入被引用文档的相对链接地址，而 `ref` 则插入被引用文档的完整链接地址。

 `ref` 和 `relref` 的唯一参数是文档路径+锚点组成的字符串，并且文档路径和锚点都是可选的。当参数中只含有文档路径时，会插入被引用文档的链接地址；当参数中只含有锚点时，会插入当前文档的锚点链接地址；当参数中二者都存在时，会插入被引用文档的锚点链接地址。样例如下：

```
[Anchor]({{</* relref "#anchors" */>}}) => <a href="#anchors:9decaf7">Anchor</a>
[Hugo]({{</* relref "about/hugo.md" */>}}) => <a href="/about/hugo/">About</a>
[Who]({{</* relref "about/hugo.md#who" */>}}) => <a href="/about/hugo/#who:badcafe">About</a>
```

注：由于 `ref` 和 `relref` 仅用来生成链接地址，要在 Markdown 文档中插入链接引用，还需要结合 Markdown 链接语法一起使用才行。

到底该使用 `ref` 还是 `relref` 呢？二者的区别就在于生成的链接是否含有网站的 baseURL ，不含 baseURL 的好处在于，当 baseURL 改变时网站不需要重新生成。

## 文档路径

 `ref` 和 `relref` 参数中的文档路径，是被引用文档相对于内容目录 `content/` 的路径。假设被引用文档完整路径为 `mysite/content/blog/about.md` ，那么提供给 `ref` 和 `relref` 参数中的文档路径内容为 `blog/about.md` （注：不能以 `/` 开头）。此外还可以直接输入文档名，但当不同 Section 中含有相同文档名时就会产生二义性，因此建议使用相对于 `content/` 的路径名。

## 文档锚点

文档锚点用来定位文档中的特定部分，比如锚点 `#who` 就会定位文档中 `id="who"` 的元素。那么文档锚点是怎么来的，是 Hugo 自动生成的，还是我们自定义的呢？其实在 Hugo 将 Markdown 文档转换为网页时，自动为 Markdown 文档内的标题级内容生成了锚点以供引用，并且 Hugo 还保证了锚点在文档内以及整个站点内的唯一性。

那么 Markdown 文档中标题级内容是如何转化为锚点的呢？假设标题内容为：Hugo: A Fast & Modern Static Web Engine，最终被转换成锚点 `hugo-a-fast-modern-static-web-engine` 。标题到锚点转换过程为：先将其转换为小写内容，再将非字母数字字符用连字符替换，并将多余的连字符去除。

除了让 Hugo 自动将标题内容转为锚点，Hugo 还支持文档创作者为标题指定固定的锚点，比如这样定义标题内容：`# Hugo: A Fast & Modern Static Web Engine {#hugo-main}` ，然后 Hugo 在生成站点时就会为该标题生成锚点 `hugo-main` 。此外当页面中出现相同锚点时，Hugo 会自动为这些锚点添加后缀 `-1` 、`-2` 等。