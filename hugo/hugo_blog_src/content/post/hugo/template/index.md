---
title: "Hugo 模板"
author: "Xiao Wenbin"
date: 2017-10-05T10:58:08-04:00
draft: true
tags: ["hugo"]
---

### 模板变量

如果说模板是待填充的网页，则模板变量是用来填充模板的内容。Hugo 内置了许多可以在模板中访问的变量，这些变量可以分为以下几种类型

- 网站变量

  通过网站变量，我们可以访问网站级别的配置和数据。

  ```
  .Site.BaseURL 			配置文件中为网站指定的 basse URL
  .Site.RSSLink 			网站的 RSS 链接
  .Site.Taxonomies 		网站所有的分类标签
  .Site.Pages				网站所有页面（仅含当前语言）
  .Site.AllPages			网站所有页面（含多语言）
  .Site.Params			配置文件中通过 params 定义的网站参数
  .Site.Sections			网站所有 Section（也即网站的顶级目录）
  .Site.Title				配置文件中为网站指定的 title
  .Site.Author			配置文件中为网站指定的 author
  .Site.Copyright			配置文件中为网站指定的 copyright
  .Site.LastChange		网站最后更新时间，格式跟内容文档头部 date 保持一致
  .Site.Data				网站自定义数据文件的访问接口
  .Site.RegularPages		网站中所有常规页面
  .Site.Files				网站所有源文件
  .Site.Menus				网站所有菜单
  .Site.LanguageCode		配置文件中为网站指定的 language code
  .Site.DisqusShortname	配置文件中为网站指定的 disqus 评论id
  .Site.GoogleAnalytics   配置文件中为网站指定的 google analytics tracking code
  .Site.Permalinks		配置文件中为网站指定的 permalink format
  .Site.BuildDrafts		配置文件中为网站指定的 build drafts
  .Site.IsMultiLingual	网站是否支持多语言
  .Site.Language			配置文件中指定的 language
  ```

- 页面变量

  通过页面变量，我们可以访问内容文档级别的配置和数据。

  ```
  .Title					内容文档的标题
  .Content				内容文档的内容
  .Date					内容文档的日期
  .PublishDate			页面发布日期
  .FuzzyWordCount			内容的近似字数
  .WordCount				内容的字数
  .Type					内容文档的内容类型
  .URL					页面的相对 URL
  .UniqueID				内容文档路径的md5值
  .Weidht					内容文档中定义的排序权重
  .Kind					页面类型
  .Params					内容文档头部定义的任意元数据都可以通过 .Params 来访问（不同定义如何命名，均以字母小写的名字访问）
  						补充：网站变量中也有 .Site.Params 来定义网站参数，一般来说页面参数比网站参数更具体，
  						可以使用模板函数 $.Param "header_image" 来访问网站和页面的同名参数
  .IsHome					页面是否为首页
  .IsPage					是否为常规内容页面
  .Next					下一个页面（根据页面发布日期）
  .Prev					上一个页面（根据页面发布日期）
  .NextInSection			当天Section中的下一个页面（根据页面分布日期）
  .PrevInSection			当天Section中的上一个页面（根据页面分布日期）
  .TableOfContents		页面目录
  .Permalink				页面的永久链接
  .RelPermalink			页面永久链接的相对路径
  .RawContent				页面的 Markdown 内容，当想要在网站中集成https://github.com/gnab/remark时，就需要提取页面的 Markdown 内容了
  .ReadingTime			页面大概需要花费的阅读时间
  .Section				页面所在 Section
  .Summary				页面摘要
  .Truncated				摘要是否截断页面
  .Description			描述
  .Keywords				关键词
  .LinkTitle				链接到当前页面时使用的 title
  .ExpiryDate				页面失效日期
  .Draft					页面是否为草稿
  .IsTranslated			页面是否有多语言版本
  .Translations			页面的多语言页面
  .Lang					语言
  .Language				语言对象
  ```

- 文件变量

  当页面的生成来源于内容文档时，可以访问内容文档文件相关信息。

  ```
  .File.Path				内容文档的相对路径，比如：content/posts/first.en.md
  .File.Dir				内容文档所在目录
  .File.LogicalName		内容文档文件名，比如：first.en.md
  .File.TranslationBaseName 内容文档根文件名，比如：first
  .File.Ext				内容文档扩展名，比如：md
  .File.Lang				内容文档的语言
  ```

