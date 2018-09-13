---
title: "Hugo 内容文档介绍"
author: "Xiao Wenbin"
date: 2017-08-18T10:58:08-04:00
draft: false
tags: ["hugo"]
---

## 简介

内容文档是创作的核心，是静态网站的数据来源。Hugo 支持各种内容文档类型，并可以根据创作者对内容文档目录结构的组织，来生成相同结构的静态网站。

## 内容组织

内容文档都存放在 `content` 目录中，我们可以在该目录下嵌套任意层级的子目录来组织内容文档，并且 Hugo 假定我们对该目录的组织是有特定意图的，因此在生成网站时，Hugo 会按照该目录的结构来规划静态网站的目录结构，也即该目录下的文件和目录都被原样复制到静态网站相同的目录中（内容文档需要被转换为 HTML 文件）。下面将通过样例来说明，内容文档结构和静态网站结构之间的这种对应关系。

### 内容文档目录样例

假设内容文档目录的结构如下：

```
└── content
    ├─about
    │      _index.md
    │
    ├─post
    │  │  firstpost.md
    │  │  secondpost.md
    │  │
    │  └─happy
    │          ness.md
    │
    └─quote
            first.md
            second.md
```

运行生成静态网站的命令 `hugo` ，将得到如下静态网站目录结构：

```
└── public
    ├─about
    │     index.html
    ├─post
    │  │  index.html
    │  ├─firstpost
    │  │      index.html
    │  ├─happy
    │  │  └─ness
    │  │          index.html
    │  └─secondpost
    │          index.html
    └─quote
       │  index.html
       ├─first
       │      index.html
       └─second
               index.html
```

在 Windows 上可以通过命令 `tree /F` 来显示目录的树形结构，不过样例为了方便理解，删除了一些目录结构。

### 内容组织和静态网站结构相对应

Hugo 假定我们对内容的组织是有特定意图的，因此生成的静态网站结构跟内容文档的目录结构相对应：

* 内容目录 `content` 中的每个顶级子目录对应静态网站目录中一个同名的顶级子目录，这类子目录被叫作 section（下文会有介绍）。并且静态网站的顶级子目录中会额外增添一个 `index.html` 文件，该文件一般是用来对该 section 内容进行汇总展示的首页，比如展示一个列表页面。
* 每个内容文档文件最终对应静态网站一个子目录，且该子目录跟内容文档同名。比如：`content/post/happy/ness.md` 对应 `public/happy/ness` ，内容文档的内容则被转换到了该子目录中的 `index.html` 文件中。

### Section

Hugo 内容文档目录 `content` 中的顶级子目录被叫做 section。上面样例含有三个 section，分别是 about 和 post  以及 quote ，section 的含义就是将网站内容划分为几部分，可以看成网站内容的大类别。

Hugo 默认会为每个 section 自动创建一个页面用来展示该 section 的汇总内容，姑且将该页面称为 section 首页。同时 Hugo 支持自定义 section 首页的内容，只需要在 section 目录中创建文件 `_index.md` 并添加该页面的内容和文档头即可。此外我们还可以自定义 section 首页的外观，只需要创建模板文件 `/layouts/section/your-section-name.html` 即可，其中 `your-section-name` 跟 section 目录同名。

## 内容类型

网站往往会含有多种内容，比如：博客、照片墙以及摘录等，而这些内容应该关联不同的元数据（Front Matter 文档头）以及使用不同的方式去呈现（布局模板文件）。在 Hugo 中不同种类的内容是通过内容类型这个概念来区分。

### 文档的内容类型

Hugo 默认将内容目录 `content` 的第一级子目录（又叫 section ）与文档的内容类型关联了起来。即某个 section 目录中所有文档的内容类型名，默认都跟该 section 同名，也即文档所在的 section 目录决定了该文档的内容类型。可见对 `content` 目录第一级子目录的划分，等价于对内容类型进行划分。

此外 Hugo 也支持通过 Front Matter 文档头的 `type` 来为文档指定不同的内容类型。即只要文档头中出现了 `type` 配置项，该文档的内容类型就同所在的 `section` 目录无关。

总之，一个文档的内容类型，取决于文档头的 `type` 配置项或者文档所在的 section 目录。

### 内容类型和 section

虽然 Hugo 默认将 section 跟文档的内容类型关联了起来。不过，section 和文档的内容类型是两个独立的概念。内容文档属于哪个 section，完全取决于内容文档所在的目录位置，是无法通过 Front Matter 文档头配置的。而文档的内容类型则是可配置的，且内容类型是针对内容而言的，对于不同类型的内容，往往需要不同的文档头来描述它们、不同的布局去展示它们，这些都是跟内容类型相关联的。不过，通常人们喜欢把相同类型的内容放在同一个 section 目录下，因此 Hugo 默认将文档的内容类型和文档所在的 section 关联了起来。

### 自定义内容类型

