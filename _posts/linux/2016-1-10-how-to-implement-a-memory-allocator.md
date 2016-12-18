---
layout: article
title:  "how to implement a memory allocator"
categories: linux
---

> USTC CSAPP LAB5

## 一、隐式链表——首次适配
具体请看CSAPP第九章

### 1.带脚标的隐式链表：

![](http://i.imgur.com/AfiEJGt.png)

> Prologue何Epilogue的作用： 让后面分配的块合并时无需检查边界条件

> heap_listp为可用内存起始处；mem_brk为结尾处，每次extend_heap都会后移

### 2.块内结构：

![](http://i.imgur.com/I4wb1oh.png)

> Payload为实际使用空间

### 3.空闲快合并时的情况处理

![](http://i.imgur.com/EZOz5mv.png)

### 4. 关键代码

1.堆初始化时对Prologue和Epilogue进行初始化

```c
if ((heap_listp = mem_sbrk(4*WSIZE)) == NULL)
  return -1;
PUT(heap_listp, 0);/* alignment padding */
PUT(heap_listp+WSIZE, PACK(OVERHEAD, 1));  /* prologue header */
PUT(heap_listp+DSIZE, PACK(OVERHEAD, 1));  /* prologue footer */
PUT(heap_listp+WSIZE+DSIZE, PACK(0, 1));   /* epilogue header */
heap_listp += DSIZE;
```

2.malloc时先对链表进行线性查找find_fit，时间复杂度O(N)

```c
for(bp = heap_listp; GET_SIZE(HDRP(bp)) != 0; bp = NEXT_BLKP(bp))
{
	if(!GET_ALLOC(HDRP(bp)) && size <= GET_SIZE(HDRP(bp)) )
		return bp;
}

```

3.没有空间则调用extend_heap扩充块链表，考虑合并

4.找到适合空间，使用place为块设置header和footer。但是需要考虑块分配后的空间是否满足最小快大小（HEADER+FOOTER+WSIZE=16）


5.释放块空间（free）时将只需将header和footer的标志位设为1，然后考虑合并（coalesce），如下：

```c
if(!GET_ALLOC(HDRP(next_bp)))
{
	size_t total_size=GET_SIZE(HDRP(bp))+GET_SIZE(HDRP(next_bp));
	PUT(HDRP(bp), PACK(total_size, 0));
	PUT(FTRP(next_bp), PACK(total_size, 0));
}
if(!GET_ALLOC(HDRP(pre_bp)))
{
	size_t total_size=GET_SIZE(HDRP(pre_bp))+GET_SIZE(HDRP(bp));
	PUT(HDRP(pre_bp), PACK(total_size, 0));
	PUT(FTRP(bp), PACK(total_size, 0));
	return pre_bp;
}
```


## 二、显示链表--BST

上面的首次适配的性能问题主要在于空闲块的查找上，所以采用BST来组织空闲块，

* 块内结构
块基本结构不变（包括header和footer），但是空闲块增加了三个指针域（left,right,parent）给查找，插入和删除操作时使用。因此最小块大小为(header+footer+12=20),满足对齐要求后为24


![](http://i.imgur.com/RMrvCSB.png)

BST代码如下(其中原来的first-fit被注释)


```c
/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * mail: yangfan036@gmail.com
 * author: c0ks
 ********************************************************/

/*********************************************************
 * 本内存分配器基于CSAPP上的分配器进行修改，使用二叉查找树进行
 * 空闲块的查找,时间复杂度为O(logN)，而不是书上的首次适应:O(N)。
 * 实现：使用显示链表，空闲结点增加了left,right,parent三个指针域，
 * 空闲结点最小长度为24。
 ********************************************************/



/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* $begin mallocmacros */
/* Basic constants and macros */
#define WSIZE       4       /* word size (bytes) */
#define DSIZE       8       /* doubleword size (bytes) */
//#define CHUNKSIZE  (1<<12)  /* initial heap size (bytes) */
#define OVERHEAD    8       /* overhead of header and footer (bytes) */
#define MIN_BLK_SIZE 16

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(size_t *)(p))
#define PUT(p, val)  (*(size_t *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

#define SET_HEAD_FREE(bp) (GET(HDRP(bp)) &= ~0x7)
#define SET_HEAD_ALLOC(bp) (GET(HDRP(bp)) |= 0x1)

#define SET_FOOT_FREE(bp) (GET(FTRP(bp)) &= ~0x7)
#define SET_FOOT_ALLOC(bp) (GET(FTRP(bp)) |= 0x1)
//#define GET_PRE_ALLOC(p)    (GET(p) & 0x2)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)



/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))
/* $end mallocmacros */

/* Given block ptr bp,set left , right  child and parent */
#define LEFT(bp)    ((char *)(bp))
#define RIGHT(bp)   ((char *)(bp) + WSIZE)
#define PARENT(bp)   ((char *)(bp) + DSIZE)
#define GET_LEFT(bp)    (*(void **)LEFT(bp))
#define GET_RIGHT(bp)    (*(void **)RIGHT(bp))
#define GET_PARENT(bp)    (*(void **)PARENT(bp))

#define SET_LEFT(bp,left)    (*(void **)LEFT(bp) = left)
#define SET_RIGHT(bp,right)  (*(void **)RIGHT(bp) = right)
#define SET_PARENT(bp,parent)  (*(void **)PARENT(bp) = parent)


/* The only global variable is a pointer to the first block */
static char *heap_listp;    //>=16 ,because at least 3 pointer(parent,left,right) costed by 12 bytes
//static char *small_listp; //<16
static void *root = NULL;

static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void *insert_node(void *bp);
static void delete_node(void *bp);


static void printblock(void *bp);
static void checkblock(void *bp);

size_t CHUNKSIZE;

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    CHUNKSIZE = mem_pagesize();
    /* create the initial empty heap */
    if ((heap_listp = mem_sbrk(4*WSIZE)) == NULL)
        return -1;
    PUT(heap_listp, 0);                        /* alignment padding */
    PUT(heap_listp+WSIZE, PACK(OVERHEAD, 1));  /* prologue header */
    PUT(heap_listp+DSIZE, PACK(OVERHEAD, 1));  /* prologue footer */
    PUT(heap_listp+WSIZE+DSIZE, PACK(0, 3));   /* epilogue header */
    heap_listp += DSIZE;

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE) == NULL)
        return -1;
    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{

    size_t newsize,extend_size;
    char *bp;
    /* Ignore spurious requests */
    if (size <= 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= 2 * DSIZE) //at least 16 bytes
        newsize = 2 * DSIZE + OVERHEAD;
    else
        newsize = DSIZE * ((size + (OVERHEAD) + (DSIZE-1)) / DSIZE); //size向上取DSIZE的整数倍

    if((bp = find_fit(newsize)) != NULL)
    {
        place(bp,newsize);
        return bp;
    }

    extend_size = MAX(newsize, CHUNKSIZE);
    if((bp = extend_heap(extend_size)) != NULL)
    {
        if((bp = find_fit(newsize)) != NULL);
            place(bp,newsize);
        return bp;
    }
    return NULL;
}


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    PUT(HDRP(ptr), PACK(GET_SIZE(HDRP(ptr)), 0));
    PUT(FTRP(ptr), PACK(GET_SIZE(FTRP(ptr)), 0));
    insert_node(coalesce(ptr));
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;

    newptr = mm_malloc(size);
    if (newptr == NULL)
        return NULL;
    //copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    copySize = GET_SIZE(HDRP(oldptr)) - OVERHEAD;
    if (size < copySize)
        copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}

static void *find_fit(size_t size)
{
    // void *bp;
    // for(bp = heap_listp; GET_SIZE(HDRP(bp)) != 0; bp = NEXT_BLKP(bp))
    // {
    //     if(!GET_ALLOC(HDRP(bp)) && size <= GET_SIZE(HDRP(bp)) )
    //         return bp;
    // }
    // return NULL;
    void *temp = root;
    void *fit = NULL;
    while(temp != NULL)
    {
        if(size <= GET_SIZE(HDRP(temp)))
        {
            fit = temp;
            temp = GET_LEFT(temp);
        }
        else
            temp = GET_RIGHT(temp);
    }
    return fit;
}

static void place(void *bp,size_t size)
{
    delete_node(bp);
    size_t block_size=GET_SIZE(HDRP(bp));
    if(block_size - size < 24) //freelist node minsize
    {
        PUT((HDRP(bp)), PACK(block_size, 1));
        PUT((FTRP(bp)), PACK(block_size, 1));
        return ;
    }
    PUT((HDRP(bp)), PACK(size, 1));
    PUT((FTRP(bp)), PACK(size, 1));
    PUT((HDRP(NEXT_BLKP(bp))), PACK(block_size - size, 0));
    PUT((FTRP(NEXT_BLKP(bp))), PACK(block_size - size, 0));

    insert_node(coalesce(NEXT_BLKP(bp)));

}

static void *extend_heap(size_t size)
{
    // char *bp;
    // if((int)(bp = mem_sbrk(size)) == -1)
    //     return NULL;
    // PUT((HDRP(bp)), PACK(size, 0));
    // PUT((FTRP(bp)), PACK(size, 0));
    // PUT((HDRP(NEXT_BLKP(bp))), PACK(0, 1));
    // return coalesce(bp);
    char *bp;
    if((int)(bp = mem_sbrk(size)) == -1)
        return NULL;
    PUT((HDRP(bp)), PACK(size, 0));
    PUT((FTRP(bp)), PACK(size, 0));
    PUT((HDRP(NEXT_BLKP(bp))), PACK(0, 1));
    return insert_node(coalesce(bp));

}


static void *coalesce(void *bp)
{
    // void *pre_bp=(void *)PREV_BLKP(bp);
    // void *next_bp=(void *)NEXT_BLKP(bp);
    // if(!GET_ALLOC(HDRP(next_bp)))
    // {
    //     PUT((HDRP(bp)), PACK(GET_SIZE(HDRP(bp))+GET_SIZE(HDRP(next_bp)), 0));
    //     PUT((FTRP(bp)), PACK(GET_SIZE(HDRP(bp))+GET_SIZE(HDRP(next_bp)), 0));
    // }
    // if(!GET_ALLOC(HDRP(pre_bp)))
    // {

    //     PUT((HDRP(pre_bp)), PACK(GET_SIZE(HDRP(pre_bp))+GET_SIZE(HDRP(bp)), 0));
    //     PUT((FTRP(pre_bp)), PACK(GET_SIZE(HDRP(pre_bp))+GET_SIZE(HDRP(bp)), 0));
    //     return pre_bp;
    // }
    // return bp;
    void *pre_bp=(void *)PREV_BLKP(bp);
    void *next_bp=(void *)NEXT_BLKP(bp);
    if(!GET_ALLOC(HDRP(next_bp)))
    {
        size_t total_size=GET_SIZE(HDRP(bp))+GET_SIZE(HDRP(next_bp));
        PUT(HDRP(bp), PACK(total_size, 0));
        PUT(FTRP(next_bp), PACK(total_size, 0));
        delete_node(next_bp);
    }
    if(!GET_ALLOC(HDRP(pre_bp)))
    {

        size_t total_size=GET_SIZE(HDRP(pre_bp))+GET_SIZE(HDRP(bp));
        PUT(HDRP(pre_bp), PACK(total_size, 0));
        PUT(FTRP(bp), PACK(total_size, 0));
        delete_node(pre_bp);
        return pre_bp;
    }
    return bp;
}

static void *insert_node(void *bp)
{
    SET_HEAD_FREE(bp);
    SET_FOOT_FREE(bp);
    if(root == NULL)
    {
        root = bp;
        SET_LEFT(bp, NULL);
        SET_RIGHT(bp, NULL);
        SET_PARENT(bp,NULL);
        return bp;
    }
    void *temp = root;
    void *prev = temp;
    size_t size = GET_SIZE(HDRP(bp));

    while(temp != NULL) //look up the node with fit size
    {
        if(size <= GET_SIZE(HDRP(temp)))
        {
            prev = temp;
            temp = GET_LEFT(temp);
        }
        else if(size > GET_SIZE(HDRP(temp)))
        {
            prev = temp;
            temp = GET_RIGHT(temp);
        }

    }
    if(size <= GET_SIZE(HDRP(prev)))
        SET_LEFT(prev, bp);
    else
        SET_RIGHT(prev, bp);

    SET_LEFT(bp,NULL);
    SET_RIGHT(bp,NULL);
    SET_PARENT(bp,prev);
    return bp;
}

static void delete_node(void *bp)
{
    SET_HEAD_ALLOC(bp);
    SET_FOOT_ALLOC(bp);
    void *parent = GET_PARENT(bp);
    void *right = GET_RIGHT(bp);
    void *left = GET_LEFT(bp);
    if(bp == root)
    {
        if(right == NULL)
            root = left;
        else
        {
            void *temp = right;
            while(GET_LEFT(temp) != NULL) //look up the new root
                temp = GET_LEFT(temp);
            if(temp == right)  //right child of bp has not left child
            {
                if(left == NULL)
                {
                    root = right;
                    SET_PARENT(right,NULL);
                    return;
                }
                else
                {
                    root=left;
                    SET_LEFT(right, GET_RIGHT(left));

                    SET_RIGHT(left, right);
                    SET_PARENT(left,NULL);
                    return ;
                }
            }
            void *temp_parent = GET_PARENT(temp);
            void *temp_right = GET_RIGHT(temp);
            SET_LEFT(temp_parent,temp_right); //right node of temp will replace temp
            if(temp_right != NULL)
                SET_PARENT(temp_right,temp_parent);

            root = temp;
            SET_LEFT(root,left);
            SET_RIGHT(root,right);
            SET_PARENT(root,NULL);
        }
    }
    else
    {
        if(right == NULL)
        {
            if(bp == GET_LEFT(parent))
                SET_LEFT(parent,left);
            else
                SET_RIGHT(parent,left);
            if(left != NULL)
                SET_PARENT(left,parent);

        }
        else
        {
            void *temp = right;
            while(GET_LEFT(temp) != NULL) //look up the root
                temp = GET_LEFT(temp);
            void *temp_parent = GET_PARENT(temp);
            void *temp_right = GET_RIGHT(temp);
            SET_LEFT(temp_parent,temp_right); //right node of temp will replace temp
            if(temp_right != NULL)
                SET_PARENT(temp_right,temp_parent);
            if(bp == GET_LEFT(parent))
                SET_LEFT(parent,temp);
            else
                SET_RIGHT(parent,temp);

            //set new node that replace bp
            SET_LEFT(temp,left);
            SET_RIGHT(temp,right);
            SET_PARENT(temp,parent);
        }
    }


}


```