- Hugo 变量

  ```
  .Hugo.Generator			Hugo 版本号的 meta tag，例如：<meta name="generator" content="Hugo 0.15" />
  .Hugo.Version			Hugo 二进制程序版本号
  ```

模板变量的作用域问题

单页模板、Section 列表模板以及 Taxonomy 列表模板均可以访问网站变量和页面变量，此外Taxonomy 列表模板可以访问代表其自身的 `.Data.Singular` 变量。

### 模板角色

模板文件混杂了 HTML 代码和模板标识符，用来设计网页布局的。Hugo 支持 Go 语言的 HTML 模板库来对网站进行布局规划，虽然模板文件本质上没有不同，可 Hugo 结合常用网站布局结构的需要将模板分为了几种角色，下面将依次介绍这些模板角色

也即页面类型

page home section taxonomy or taxonomy Term 

rss sitemap robotsTXT 404

#### 首页模板

Hugo 使用首页模板（homepage template）来渲染网站首页。一般来说网站首页同其它页面具有不一样的风格，因此需要专门为其使用特定的模板进行渲染。Hugo 在生成网站时，通常会依次从下面路径中查找首页模板，将找到的第一个文件作为首页模板：

- /layouts/index.html
- /layouts/_default/list.html
- /layouts/_default/single.html
- /themes/`THEME`/layouts/index.html
- /themes/`THEME`/layouts/_default/list.html
- /themes/`THEME`/layouts/_default/single.html

也即默认首页模板是 `index.html` ，当该文件不存在时，依次使用 `list.html` 和 `single.html` 来充当首页模板。另外首页模板中可以通过模板变量 `.Data.Pages` 来访问网站中所有内容文档，通常我们会遍历该变量在首页创建一个文档展示列表，不过Hugo 不会对模板的创建有任何限制，如何定义首页模板完全取决于自己。

#### 单页模板

Hugo 使用单页模板（single template）来渲染内容文档。换句话说，内容文档的内容将嵌入单页模板设计好的网页结构中，以此生成网页。那么当生成静态网站时，Hugo 会使用哪个单页模板来渲染内容文档呢？Hugo 会依次从下面路径列表中查找可用的单页模板，将找到的第一个单页模板文件作为当前内容文档的渲染模板：

- /layouts/`TYPE`/`LAYOUT`.html
- /layouts/`SECTION`/`LAYOUT`.html
- /layouts/`TYPE`/single.html
- /layouts/`SECTION`/single.html
- /layouts/_default/single.html
- /themes/`THEME`/layouts/`TYPE`/`LAYOUT`.html
- /themes/`THEME`/layouts/`SECTION`/`LAYOUT`.html
- /themes/`THEME`/layouts/`TYPE`/single.html
- /themes/`THEME`/layouts/`SECTION`/single.html
- /themes/`THEME`/layouts/_default/single.html

其中 `TYPE` 表示内容文档的类型名称，`SECTION` 表示内容文档的 Section ，`THEME` 表示主题名称，`LAYOUT` 表示内容文档指定的模板名。`TYPE` 和 `LAYOUT` 可分别通过内容文档头部的 `type` （默认跟所在 Section 同名）和 `layout` （默认为单页模板）进行设置 ，`SECTION` 则由内容文档磁盘路径对应的 Section 决定。

可以看出 Hugo 默认会先从 `TYPE` 和 `SECTION` 这些模板目录中查找文档指定的布局 `LAYOUT` ，再查找相应的单页模板，然后再从网站源默认的布局目录 `_default` 中查找单页模板，最后会查找当前主题的相关布局目录，可见 Hugo 奉行的准则是：先精确查找，再回退默认。

在单页模板中可以访问网站变量和页面变量以及模板函数，通常我们会将内容文档的内容嵌入到单页模板中，有时也许还想为模板创建一个侧变量用来显示相关信息等，怎样定义单页模板完全取决于自己。

一般情况下，当我们为网站添加过主题之后，主题都会有单页模板的，如果想要覆盖主题中定义的单页模板，可以在网站源的模板目录下面创建相应的单页模板，或者直接创建单页模板 `layouts/_default/single.html` 作为内容文档未找到单页模板时的默认模板。

#### 内容视图

