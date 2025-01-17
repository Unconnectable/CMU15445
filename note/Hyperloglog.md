对于一个hash后的2^k条目的序列(字符串)
```
3位的条目
000
001
010
011
100
101
110
111
```

$$
\begin{align}
&\color{Red}至少i个0结尾的概率是1/2^i \\
& 在这2^n序列中,每个是长度是k的01串\\\\
& 归纳证明:至少有n个0结尾的串概率是1/2^n \\
& 至少有n-1的0结尾的串有两个,也就是2^1,概率是2/2^n=1/2^{n-1} \\
& 至少有n-2结尾的串有 2^2个,也就是1/2^{n-2}\\
& 同理可得 至少有k个0结尾的串有2^{n-k}个,概率是1/2^{k} \\
& 
\end{align}
$$

[Meta的HyerLogLog解释](https://engineering.fb.com/2018/12/13/data-infrastructure/hyperloglog/)
[HyperLogLog的解释](https://www.cnblogs.com/linguanh/p/10460421.html)
[C++实现HyperLogLog](https://github.com/arashbm/HyperLogLogs)


```cpp
- APPROX_SET(x) -> HYPERLOGLOG
    该函数将返回输入数据集 x 的 HLL（HyperLogLog）草图。
- MERGE(hlls) -> HYPERLOGLOG
    该函数对一组 HLL 草图进行集合并操作。
- CARDINALITY(hll) -> BIGINT
    该函数将对 HLL 草图应用去重计数公式，并返回最终的唯一值数量。
- CAST(hll AS VARBINARY) -> VARBINARY
    为了将 HLL 草图存储到 Hive 表中，必须将其转换为 VARBINARY 类型。
- CAST(varbinary AS HYPERLOGLOG) -> HYPERLOGLOG
    在从表中读取存储的 HLL 草图时，必须将其转换为 HLL 类型，以便进一步合并和基数估计。
- CAST(hll AS P4HYPERLOGLOG) -> P4HYPERLOGLOG
    为了强制 HLL 草图使用密集布局，可以将其转换为 P4HYPERLOGLOG 类型。
- EMPTY_APPROX_SET() -> HYPERLOGLOG
    该函数返回一个空的 HLL 草图。它可以用于处理本应返回 HYPERLOGLOG 类型数据但返回 NULL 的函数。例如，APPROX_SET(NULL) 会返回 NULL 而不是空的 HYPERLOGLOG 数据结构，这可能会导致其他错误。

以下是解释
1. APPROX_SET(x) -> HYPERLOGLOG
功能：生成输入数据集 x 的 HyperLogLog (HLL) 草图，用于高效估计数据集的基数（唯一值数量）。

输入：数据集 x（通常是一个列或一组值）。

输出：一个 HLL 草图，表示输入数据集的基数估计。

用途：适用于需要快速估计大规模数据集中唯一值数量的场景，例如统计独立访客数（UV）。

2. MERGE(hlls) -> HYPERLOGLOG
功能：将多个 HLL 草图合并为一个 HLL 草图，合并操作是对这些草图的集合并操作。

输入：一组 HLL 草图（hlls）。

输出：一个新的 HLL 草图，表示所有输入草图的并集。

用途：适用于需要合并多个数据集的基数估计的场景，例如分布式计算中合并多个分区的结果。

3. CARDINALITY(hll) -> BIGINT
功能：对 HLL 草图应用去重计数公式，计算并返回估计的唯一值数量。

输入：一个 HLL 草图（hll）。

输出：一个整数（BIGINT），表示估计的唯一值数量。

用途：适用于从 HLL 草图中提取基数估计值，例如统计唯一用户数或唯一事件数。

4. CAST(hll AS VARBINARY) -> VARBINARY
功能：将 HLL 草图转换为二进制格式（VARBINARY），以便存储到支持二进制数据的系统中（如 Hive 表）。

输入：一个 HLL 草图（hll）。

输出：二进制格式的 HLL 草图（VARBINARY）。

用途：适用于将 HLL 草图持久化存储到数据库或文件系统中。

5. CAST(varbinary AS HYPERLOGLOG) -> HYPERLOGLOG
功能：将二进制格式的 HLL 草图（VARBINARY）转换回 HLL 草图，以便进行进一步的操作（如合并或基数估计）。

输入：二进制格式的 HLL 草图（VARBINARY）。

输出：一个 HLL 草图。

用途：适用于从存储中读取 HLL 草图并恢复其功能。

6. CAST(hll AS P4HYPERLOGLOG) -> P4HYPERLOGLOG
功能：将 HLL 草图转换为 P4HYPERLOGLOG 格式，这是一种更紧凑的存储格式，适合高精度场景。

输入：一个 HLL 草图（hll）。

输出：P4HYPERLOGLOG 格式的 HLL 草图。

用途：适用于需要更高精度或更高效存储的场景。

7. EMPTY_APPROX_SET() -> HYPERLOGLOG
功能：返回一个空的 HLL 草图。

输入：无。

输出：一个空的 HLL 草图。

用途：用于处理可能返回 NULL 的场景，避免因 NULL 值导致的错误。例如，APPROX_SET(NULL) 会返回 NULL，而使用 EMPTY_APPROX_SET() 可以返回一个空的 HLL 草图。
```

