
 * SkipList的实现代码以及测试代码
 * SkipList的特点：
   * 对数时间(logN)操作，性能媲美平衡二叉树
   * 采用索引加速查找，索引数量随机，能够快速跳过不必要的查找
   * 实现简单，实现复杂度远小于平衡树；没有类似平衡树的大量旋转
 * 附带一张结构图(/structure daigram of skiplist.jpg)