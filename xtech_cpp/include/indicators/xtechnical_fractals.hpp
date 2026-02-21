#ifndef XTECHNICAL_FRACTALS_HPP_INCLUDED
#define XTECHNICAL_FRACTALS_HPP_INCLUDED

#include "../xtechnical_common.hpp"

namespace xtechnical {

	/** \brief 比尔·威廉姆斯分形指标
 * 原始来源: https://www.mql5.com/en/code/viewcode/7982/130162/Fractals.mq4
 */
	template <typename T>
	class Fractals {
	private:
		circular_buffer<T> buffer_up;
		circular_buffer<T> buffer_dn;

		T save_output_up = std::numeric_limits<T>::quiet_NaN();
		T save_output_dn = std::numeric_limits<T>::quiet_NaN();
		T output_up = std::numeric_limits<T>::quiet_NaN();
		T output_dn = std::numeric_limits<T>::quiet_NaN();

	public:
	
		Fractals() : buffer_up(9), buffer_dn(9) {};

		/** \brief 更新指标状态
		 * \param high		K线最高价
		 * \param low		K线最低价
		 * \param on_up		上部分形的回调函数
		 * \param on_dn		下部分形的回调函数
		 * \return 成功返回0，否则参见ErrorType
		 */
		int update(
				const T high, 
				const T low, 
				std::function<void(const double value)> on_up = nullptr, 
				std::function<void(const double value)> on_dn = nullptr) noexcept {
			buffer_up.update(high);
			buffer_dn.update(low);

			if(buffer_up.full()) {
				// Fractals up
				const std::vector<T> values = buffer_up.to_vector();
				// 0 1 2 3 4 5 6 7 8

				// 5 bars Fractal
				if (values[6] > values[4] &&
					values[6] > values[5] &&
					values[6] > values[7] &&
					values[6] > values[8]) {
					save_output_up = output_up = values[6];
					if (on_up) on_up(values[6]);
				} else
				// 6 bars Fractal
				if (values[6] > values[3] &&
					values[6] > values[4] &&
					values[6] == values[5] &&
					values[6] > values[7] &&
					values[6] > values[8]) {
					save_output_up = output_up = values[6];
					if (on_up) on_up(values[6]);
				} else
				// 7 bars Fractal
				if (values[6] > values[2] &&
					values[6] > values[3] &&
					values[6] == values[4] &&
					values[6] >= values[5] &&
					values[6] > values[7] &&
					values[6] > values[8]) {
					save_output_up = output_up = values[6];
					if (on_up) on_up(values[6]);
				} else
				// 8 bars Fractal
				if (values[6] > values[1] &&
					values[6] > values[2] &&
					values[6] == values[3] &&
					values[6] == values[4] &&
					values[6] >= values[5] &&
					values[6] > values[7] &&
					values[6] > values[8]) {
					save_output_up = output_up = values[6];
					if (on_up) on_up(values[6]);
				} else
				// 9 bars Fractal
				if (values[6] > values[0] &&
					values[6] > values[1] &&
					values[6] == values[2] &&
					values[6] >= values[3] &&
					values[6] == values[4] &&
					values[6] >= values[5] &&
					values[6] > values[7] &&
					values[6] > values[8]) {
					save_output_up = output_up = values[6];
					if (on_up) on_up(values[6]);
				} else {
					output_up = save_output_up;
				}
			} else return common::INDICATOR_NOT_READY_TO_WORK;
			if(buffer_dn.full()) {
				// Fractals down
				const std::vector<T> values = buffer_dn.to_vector();
				// 0 1 2 3 4 5 6 7 8

				// 5 bars Fractal
				if (values[6] < values[4] &&
					values[6] < values[5] &&
					values[6] < values[7] &&
					values[6] < values[8]) {
					save_output_dn = output_dn = values[6];
					if (on_dn) on_dn(values[6]);
				} else
				// 6 bars Fractal
				if (values[6] < values[3] &&
					values[6] < values[4] &&
					values[6] == values[5] &&
					values[6] < values[7] &&
					values[6] < values[8]) {
					save_output_dn = output_dn = values[6];
					if (on_dn) on_dn(values[6]);
				} else
				// 7 bars Fractal
				if (values[6] < values[2] &&
					values[6] < values[3] &&
					values[6] == values[4] &&
					values[6] <= values[5] &&
					values[6] < values[7] &&
					values[6] < values[8]) {
					save_output_dn = output_dn = values[6];
					if (on_dn) on_dn(values[6]);
				} else
				// 8 bars Fractal
				if (values[6] < values[1] &&
					values[6] < values[2] &&
					values[6] == values[3] &&
					values[6] == values[4] &&
					values[6] <= values[5] &&
					values[6] < values[7] &&
					values[6] < values[8]) {
					save_output_dn = output_dn = values[6];
					if (on_dn) on_dn(values[6]);
				} else
				// 9 bars Fractal
				if (values[6] < values[0] &&
					values[6] < values[1] &&
					values[6] == values[2] &&
					values[6] <= values[3] &&
					values[6] == values[4] &&
					values[6] <= values[5] &&
					values[6] < values[7] &&
					values[6] < values[8]) {
					save_output_dn = output_dn = values[6];
					if (on_dn) on_dn(values[6]);
				} else {
					output_dn = save_output_dn;
				}
			} else return common::INDICATOR_NOT_READY_TO_WORK;
			return common::OK;
		}

