---
title: "Hugo 集成 MathJax"
author: "Xiao Wenbin"
date: 2017-03-16T09:46:42+08:00
draft: false
tags: ["hugo", "mathjax"]
---

## 简介

[MathJax](http://www.mathjax.org/) 是一款用于显示 LaTex 等风格数学公式的利器，只需要将其简单几步就可以将其集成到 Hugo 中。


## 引用 MathJax

为了展示文章中编写的 LaTeX 数学公式，需要集成 [MathJax](https://www.mathjax.org/)。MathJax 是一个 Javascript 库，通过官方提供的 CDN 集成到自己站点十分简单，只需要将以下代码添加到站点的每一个 HTML 页面中

```html
<script type="text/javascript" async
  src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
```

为 HTML 页面添加以上代码后，页面中所有 [LaTex](https://en.wikibooks.org/wiki/LaTeX/Mathematics) , [MathML](https://www.w3.org/Math/) 以及 [AsciiMath](http://asciimath.org/) 风格的数学公式都会得以显示，当然在页面中查找并显示这三种公式会对性能有所损耗，如果文章中仅仅含有LaTex编写的数学公式，可以将下面代码插入到站点每一个页面中

```html
<script type="text/javascript" async src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS_CHTML"></script>
```

可以看出唯一的不同之处在于脚本源 `src` 中的 `config` 参数，不同的`config`定义了不同数学公式的输入以及输出，更多 MathJax 配置内容[参见官网](http://docs.mathjax.org/en/latest/config-files.html#common-configurations)。

## 集成 MathJax

为了使得引用 MathJax 的代码可以添加到站点每一个页面，可以将其插入到站点每个页面都会包含的模板中，比如页脚模板 `partials/default_footer.html` 中，并且可以在站点配置文件中添加配置参数，然后模板中就可以根据配置参数决定是否插入集成 MathJax 的代码。

比如 `config.toml` 配置内容为

```
[Params]
  mathjax = true
```

然后在模板 `partials/default_footer.html` 中添加如下代码：

```
{{ if or (eq .Params.mathjax true) (eq .Site.Params.mathjax true) }}
<script type="text/javascript" async src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS_CHTML"></script>
{{ end }}
```

## MathJax 跟 Markdown 冲突

Markdown 将 `_` 解析为强调，因此 `_..._` 会被 Markdown 引擎转换为 `<em>...</em>`；而 MathJax 将 `_` 解析为下标，位于 `_` 之后的内容会作为 `_` 之前内容的下标。显然在生成静态站点时，Markdown 引擎已经将内容中所有 `_` 转换为 `<em>` 标签，即便是 LaTeX 数学公式中的 `_` 也被转换成了 `<em>`，使得数学公式被破坏了，这样在使用浏览器浏览页面时，MathJax 将无法正确渲染被破坏的数学公式，为了解决这个问题下面有几种解决方案：

### 方案一

数学公式中使用 `_` 的地方都转义为 `\_`，这样在 Markdown 引擎转换文档时会将 `\_` 转换为 `_` ，可以使得 MathJax 能够正确的渲染该公式。该方法的缺点也很明显，将数学公式中的 `_` 转义为 `\_` 会很繁琐，而且如果用户使用的 Markdown 编辑器支持 LaTeX 公式预览功能，转义为 `\_` 可能被预览功能识别为 LaTeX 中对下标符 `_` 的转义，将使得下标功能失效，下标符原样输出。

### 方案二

鉴于嵌套在 HTML 区块标签内的 Markdown 语法不被 Markdown 引擎处理，会原样输出，因此可以考虑将 LaTeX 块级数学公式放在标签 `<div></div>` 内，数学公式就不会被 Markdown 引擎处理。至于 LaTeX 行内数学公式，如果放在 `<div></div>` 内则会独立成段打乱内容排版，为此需要考虑使用其它方法避免行内数学公式被 Markdown 引擎处理。Markdown 语法支持行内代码，可以考虑将行内数学公式放在 Markdown 的行内代码语法中，这样就可以避免 Markdown 引擎处理其中的 `_`。

```markdown
`$x_\alpha$`
以上内容会被Markdown引擎解析为 <code>$x_\alpha$</code>
```

不过 MathJax 默认不会对 `<script>`, `<pre>`, `<code>`等 HTML 标签内的数学公式进行解析。因此需要配置 MathJax 使其可以解析 `<code>` 中的数学公式。添加以下代码到站点所有页面中

```html
<script type="text/x-mathjax-config">
MathJax.Hub.Config({
  tex2jax: {
    inlineMath: [['$','$'], ['\\(','\\)']],
    displayMath: [['$$','$$'], ['\[','\]']],
    processEscapes: true,
    processEnvironments: true,
    skipTags: ['script', 'noscript', 'style', 'textarea', 'pre'],
    TeX: { equationNumbers: { autoNumber: "AMS" },
         extensions: ["AMSmath.js", "AMSsymbols.js"] }
  }
});
</script>
```

这样所有位于 `<code></code>` 标签内的数学公式都会被 MathJax 渲染（当然如果正好想要让 `<code></code>` 标签内数学公式不渲染而原样输出，就办不到了，因为所有公式都将会被渲染），除了配置 MathJax 使得 `<code></code>` 内的数学公式可以被渲染外，`<code></code>` 的显示样式同普通文本也是不一样的，因此还需要配置含有数学公式 `<code>` 标签的样式，需要将如下代码添加到站点的所有页面中

```html
<script type="text/x-mathjax-config">
  MathJax.Hub.Queue(function() {
    // Fix <code> tags after MathJax finishes running. This is a
    // hack to overcome a shortcoming of Markdown. Discussion at
    // https://github.com/mojombo/jekyll/issues/199
    var all = MathJax.Hub.getAllJax(), i;
    for(i = 0; i < all.length; i += 1) {
        all[i].SourceElement().parentNode.className += ' has-jax';
    }
});
</script>
```

并将如下代码添加到站点的 CSS 文件中

```css
code.has-jax {font: inherit;
              font-size: 100%;
              background: inherit;
              border: inherit;
              color: #515151;}
```

此外如果使用了高亮工具 [Pgyments](http://pygments.org/) ，数学公式可能不能够正确显示为黑色，为了修正这个问题需要将以下代码添加到站点每个页面中

```html
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    "HTML-CSS": {
      styles: {
        ".MathJax .mo, .MathJax .mi": {color: "black ! important"}
      }
    }
  });
</script>
```

该方案的缺点也很明显，需要把块级数学公式写在 `<div></div>` 中，行级数学公式写在 Markdown 的行内代码中，并且配置 MathJax 对 `<code></code>` 标签内容进行解析可能会使得原本要原样输出的公式被渲染显示。

### 方案三

以上两种方案都有一个明显的缺点，为了使得 MathJax 能够正确渲染数学公式而要求在撰写 Markdown 文档时添加某些特殊符号，甚至在行内代码语法内嵌套行级数学公式，显然违背了 Markdown 行内代码语法的语义，理想的方式是用户可以像普通文本一样将数学公式添加到 Markdown 文档中，然后由 Markdown 引擎来负责正确解析文档，经过研究（主要参考了两篇文章[1](https://github.com/spf13/hugo/issues/1666), [2](http://nosubstance.me/post/a-great-toolset-for-static-blogging/)）发现 Hugo 目前搭载了两个 Markdown 引擎[Blackfriday](https://github.com/russross/blackfriday) 和 [Mmark](https://github.com/miekg/mmark)，默认解析 Markdown 文档的是Blackfirday引擎，该引擎简单快速，仅支持基本的 Markdown 语法，而 Mmark 引擎则支持 Markdown [语法超集](https://github.com/miekg/mmark/wiki/Syntax)，参考该语法 `Math Blocks` 部分的介绍，发现 Mmark 引擎对数学公式的书写十分友好，要想插入数学公式只需要使用 `$$` 来包围数学公式即可，Mmark 会自动根据数学公式的位置判断数学公式是要转换为行级公式还是块级公式，如果数学公式处于段落内就会被转换 LaTeX 行级公式的语法 `\(...\)`，反之如果数学公式独立成段就会被转换为 LaTeX 块级公式语法 `\[...\]`，而且 Mmark 不会对 `$$` 包围的内容做任何解析，看来使用 Mmark 引擎就是最简单的方案了，如何优雅的使用该方案呢？

1. 配置 hugo 在处理文档时使用 Mmark 引擎。要么将文档扩展名从 `*.md` 改为 `*.mmark`，要么在文档的 Front Matter 中配置 `markup` 为 `mmark` 。

2. 文档中插入数学公式，行级公式直接使用 `$$` 包围插入，块级公式需要将公式独立成段落并用 `$$` 包围

3. MathJax 默认 LaTeX 行级的定界符为 `\(...\)`，默认块级定界符为 `$$...$$` 和 `\[...\]`。而 Mmark 生成的行级和块级公式语法正好可以被 MathJax识别，想要明确声明 MathJax 数学公式的定界符，使用如下代码：

```html
<script type="text/x-mathjax-config">
MathJax.Hub.Config({
  tex2jax: {inlineMath: [['\\(','\\)']]},
  displayMath: [['$$','$$'], ['\\[','\\]']]
});
</script>
```