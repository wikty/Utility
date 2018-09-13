---
title: "Hugo 文档处理"
author: "Xiao Wenbin"
date: 2017-08-15T10:58:08-04:00
draft: true
tags: ["hugo"]
---

### 文档排序

当在列表页面展示多篇文档时，就涉及到文档先后顺序的问题了。Hugo 中文档默认是以元信息 `weight` 来排序，当文档未指定 `weight` 时，就以元信息 `date` 来排序，如果这两项都没有指定的话，列表页面看到的文档就是无序的。

不过除了上面 `weight` 和 `date` 外，Hugo 还支持我们以更多方式来排序列表页面，我们需要在列表模板文件中使用以下一些模板变量来控制文档的排序

- 按照元信息权重和日期排序（默认排序方式）

  ```
  {{ range .Data.Pages }}
  <li>
  <a href="{{ .Permalink }}">{{ .Title }}</a>
  <div class="meta">{{ .Date.Format "Mon, Jan 2, 2006" }}</div>
  </li>
  {{ end }}
  ```

- 按照元信息日期排序

  ```
  {{ range .Data.Pages.ByDate }}
    <!-- ... -->
  {{ end }}
  ```

- 按照发布日期排序

  ```
  {{ range .Data.Pages.ByPublishDate }}
    <!-- ... -->
  {{ end }}
  ```

- 按照失效日期排序

  ```
  {{ range .Data.Pages.ByExpiryDate }}
    <!-- ... -->
  {{ end }}
  ```

- 按照修改日期排序

  ```
  {{ range .Data.Pages.ByLastmod }}
    <!-- ... -->
  {{ end }}
  ```

- 按照文档内容长度排序

  ```
  {{ range .Data.Pages.ByLength }}
    <!-- ... -->
  {{ end }}
  ```

- 按照文档标题排序

  ```
  {{ range .Data.Pages.ByTitle }}
    <!-- ... -->
  {{ end }}
  ```

- 按照链接标题排序

  ```
  {{ range .Data.Pages.ByLinkTitle }}
    <!-- ... -->
  {{ end }}
  ```

- 按照其它元信息排序

  ```
  {{ range (.Date.Pages.ByParam "author.last_name") }}
    <!-- ... -->
  {{ end }}
  ```

- 反转排序（以上所有排序都可反转）

  ```
  {{ range .Data.Pages.ByTitle.Reverse }}
    <!-- ... -->
  {{ end }}
  ```

除此之外，文档还可以按照分类进行排序，而分类标签本身可以按照标签字母序来排序

```
<ul>
{{ $data := .Data }}
{{ range $key, $value := .Data.Taxonomy.Alphabetical }}
<li><a href="{{ .Site.LanguagePrefix }}/{{ $data.Plural }}/{{ $value.Name | urlize }}"> {{ $value.Name }} </a> {{ $value.Count }} </li>
{{ end }}
</ul>
```

或者按照关联到该分类标签的文档数量排序（即按照分类的热门程度排序）

```
<ul>
{{ $data := .Data }}
{{ range $key, $value := .Data.Taxonomy.ByCount }}
<li><a href="{{ .Site.LanguagePrefix }}/{{ $data.Plural }}/{{ $value.Name | urlize }}"> {{ $value.Name }} </a> {{ $value.Count }} </li>
{{ end }}
</ul>
```

属于某个分类的文档默认按照 `weight` 和 `date` 来排序，并且支持为文档指定分类排序时的权重，这样可以调整文档在分类中的顺序，这个功能通过文档中指定元数据 `taxonomyname_weight` 来实现，其中 `taxonomyname` 代表分类名。



### 文档分组

当在列表页面展示多篇文档时，Hugo 支持我们根据文档类型、日期或者 Section 来分组显示文档。

- 按照 Section 分组

  ```
  {{ range .Data.Pages.GroupBy "Section" }}
  <h3>{{ .Key }}</h3>
  <ul>
      {{ range .Pages }}
      <li>
      <a href="{{ .Permalink }}">{{ .Title }}</a>
      <div class="meta">{{ .Date.Format "Mon, Jan 2, 2006" }}</div>
      </li>
      {{ end }}
  </ul>
  {{ end }}
  ```

- 按照日期分组

  ```
  {{ range .Data.Pages.GroupByDate "2006-01" }}
    <!-- ... -->
  {{ end }}
  ```

- 按照发布日期分组

  ```
  {{ range .Data.Pages.GroupByPublishDate "2006-01" }}
    <!-- ... -->
  {{ end }}
  ```

- 按照其它元信息分组

  ```
  {{ range .Data.Pages.GroupByParam "param_key" }}
    <!-- ... -->
  {{ end }}
  ```

- 反转分组排序

  ```
  {{ range (.Data.Pages.GroupByDate "2006-01").Reverse }}
    <!-- 利用模板函数Reverse来反转 -->
  {{ end }}

  {{ range .Data.Pages.GroupByDate "2006-01" "desc" }}
    <!-- 或者直接指定排序方向 -->
  {{ end }}
  ```

- 组内文档排序

  ```
  {{ range .Data.Pages.GroupByDate "2006-01" "asc" }}
  <h3>{{ .Key }}</h3>
  <ul>
      {{ range .Pages.ByTitle }}
      <!-- 可以按照之前介绍排序文档的各种方法来排序组内文档 -->
      {{ end }}
  </ul>
  {{ end }}
  ```

### 文档过滤

有时候也许想要排除某些文档在列表页面显示，Hugo 支持我们在列表页面限制文档显示数量以及限制显示的文档种类。

- 限制文档显示数量

  ```
  {{ range first 10 .Data.Pages }}
      <!-- 利用模板函数first，只显示排在前面的10篇文档 -->
      {{ .Render "summary" }}
  {{ end }}
  ```

- 根据条件过滤某些文档

  ```
  {{ range where .Data.Pages "Section" "post" }}
     <!-- 利用模板函数where，只筛选显示Section为post的文档 -->
     {{ .Content }}
  {{ end }}

  {{ range first 5 (where .Data.Pages "Section" "post") }}
     <!-- 同时使用where和first -->
     {{ .Content }}
  {{ end }}
  ```

### 文档摘要

Hugo 默认会截取文档前70个词作为文档摘要，并将摘要内容存放在模板页面变量 `.Summary` ，同时提供模板变量 `.Truncated` 来记录截取的摘要是否包含了文档的全部内容。同时 Hugo 还支持我们在内容文档中明确指定将哪些内容作为该文档的摘要，具体来说需要在文档中插入一行 `<!--more-->` 来标识位于该行之前的内容作为摘要，同理 Hugo 会将摘要存放在模板页面变量 `.Summary` ，并用模板变量 `.Truncated` 标识摘要是否包含了文档全部内容。

利用文档的摘要功能可以实现“阅读更多...”这样的功能，示例如下

```
{{ range first 10 .Data.Pages }}
  <div class="summary">
    <h4><a href="{{ .RelPermalink }}">{{ .Title }}</a></h4>
    {{ .Summary }}
  </div>
  {{ if .Truncated }}
  <div class="read-more-link">
    <a href="{{ .RelPermalink }}">Read More…</a>
  </div>
  {{ end }}
{{ end }}
```