当使用命令 `hugo new post/first-post.md` 创建文档时，其实就已经在使用自定义内容类型 `post` 了。虽然前面提到内容类型会关联元数据以及呈现方式，但这些并不是必须的，当没有为内容类型提供它们时，Hugo 将使用默认的元数据来创建文档，使用默认的布局模板来渲染文档。如果想要为内容类型提供特定的元数据和布局模板时，需要按照 Hugo 的约定来实现。

#### 内容类型的元数据

在 Hugo 中，将内容类型关联的元数据称为文档原型（ Archetype ），所谓文档原型可以理解为是文档创作的起点。文档原型的创建十分简单，只需要在 `archetypes` 目录中，创建一个 Markdown 文件，然后为该文档添加 Front Matter 文档头即可。该 Markdown 文件的命名就是内容类型名，其中添加的文档头就是为该内容类型关联的元数据。更多原型文档创建，参见[官网](https://gohugo.io/content-management/archetypes/)。

#### 内容类型的布局模板

文档进行渲染并生成静态网页，依赖布局模板文件。而且通常特定的内容类型会具有不同的呈现方式，要为内容类型指定专用的布局模板文件也十分简单，只需要在 `layouts` 目录中，创建一个跟内容类型同名的目录（称为布局目录），然后就可以在布局目录中创建各种用于渲染该内容类型的模板文件。

内容类型的渲染往往会用到多个模板文件（就是一些 HTML 文件），这些模板文件都存放在该内容类型的布局目录中。更多关于如何创建模板文件，参见[官网](https://gohugo.io/templates/)。

#### 为文档指定模板文件

文档可以通过文档头的 `layout` 来指定生成静态网站时，Hugo 将使用哪个模板文件来渲染自己，这里的 `layout` 跟模板文件同名（除去后缀名）。比如要使用模板文件 `layouts/post/bar.html` 来渲染文档，首先文档的内容类型需要是 `post` ，然后在文档头中使用 `layout = "bar"` （TOML 格式文档头）来指定该模板。

一般来说一种内容类型的大多数文档都会以相同的方式来展示，显然如果要在所有文档中添加 `layout` 文档头来指定渲染模板的话，是十分繁琐的。为此 Hugo 提供了内容类型默认模板来省去这个麻烦，也即如果没有使用文档头 `layout` 来指定渲染模板的话，默认会使用相应内容类型的布局目录中的 `single.html` 模板来渲染该文档。更多关于文档渲染模板的查找，参见[官网](https://gohugo.io/templates/single-page-templates/#single-page-template-lookup-order)。

## 内容 URL 地址

上面只是介绍了静态网站的目录结构和内容文档目录结构的关系。在静态网站最终发布到互联网时，其实相应的每个内容文档会最终被映射一个 URL 地址。这个映射过程可以完全由 Hugo 来自动执行，同时也支持手动配置。

### URL 构成和映射

```
permalink = baseURL + url
```

`permalink` 表示某个内容文档映射的完整 URL 地址，其中 `baseURL` 是配置文件中为静态网站指定的根路径地址，`url` 则表示为内容文档映射的相对 URL 地址。Hugo 为文档映射相对 URL 地址的规则根据文档种类不同而有差别，具体规则如下

* section 中的普通内容文档，映射规则为 `url = section + slug`。比如：`content/post/first-post.md` 映射的相对 URL 地址为 `/post/first-post/`，其中 `section` 和 `slug` 分别是 `post` 和 `first-post` 。这里 `slug` 用来表示内容文档的文件名（除去扩展名`.md`）。
* section 中嵌套在子目录中的内容文档，映射规则为 `url = path + slug`。比如：`content/post/happy/ness.md` 映射的相对 URL 地址为 `/post/happy/ness/`，其中 `path` 和 `slug` 分别为 `post/happy` 和 `ness` 。这里 `path` 用来代表内容文档的相对文件路径，即从内容文档到 `content` 目录之间的路径。
* section 中的首页内容文档 `_index.md`。映射规则为 `url = section`，比如 `content/about/_index.md` 映射的相对 URL 地址为 `/about/` 。其中 `section` 是 `about`。

### 配置 URL 映射

* 通过网站配置文件中的 `baseURL` 配置项来为静态网站指定根路径 URL 地址。
* 通过 Front Matter 文档头来配置 `slug` 值。 `slug` 用来指代内容文档名，默认跟内容文档同名（除去扩展名`.md`），如果在文档头部指定了 `slug`，Hugo 在映射 URL 时就会使用它而不是内容文档的文件名了。
* 通过 Front Matter 文档头来配置 `url` 值，来作为该文档最终映射到的相对 URL 地址。要注意 `url` 配置项的优先级比 `slug` 要高。
* 此外还可以通过更改内容文档的文件名以及内容文档所在 section 目录，进而来改变内容文档映射到的相对 URL 地址。不过严格来说这种方法不能算是配置了，而且优先级没有通过设置 `url` 和 `slug` 配置项高。




