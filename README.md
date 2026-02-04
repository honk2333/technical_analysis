# xtechnical_analysis
C++ 头文件技术分析库，用于算法交易

## 项目信息

- **原作者**：[NewYaroslav](https://github.com/NewYaroslav)
- **原始仓库**：[https://github.com/NewYaroslav/xtechnical_analysis](https://github.com/NewYaroslav/xtechnical_analysis)

## 指标

该库包含多种单个指标和一个通用指标 MW（可同时执行多个指标功能的滑动窗口指标）。

## 指标实现特点

指标有两种加载价格值的方法：

1. **update 方法**：会更改指标的内部状态，应在柱线关闭时调用。
2. **test 方法**：用于了解尚未完全形成的柱线上的指标值，本质上是"测试"当前价格下指标的实时值。

*test* 方法不影响指标的内部状态（指标不会记住 *test* 方法中传递的价格），但会将指标切换到"测试"模式，因此指标的所有其他方法将返回与 *test* 方法中传递的价格相关的值。

这种设计使得在图表上使用指标变得非常简单，特别是当需要随着新 tick 的到来重新绘制尚未形成的柱线上的指标值时。

### 标准指标

* MinMax - 滑动窗口最大值和最小值
* FastMinMax - 快速最大值和最小值
* MinMaxDiff - 最大值和最小值差异
* PRI - 价格相对强度指标
* RSI - 可使用任何移动平均线的相对强弱指标
* RSHILLMA - 带布林带的相对强弱指标
* BollingerBands - 布林带指标
* CMA - 累积移动平均线
* VCMA - 成交量加权累积移动平均线
* SMA - 简单移动平均线
* WMA - 加权移动平均线
* VWMA - 成交量加权移动平均线
* EMA - 指数移动平均线
* MMA - 修正移动平均线
* AMA - 自适应移动平均线
* LRMA - 线性回归移动平均线
* NoLagMa - 无延迟移动平均线
* LowPassFilter - 低通滤波器
* AverageSpeed - (指标未验证!)
* CurrencyCorrelation - 货币相关性计算
* DetectorWaveform - (指标未验证! 实验性指标)
* Zscore - Z分数指标
* StdDev - 标准偏差
* DelayEvent - 事件延迟线
* DelayLine - 延迟线 (指标未验证!)
* OsMa - 振荡器移动平均线 (指标未验证!)
* TrendDirectionForceIndex - 可使用任何移动平均线的趋势方向力量指标
* MAV - 移动平均线速度
* MAZ - 移动平均线区域

### MW 指标

开发重点放在通用指标的开发上。在该库中，这是 *"滑动窗口"* 指标或简称 *MW*。

MW 类有几种方法可以获取分析报价的各种数据。

* 构造函数 *MW* - 在构造函数中可以设置滑动窗口的最大周期。

```cpp
/** \brief 初始化滑动窗口
 * \param period 周期
 */
MW(const size_t period);
```

* 方法 *is_init()* - 当滑动窗口缓冲区完全填满时，此方法将返回 true。即，当滑动窗口包含最大可能数量的报价时。

```cpp
/** \brief 检查滑动窗口缓冲区的初始化
 * \return 如果滑动窗口缓冲区完全填充了值，则返回 true。
 */
bool is_init();
```

* 方法 *update* - 每次需要将新的报价值加载到指标中时，都应调用此方法。此方法有几种变体：

```cpp
/** \brief 更新指标状态
 * \param in 输入信号（报价，柱线的收盘价/开盘价等）
 * \return 成功返回 0，否则参见 ErrorType
 */
int update(const T &in);
		
/** \brief 更新指标状态
 * \param in 输入信号（报价，柱线的收盘价/开盘价等）
 * \param out 输出数组
 * \return 成功返回 0，否则参见 ErrorType
 */
int update(const T &in, std::vector<T> &out);
```
* 方法 *test* - 此方法类似于 *update* 方法，但有根本区别。**此方法不影响指标的内部状态。** 此方法用于"测试"指标，即获取指标值而不更改内部状态。当您需要获取当前报价的 RSI 值，但指标本身应仅根据柱线的收盘价计算时，此方法可能很有用。

```cpp
/** \brief 测试指标
 * 此函数与 update 的不同之处在于它不影响指标的内部状态
 * \param in 输入信号
 * \return 成功返回 0，否则参见 ErrorType
 */
int test(const T &in);
		
/** \brief 测试指标
 * 此函数与 update 的不同之处在于它不影响指标的内部状态
 * \param in 输入信号
 * \param out 输出信号
 * \return 成功返回 0，否则参见 ErrorType
 */
int test(const T &in, std::vector<T> &out);
```

根据选择的方法（*update* 或 *test*），其他获取各种值的方法将使用临时缓冲区（*包含来自 test 方法的最后一个报价*）或指标的主缓冲区（*仅由 update 方法更改*）。

* 方法 get_data - 允许获取缓冲区（主缓冲区或测试缓冲区）。

```cpp
/** \brief 获取指标内部缓冲区的数据
 * \param buffer 缓冲区
 */
void get_data(std::vector<T> &buffer);
```

* 方法 *get_max_value* 和 *get_min_value* 允许从主缓冲区或测试缓冲区获取最大值和最小值。

**注意！确保缓冲区至少包含 N = *period* 个值！**

```cpp
/** \brief 获取缓冲区的最大值
 * \param max_value 最大值
 * \param period 最大数据的周期
 * \param offset 数组中的偏移量。默认为 0
 */
void get_max_value(T &max_value, const size_t period, const size_t offset = 0);
```

* 方法 *get_average* 允许从主缓冲区或测试缓冲区获取平均值。

**注意！确保缓冲区至少包含 N = *period* 个值！**

```cpp
/** \brief 获取缓冲区的平均值
 * \param average_value 平均值
 * \param period 平均值的周期
 * \param offset 数组中的偏移量。默认为 0
 */
void get_average(T &average_value, const size_t period, const size_t offset = 0);
```

* 方法 *get_std_dev* 允许从主缓冲区或测试缓冲区获取标准偏差。

**注意！确保缓冲区至少包含 N = *period* 个值！**

```cpp
/** \brief 获取缓冲区的标准偏差
 * \param std_dev_value 标准偏差
 * \param period 标准偏差的周期
 * \param offset 数组中的偏移量。默认为 0
 */
void get_std_dev(T &std_dev_value, const size_t period, const size_t offset = 0)
```

* 方法 *get_average_and_std_dev_array* 允许从主缓冲区或测试缓冲区获取平均值和标准偏差的数组。

**注意！确保缓冲区至少包含 N = *period* 个值！**

```cpp
/** \brief 获取缓冲区的平均值和标准偏差数组
 *
 * 最小周期等于 2
 * \param average_data 平均值数组
 * \param std_data 标准偏差数组
 * \param min_period 最小周期
 * \param min_period 最大周期
 * \param step_period 周期步长
 */
void get_average_and_std_dev_array(
		std::vector<T> &average_data,
		std::vector<T> &std_data,
		size_t min_period,
		size_t max_period,
		const size_t &step_period);
```

* 方法 *get_rsi* 允许从主缓冲区或测试缓冲区获取 RSI 值。

**注意！确保缓冲区至少包含 N = *period + 1* 个值！**

```cpp
/** \brief 获取 RSI 值
 * \param rsi_value RSI 值
 * \param period RSI 周期
 */
void get_rsi(T &rsi_value, const size_t period);
```

* 方法 *get_rsi_array* 允许从主缓冲区或测试缓冲区获取 RSI 值的数组。

**注意！确保缓冲区至少包含 N = *max_period + 1* 个值！**

```cpp
/** \brief 获取 RSI 值的数组
 * \param rsi_data RSI 值的数组
 * \param min_period 最小周期
 * \param max_period 最大周期
 * \param step_period 周期步长
 */
void get_rsi_array(
		std::vector<T> &rsi_data,
		size_t min_period,
		size_t max_period,
		const size_t &step_period);
```

* 方法 *clear()* 清除指标的内部状态。它将主缓冲区和测试缓冲区的大小重置为零。

```cpp
/** \brief 清除指标数据
 */
void clear();
```

### 程序示例

```cpp
#include <iostream>
#include "xtechnical_indicators.hpp"

using namespace std;

int main() {
    cout << "Hello world!" << endl;
	/* 最大周期为 30 的 "滑动窗口" 指标 */
    xtechnical_indicators::MW<double> iMW(30);
	
	/* 将测试值加载到指标中 */
    for(int i = 1; i <= 50; ++i) {
        int temp = i % 4; // 根据迭代次数，值从 0 到 3
        iMW.update(temp);
		/* 获取指标值数组 */
        std::vector<double> rsi;
        std::vector<double> sma;
        std::vector<double> std_dev;
        iMW.get_rsi_array(rsi, 3, 15, 3);
        iMW.get_average_and_std_dev_array(sma, std_dev, 10, 22, 3);

        std::cout << "step: " << i << " temp " << temp << " is init: " << iMW.is_init() << std::endl;
        for(size_t i = 0; i < sma.size(); ++i) {
            std::cout << "rsi: " << rsi[i] << " sma: " << sma[i] << " std_dev: " << std_dev[i] << std::endl;
        }
    }

    double rsi_value = 0;
    iMW.get_rsi(rsi_value, 15);
    std::cout << "rsi_value: " << rsi_value << std::endl;
    double aver_value = 0;
    iMW.get_average(aver_value, 22);
    std::cout << "aver_value: " << aver_value << std::endl;
    double std_dev_value = 0;
    iMW.get_std_dev(std_dev_value, 22);
    std::cout << "std_dev: " << std_dev_value << std::endl;
    return 0;
}
```

测试程序将在屏幕上输出以下内容：
![example_0](doc/example_0.png)

## 数据标准化

* calculate_min_max
* calculate_zscore
* calculate_difference
* normalize_amplitudes
* calculate_log

## 统计

* calc_root_mean_square
* calc_mean_value
* calc_harmonic_mean
* calc_geometric_mean
* calc_median
* calc_std_dev_sample
* calc_std_dev_population
* calc_mean_absolute_deviation
* calc_skewness
* calc_standard_error
* calc_sampling_error
* calc_coefficient_variance
* calc_signal_to_noise_ratio
* calc_excess

## 回归分析

目前仅实现了最小二乘法

* calc_least_squares_method
* calc_line

## 有用的链接

* 方便处理报价的库 [xquotes_history](https://github.com/NewYaroslav/xquotes_history)
* C++ 绘图库 [easy_plot_cpp](https://github.com/NewYaroslav/easy_plot_cpp)
* 外汇指标和顾问（源代码）：[https://trueforex.pp.ua/](https://trueforex.pp.ua/)