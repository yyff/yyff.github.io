---
layout: post
title:  "在ES API中运行脚本？ES 脚本介绍"
categories: tech
tags: Elasticsearch
---



## 1. 介绍

如何在查询时转换字段的值？如何对文档执行复杂的更新操作？如何在ingest processor中指定执行条件？

针对上述等一系列问题，都可以通过ES脚本来解决，其允许用户在一些特定的API中对自定义表达式进行求值。

先来看一个例子：索引存在一个字段`DiskUsage`表示磁盘的使用量，单位是MB，现在希望查询结果展现以GB为单位的值。

这种情况可以使用[Script Fields](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields)功能：使用脚本对`DiskUsage`字段的值进行转换，转换后的值通过新字段`DiskUsageInGB`返回。

```
GET metrics/_search
{
  "query": {
    "match_all": {}
  },
  "script_fields": {
    "DiskUsageInGB": {
      "script": {
        "lang": "painless",
        "source": "doc['DiskUsage'].value / 1024"
      }
    }
  }
}
```



## 2. ES API使用脚本格式

在任意ES API中使用脚本都遵循以下格式

```
	"script": {
    "lang":   "...",  
    "source" | "id": "...", 
    "params": { ... } 
  }
```

- lang: 指定脚本语言， 默认是 `painless`.
- source, id: 指定脚本代码 或 使用存储的脚本id（见 [Stored script](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/modules-scripting-using.html#modules-scripting-stored-scripts)）
- params: 指定参数作为脚本执行的变量



## 3. 脚本语言

ES脚本语言包括通用目的语言和特定目的语言两种类型，通用目的语言能在任何允许脚本的API中使用，而特定目的语言则只能在特定API中使用（拥有更高的性能）。

通用目的语言

在6.0之前的版本，ES通用目的语言内置支持painless、groovy以及允许通过安装lang插件的方式支持python和javascript。但在6.0之后的版本则仅支持 Painless，原因可能在于painless专门为ES设计，相比其他通用语言拥有更高的安全性。

| Language                                                     | Sandboxed                                                    | Required plugin                                              |
| :----------------------------------------------------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| [`painless`](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) | yes                                                          | built-in                                                     |
| [`groovy`](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) | [no](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) | built-in                                                     |
| [`javascript`](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) | [no](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) | [`lang-javascript`](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) |
| [`python`](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) | [no](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) | [`lang-python`](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) |


> Sandboxed：沙箱语言在设计时考虑了安全因素（比如限制了可以调用的方法），能被用户或应用安全的使用；非沙盒脚本具有对ES内部未经检查的访问权限，滥用可能会导致各种问题，见 [Scripting and security](https://www.elastic.co/guide/en/elasticsearch/reference/5.6/modules-scripting-security.html#modules-scripting-security)

特定目的语言

| Language                                                     | Sandboxed | Required plugin | Purpose                         |
| :----------------------------------------------------------- | :-------- | :-------------- | :------------------------------ |
| [`expression`](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) | yes       | built-in        | fast custom ranking and sorting |
| [`mustache`](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html#search-request-script-fields) | yes       | built-in        | templates                       |



## 4. 在脚本中访问字段和特殊变量

通常情况下，在API中使用脚本时会需要访问文档中的一些字段或特殊的变量。而在不同类型ES API中使用脚本，其访问方式也有所不同。

### 4.1 更新脚本

在 [update](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/docs-update.html), [update-by-query](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/docs-update-by-query.html), 或 [reindex](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/docs-reindex.html) API中使用脚本时，需要通过`ctx`去访问文档中的字段。

- `ctx._source`： 访问文档`_source`字段
- `ctx.op`: 对文档对应的操作，包括`index`和`delete`
- `ctx._index` : 访问文档的 [meta-fields](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/mapping-fields.html)
- 其他字段或变量的访问见： [update context](https://www.elastic.co/guide/en/elasticsearch/painless/7.2/painless-update-context.html)

### 4.2 查询和聚合脚本

除了每次搜索命中执行一次的脚本字段（[script fields](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/search-request-script-fields.html)）之外，搜索和聚合中使用的脚本将针对可能与查询或聚合匹配的每个文档执行一次，这种情况下需要脚本能执行的非常快！

在此种API的脚本中，字段值能从[doc-values](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/modules-scripting-fields.html#modules-scripting-doc-vals),  [stored fields 或`_source` 字段](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/modules-scripting-fields.html#modules-scripting-stored)访问。

>  \_source访问字段值比doc-values慢得多，原因在于：它是针对每个结果返回多个字段进行了优化，而doc-values则针对访问许多文档中特定字段的值进行了优化。

- doc-values访问字段值：`doc['FIELD_NAME']` 或`doc['FIELD_NAME'].value`: 
- `_source`访问字段值：`_source.field_name`
- 访问[stored fields](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/mapping-store.html)：`_fields['field_name'].value` 或 `_fields['field_name']`

更多API中使用脚本访问字段或特殊变量见：[Painless contexts](https://www.elastic.co/guide/en/elasticsearch/painless/7.2/painless-contexts.html)

## 5. painless

painless作为ES中默认且最常用的脚本语言 ，这里对其做简单的介绍。

### 5.1 painless语法

painless语法中除了作为Java语法子集的部分外，但其附加了一些其他特性，如动态类型，Map和List访问器快捷方式等。painless脚本使用[ANTLR4](http://www.antlr.org/) 和 [ASM](http://asm.ow2.org/) 库解析和编译，编译生成的字节码直接在JVM中运行。

详细语法见：  [Painless Language Specification](https://www.elastic.co/guide/en/elasticsearch/painless/7.2/painless-lang-spec.html)

### 5.2 Painless上下文

一个Painless脚本在一个上下文中被求值。每个上下文对应于一个或多个能使用脚本的ES API，都有相应的**可用作局部变量的值**，详见：[Painless contexts](https://www.elastic.co/guide/en/elasticsearch/painless/7.2/painless-contexts.html)

### 5.3 Painless API

Painless对每个上下文的方法和类都有严格的白名单，以确保所有Painless脚本都是安全的。大部分方法都是可以共享使用的，但有少部分只能在特定的ES API中使用，详见：[Painless API Reference](https://www.elastic.co/guide/en/elasticsearch/painless/7.2/painless-api-reference.html)

## 6. 更多例子

1、reindex 时更新字段值

```
POST _reindex
{
  "source": {
    "index": "blogs"
  },
  "dest": {
    "index": "blogs_new"
  },
  "script": {
    "source": """
ctx._source.number_of_views += 10;
ctx._source.reindexBatch = 1;
"""
  }
}
```

2、在update by query API中使用脚本：对row<=3且sold为false的，将cost字段值减2

```
POST /seats/_update_by_query
{
    "query": {
        "bool": {
            "filter": [
            {
                "range": {
                    "row": {
                        "lte": 3
                    }
                }
            },
            {
                "match": {
                    "sold": false
                }
            }]
        }
    },
    "script": {
        "source": "ctx._source.cost -= params.discount",
        "lang": "painless",
        "params": {
            "discount": 2
        }
    }
}
```

3、在ingest pipeline中使用[script processor](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/script-processor.html)：如果字段`locales`值为空，则其设置成"en-en"

```
PUT _ingest/pipeline/fix_locales
{
  "processors": [
    {
      "script": {
        "source": """
  if (ctx.locales == "") {
    ctx.locales = "en-en";
  }
"""
      }
    }
  ]
}
```

> 值得注意的是，在 [ingest processor上下文](https://www.elastic.co/guide/en/elasticsearch/painless/7.2/painless-ingest-processor-context.html) 中，通过ctx变量访问文档字段是不需要加`_source`的。

关于更多例子，读者可以参考各个Painless contexts中的例子。

## 7. 参考：

[Elasticsearch Reference 7.2 » Scripting： [https://www.elastic.co/guide/en/elasticsearch/reference/7.2/modules-scripting.html](https://www.elastic.co/guide/en/elasticsearch/reference/7.2/modules-scripting.html)

Painless contexts： [https://www.elastic.co/guide/en/elasticsearch/painless/7.2/painless-contexts.html](https://www.elastic.co/guide/en/elasticsearch/painless/7.2/painless-contexts.html)