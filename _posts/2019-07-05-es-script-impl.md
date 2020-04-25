---
layout: post
title:  "ES 脚本实现"
categories: tech
tags: Elasticsearch
---



在我的上篇文章"在ES API中运行脚本？ES 脚本介绍"中，介绍了ES 脚本的基本概念和使用，而本文将基于ES 6.4版本对其内部实现做一个分析。

在分析前，我们先来看一个在ES [update API](https://www.elastic.co/guide/en/elasticsearch/reference/current/docs-update.html)中使用脚本文档进行更新的例子：

```
POST test/_update/1
{
    "script" : {
        "source": "ctx._source.counter += params.count",
        "lang": "painless",
        "params" : {
            "count" : 4
        }
    }
}
```

此API中定义了一个脚本：

- 指定painless作为脚本语言
- 指定参数count，值为4
- 指定脚本源码："ctx.\_source.counter += params.count", 使文档字段`counter`的值加上参数count的值

接下来，我们从update API的入口出发，来看这个脚本最终是如何被执行的。

## 1. 调用栈分析

每个Rest接口都有uri和相应的RestAction，在构造函数中被注册，对更新接口来说，其uri格式和RestAction分别是：`/{index}/{type}/{id}/_update` 和`RestUpdateAction`

![image-]({{ site.url }}/images/tech/es_script/1.png)

请求在`RestUpdateAction`的prepareRequest方法中被简单处理后，经过`NodeClient`转发，最终在`TransportUpdateAction`类中执行实际操作。

> 大部分RestAction只负责转发，实际则由相应的TransportAction处理

由于更新是单(single)操作，在`TransportUpdateAction` 将通过`TransportInstanceSingleOperationAction`：查找请求中的文档id对应shard，再转发给拥有相应shard的node去执行，见`TransportInstanceSingleOperationAction:170`行：

![image-]({{ site.url }}/images/tech/es_script/2.png)

转发的请求最终进入`TransportUpdateAction`类的`shardOperation`方法。

在shardOperation方法中，执行的操作为：

1. 调用updateHelper.prepare 获取要更新的文档结果
2. 执行更新操作

![image-]({{ site.url }}/images/tech/es_script/3.png)

而在第一步调用`updateHelper.prepare`时，会根据更新API的请求内容，构造更新后的文档内容：

![image-]({{ site.url }}/images/tech/es_script/4.png)

对于脚本请求，则调用`prepareUpdateScriptRequest`方法处理文档。

prepareUpdateScriptRequest方法

- 构造更新脚本上下文（[update Context](https://www.elastic.co/guide/en/elasticsearch/painless/6.4/painless-update-context.html)）：设置可以在脚本中访问的内置字段、变量
- 执行脚本获取执行结果
- 使用结果构造更新后的文档

![image-]({{ site.url }}/images/tech/es_script/5.png)

executeScript方法：

- 调用 scriptService.compile(script, ExecutableScript.UPDATE\_CONTEXT)： 编译可执行脚本
- 调用 executableScript.run()： 运行脚本

![image-]({{ site.url }}/images/tech/es_script/6.png)

更新脚本属于其中一种可执行脚本ExecutableScript，其它的还包括：

![image-]({{ site.url }}/images/tech/es_script/7.png)

## 2. 脚本执行框架

在单独分析更新脚本的执行后，这里我们再来剖析ES的脚本框架。

### 2.1 脚本interface

对于在不同类型API中执行的脚本，其执行方式也有所不同（包括返回值类型、参数等不同），ES为其都声明了相应的interface。比如上述的Update API中的脚本对应于ExecutableScript  interface。

其他interface还包括：FilterScript（用于query filter）、IngestScript（用于Ingest Script Processor）、SearchScript（用于搜索、聚合等请求中的请求）等

### 

### 2.2 ScriptService

在上述executeScript方法中可以看到，更新接口中的脚本是通过scriptService.compile方法编译的。实际上，所有API中的脚本、所有的脚本语言，也都是通过此方法编译的。其内部逻辑主要包括：

- 获取脚本语言对应的脚本执行引擎ScriptEngine
- 查询缓存的脚本。若不存在，则使用ScriptEngine重新编译脚本

### 2.3 ScriptEngine

ScriptService类内部封装了用于执行不同脚本语言的执行引擎ScriptEngine，实现类包括`PainlessScriptEngine`、`ExpressionScriptEngine`和`MustacheScriptEngine`。

![image-]({{ site.url }}/images/tech/es_script/8.png)

### 2.4 PainlessScriptEngine

这里以Painless脚本语言为例进行分析。在ScriptService中会调用相应语言ScriptEngine的compile方法，以下为PainlessScriptEngine的compile方法实现：

![image-]({{ site.url }}/images/tech/es_script/9.png)

对于可执行脚本，其执行逻辑为：

- 编译painless脚本代码：使用[ANTLR4](http://www.antlr.org/) 和 [ASM](http://asm.ow2.org/) 库，生成GenericElasticsearchScript对象painlessScript
    - 调用GenericElasticsearchScript的execute方法将执行执行脚本，产生输出结果
- 使用可执行的脚本对象painlessScript构造ScriptImpl类（实现了SearchScript接口）对象并返回

> 关于painless脚本代码是如果使用[ANTLR4](http://www.antlr.org/) 和 [ASM](http://asm.ow2.org/) 库解析和编译的，读者可自行阅读代码:P