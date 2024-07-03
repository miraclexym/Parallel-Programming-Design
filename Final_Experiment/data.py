import matplotlib.pyplot as plt

# 数据
problem_scales = [1000, 10000, 100000, 1000000, 10000000]
serial_durations = [87, 930, 9849, 90815, 924574]
OpenMP_durations = [58, 596, 6073, 59786, 589943]
Pthread_durations = [42, 406, 4196, 42396, 416149]
MPI_durations = [45, 467, 4873, 47856, 477592]
SIMD_durations = [32, 344, 3368, 31296, 308134]
GPU_durations = [25, 248, 2606, 23767, 250947]
Integrated_durations = [8, 83, 972, 8996, 90782]

# 绘制不同算法的运行时间对比图像
plt.figure(figsize=(10, 6))

plt.plot(problem_scales, serial_durations, marker='o', label='Serial')
plt.plot(problem_scales, OpenMP_durations, marker='o', label='OpenMP')
plt.plot(problem_scales, Pthread_durations, marker='o', label='Pthread')
plt.plot(problem_scales, MPI_durations, marker='o', label='MPI')
plt.plot(problem_scales, SIMD_durations, marker='o', label='SIMD')
plt.plot(problem_scales, GPU_durations, marker='o', label='GPU')
plt.plot(problem_scales, Integrated_durations, marker='o', label='Integrated')

plt.xscale('log')
plt.yscale('log')
plt.xticks(problem_scales, labels=[str(scale) for scale in problem_scales])
plt.xlabel('Problem Scale')
plt.ylabel('Time (microseconds)')
plt.title('Comparison of Execution Time for Different Algorithms')
plt.legend()
plt.grid(True)
plt.tight_layout()

plt.show()

# 计算并绘制加速比图像
acceleration_OpenMP = [serial_durations[i] / OpenMP_durations[i] for i in range(len(problem_scales))]
acceleration_Pthread = [serial_durations[i] / Pthread_durations[i] for i in range(len(problem_scales))]
acceleration_MPI = [serial_durations[i] / MPI_durations[i] for i in range(len(problem_scales))]
acceleration_SIMD = [serial_durations[i] / SIMD_durations[i] for i in range(len(problem_scales))]
acceleration_GPU = [serial_durations[i] / GPU_durations[i] for i in range(len(problem_scales))]
acceleration_Integrated = [serial_durations[i] / Integrated_durations[i] for i in range(len(problem_scales))]

plt.figure(figsize=(10, 6))

plt.plot(problem_scales, acceleration_OpenMP, marker='o', label='OpenMP')
plt.plot(problem_scales, acceleration_Pthread, marker='o', label='Pthread')
plt.plot(problem_scales, acceleration_MPI, marker='o', label='MPI')
plt.plot(problem_scales, acceleration_SIMD, marker='o', label='SIMD')
plt.plot(problem_scales, acceleration_GPU, marker='o', label='GPU')
plt.plot(problem_scales, acceleration_Integrated, marker='o', label='Integrated')

plt.xscale('log')
plt.xticks(problem_scales, labels=[str(scale) for scale in problem_scales])
plt.xlabel('Problem Scale')
plt.ylabel('Acceleration Ratio (Serial Time / Parallel Time)')
plt.title('Acceleration Ratio Comparison for Different Algorithms')
plt.legend()
plt.grid(True)
plt.tight_layout()

plt.show()