		/** \brief 测试指标
		 * \param high		K线最高价
		 * \param low		K线最低价
		 * \param on_up		上部分形的回调函数
		 * \param on_dn		下部分形的回调函数
		 * \return 成功返回0，否则参见ErrorType
		 */
		int test(
				const T high, 
				const T low, 
				std::function<void(const double value)> on_up = nullptr, 
				std::function<void(const double value)> on_dn = nullptr) noexcept {
			buffer_up.test(high);
			buffer_dn.test(low);

			if(buffer_up.full()) {
				// Fractals up
				const std::vector<T> values = buffer_up.to_vector();
				// 0 1 2 3 4 5 6 7 8

				// 5 bars Fractal
				if (values[6] > values[4] &&
					values[6] > values[5] &&
					values[6] > values[7] &&
					values[6] > values[8]) {
					output_up = values[6];
					if (on_up) on_up(values[6]);
				} else
				// 6 bars Fractal
				if (values[6] > values[3] &&
					values[6] > values[4] &&
					values[6] == values[5] &&
					values[6] > values[7] &&
					values[6] > values[8]) {
					output_up = values[6];
					if (on_up) on_up(values[6]);
				} else
				// 7 bars Fractal
				if (values[6] > values[2] &&
					values[6] > values[3] &&
					values[6] == values[4] &&
					values[6] >= values[5] &&
					values[6] > values[7] &&
					values[6] > values[8]) {
					output_up = values[6];
					if (on_up) on_up(values[6]);
				} else
				// 8 bars Fractal
				if (values[6] > values[1] &&
					values[6] > values[2] &&
					values[6] == values[3] &&
					values[6] == values[4] &&
					values[6] >= values[5] &&
					values[6] > values[7] &&
					values[6] > values[8]) {
					output_up = values[6];
					if (on_up) on_up(values[6]);
				} else
				// 9 bars Fractal
				if (values[6] > values[0] &&
					values[6] > values[1] &&
					values[6] == values[2] &&
					values[6] >= values[3] &&
					values[6] == values[4] &&
					values[6] >= values[5] &&
					values[6] > values[7] &&
					values[6] > values[8]) {
					output_up = values[6];
					if (on_up) on_up(values[6]);
				} else {
					output_up = save_output_up;
				}
			} else return common::INDICATOR_NOT_READY_TO_WORK;
			if(buffer_dn.full()) {
				// Fractals down
				const std::vector<T> values = buffer_dn.to_vector();
				// 0 1 2 3 4 5 6 7 8

				// 5 bars Fractal
				if (values[6] < values[4] &&
					values[6] < values[5] &&
					values[6] < values[7] &&
					values[6] < values[8]) {
					output_dn = values[6];
					if (on_dn) on_dn(values[6]);
				} else
				// 6 bars Fractal
				if (values[6] < values[3] &&
					values[6] < values[4] &&
					values[6] == values[5] &&
					values[6] < values[7] &&
					values[6] < values[8]) {
					output_dn = values[6];
					if (on_dn) on_dn(values[6]);
				} else
				// 7 bars Fractal
				if (values[6] < values[2] &&
					values[6] < values[3] &&
					values[6] == values[4] &&
					values[6] <= values[5] &&
					values[6] < values[7] &&
					values[6] < values[8]) {
					output_dn = values[6];
					if (on_dn) on_dn(values[6]);
				} else
				// 8 bars Fractal
				if (values[6] < values[1] &&
					values[6] < values[2] &&
					values[6] == values[3] &&
					values[6] == values[4] &&
					values[6] <= values[5] &&
					values[6] < values[7] &&
					values[6] < values[8]) {
					output_dn = values[6];
					if (on_dn) on_dn(values[6]);
				} else
				// 9 bars Fractal
				if (values[6] < values[0] &&
					values[6] < values[1] &&
					values[6] == values[2] &&
					values[6] <= values[3] &&
					values[6] == values[4] &&
					values[6] <= values[5] &&
					values[6] < values[7] &&
					values[6] < values[8]) {
					output_dn = values[6];
					if (on_dn) on_dn(values[6]);
				} else {
					output_dn = save_output_dn;
				}
			} else return common::INDICATOR_NOT_READY_TO_WORK;
			return common::OK;
		}

		/** \brief 获取上部分形值
		 * \return 上部分形值
		 */
		inline T get_up() const noexcept {
			return output_up;
		}

		/** \brief 获取下部分形值
		 * \return 下部分形值
		 */
		inline T get_dn() const noexcept {
			return output_dn;
		}

		/** \brief 清除指标数据
		 */
		inline void clear() noexcept {
			buffer_up.clear();
			buffer_dn.clear();
			output_up = std::numeric_limits<T>::quiet_NaN();
			output_dn = std::numeric_limits<T>::quiet_NaN();
			save_output_up = std::numeric_limits<T>::quiet_NaN();
			save_output_dn = std::numeric_limits<T>::quiet_NaN();
		}
	};

}; // xtechnical

#endif // XTECHNICAL_FRACTALS_HPP_INCLUDED
