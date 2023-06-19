# ZAFU ACM Rating System

仿照 Codeforces 的 [Elo Rating System](https://en.wikipedia.org/wiki/Elo_rating_system)。

初始分 1400，在前 6 次训练赛中逐步发放。

## 使用

需要 C++ 和 Python 环境，使用 [Just](https://github.com/casey/just) 作为命令执行器。

> 不安装 Just 环境也可以，手动执行 `justfile` 内容即可。

在 `config.yaml` 中加入新的比赛榜单，只需要姓名、排名即可，可以直接从 Excel 中复制过来，第一行增加为榜单人数。

之后执行 `just generate` 生成新的比赛信息，在 `rating` 文件夹下。

网页数据无需手动更新。核验无误并 Push 之后，GitHub 会通过 Action 自动更新网页数据。
