---
title: "Hugo Shortcode"
author: "Xiao Wenbin"
date: 2017-10-05T10:58:08-04:00
draft: false
tags: ["hugo"]
---

## 简介

Markdown 语法十分简洁，如果想要插入更加复杂的内容就需要直接使用 HTML 代码，比如通过 `<img>` 来自定义图片尺寸，通过 `<iframes>` 来插入视频。显然这样做，虽然扩展了 Markdown 文档的表达能力，但却牺牲了 Markdown 语法的简洁性，而且插入的 HTML 代码不利于后续对文档的维护和更新。

问题的关键在于，Markdown 的简洁性本身就意味着它难以用来书写复杂的内容，尤其是涉及到展示效果的内容。Hugo 提供了 shortcode 来解决这一问题，既使内容保持了 Markdown 的简洁性，又允许创作者在文档中嵌入一些 Markdown 不支持的形式复杂的内容。

简单来说，shortcode 是一些可以直接插入内容文档中的助记符，在 Hugo 生成网站时，会将这些助记符替换为相应的 HTML 代码片段（严格来说是模板片段）。这样的好处在于，在创作内容时，只要了解这些助记符的用法而不必关心它们是如何实现和转换的。另一方面来看，更新助记符对应的 HTML 片段时，内容文档不会受到影响。接下来将分别介绍：如何在内容文档中使用 shortcode ，Hugo 内置了哪些 shortcode 以及如何自定义 shortcode ？

## 用法

Shortcode 语法有些类似 HTML 标记，一个完整的 shortcode 包含以下几个部分

````
{{%/* shorcodename parameters */%}}some content for shortcode template{{%/* /shortcodename */%}}
````

其中 `{{%/* shorcodename parameters */%}}` 表示开标记，相应的 `{{%/* /shortcodename */%}}` 表示闭标记，闭标记是可选的，同样在开闭标记之间的内容也是可选的。

开标记中的参数，最终会被传入 shortcode 模板文件中，影响模板的渲染。参数允许以位置参数或命名参数的形式输入（但不能同时传递这两种参数），参数之间用空格间隔，如果参数本身含有空格则需要为其添加双引号。命名参数的格式为 `name="value"` 。

除了使用上面的 `%` 外，还可以使用 `<>` 来作为 shortcode 的定界符，比如 `{{</* gist spf13 7896402 */>}}` 。后者跟前者的唯一区别在于，包含在开闭标记之间的内容将不会被 Markdown 引擎处理。

另外，本文中的许多 shortcode 只是用于显示，并不想要被 Hugo 处理，此时需要在 `%` 或 `<>` 定界符内侧添加 `/*` 和 `*/` 。

## 内置 Shortcode

