#include <iostream>
#include "xtechnical_indicators.hpp"
#include <random>
#include <ctime>
#include <iterator>
#include <string>
#include <opencv2/opencv.hpp>

// 简单的函数，使用OpenCV生成png图片
void plot_to_png(const std::vector<double> &data, const std::string &title, const std::string &filename) {
    // 检查数据是否为空
    if (data.empty()) {
        std::cout << "Error: Empty data for plotting" << std::endl;
        return;
    }

    // 计算图像尺寸
    const int width = 800;
    const int height = 600;
    const int margin = 50;

    // 计算数据范围
    double min_val = *std::min_element(data.begin(), data.end());
    double max_val = *std::max_element(data.begin(), data.end());
    double val_range = max_val - min_val;
    if (val_range == 0) val_range = 1.0; // 避免除零错误

    // 创建空白图像
    cv::Mat image(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    // 绘制坐标轴
    cv::line(image, cv::Point(margin, margin), cv::Point(margin, height - margin), cv::Scalar(0, 0, 0), 2);
    cv::line(image, cv::Point(margin, height - margin), cv::Point(width - margin, height - margin), cv::Scalar(0, 0, 0), 2);

    // 绘制数据曲线
    for (size_t i = 1; i < data.size(); ++i) {
        int x1 = margin + static_cast<int>((i - 1) * (width - 2 * margin) / (data.size() - 1));
        int y1 = height - margin - static_cast<int>((data[i - 1] - min_val) * (height - 2 * margin) / val_range);
        int x2 = margin + static_cast<int>(i * (width - 2 * margin) / (data.size() - 1));
        int y2 = height - margin - static_cast<int>((data[i] - min_val) * (height - 2 * margin) / val_range);
        cv::line(image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 2);
    }

    // 添加标题
    cv::putText(image, title, cv::Point(margin, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 0), 2);

    // 添加坐标轴标签
    cv::putText(image, "Index", cv::Point(width / 2, height - 10), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1);
    cv::putText(image, "Value", cv::Point(10, height / 2), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1);

    // 保存图像
    std::string output_filename = filename + ".png";
    if (cv::imwrite(output_filename, image)) {
        std::cout << "Plot saved to " << output_filename << std::endl;
    } else {
        std::cout << "Error: Failed to save plot to " << output_filename << std::endl;
    }
}

// 绘制两条曲线的函数
void plot_two_curves_to_png(const std::vector<double> &data1, const std::vector<double> &data2, const std::string &title, const std::string &filename) {
    // 检查数据是否为空
    if (data1.empty() || data2.empty()) {
        std::cout << "Error: Empty data for plotting" << std::endl;
        return;
    }

    // 计算图像尺寸
    const int width = 800;
    const int height = 600;
    const int margin = 50;

    // 计算数据范围
    double min_val = std::min(*std::min_element(data1.begin(), data1.end()), *std::min_element(data2.begin(), data2.end()));
    double max_val = std::max(*std::max_element(data1.begin(), data1.end()), *std::max_element(data2.begin(), data2.end()));
    double val_range = max_val - min_val;
    if (val_range == 0) val_range = 1.0; // 避免除零错误

    // 创建空白图像
    cv::Mat image(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

    // 绘制坐标轴
    cv::line(image, cv::Point(margin, margin), cv::Point(margin, height - margin), cv::Scalar(0, 0, 0), 2);
    cv::line(image, cv::Point(margin, height - margin), cv::Point(width - margin, height - margin), cv::Scalar(0, 0, 0), 2);

    // 绘制第一条曲线（数据）
    for (size_t i = 1; i < data1.size(); ++i) {
        int x1 = margin + static_cast<int>((i - 1) * (width - 2 * margin) / (data1.size() - 1));
        int y1 = height - margin - static_cast<int>((data1[i - 1] - min_val) * (height - 2 * margin) / val_range);
        int x2 = margin + static_cast<int>(i * (width - 2 * margin) / (data1.size() - 1));
        int y2 = height - margin - static_cast<int>((data1[i] - min_val) * (height - 2 * margin) / val_range);
        cv::line(image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 2);
    }

    // 绘制第二条曲线（三角形分布）
    size_t min_size = std::min(data1.size(), data2.size());
    for (size_t i = 1; i < min_size; ++i) {
        int x1 = margin + static_cast<int>((i - 1) * (width - 2 * margin) / (min_size - 1));
        int y1 = height - margin - static_cast<int>((data2[i - 1] - min_val) * (height - 2 * margin) / val_range);
        int x2 = margin + static_cast<int>(i * (width - 2 * margin) / (min_size - 1));
        int y2 = height - margin - static_cast<int>((data2[i] - min_val) * (height - 2 * margin) / val_range);
        cv::line(image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0), 2);
    }

    // 添加标题
    cv::putText(image, title, cv::Point(margin, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 0), 2);

    // 添加图例
    cv::putText(image, "Data", cv::Point(width - margin - 100, margin + 20), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255), 1);
    cv::putText(image, "Triangular", cv::Point(width - margin - 100, margin + 40), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 1);

    // 添加坐标轴标签
    cv::putText(image, "Index", cv::Point(width / 2, height - 10), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1);
    cv::putText(image, "Value", cv::Point(10, height / 2), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1);

    // 保存图像
    std::string output_filename = filename + ".png";
    if (cv::imwrite(output_filename, image)) {
        std::cout << "Plot saved to " << output_filename << std::endl;
    } else {
        std::cout << "Error: Failed to save plot to " << output_filename << std::endl;
    }
}

