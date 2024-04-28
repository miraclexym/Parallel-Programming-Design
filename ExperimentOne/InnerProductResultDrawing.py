import matplotlib.pyplot as plt

# Data
sizes = [300, 600, 900, 1200, 1500, 1800, 2100, 2400, 2700, 3000, 3300, 3600, 3900, 4200, 4500, 4800, 5100, 5400, 5700, 6000]
ordinary_times = [104.3420, 436.1855, 1349.4865, 3775.6530, 7307.4667, 10036.7709, 16863.1149, 25771.7146, 36895.1162, 47709.7364,
                  62828.3900, 79125.1000, 96612.9320, 116193.7652, 136695.6909, 157796.1100, 179642.2316, 206606.5667, 231692.3294, 258681.7200]
optimized_times = [184.2318, 738.8054, 1728.3757, 3100.4783, 4835.8121, 6992.9455, 9393.3234, 12370.7268, 15584.7270, 19327.0455,
                   23453.7733, 27884.7481, 32682.0040, 38063.8826, 43416.5591, 49563.7600, 55744.2789, 63005.9111, 69994.4706, 77125.2400]
acceleration_ratio = [ordinary_times[i] / optimized_times[i] for i in range(len(sizes))]

# Plotting Runtime
plt.figure(figsize=(10, 5))
plt.plot(sizes, ordinary_times, label='Ordinary')
plt.plot(sizes, optimized_times, label='Optimized')
plt.xlabel('Size')
plt.ylabel('Runtime (us)')
plt.legend()
plt.grid(True)
plt.show()

# Plotting Acceleration Ratio
plt.figure(figsize=(10, 5))
plt.plot(sizes, acceleration_ratio, label='Acceleration ratio')
plt.xlabel('Size')
plt.ylabel('Acceleration ratio')
plt.legend()
plt.grid(True)
plt.show()