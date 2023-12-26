For part 2, we are asked to find a rock trajectory which will hit every single hailstone.

Each hailstone trajectory is described in the form of an initial position $p$ and a velocity $v$.

For example, hailstone $H_i$ will be described by

$$
    p = [x_i, y_i, z_i] \text{ and } v = [\dot{x}_i, \dot{y}_i, \dot{z}_i]
$$

We can also write $H_i$ line this:

$$
    \begin{bmatrix} x\\ y\\ z \end{bmatrix} = \begin{bmatrix} x_i\\ y_i\\ z_i \end{bmatrix} + t \begin{bmatrix} \dot{x}_i\\ \dot{y}_i\\ \dot{z}_i \end{bmatrix}
$$

This leads to the parametric form:

$$
    \begin{aligned}
        x = x_i + t_i \cdot \dot{x}_i\\
        y = y_i + t_i \cdot \dot{y}_i\\
        z = z_i + t_i \cdot \dot{z}_i\\
    \end{aligned}
$$

We have many such hailstones $H_i$ but the problem states that they don't actually intersect (they are _askew_).

We should find a rock parametric form:

$$
    \begin{aligned}
        x = x_r + t_r \cdot \dot{x}_r\\
        y = y_r + t_r \cdot \dot{y}_r\\
        z = z_r + t_r \cdot \dot{z}_r\\
    \end{aligned}
$$

If the rock will hit the hailstone, then at some point in time their $x, y, z$ coordinates will be equal:
$$
    x = x_i + t_i \dot{x}_i = x_r + t_r \dot{x}_r\\
    y = y_i + t_i \dot{y}_i = y_r + t_r \dot{y}_r\\
    z = z_i + t_i \dot{z}_i = z_r + t_r \dot{z}_r\\
$$