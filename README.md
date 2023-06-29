# ZAFU ACM Rating System

仿照 Codeforces 的 [Elo Rating System](https://en.wikipedia.org/wiki/Elo_rating_system)。

初始分 1400，在前 6 次训练赛中逐步发放。

### 每场分数计算（[Codeforces 博客](https://codeforces.com/blog/entry/20762)）：
1. 计算选手 $i$ 与 选手 $j$ 相比获胜的概率 $P_{i,j}$，$r_i$ 表示选手 $i$ 的 rating：

$$
P_{i,j} = \frac{1}{1 + 10\frac{r_j-r_i}{400}}\ ,r_i > r_j
$$

2. 计算选手 $i$ 的预期排名 $seed_i$：
$$
seed_i = 1 + \sum^{n}_{j=1,j\neq i}{P_{j, i}}
$$

3. 通过选手的真实排名 $a_i$ 和预期排名 $seed_i$ 计算出新预期排名 $m_i$：
$$
m_i = \sqrt{seed_i \times a_i}
$$

4. 用二分法找到一个新 rating 值 $R_i$，用该值算出来的 $seed$ 值等于 $m_i$。计算 $r_i$ 与 $R_i$ 的差值 $d_i$：
$$
d_i = \frac{R_i - r_i}{2}
$$ 

5. 为防止分数膨胀，引入 $inc$：
$$
inc = \rm{min}(\rm{max}(-\frac{\sum^n_1{d_i}}{n}, -10), 0)
$$
6. 最终选手 $i$ 的 rating = $r_i + d_i + inc$。

### 初始分发放

初始分 1400，在前 6 次训练赛中逐步发放。

从开始的第一场到第六场每场发放的分数分别为500, 350, 250, 150, 100, 50。总共 1400 分。

比如：第一场给你 500 分，再加上第一场的表现分，最终加了 368 分，那么你实际的表现分是 -132 分。其他五场同理。

## 使用

需要 C++ 和 Python 环境，使用 [Just](https://github.com/casey/just) 作为命令执行器。

> 不安装 Just 环境也可以，手动执行 `justfile` 内容即可。

在 `config.yaml` 中加入新的比赛榜单，只需要班级姓名、排名即可，可以直接从 Excel 中复制过来，第一行增加为榜单人数。

之后执行 `just generate` 生成新的比赛信息，在 `rating` 文件夹下。

网页数据无需手动更新。核验无误并 Push 之后，GitHub 会通过 Action 自动更新网页数据。
