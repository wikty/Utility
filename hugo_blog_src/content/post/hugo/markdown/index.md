---
title: "Hugo 中的 Markdown 语法"
author: "Xiao Wenbin"
date: 2017-09-06T10:58:08-04:00
draft: false
tags: ["hugo"]
---

## 简介

Hugo 中用于书写的标记语言主要是 Markdown。Markdown 作为一门标记语言，其核心[语法][1]十分精炼易用，有许多优秀的 Markdown 解释器可以将 Markdown 文档转换为 HTML 等便于阅览的文档。Markdown 的巨大优势在于，将内容创作和内容展示剥离开来，给予内容创作者极大的自由。

不过 Markdown 的核心语法十分精炼，甚至某些功能是不被支持，比如任务列表。为此开发者们，开发出众多解释器来扩展 Markdown 核心语法。Hugo 支持两个 Markdown 扩展语法（解释器）：[Blackfriday][2] 和 [Mmark][3]。可以将 Blackfriday 看成是对基本 Markdown 语法的简单扩展，Mmark 是 Markdown 语法的超集。Hugo 通过文档后缀名或者文档头 Front Matter `markup` 来识别这两类 Markdown 文档。Blackfriday Markdown 文档后缀名为 `.md` 或者 `markup="markdown"` ，Mmark Markdown 文档后缀名为 `.mmark` 或者 `markup="mmark"`。

## 配置 Markdown 解释器

Hugo 允许我们配置解释器，来扩展 Markdown 语法，改变解释器的工作过程。由于 Mmark 是 Markdown 语法的超集，语言本身就支持许多特色功能，是无需通过配置来扩展的，因此这里所说的配置是针对 Blackfriday 解释器而言的。更多关于 Mmark 语法参见[此处][6]。

### 如何配置

既可以对解释器进行全局配置，也可以针对一个文档进行配置。全局配置是通过在网站配置文件中添加 `blackfriday` 配置项来实现的，而针对文档的配置是通过文档头 Front Matter 中添加 `blackfriday` 配置项来实现的，且后者配置的优先级高于前者。

配置项 `blackfriday`  又由许多配置子项组成的，因此 `blackfriday` 的配置内容会被写为一个分组，下面分别用 TOML 和 YAML 语法来做样例：

```toml
[blackfriday]
  angledQuotes = true
  fractions = false
  plainIDAnchors = true
  extensions = ["hardLineBreak"]
```

```yaml
blackfriday:
  angledQuotes: true
  fractions: false
  plainIDAnchors: true
  extensions:
    - hardLineBreak
```

不论是全局配置还是针对文档的配置，只要添加以上配置内容就可以改变 Markdown 解释器的行为。

### 常用配置项

* `taskLists`，默认为 `true` 。控制是否支持 Github 风格的任务列表语法
* `smartypants`，默认为 `true`。控制是否开启标点符号（双引号、分子符号、连字符）的转换。
* `angledQuotes`，默认为 `false`。控制是否将中文双引号 `“hugo”` 转换为 `«hugo»`。
* `fractions`，默认为 `true`。控制是否将分子式 `5/7` 转换为 HTML 格式 `<sup>5</sup>&frasl;<sub>7</sub>`。
* `smartDashes` 和 `latexDashes` 共同控制多个连字符如何转换为 `–` 和 `—`。
* `hrefTargetBlank`，默认为 `false`。控制打开外部链接时是否打开新的浏览器窗口。
* `plainIDAnchors`，默认为 `true`。不向标题内容添加文档 ID。
* `extensions`，是列表项。包含于该列表中的 Blackfriday Markdown 扩展语法标识，将开启对应的扩展语法的支持。
* `extensionsmask`，是列表项。包含于该列表中的 Blackfriday Markdown 扩展语法标识，将关闭对应的扩展语法的支持。

### 扩展语法支持

Hugo 的 Blackfriday 解释器扩展了核心的 Markdown 语法，下面将介绍一些常用的扩展语法，更多扩展参见[官网][7]

* 禁止解析单词内的下划线

  扩展语法标识为：`noIntraEmphasis`，默认开启该扩展。由于 `_` 字符是 Markdown 语法的组成部分，所以如果代码中出现类似 `init_priority_list` 这样的内容，将会被误当成 Markdown 语法来解析。

* 开启对表格语法的支持

  扩展语法标识为：`tables`，默认开启该扩展。表格语法如下

  ```markdown
     Name | Age
  --------|------
      Bob | 27
    Alice | 23
  ```

* 代码块

  扩展语法标识为：`fencedCode`，默认开启该扩展。代码块语法如下

  ```markdown
  ​```markdown
  # h1
  # h2
  ​```
  ```

* 自动转换 URL

  扩展语法标识为：`autolink`，默认开启该扩展。将内容中那些没有使用 Markdown 语法书写的 URL，转换为 Markdown 格式的 URL。

* 删除线

  扩展语法标识为：`strikethrough`，默认开启该扩展。删除线使用两个波浪线的语法 `~~删除线~~`。

* 强制换行

  扩展语法标识为：`hardLineBreak`，默认关闭该扩展。默认的 Markdown 语法中，没有用空行间隔开的多行内容最终会被转换为一行内容来输出。开启该扩展后，内容中只要有换行，输出中就会换行。

* Tab 空格数

  扩展语法标识为：`tabSizeEight`，默认关闭该扩展。开启该扩展后，每个制表符 Tab 将被转换为 8 个空格（默认是 4 个）。

