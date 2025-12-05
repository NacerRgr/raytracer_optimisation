
## Performance Benchmarks

### Evaluation 4: AABB Implementation

#### Scene: `sphere-galaxy-on-plane`
| Configuration | Time | Speedup vs Baseline |
|--------------|------|---------------------|
| **Baseline** (no optimizations) | 31.99s | 1.0× |
| **Multithreading only** | 9.55s | **3.35×** |
| **AABB only** | 34.73s | 0.92× |
| **AABB + Multithreading** | 10.35s | 3.09× |



---

#### Scene: `all`
| Configuration | Time | Speedup vs Baseline |
|--------------|------|---------------------|
| **Baseline** (no optimizations) | 645.67s | 1.0× |
| **Multithreading only** | 187.86s | 3.44× |
| **AABB only** | 461.24s | **1.40×** |
| **AABB + Multithreading** | 144.86s | **4.46×** |


---

#### Scene: `iso-sphere` (mesh with triangles)
| Configuration | Time | Speedup vs Baseline |
|--------------|------|---------------------|
| **Baseline** (no optimizations) | 25.6141s | 1.0× |
| **Multithreading only** | 12.6266s | 2.03× |
| **AABB only** | 20.7164s | **1.24×** |
| **AABB + Multithreading** | 5.76404s | **4.44×** |



---

### Evaluation 5: BSP Tree Implementation

#### Scene: `two-spheres-on-plane`
| Configuration | Time |
|--------------|------|
| **BSP** | 4.78719s |
| **Multithreading** | 2.0666s |

---

#### Scene: `two-triangles-on-plane`
| Configuration | Time |
|--------------|------|
| **BSP** | 2.99357s |
| **Multithreading** | 1.10603s |

---

#### Scene: `sphere-galaxy-on-plane`
| Configuration | Time |
|--------------|------|
| **BSP** | 46.072s |
| **Multithreading** | 10.2437s |

---

#### Scene: `monkey-on-plane`
| Configuration | Time |
|--------------|------|
| **BSP** | 277.49s |
| **Multithreading** | 75.4676s |

---

#### Scene: `all`
| Configuration | Time | Speedup vs BSP |
|--------------|------|----------------|
| **BSP** | 129.57s | 1.0× |
| **Multithreading** | 184.061s | 0.70× |
| **BSP + Multithreading** | 126.501s | **1.02×** |



---

#### Scene: `iso-sphere`
| Configuration | Time |
|--------------|------|
| **BSP** | 21.8905s |
| **Multithreading** (without AABB) | 7.38742s |
| **BSP + Multithreading** | - |

---


