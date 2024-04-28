import matplotlib.pyplot as plt

# 数据
sizes = [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072]
ordinary_times = [0.0021, 0.0038, 0.0074, 0.0148, 0.0294, 0.0588, 0.1150, 0.2324, 0.4569, 0.9131, 1.8490, 3.8345, 7.3416, 14.6238, 29.3984, 59.8500, 125.3469]
multi_link_times = [0.0017, 0.0024, 0.0043, 0.0078, 0.0147, 0.0290, 0.0577, 0.1147, 0.2303, 0.4568, 0.9130, 1.8909, 3.6500, 7.6148, 14.9148, 29.1953, 61.2875]
recursive_times = [0.0391, 0.0447, 0.0474, 0.0543, 0.0756, 0.1139, 0.1938, 0.3634, 0.7060, 1.3777, 2.6146, 5.3743, 17.9535, 35.8891, 70.7352, 161.1562, 400.8625]
circulate_times = [0.0382, 0.0425, 0.0440, 0.0511, 0.0701, 0.1077, 0.1850, 0.3427, 0.6563, 1.3399, 2.6786, 5.3168, 18.3652, 34.2469, 67.2195, 160.2422, 396.4906]

# 绘制四种算法的运行时间随数据规模增长的变化
plt.figure(figsize=(10, 6))
plt.plot(sizes, ordinary_times, marker='o', label='Ordinary')
plt.plot(sizes, multi_link_times, marker='o', label='MultiLink')
plt.plot(sizes, recursive_times, marker='o', label='Recursive')
plt.plot(sizes, circulate_times, marker='o', label='Circulate')
plt.xlabel('Data Size')
plt.ylabel('Runtime (us)')
plt.legend()
plt.grid(True)
plt.xscale('log')
plt.yscale('log')
plt.show()

# 计算加速比
ordinary_acceleration = [ordinary_times[i] / multi_link_times[i] for i in range(len(sizes))]
recursive_acceleration = [ordinary_times[i] / recursive_times[i] for i in range(len(sizes))]
circulate_acceleration = [ordinary_times[i] / circulate_times[i] for i in range(len(sizes))]

# 绘制三种优化算法的加速比随数据规模增长的变化
plt.figure(figsize=(10, 6))
plt.plot(sizes, ordinary_acceleration, marker='o', label='MultiLink')
plt.plot(sizes, recursive_acceleration, marker='o', label='Recursive')
plt.plot(sizes, circulate_acceleration, marker='o', label='Circulate')
plt.xlabel('Size')
plt.ylabel('Acceleration ratio')
plt.legend()
plt.grid(True)
plt.xscale('log')
plt.show()