Hugo 使用内容视图（content views）来以不同于单页模板的方式展示内容文档。比如有时，我们只想要展示文档摘要或者文档列表项而非整个文档，内容视图在此时就特别有用了。

内容视图也是普通的模板文件，Hugo 查找内容视图时会根据当前文档的内容类型进行查找，也就是说同名的内容视图对不同内容类型渲染效果是不同的。Hugo 会依次从以下路径列表中查找可用的内容视图，将找到的第一个模板文件来作为渲染模板

- /layouts/`TYPE`/`VIEW`.html
- /layouts/_default/`VIEW`.html
- /themes/`THEME`/layouts/`TYPE`/`VIEW`.html
- /themes/`THEME`/layouts/_default/`view`.html

假定我们要为内容类型 `post` 和 `project` 分别创建内容视图 `li.html`  ，则对应的模板文件路径为：`/layouts/post/li.html` 和 `/layouts/project/li.html` 。如果我们在网站首页使用如下代码罗列所有文档

```
{{ range .Data.Pages }}
{{ .Render "li"}}
{{ end }}
```

其中 `{{ .Render "li" }}` 表示引用当前内容文档对应内容视图 `li.html` （`post` 和 `project` 使用各自的内容视图文件），在内容视图 `li.html` 中可以访问任何页面变量，下面是 `li.html` 示例

```html
<li>
<a href="{{ .Permalink }}">{{ .Title }}</a>
<div class="meta">{{ .Date.Format "Mon, Jan 2, 2006" }}</div>
</li>
```

#### 列表模板

Hugo 使用列表模板（list template）渲染多个被罗列的内容文档，比如：分类标签页面和 Section 页面通常需要罗列逻辑上从属于该类别的所有文档。值得注意的是，不同于单页文档总是被内容文档填充，列表模板一般却不会被内容文档填充（下文会介绍什么情况下列表模板也会填充内容文档）。

Hugo 中列表模板常见的应用场景有：Section 列表页、Taxonomy 列表页、Section RSS 以及 Taxonomy RSS等（注：网站首页虽然也是列表页，可因其特殊性，需要使用特定的模板渲染）。这些页面渲染后的 URL 路径分别如下

- Section 列表页

  `baseURL/SECTION/` ，例如：`http://1.com/post/` 

- Taxonomy 列表页

  `baseURL/PLURAL/TERM/` ，例如：`http://1.com/tags/python/` 

- Section RSS

  `baseURL/SECTION/index.html` ，例如：`http://1.com/post/index.html` 

- Taxonomy RSS

  `baseURL/PLURAL/TERM/index.html` ，例如：`http://1.com/tags/python/` 

此外，Hugo 会依次从路径列表中查找可用的列表模板，将找到的第一个列表模板文件来作为渲染模板。以上介绍的常见列表页面的查找路径如下

- Section 列表
  - /layouts/section/`SECTION`.html
  - /layouts/_default/section.html
  - /layouts/_default/list.html
  - /themes/`THEME`/layouts/section/`SECTION`.html
  - /themes/`THEME`/layouts/_default/section.html
  - /themes/`THEME`/layouts/_default/list.html
- Taxonomy 列表
  - /layouts/taxonomy/`SINGULAR`.html
  - /layouts/_default/taxonomy.html
  - /layouts/_default/list.html
  - /themes/`THEME`/layouts/taxonomy/`SINGULAR`.html
  - /themes/`THEME`/layouts/_default/taxonomy.html
  - /themes/`THEME`/layouts/_default/list.html
- Section RSS
  - /layouts/section/`SECTION`.rss.xml
  - /layouts/_default/rss.xml
  - /themes/`THEME`/layouts/section/`SECTION`.rss.xml
  - /themes/`THEME`/layouts/_default/rss.xml
- Taxonomy RSS
  - /layouts/taxonomy/`SINGULAR`.rss.xml
  - /layouts/_default/rss.xml
  - /themes/`THEME`/layouts/taxonomy/`SINGULAR`.rss.xml
  - /themes/`THEME`/layouts/_default/rss.xml

从上面模板的查找路径可以看出，Hugo 首先会查找为特定 `SECTION` 和 `TAXONOMY` 定义的模板文件，如果查找失败，会再查找 Section 和 Taxonomy 通用的模板文件，如果还是找不到就使用 `layouts/_defaults/list.html` 和 `layouts/_defaults/rss.xml` 。

