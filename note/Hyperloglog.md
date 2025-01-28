# [CMU15445-P0-Hyperloglog](https://15445.courses.cs.cmu.edu/fall2024/project0/))

[基本原理-Meta的博客]([HyperLogLog in Presto: Faster cardinality estimation - Engineering at Meta](https://engineering.fb.com/2018/12/13/data-infrastructure/hyperloglog/))

考虑哈希函数函数为 哈希为散列值

```
00
01
10
11
结尾有i个0的概率是1/2^i
```

$$
&\text{Cardinility}_{\text{HLL}}=\text{CONSTANT}\times \text{寄存器数量} \times 2^{\frac{\sum R_j}{m}}\\
&\Large\color{Red}以下是修正版\\
&\text{Cardinility}_{\text{HLL}}=\text{CONSTANT}\times \text{寄存器数量} \times \frac{\text{寄存器数量}}{\sum2^{-R_j}}\\
&R[j] 是寄存器 j 中的值\\
& N个数据 ,前k位确定寄存器索引,k=logN
$$

