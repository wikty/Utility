---
title: "Hugo 静态页面"
author: "Xiao Wenbin"
date: 2017-08-17T10:58:08-04:00
draft: true
tags: ["hugo"]
---

### 自定义404页面

使用 Hugo 创建404页面十分简单，创建模板文件 `/layouts/404.html` ，Hugo 生成网站时在网站根目录就会有一个 `404.html` 文件，至于如何在用户访问错误时展示404页面则取决于网站部署在了哪里。

404模板示例

```html
{{ partial "header.html" . }}
{{ partial "subheader.html" . }}

<section id="main">
  <div>
   <h1 id="title">{{ .Title }}</h1>
  </div>
</section>

{{ partial "footer.html" . }}
```

网站部署在 GitHub 时，用户访问错误会自动重定向到404页面。如果网站是部署到自己服务器上的，则需要配置服务器在访问错误时加载404页面。对于 Apache 服务器，需要在网站根目录创建 `.htaccess` 并写入 `ErrorDocument 404 /404.html` 。对于 Nginx 服务器，需要在服务器配置文件 `nginx.conf` 中写入 `error_page 404 = /404.html;` 。