* 脚注

  扩展语法标识为：`footnotes`，默认开启该扩展。支持 Pandoc 风格的脚注语法，样例如下

  ```markdown
  文章里有一个脚注[^1]

  定义在文档尾部的脚注内容如下
  [^1]: 这里是脚注的内容
  ```

* 标题 ID

  扩展语法标识为：`headerIds`，默认开启该扩展。允许通过 `{#id}` 来为标题指定 ID。此外还有另外一个扩展标识 `autoHeaderIds`，用来控制是否自动为标题创建 ID。

* 定义列表

  扩展语法标识为：`definitionLists`，默认开启该扩展。定义列表的语法如下

  ```markdown
  汽车
  : 汽车是一种机动车

  铁
  : 铁是广泛存在于自然界的一种常见元素，铁的冶炼和制作对现代工业革命中关键性的作用
  ```

## 其它标记语言

除了 Markdown 语言外，Hugo 还支持 Emacs Org-Mode 语言（文档后缀名为 `.org` 或者 `markup="org"`）。此外 Hugo 还可以通过外部程序来支持其它标记语言，比如：[Asciidoc][4], [reStructuredText][5] 等。要使用这些标记语言来书写文档也十分简单，只要将文档的扩展名，命名为该标记语言标准的扩展名即可。Hugo 在生成静态网站时就会自动调用相应标记语言的解释器来渲染它们。当然，前提是已经在电脑上安装好了这些解释器。

## 数学公式显示

可以借助一款 Javascript 函数库 [MathJax][8] 来显示嵌入在 HTML 文档中 LaTex 风格的数学公式。

### 添加 MathJax 到模板中

为了可以在所有页面中显示数学公式，建议将使用 MathJax 的代码添加到被所有页面包含的模板文件中，比如 `footer.html` 模板文件中。使用 MathJax 的代码如下：

```html
<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.1/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>
```

除了上面这种方式使用 MathJax 外，还有[其它方式][9]。

### Markdown 和 MathJax 的冲突

任何 MathJax 可以解析的数学公式[语法][10]，都可以直接书写在 Markdown 文档中，在 Markdown 引擎将 Markdown 文档解析为 HTML 后，MathJax 会负责解析 HTML 文档中数学公式语法。

不过这里有两个问题需要注意：

* 下划线 `_` 同时是 Markdown 语法和数学公式语法的组成部分。解析 Markdown 文档时，文档中所有 `_` 语法会被处理，甚至在数学公式中使用的 `_` 也会被处理。而 Markdown 引擎处理了数学公式中的 `_` ，将导致数学公式显示时出错。
* MathJax 要显示 HTML 文档中的数学公式，就需要某种机制来查找到这些数学公式代码。MathJax 是通过数学公式的定界符来查找数学公式的，并且支持自定义定界符。

要解决 Mathdown 和 MathJax 对下划线解析的冲突问题，一种直观的方案是将 Markdown 文档中数学公式中的 `_` 转义为 `\_` 。Markdown 引擎处理过后，`\_` 将会被转换为 `_` ，因此数学公式语法是有效的。不过该方案一来要插入许多转义符，二来使得 Markdown 文档中书写的数学公式不直观。

另外一种解决下划线冲突的方案是，使用 HTML 标签 `<div></div>` 来将数学代码包含在内，由于 Markdown 引擎不会处理包含在 HTML 标签中的内容，因此 Markdown 引擎也就不会解析数学代码中的下划线了。不过这只适用于块级数学公式（即独立成行的数学公式），并不适用于行内数学公式（即嵌套在行内的数学公式），对于行内数学公式可以使用 Markdown 中的行内代码语法来包裹起来，这样 Markdown 引擎就不会处理里面的内容了，不过由于行内代码的显示样式跟普通文本不一样，所以需要利用 CSS 代码来将其更改为正常的文本样式，具体[方法][11]如下：

添加 Javascript 代码到所有页面中

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

添加 CSS 代码到所有页面中

```css
code.has-jax {
    font: inherit;
    font-size: 100%;
    background: inherit;
    border: inherit;
    color: #515151; /* 应该设置为跟 body 标签一样的颜色 */
}
```

最终在 Markdown 文档中书写块级数学公式的语法为：`<div>$$TeX Code$$</div>`，行内数学公式的语法为：<code>`$ TeX Code $`</code> 或 <code>`\( TeX Code \)`</code>。

[1]: https://daringfireball.net/projects/markdown/	"Markdown Syntax"
[2]: https://github.com/russross/blackfriday	"Blackfriday Markdown"
[3]: https://github.com/miekg/mmark	"Mmark Markdown"
[4]: http://asciidoctor.org/	"Asciidoc"
[5]: http://docutils.sourceforge.net/rst.html	"reStructuredText"
[6]: https://miek.nl/2016/March/05/mmark-syntax-document/	"Mmark Syntax"
[7]: https://gohugo.io/getting-started/configuration/#blackfriday-extensions	"Blackfriday Markdown Extensions"
[8]: http://www.mathjax.org/	"MathJax"
[9]: http://docs.mathjax.org/en/latest/configuration.html	"MathJax Loading"
[10]: https://docs.mathjax.org/en/latest/start.html#putting-mathematics-in-a-web-page	"MathJax Syntax"
[11]: http://doswa.com/2011/07/20/mathjax-in-markdown.html	"Markdown and MathJax"