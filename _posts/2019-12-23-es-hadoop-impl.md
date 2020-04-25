---
layout: post
title:  "ES-hadoop插件实现和使用"
categories: tech
tags: Elasticsearch hadoop
---



## 1. 介绍

在应用场景方面，hadoop是一个优秀的离线批量数据处理系统，能对数据进行复杂计算，而ES则是一个实时全文搜索系统。那么如何能够只存储一份数据，又能够同时拥有两者的能力呢？答案是es-hadoop插件。

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ge6e15zt5xj31900qhtci.jpg)

ES hadoop是一个ES对接hadoop生态的工具，它允许hadoop任务（比如MR、hive、pig、spark等）与ES交互，比如让hadoop以ES作为数据源进行计算、将计算的中间或结果数据存储到ES中等，这意味着它将能够整合ES和hadoop各自的优势。

另外，ES-hadoop还提供插件[Hadoop HDFS Repository Plugin](https://www.elastic.co/guide/en/elasticsearch/plugins/master/repository-hdfs.html)允许将ES数据备份到hdfs或从其恢复。

## 2. 实现

这部分将介绍ES-hadoop是如何将ES和hadoop的数据实体进行映射的。

在分布式系统中，扩展计算能力的一个关键因素是：并行，简单来说就是将一个任务划分成多个更小的任务，使他们并行的运行在集群的不同节点上，读取数据的不同部分进行计算。这个概念对应hadoop是splits、对应spark是partition、对应Elasticsearch则是shards。简单来讲，更多的splits、partition或shards意味着能有更多的任务同时读数据源的不同部分并进行计算，提高了计算能力。

既然并行如此重要，那么在hadoop中使用es-hadoop与ES进行数据交互时，它仍然应该能够并行的读写数据的不同部分，否则计算能力将大大降低。下面将介绍es-hadoop是如何做到这点的。

### 2.1 从ES读取数据

在spark、MR等系统中使用elasticsearch-hadoop从ES读取数据时，shard是一个关键的角色，因为elasticsearch-hadoop将为ES索引中的每个shard创建一个Hadoop InputSplit或Spark Partition。

大家看到这里可能会有疑问：es-hadoop是如何同时读取ES索引中不同shard数据的呢？

通过文章[Spark Core读取ES的分区问题分析](https://zhuanlan.zhihu.com/p/69906545)中的源码分析了解到，当es-hadoop从ES读取索引数据时，它获取索引各个shard的信息，包括：shard id、所在节点id等，并创建对应的Spark partition；除了按shard创建parition的方式外，es-hadoop还允许按shard最大doc数创建partition(配置项：[es.input.max.docs.per.partition](https://www.elastic.co/guide/en/elasticsearch/hadoop/current/configuration.html#_input))，这意味着es-hadoop能将shard切分成更小的数据集对应于partition，这对shard容量过大的情况将非常适用。 但说到这里，其实还是没能解答疑问：如何同时获取不同shard的数据呢？通过阅读[elasticsearch-hadoop](https://github.com/elastic/elasticsearch-hadoop)源码我找到了答案：

在文件mr/src/main/java/org/elasticsearch/hadoop/rest/RestService.java:389，createReader方法创建了PartitionReader，用于将读取ES shards生成partition。在方法中可以看到其构造了ES 查询请求：

```js
    public static PartitionReader createReader(Settings settings, PartitionDefinition partition, Log log) {
        // ...
        SearchRequestBuilder requestBuilder =
                new SearchRequestBuilder(clusterInfo.getMajorVersion(), includeVersion)
                        .resource(read)
                        // Overwrite the index name from the resource to be that of the concrete index in the partition definition
                        .indices(partition.getIndex())
                        .query(QueryUtils.parseQuery(settings))
                        .scroll(settings.getScrollKeepAlive())
                        .size(settings.getScrollSize())
                        .limit(settings.getScrollLimit())
                        .fields(SettingsUtils.determineSourceFields(settings))
                        .filters(QueryUtils.parseFilters(settings))
                        .shard(Integer.toString(partition.getShardId()))
                        .readMetadata(settings.getReadMetadata())
                        .local(true)
                        .preference(settings.getShardPreference())
                        .excludeSource(settings.getExcludeSource());
```

可以看到：

1. 请求使用[scroll API](https://www.elastic.co/guide/en/elasticsearch/reference/7.x/search-request-body.html#request-body-search-scroll)查询批量的数据，这是ES用来查询批量数据非常常见的用法，因为它能避免深分页问题。
2. 使用[preference](https://www.elastic.co/guide/en/elasticsearch/reference/master/search-request-preference.html)参数指定要查询的某个shard，每个partition都从指定shard获取数据，这样就能做到从多个shard并行的获取数据。除此之外，preference 还能从指定节点、主副shard、本地节点等条件查询数据。

### 2.2 向ES写入数据

和读取类似的，es-hadoop能够将hadoop的splits或spark partition数据对应成shard并行的写入ES。

## 3. 实践

这里以一个使用spark对es索引数据进行单词计数（wordcount）的使用示例，介绍es-hadoop中spark是如何操作es数据的。

demo代码位于：https://github.com/yyff/es-spark-wordcount

### 3.1 生成es-hadooop配置

```js
        SparkConf sparkConf = new SparkConf().setAppName("wordcount")
                .setMaster("local[*]").set("es.index.auto.create", "true")
                .set("es.nodes", esHost).set("es.port", esPort).set("es.nodes.wan.only", "true")
                .set("es.net.http.auth.user", "elastic")
                .set("es.net.http.auth.pass", password);
```

详细配置见：https://www.elastic.co/guide/en/elasticsearch/hadoop/current/mapreduce.html

### 3.2 使用spark native 读取ES数据

1、 调用JavaEsSpark.esRDD从索引查询中创建RDD

```js
        JavaSparkContext jsc = new JavaSparkContext(sparkConf);
        JavaRDD<Map<String, Object>> esRDD = JavaEsSpark.esRDD(jsc, indexName, query).values();
```

2、 统计词频

```js
        JavaPairRDD<String, Integer> counts = esRDD.flatMap(doc -> {
           String fieldValue = (String)doc.get(field);
           if (fieldValue == null) {
               fieldValue = "";
           }
           return Arrays.asList(fieldValue.split(" ")).iterator();
        }).mapToPair(word -> new Tuple2<String, Integer>(word, 1)).reduceByKey((x, y) -> x + y);
```

3、 结果输出到文件和ES

```js
        counts.saveAsTextFile(outDir);
        EsSpark.saveToEs(counts.rdd(), "spark-native");
```

4、 查看结果

- 文件：

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ge6e2eex5uj30oy04674k.jpg)

- es 索引

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ge6e2jvg1oj30ry0yy0ww.jpg)

### 3.3 使用spark sql 读取ES数据

1、 使用配置创建spark session

```js
        SparkSession sparkSession = SparkSession.builder().config(sparkConf).getOrCreate();
        JavaSparkContext jsc = new JavaSparkContext(sparkSession.sparkContext());//adapter
        SQLContext sql = new SQLContext(jsc);
```

2、 调用JavaEsSparkSQL.esDF从es sql查询中生成Dataset

```js
        Dataset<Row> ds = JavaEsSparkSQL.esDF(sql, indexName);
        ds = ds.select(ds.col(field));
        for (String cond : conditions) {
            ds = ds.where(cond);
        }
```

3、 统计词频

```js
        JavaPairRDD<String, Integer> counts = ds.javaRDD().flatMap(
                row -> {
                    String line = row.getAs(field);
                    if (line == null) {
                        line = "";
                    }
                    return Arrays.asList(line.split(" ")).iterator();
                }).mapToPair(word -> new Tuple2<String, Integer>(word, 1)).reduceByKey((x, y) -> x + y);
```

4、 结果输出到文件和ES，关闭session

```js
        counts.saveAsTextFile(outDir);
        EsSpark.saveToEs(counts.rdd(), "spark-sql");
        sparkSession.stop();
```

5、 查看结果

- 文件

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ge6e2uhw3vj30oc046q39.jpg)

- es 索引

![](https://tva1.sinaimg.cn/large/007S8ZIlgy1ge6e31zigbj30ti0pwn03.jpg)

## 4. 总结

通过es-hadoop，ES可以作为MR、Hive、Spark等的数据源，这意味着什么呢？意味着对于既需要使用Spark等工具进行批量分析和计算、又需要使用ES做实时搜索的数据，比如常见的业务日志，可以只存在于ES中，而无需重复存储于HDFS等存储中，极大的节省了存储成本。

在使用方面，通过ES-hadoop的实现可以看到，ES的shard和hadoop splits、spark partition有着对应关系，因此对要用于hadoop分析的索引设置合理的分片数变得十分重要，因为这将充分利用hadoop的并行计算能力。

## 5. 参考

- [Spark Core读取ES的分区问题分析](https://zhuanlan.zhihu.com/p/69906545)
- [ES-hadoop 官方文档](https://www.elastic.co/guide/en/elasticsearch/hadoop/current/reference.html)
- [ES scroll API](https://www.elastic.co/guide/en/elasticsearch/reference/7.x/search-request-body.html#request-body-search-scroll)
- [ES preference parameter](https://www.elastic.co/guide/en/elasticsearch/reference/master/search-request-body.html#request-body-search-preference)