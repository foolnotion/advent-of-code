In this problem we are required to find which tiles in a given grid layout can be reached in exactly $n$ steps.
For **part one** we have $n=64$ while for part 2 we have $n = 26501365$ and the grid is infinite.

While **part one** can be easily solved by any kind of path finding approach (e.g. BFS), this would be infeasible for **part two**. We have to try to find special properties of the input and exploit them in order to have an efficient solution.

Upon visual inspection of the reachable squares for varying number of steps $n=1,...,65$, we note that the reachable squares form a **diamond-shaped pattern**. This is because the start point is exactly in the center of the grid and its corresponding row and column do not contain any rocks and are completely unobstructed. Furthermore, the reachable squares (at least for my input) are always the same parity as the desired number of steps. That is, any point $\mathbf{p}$ on the grid is reachable in exactly $n$ steps if:
$$
    \|\mathbf{s}-\mathbf{p} \|_1 \bmod 2 = n \bmod 2
$$
where $\mathbf{s}$ is the starting point.

Thus, a (relatively naive) solution for **part one** can be achieved by counting all the tiles which have this property and are not a rock (or the starting point).

```cpp
constexpr auto n1{64L};
auto sum = 0L;
for (auto i = 0; i < grid.extent(0); ++i) {
    for (auto j = 0; j < grid.extent(1); ++j) {
        if (grid(i, j) == '#') { continue; }
        auto d = (start - point{i, j}).template lpNorm<1>();
        sum += d > 0 && d <= n1 && d % 2 == n1 % 2;
    }
}
auto const p1 = sum;
```

For part two, further investigation is necessary. It is clear that such a large number of steps will lead to to a very large diamond which will cover many full grids. The point then is to investigate how many