既然知道了列表模板的用途，也知道了模板文件的查找路径，那么列表模板文件中该写些什么呢？列表文件也是一个普通的模板文件，在模板中可以使用任何 Go 内置模板函数，还可以访问网站模板变量和页面模板变量（用于 Taxonomy 的模板还可以访问代表当前分类的变量 `.Data.Singular` ）。根据列表模板的用途一般来说会在模板中为内容文档创建一个展示列表，此外也许希望对这个内容文档分类或者剔除某些文档，利用简洁而强大的 Go 模板方法可以自定义任何复杂的列表页面。下面是一个用于 Section 的列表模板示例

```html
{{ partial "header.html" . }}
{{ partial "subheader.html" . }}

<section id="main">
  <div>
   <h1 id="title">{{ .Title }}</h1>
        <ul id="list">
            {{ range .Data.Pages }}
                {{ .Render "li"}}
            {{ end }}
        </ul>
  </div>
</section>

{{ partial "footer.html" . }}
```

#### 分类模板

Hugo 使用分类模板（taxonomy terms template）来渲染当前分类下的所有标签。

要注意同Taxonomy 列表页相区分，Taxonomy 列表页用来罗列属于某个标签下所有的内容文档，优先查找模`/layouts/taxonomy/SINGULAR.html` 作为该标签列表页的模板，且将页面渲染于 `baseURL/PLURAL/TERM/` 。而分类模板页面是用来罗列当前分类下所有标签的，优先查找 `/layouts/taxonomy/SINGULAR.terms.html` 作为页面模板，且渲染于 `baseURL/PLURAL/` 。

Hugo 会依次从路径列表中查找可用的模板，将找到的第一个模板文件来作为渲染模板

- /layouts/taxonomy/`SINGULAR`.terms.html
- /layouts/_default/terms.html
- /themes/`THEME`/layouts/taxonomy/`SINGULAR`.terms.html
- /themes/`THEME`/layouts/_default/terms.html

如果以上模板都不存在，Hugo 就不会渲染分类标签页面。换句话说，分类标签页面的渲染也不一定必须单独使用一个模板文件，我们可以在页面侧边栏之类的地方来渲染分类标签（比如：侧边栏实现一个标签云）。

分类模板中除了可以访问网站变量和页面变量外，还有一些关于分类标签的变量可供我们使用：

```
.Data.Singular						分类的单数名称，比如：tag
.Data.Plural						分类的复数名称，比如：tags
.Data.Pages							属于当前分类的所有页面
.Data.Terms							属于当前分类的所有标签
.Data.Terms.Alphabetical			属于当前分类的所有标签（字母序）
.Data.Terms.ByCount					属于当前分类的所有标签（根据标签下文档数量排序）
```

下面是一个示例分类模板，该模板罗列出了当前分类下的所有标签，并给出了标签下所有文档的链接

```html
{{ partial "header.html" . }}
{{ partial "subheader.html" . }}

<section id="main">
  <div>
    <h1 id="title">{{ .Title }}</h1>

    {{ $data := .Data }}
    {{ range $key,$value := .Data.Terms.ByCount }}
    <h2><a href="{{ .Site.LanguagePrefix }}/{{ $data.Plural }}/{{ $value.Name | urlize }}">{{ $value.Name }}</a> {{ $value.Count }}</h2>
    <ul>
    {{ range $value.Pages.ByDate }}
      <li><a href="{{ .Permalink }}">{{ .Title }}</a></li>
    {{ end }}
    </ul>
    {{ end }}
  </div>
</section>

{{ partial "footer.html" . }}
```

#### 片段模板

Hugo 使用片段模板（partial template）作为其它模板文件的原材料，比如首页模板、单页模板、列表模板等这些模板通常会使用片段模板来创建。这里之所以将片段模板比作原材料，是因为片段模板通常包含了其它模板中的公共部分，反过来说，我们应该将多个模板中的公共内容分离出来创建片段模板文件，然后可以在其它模板中引用该片段文件。使用片段模板的好处在于，不需要重复定义相同的模板内容，而且片段模板十分有利于主题资源的开发，主题中应该将那些想要让用户覆盖的模板内容单独作为一个片段模板，这样主题的使用者只需要定义相同的片段模板就可以对主题片段模板进行替换，片段模板文件是比普通模板文件更加细粒度的模板内容容器。

