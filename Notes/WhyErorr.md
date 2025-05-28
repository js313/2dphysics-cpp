## Why implicit Euler have errors in calculations

### Chat logs with chatgpt

#### Question

Ok, lets say I have an object I am simulating it has position, vel, acc. initial position (10, 100) initial velocity(0,0), and only acc will be gravity (9.8m/s^2) assume 1pixel = 1m for this example. So after one second the velocity will be 9.8m/s and that is what euler integration will give me(ignoring floating point errors). and the position will be 9.8m(as we added the celocity to position) but if we do it using the high scchool formulas the position after 1s comes out to be 4.9m.

Is this the error you are referring to?

#### Response

**Yes, exactly. You've nailed the core of the problem.** 🎯

Let’s walk through this clearly because what you described is the **precise type of error** Euler integration introduces:

---

## 🔢 Your Setup

- Initial position: $y_0 = 100$
- Initial velocity: $v_0 = 0$
- Constant acceleration: $a = 9.8\, \text{m/s}^2$
- Time step: $\Delta t = 1 \, \text{second}$

Let’s compute using **three methods**: High school (analytical), Euler, and Semi-implicit Euler.

---

### ✅ 1. **High School (Analytical Integration)**

We use:

$$
x(t) = x_0 + v_0 t + \frac{1}{2} a t^2
$$

Plug in:

$$
x(1) = 100 + 0 \cdot 1 + \frac{1}{2} \cdot 9.8 \cdot (1)^2 = 100 + 4.9 = 104.9
$$

**Velocity after 1s:**

$$
v(1) = v_0 + a t = 0 + 9.8 \cdot 1 = 9.8
$$

✅ So:

- Final position: **104.9**
- Final velocity: **9.8**

---

### ❌ 2. **Explicit Euler Integration**

Euler updates are:

```plaintext
v = v + a * dt
x = x + v * dt
```

But remember: **Euler uses current velocity** to update position. So order matters:

1. Start with $v = 0$
2. $x = x + v \cdot 1 = 100 + 0 = 100$
3. Then update velocity: $v = 0 + 9.8 \cdot 1 = 9.8$

Result:

- Final position: **100**
- Final velocity: **9.8**

So it's **way off** on position — error of 4.9 meters.

---

### ⚠️ 3. **Semi-Implicit Euler (Symplectic Euler)**

Do velocity update **before** position:

```plaintext
v = v + a * dt
x = x + v * dt
```

So:

1. $v = 0 + 9.8 \cdot 1 = 9.8$
2. $x = 100 + 9.8 \cdot 1 = 109.8$

✅ Velocity is correct.
❌ But now position is **overshooting** by 4.9 meters.

---

## 🧠 So What’s Going On?

The real position after 1s should be:

- Averaging the velocity from 0 to 9.8 (average: 4.9)
- So: $x = x_0 + \text{avg v} \cdot t = 100 + 4.9 \cdot 1 = 104.9$

But **Euler integration just uses the velocity at one end of the time step**:

- Explicit Euler: $v = 0$ → too little displacement (100)
- Semi-Implicit: $v = 9.8$ → too much displacement (109.8)

---

## ✅ Takeaway

Yes — this **is the core numerical error in Euler integration**:

- It assumes constant velocity over the time step.
- Real motion with acceleration curves between velocities.
- Euler can't capture that curve — it's just a straight-line estimate.

That’s why methods like Verlet or RK4 aim to approximate the “area under the curve” better, using more points within the time step.

---

Would you like to see how Verlet or RK4 handle this same 1-second fall?