void print_map(const std::map<int, int> &distribution) {
    for (auto it = distribution.begin(); it != distribution.end(); ++it) {
        std::cout << it->first << " " << it->second << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Hello world!" << std::endl;
    // 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
    xtechnical::ClusterShaper cluster_shaper(60, 0.0001d);
    cluster_shaper.on_close_bar = [&](const xtechnical::ClusterShaper::Cluster &cluster) {
        auto normalized_cluster = cluster.get_normalized_array();
        auto no_normalized_cluster = cluster.get_array();

        if (false) {
            std::copy(normalized_cluster.begin(), normalized_cluster.end(), std::ostream_iterator<double>(std::cout, " "));
            std::cout << std::endl;
            std::copy(no_normalized_cluster.begin(), no_normalized_cluster.end(), std::ostream_iterator<double>(std::cout, " "));
            std::cout << std::endl;
            print_map(cluster.distribution);
        }

        std::cout << "o: " << cluster.open << " c: " << cluster.close << " m: " << cluster.get_center_mass() << std::endl;
        const double center = cluster.get_center_mass_norm();
        std::cout << "center: " << center << std::endl;
        if (center < 0.38) {
            std::copy(no_normalized_cluster.begin(), no_normalized_cluster.end(), std::ostream_iterator<double>(std::cout, " "));
            std::cout << std::endl;
            // 使用gnuplot生成png图片
            static int plot_count = 0;
            std::string filename = "center_plot_" + std::to_string(plot_count++);
            plot_to_png(normalized_cluster, "Center Mass Plot", filename);
        }

        //auto triangular_distribution = xtechnical::ClusterShaper::get_triangular_distribution(normalized_cluster.size(), normalized_cluster.size()/2);

        auto triangular_distribution = xtechnical::ClusterShaper::get_triangular_distribution(normalized_cluster.size(), normalized_cluster.size() * 0.0);
        double similarity = xtechnical::ClusterShaper::get_cosine_similarity(normalized_cluster, triangular_distribution);
        double dist = xtechnical::ClusterShaper::get_euclidean_distance(normalized_cluster, triangular_distribution);
        std::cout << "similarity: " << similarity << std::endl;
        if (similarity > 0.55) {
            std::copy(no_normalized_cluster.begin(), no_normalized_cluster.end(), std::ostream_iterator<double>(std::cout, " "));
            std::cout << std::endl;
            // 使用gnuplot生成png图片
            static int plot_count = 0;
            std::string filename = "similarity_plot_" + std::to_string(plot_count++);
            plot_two_curves_to_png(normalized_cluster, triangular_distribution, "Similarity Plot", filename);
        }
        std::cout << "dist: " << dist << std::endl;
        if (dist < 0.02) {
            std::copy(no_normalized_cluster.begin(), no_normalized_cluster.end(), std::ostream_iterator<double>(std::cout, " "));
            std::cout << std::endl;
            // 使用gnuplot生成png图片
            static int plot_count = 0;
            std::string filename = "euclidean_plot_" + std::to_string(plot_count++);
            plot_two_curves_to_png(normalized_cluster, triangular_distribution, "Euclidean Distance Plot", filename);
        }

    };
    cluster_shaper.on_unformed_bar = [&](const xtechnical::ClusterShaper::Cluster &cluster) {
        auto normalized_cluster = cluster.get_normalized_array();
        //std::copy(normalized_cluster.begin(), normalized_cluster.end(), std::ostream_iterator<double>(std::cout, " "));
    };

    std::mt19937 gen(time(0));
    //std::normal_distribution<> urd(-0.5, 1.0);
    //std::lognormal_distribution<> urd(-0.5, 1.0);

    std::uniform_real_distribution<> urd(-0.5, 1.0);

    const std::size_t price_size = 5000000;
    std::vector<double> prices(price_size);
    double last_price = 10 + urd(gen);
    std::generate(prices.begin(), prices.begin() + price_size, [&]() -> double {
        last_price = last_price + 0.001d* urd(gen);
        return last_price;
    });

    for (size_t i = 0; i < price_size; ++i) {
        //std::cout << "price " << prices[i] << " " << i << std::endl;
        cluster_shaper.update(prices[i], i/2);
    }

    {
        auto triangular_distribution = xtechnical::ClusterShaper::get_triangular_distribution(10, 5);
        std::copy(triangular_distribution.begin(), triangular_distribution.end(), std::ostream_iterator<double>(std::cout, " "));
        std::cout << std::endl;
        // 使用gnuplot生成png图片
        plot_to_png(triangular_distribution, "Triangular Distribution (10,5)", "triangular_10_5");
    }
    {
        auto triangular_distribution = xtechnical::ClusterShaper::get_triangular_distribution(3, 1);
        std::copy(triangular_distribution.begin(), triangular_distribution.end(), std::ostream_iterator<double>(std::cout, " "));
        std::cout << std::endl;
        // 使用gnuplot生成png图片
        plot_to_png(triangular_distribution, "Triangular Distribution (3,1)", "triangular_3_1");
    }
    {
        auto triangular_distribution = xtechnical::ClusterShaper::get_triangular_distribution(5, 0);
        std::copy(triangular_distribution.begin(), triangular_distribution.end(), std::ostream_iterator<double>(std::cout, " "));
        std::cout << std::endl;
        // 使用gnuplot生成png图片
        plot_to_png(triangular_distribution, "Triangular Distribution (5,0)", "triangular_5_0");
    }
    {
        auto triangular_distribution = xtechnical::ClusterShaper::get_triangular_distribution(5, 4);
        std::copy(triangular_distribution.begin(), triangular_distribution.end(), std::ostream_iterator<double>(std::cout, " "));
        std::cout << std::endl;
        // 使用gnuplot生成png图片
        plot_to_png(triangular_distribution, "Triangular Distribution (5,4)", "triangular_5_4");
    }

    return 0;
}