如何创建片段模板呢？Hugo 默认将模板目录 `/layouts/partials/` 及其子目录中的模板文件看作片段模板，片段模板的内容如同普通模板一样可以访问各种模板变量和模板函数，不过片段模板可以访问到的模板变量取决于引用该模板时传入了怎样的变量进来（后面会有讲，如何引用片段模板以及如何传递变量到片段模板）。在网站中最为常见的片段模板也许就是网页头和网页脚，因为网页头和网页脚在网站大多数页面中都是相同的，将其分离于片段模板中是明智的选择，假设我们创建了 `/layouts/partials/header.html` 和 `/layouts/partials/footer.html` 片段模板文件，它们的内容分别为

```html
<!DOCTYPE html>
<html class="no-js" lang="en-US" prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb#">
<head>
    <meta charset="utf-8">

    {{ partial "meta.html" . }}

    <base href="{{ .Site.BaseURL }}">
    <title> {{ .Title }} : spf13.com </title>
    <link rel="canonical" href="{{ .Permalink }}">
    {{ if .RSSLink }}<link href="{{ .RSSLink }}" rel="alternate" type="application/rss+xml" title="{{ .Title }}" />{{ end }}

    {{ partial "head_includes.html" . }}
</head>
<body lang="en">
```

和

```html
<footer>
  <div>
    <p>
    &copy; 2013-14 Steve Francia.
    <a href="http://creativecommons.org/licenses/by/3.0/" title="Creative Commons Attribution">Some rights reserved</a>;
    please attribute properly and link back. Hosted by <a href="http://servergrove.com">ServerGrove</a>.
    </p>
  </div>
</footer>
<script type="text/javascript">

  var _gaq = _gaq || [];
  _gaq.push(['_setAccount', 'UA-XYSYXYSY-X']);
  _gaq.push(['_trackPageview']);

  (function() {
    var ga = document.createElement('script');
    ga.src = ('https:' == document.location.protocol ? 'https://ssl' :
        'http://www') + '.google-analytics.com/ga.js';
    ga.setAttribute('async', 'true');
    document.documentElement.firstChild.appendChild(ga);
  })();

</script>
</body>
</html>
```

以上模板内容除了常规的 HTML 代码外，还出现了像 `{{ partial "meta.html" . }}` 这样的模板语句，这条语句在这里的作用是引用片段模板 `meta.html` 到当前模板文件中（即 `header.html` 片段模板文件），就是说 Hugo 允许我们在片段模板中再次引用片段模板。

下面让我们研究一下，如何引用一个片段模板文件，引用片段模板的语法为：`{{ partial "path/to/file.html" variables }}` ，其中 `path/to/file.html` 表示被引用的片段模板文件相对于 `/layouts/partials/` 目录的路径，比如想要引用 `/layouts/partials/post/sidebar.html` ，则对应的引用路径为 `post/sidebar.html` 。其中 `variables` 表示要传入片段模板的变量（片段模板除了这些传入的变量，是无法访问其它变量的），通常我们会将代表当前模板内所有变量的 `.` 作为 `variables` 传入片段模板中。

有没有想过，很多模板引用相同的片段模板文件，在生成网页时，这些片段模板是不是在每个引用模板中都要重新渲染一次呢？有没有办法减少片段模板的渲染次数，毕竟片段模板生成的网页片段除了根据传入变量不同会有改变外，基本的网页结构是相似的。如果想要让 Hugo 提升片段模板的渲染效率（Hugo 会自动缓存已经渲染好的片段模板供后续使用），可以在引用模板文件时用 `partialCached` 来代替 `partial` ，并且 Hugo 还支持用户按照类别缓存片段模板，比如： `{{ partialCached "footer.html" . .Section }}` 的意思是，为每个 Section 渲染一次 `footer.html` 模板。

### 模板调试

模板编写中错误在所难免，可以使用模板函数 `printf` 调试模板变量，下面是几个常见调试样例

```
{{ printf "%#v" . }}
{{ printf "%#v" $.Site }}
{{ printf "%#v" .Permalink }}
{{ range .Data.Pages }}
    {{/* The context, ".", is now a Page */}}
    {{ printf "%#v" . }}
{{ end }}
```