import matplotlib.pyplot as plt

# 实验数据
problem_scales = [1000, 10000, 100000, 1000000, 10000000]
serial_durations = [87.5, 930.4, 9849.7, 90815.3, 904574]
parallel_durations = [32.7, 344, 3368.3, 31296.9, 308134]

# 绘制实验结果图像1：问题规模与串行/并行算法运行时间的对比
plt.figure(figsize=(10, 6))
plt.plot(problem_scales, serial_durations, marker='o', label='Serial')
plt.plot(problem_scales, parallel_durations, marker='o', label='Parallel')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Problem Scale')
plt.ylabel('Time (microseconds)')
plt.title('Serial vs Parallel Execution Time')
plt.legend()
plt.grid(True)
plt.show()

# 计算加速比
speedup_ratios = [serial / parallel for serial, parallel in zip(serial_durations, parallel_durations)]

# 绘制实验结果图像2：问题规模与加速比
plt.figure(figsize=(10, 6))
plt.plot(problem_scales, speedup_ratios, marker='o', label='Speedup Ratio')
plt.xscale('log')
plt.xlabel('Problem Scale')
plt.ylabel('Speedup Ratio')
plt.title('Speedup Ratio vs Problem Scale')
plt.legend()
plt.grid(True)
plt.ylim(0, 3.5)  # 设置纵坐标范围为0到3
plt.show()
