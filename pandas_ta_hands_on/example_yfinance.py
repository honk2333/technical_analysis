import yfinance as yf
import os

# 1. 尝试通过环境变量设置代理 (这是 curl_cffi 最容易读取的方式)
os.environ['HTTP_PROXY'] = "http://127.0.0.1:6789"
os.environ['HTTPS_PROXY'] = "http://127.0.0.1:6789"

# 2. 针对 SSL 问题，新版 yfinance 如果检测到代理环境
print("测试新版 yfinance 代理下载...")

try:
    # 核心：不要传 session 参数！直接传 proxy 字符串
    data = yf.download("AAPL", period="1mo", proxy="http://127.0.0.1:6789")
    
    if data.empty:
        print("下载失败：返回数据为空，请检查代理是否支持 HTTPS 流量。")
    else:
        print(f"成功获取数据! 行数: {len(data)}")
        print(data.head())
except Exception as e:
    print(f"发生错误: {e}")