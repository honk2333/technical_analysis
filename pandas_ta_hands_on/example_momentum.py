import os
import numpy as np
import pandas as pd
import pandas_ta as ta
import yfinance as yf
import requests


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


def get_stock_data(ticker="AAPL", start_date="2023-01-01", end_date="2024-01-01"):
    """
    使用yfinance获取股票数据
    
    参数:
        ticker: 股票代码 (默认AAPL苹果公司)
        start_date: 开始日期
        end_date: 结束日期
    
    返回:
        包含收盘价的DataFrame
    """
    print(f"正在获取 {ticker} 的股票数据...")
    
    proxies = {
        'http': 'http://127.0.0.1:6789',
        'https': 'http://127.0.0.1:6789',
    }
    
    session = requests.Session()
    session.proxies.update(proxies)
    
    data = yf.download(ticker, start=start_date, end=end_date, auto_adjust=False, session=session)
    print(f"成功获取 {len(data)} 天数据")
    return data


def main():
    """
    主函数 - 演示如何使用pandas-ta和真实股票数据计算动量因子
    """
    ticker = "AAPL"
    data = get_stock_data(ticker, start_date="2023-01-01", end_date="2024-01-01")
    
    if data.empty:
        print("无法获取股票数据，使用模拟数据")
        np.random.seed(42)
        n_days = 100
        base_price = 100
        returns = np.random.normal(0.001, 0.02, n_days)
        prices = base_price * np.cumprod(1 + returns)
        prices_series = pd.Series(prices)
        dates = None
    else:
        prices_series = data["Close"]
        dates = data.index
    
    momentum_20 = calculate_momentum(prices_series, period=20)
    momentum_10 = calculate_momentum(prices_series, period=10)
    
    print(f"\npandas-ta {ticker} 动量因子计算示例")
    print("=" * 70)
    
    if dates is not None:
        print(f"\n前10天数据:")
        for i in range(min(10, len(prices_series))):
            date_str = dates[i].strftime('%Y-%m-%d')
            print(f"{date_str}: 价格 = {prices_series.iloc[i]:.2f}, 10日动量 = {momentum_10.iloc[i]:.4f}, 20日动量 = {momentum_20.iloc[i]:.4f}")
        
        print(f"\n后10天数据:")
        start_idx = max(0, len(prices_series) - 10)
        for i in range(start_idx, len(prices_series)):
            date_str = dates[i].strftime('%Y-%m-%d')
            print(f"{date_str}: 价格 = {prices_series.iloc[i]:.2f}, 10日动量 = {momentum_10.iloc[i]:.4f}, 20日动量 = {momentum_20.iloc[i]:.4f}")
    else:
        print(f"\n前10天数据:")
        for i in range(10):
            print(f"第{i+1:2d}天: 价格 = {prices[i]:.2f}, 10日动量 = {momentum_10[i]:.4f}, 20日动量 = {momentum_20[i]:.4f}")
        
        print(f"\n后10天数据:")
        for i in range(n_days - 10, n_days):
            print(f"第{i+1:3d}天: 价格 = {prices[i]:.2f}, 10日动量 = {momentum_10[i]:.4f}, 20日动量 = {momentum_20[i]:.4f}")
    
    print(f"\n" + "=" * 70)
    print("统计信息:")
    print(f"价格范围: {prices_series.min():.2f} 到 {prices_series.max():.2f}")
    print(f"10日动量范围: {momentum_10.min():.4f} 到 {momentum_10.max():.4f}")
    print(f"20日动量范围: {momentum_20.min():.4f} 到 {momentum_20.max():.4f}")
    print(f"平均10日动量: {momentum_10.mean():.4f}")
    print(f"平均20日动量: {momentum_20.mean():.4f}")


if __name__ == "__main__":
    main()