Hugo 预先定义了一些较为常用的 shortcode ，下面介绍如何使用以及在什么情形下使用它们，更详细的用法参见[官网](https://gohugo.io/content-management/shortcodes/)

### 高亮

用来高亮文档中的代码片段，用法样例：

```
{{</* highlight python */>}}
def pt(txt):
    print(txt)
{{</* /highlight */>}}
```
### 图片

用来扩展 Markdown 中插入图片的语法，该 shortcode 插入的图片支持自定义 CSS 类、添加链接和 caption 等，用法样例：

```
{{</* figure src="/media/spf13.jpg" title="Steve Francia" link="" caption="" class="" attr="" attrlink="" alt="" */>}}
```
### 文档引用

根据文档在本地文件系统中的路径，来插入文档的超链接，甚至可以引用文档标题位置，用法样例：

```
[Neat]({{</* ref "blog/neat.md" */>}})
[Who]({{</* relref "about.md#who" */>}})
```

参数为被引用文档的路径，Hugo 会自动将其替换为被引用文档的永久链接（permalink），`ref` 和 `relref` 的不同之处在于，后者给出相对链接，而前者给出完整链接。以上样例的生成结果如下

```
<a href="/blog/neat">Neat</a>
<a href="/about/#who:c28654c202e73453784cfd2c5ab356c0">Who</a>
```

### GitHub 代码片段

用来在文档中插入 GitHub 上创建的代码片段，假设代码片段链接为：[https://gist.github.com/username/id](https://gist.github.com/username/id) ，则插入语法如下：

```
{{</* gist username id */>}}
```

### Twitter 推文

用来在内容中插入一条 tweet ，推文的链接是这样的： [https://twitter.com/spf13/status/666616452582129664](https://twitter.com/spf13/status/666616452582129664) ，插入这条推文的 shortcode 如下

```
{{</* tweet 666616452582129664 */>}}
```
### YouTube 视频

用来在内容中插入 YouTube 视频，YouTube 视频资源链接是这样的： [https://www.youtube.com/watch?v=w7Ft2ymGmfc](https://www.youtube.com/watch?v=w7Ft2ymGmfc) ，插入该视频的 shortcode 如下

```
{{</* youtube w7Ft2ymGmfc */>}}
```

或者开启自动播放

```
{{</* youtube id="w7Ft2ymGmfc" autoplay="true" */>}}
```
### Vimeo 视频

同插入 YouTube 视频类似，假设资源链接为：[https://vimeo.com/channels/staffpicks/146022717](https://vimeo.com/channels/staffpicks/146022717) ，则插入语法如下

```
{{</* vimeo 146022717 */>}}
```
### Speaker Deck 演示文稿

[Speaker Deck](https://speakerdeck.com/) 是一个允许我们共享演示文稿的地方，我们可以将共享在其上的演示文稿插入到内容文档中，在 Speaker Deck 上点击分享后会生成一段 HTML 代码，假设其中的 `data-id="123456"` ，那么在文档中可以使用如下语法插入该演示文稿

```
{{</* speakerdeck 123456 */>}}
```
### Instagram 图片

插入 Instagram 上的图片，假设某张图片链接为：[https://www.instagram.com/p/BMokmydjG-M/](https://www.instagram.com/p/BMokmydjG-M/) ，则插入该图片的语法为

```
{{</* instagram BMokmydjG-M */>}}
```

或

```
{{</* instagram BMokmydjG-M hidecaption */>}}
```
## 自定义 Shortcode

### 模板文件位置

shortcode 的工作机制就是将助记符关联的 HTML 模板片段渲染后插入到文档中。Hugo 支持自定义 shortcode ，需要做的十分简单，只要在模板目录 `layouts/shortcodes/` 中创建模板文件即可，模板文件名即为 shortcode 的名称（除去文件名中的扩展名）。

同时 Hugo 还支持检索主题资源中的 shortcode，因此 shortcode 的查找顺序如下

1. `/layouts/shortcodes/<SHORTCODE>.html`
2. `/themes/<THEME>/layouts/shortcodes/<SHORTCODE>.html`

### 模板文件内容

shortcode 的模板文件就是普通的 Hugo 模板文件。只不过在 shortcode 模板文件内，可以通过模板变量来访问传入 shortcode 的参数和开闭标记之间的内容。以及其它常规模板变量都可以在 shortcode 模板中访问。

#### 访问参数

虽然在使用 shortcode 时只可以传入位置和命名参数中的一种，但是在设计 shortcode 模板时却可以考虑接受这两种参数（当然不可能同时接受），为此可以通过模板变量 `.IsNamedParams` 来判断，当前参数传入是位置的，还是命名的。

在模板文件中，位置参数和命名参数都可以通过模板方法 `.Get` 来访问：`{{ .Get 0 }}` 和 `{{ .Get "name" }}` 。或者使用 `with` 语法来访问 `{{ with .Get "class"}}class="{{.}}"{{ end }}` 。此外还可以通过模板变量 `.Params` 来访问参数。

#### 访问内容

使用 shortcode 时，位于开闭标记之间的内容，在模板文件中可以通过模板变量 `.Inner` 来访问。

#### 访问父模板

此外 shortcode 还支持嵌套，比如在内容文档中像下面这样插入 shortcode

```
{{</* parentshortcode */>}}
{{</* childshortcode */>}}
{{</* /parentshortcode */>}}
```

然后在模板文件 `layouts/shortcodes/childshortcode.html` 中可以通过模板变量 `.Parent` 来访问 `parendshortcode.html` 的模板环境。

总之 shortcode 模板文件跟普通的模板文件没有差别，基本就是 HTML 代码跟模板变量的混合体，不过 shortcode 模板除了可以访问常规模板变量外，还可以额外访问几个变量和方法：

```
{{ .Get 0 }}                    获取位置参数
{{ .Get "name" }}               获取命名参数
{{ with .Get "class"}} <p class="{{.}}"> haha! </p>{{ end }}
.Inner                          位于 shortcode 开闭之间的内容
.Params                         输入的参数列表
.IsNamedParams                  判断 shortcode 输入的是位置参数还是命名参数
.Parent                         shortcode 支持继承，该变量表示父 shortcode
.Page                           所有的页面变量在 shortcode 都可用
```

### 模板样例

年份，模板位置 `/layouts/shortcodes/year.html` ，内容：

```
{{ .Page.Now.Year }}
```

高亮，模板位置 `/layouts/shortcodes/highlight.html`，内容：

```
{{ .Get 0 | highlight .Inner  }}
```

图片，模板位置 `/layouts/shortcodes/figure.html`，内容：

```
<figure {{ with .Get "class" }}class="{{.}}"{{ end }}>
    {{ with .Get "link"}}<a href="{{.}}">{{ end }}
        <img src="{{ .Get "src" }}" {{ if or (.Get "alt") (.Get "caption") }}alt="{{ with .Get "alt"}}{{.}}{{else}}{{ .Get "caption" }}{{ end }}"{{ end }} />
    {{ if .Get "link"}}</a>{{ end }}
    {{ if or (or (.Get "title") (.Get "caption")) (.Get "attr")}}
    <figcaption>{{ if isset .Params "title" }}
        <h4>{{ .Get "title" }}</h4>{{ end }}
        {{ if or (.Get "caption") (.Get "attr")}}<p>
        {{ .Get "caption" }}
        {{ with .Get "attrlink"}}<a href="{{.}}"> {{ end }}
            {{ .Get "attr" }}
        {{ if .Get "attrlink"}}</a> {{ end }}
        </p> {{ end }}
    </figcaption>
    {{ end }}
</figure>
```

YouTube 视频，模板位置 `/layouts/shortcodes/youtube.html`，内容：

```
<div class="embed video-player">
<iframe class="youtube-player" type="text/html" width="640" height="385" src="http://www.youtube.com/embed/{{ index .Params 0 }}" allowfullscreen frameborder="0"></iframe>
</div>
```