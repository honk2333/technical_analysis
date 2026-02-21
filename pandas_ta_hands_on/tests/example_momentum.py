import numpy as np
import pandas as pd
import pandas_ta as ta


def calculate_momentum(prices, period=20):
    """
    使用pandas-ta计算动量因子
    
    参数:
        prices: 价格序列 (numpy array 或 pandas Series)
        period: 动量周期 (默认20)
    
    返回:
        动量因子序列 (pandas Series)
    """
    if not isinstance(prices, pd.Series):
        prices = pd.Series(prices)
    
    roc = ta.roc(close=prices, length=period)
    return roc / 100.0


def main():
    """
    主函数 - 演示如何使用pandas-ta计算动量因子
    """
    np.random.seed(42)
    
    n_days = 100
    base_price = 100
    returns = np.random.normal(0.001, 0.02, n_days)
    prices = base_price * np.cumprod(1 + returns)
    
    prices_series = pd.Series(prices)
    
    momentum_20 = calculate_momentum(prices_series, period=20)
    momentum_10 = calculate_momentum(prices_series, period=10)
    
    print("pandas-ta动量因子计算示例")
    print("=" * 50)
    print("\n前10天数据:")
    for i in range(10):
        print(f"第{i+1:2d}天: 价格 = {prices[i]:.2f}, 10日动量 = {momentum_10[i]:.4f}, 20日动量 = {momentum_20[i]:.4f}")
    
    print("\n后10天数据:")
    for i in range(n_days - 10, n_days):
        print(f"第{i+1:3d}天: 价格 = {prices[i]:.2f}, 10日动量 = {momentum_10[i]:.4f}, 20日动量 = {momentum_20[i]:.4f}")
    
    print("\n" + "=" * 50)
    print("统计信息:")
    print(f"价格范围: {prices.min():.2f} 到 {prices.max():.2f}")
    print(f"10日动量范围: {momentum_10.min():.4f} 到 {momentum_10.max():.4f}")
    print(f"20日动量范围: {momentum_20.min():.4f} 到 {momentum_20.max():.4f}")
    print(f"平均10日动量: {momentum_10.mean():.4f}")
    print(f"平均20日动量: {momentum_20.mean():.4f}")


if __name__ == "__main__":
    main()
