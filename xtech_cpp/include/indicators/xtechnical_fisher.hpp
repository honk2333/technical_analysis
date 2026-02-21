#ifndef XTECHNICAL_FISHER_HPP_INCLUDED
#define XTECHNICAL_FISHER_HPP_INCLUDED

#include "../xtechnical_common.hpp"
#include "xtechnical_fast_min_max.hpp"

namespace xtechnical {

    /** \brief Fisher Transform Indicator
     */
    template <class T>
    class FisherV1 {
    private:
        FastMinMax<T> max_high;
		FastMinMax<T> min_low;
		T output_value = std::numeric_limits<T>::quiet_NaN();
		T prev_value = 0;
		T prev_fish = 0;
    public:

        FisherV1() {}

        /** \brief Fisher Transform Indicator构造函数
         * \param period\t指标周期
         */
        FisherV1(const size_t period) :
            max_high(period), min_low(period) {
        }

        /** \brief 更新指标状态
         * \param high\tK线最高价
	 * \param low \tK线最低价
         * \return 成功返回0，否则参见ErrorType
         */
        int update(const T high, const T low) noexcept {
			max_high.update(high);
			min_low.update(low);
			if (std::isnan(min_low.get_min()) ||
				std::isnan(max_high.get_max())) return common::INDICATOR_NOT_READY_TO_WORK;

			const T price = (high + low) / 2.0d;
			const T diff = max_high.get_max() - min_low.get_min();

			T value = diff == 0 ? (0.33 * 2 * (0 - 0.5) + 0.67 * prev_value) :
				(0.33 * 2 * ((price - min_low.get_min()) / (diff) - 0.5) + 0.67 * prev_value);

			value = std::min(std::max(value, -0.999), 0.999);

			if ((1 - value) == 0) output_value = 0.5 + 0.5 * prev_fish;
			else output_value = 0.5 * std::log((1 + value)/(1 - value)) + 0.5 * prev_fish;

			prev_value = value;
			prev_fish = output_value;
            return common::OK;
        }

		/** \brief 更新指标状态
         * \param high\tK线最高价
	 * \param low \tK线最低价
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
		inline int update(const T high, const T low, T &out) noexcept {
            const int err = update(high, low);
            out = output_value;
            return err;
        }

		/** \brief 更新指标状态
         * \param price\tK线价格
         * \return 成功返回0，否则参见ErrorType
         */
		inline int update(const T price) noexcept {
			return update(price, price);
		}

        /** \brief 更新指标状态
         * \param price\tK线价格
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        inline int update(const T price, T &out) noexcept {
            const int err = update(price);
            out = output_value;
            return err;
        }

		/** \brief 测试指标
         * \param high\tK线最高价
	 * \param low \tK线最低价
         * \return 成功返回0，否则参见ErrorType
         */
        int test(const T high, const T low) noexcept {
			max_high.test(high);
			min_low.test(low);
			if (std::isnan(min_low.get_min()) ||
				std::isnan(max_high.get_max())) return common::INDICATOR_NOT_READY_TO_WORK;

			const T price = (high + low) / 2.0d;
			const T diff = max_high.get_max() - min_low.get_min();

			T value = diff == 0 ? (0.33 * 2 * (0 - 0.5) + 0.67 * prev_value) :
				(0.33 * 2 * ((price - min_low.get_min()) / (diff) - 0.5) + 0.67 * prev_value);

			value = std::min(std::max(value, -0.999), 0.999);

			if ((1 - value) == 0) output_value = 0.5 + 0.5 * prev_fish;
			else output_value = 0.5 * std::log((1 + value)/(1 - value)) + 0.5 * prev_fish;

            return common::OK;
        }

		/** \brief 测试指标
         * \param high\tK线最高价
	 * \param low \tK线最低价
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
		inline int test(const T high, const T low, T &out) noexcept {
            const int err = test(high, low);
            out = output_value;
            return common::OK;
        }

		/** \brief 测试指标
         * \param price\tK线价格
         * \return 成功返回0，否则参见ErrorType
         */
		inline int test(const T price) noexcept {
			return test(price, price);
		}

        /** \brief 测试指标
         * \param price\tK线价格
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        inline int test(const T price, T &out) noexcept {
            const int err = test(price);
            out = output_value;
            return err;
        }

        /** \brief 获取指标值
         * \return 指标值
         */
        inline T get() const noexcept {
            return output_value;
        }

        /** \brief 清除指标数据
         */
        void clear() noexcept {
            output_value = std::numeric_limits<T>::quiet_NaN();
			max_high.clear();
			min_low.clear();
			prev_value = 0;
			prev_fish = 0;
        }
    };

	/** \brief Fisher Transform Indicator
     */
    template <class T>
    class FisherV2 {
    private:
		FastMinMax<T> min_max;
		T output_value = std::numeric_limits<T>::quiet_NaN();
		T prev_value = 0;
		T prev_fish = 0;
    public:

        FisherV2() {}

        /** \brief Fisher Transform Indicator构造函数
         * \param period\t指标周期
         */
        FisherV2(const size_t period) :
            min_max(period) {
        }

        /** \brief 更新指标状态
         * \param price\tK线价格
         * \return 成功返回0，否则参见ErrorType
         */
        int update(const T price) noexcept {
			min_max.update(price);
			if (std::isnan(min_max.get_min()) ||
				std::isnan(min_max.get_max())) return common::INDICATOR_NOT_READY_TO_WORK;

			const T diff = min_max.get_max() - min_max.get_min();

			T value = diff == 0 ? (0.33 * 2 * (0 - 0.5) + 0.67 * prev_value) :
				(0.33 * 2 * ((price - min_max.get_min()) / (diff) - 0.5) + 0.67 * prev_value);

			value = std::min(std::max(value, -0.999), 0.999);

			if ((1 - value) == 0) output_value = 0.5 + 0.5 * prev_fish;
			else output_value = 0.5 * std::log((1 + value)/(1 - value)) + 0.5 * prev_fish;

			prev_value = value;
			prev_fish = output_value;
            return common::OK;
        }

        /** \brief 更新指标状态
         * \param price\tK线价格
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        inline int update(const T price, T &out) noexcept {
            const int err = update(price);
            out = output_value;
            return err;
        }

		/** \brief 测试指标
         * \param price\tK线价格
         * \return 成功返回0，否则参见ErrorType
         */
		int test(const T price) noexcept {
			min_max.test(price);
			if (std::isnan(min_max.get_min()) ||
				std::isnan(min_max.get_max())) return common::INDICATOR_NOT_READY_TO_WORK;

			const T diff = min_max.get_max() - min_max.get_min();

			T value = diff == 0 ? (0.33 * 2 * (0 - 0.5) + 0.67 * prev_value) :
				(0.33 * 2 * ((price - min_max.get_min()) / (diff) - 0.5) + 0.67 * prev_value);

			value = std::min(std::max(value, -0.999), 0.999);

			if ((1 - value) == 0) output_value = 0.5 + 0.5 * prev_fish;
			else output_value = 0.5 * std::log((1 + value)/(1 - value)) + 0.5 * prev_fish;
            return common::OK;
        }

        /** \brief 测试指标
         * \param price\tK线价格
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        inline int test(const T price, T &out) noexcept {
            const int err = test(price);
            out = output_value;
            return err;
        }

        /** \brief 获取指标值
         * \return 指标值
         */
        inline T get() const noexcept {
            return output_value;
        }

        /** \brief 清除指标数据
         */
        void clear() noexcept {
            output_value = std::numeric_limits<T>::quiet_NaN();
			min_max.clear();
			prev_value = 0;
			prev_fish = 0;
        }
    };

	/** \brief Fisher Transform Indicator
     */
    template <class T>
    class FisherV3 {
    private:
		FastMinMax<T> min_max;
		T output_value = std::numeric_limits<T>::quiet_NaN();
    public:

        FisherV3() {}

        /** \brief Fisher Transform Indicator构造函数
         * \param period\t指标周期
         */
        FisherV3(const size_t period) :
            min_max(period) {
        }

        /** \brief 更新指标状态
         * \param price\tK线价格
         * \return 成功返回0，否则参见ErrorType
         */
        int update(const T price) noexcept {
			min_max.update(price);
			if (std::isnan(min_max.get_min()) ||
				std::isnan(min_max.get_max())) return common::INDICATOR_NOT_READY_TO_WORK;

			const T diff = min_max.get_max() - min_max.get_min();
			const T value = diff == 0 ? 0 : ((2 * (price - min_max.get_min()) / diff) - 1);

			if ((1 - value) == 0) output_value = 0.5;
			else output_value = 0.5 * std::log((1 + value)/(1 - value));
            return common::OK;
        }

        /** \brief 更新指标状态
         * \param price\tK线价格
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        inline int update(const T price, T &out) noexcept {
            const int err = update(price);
            out = output_value;
            return err;
        }

		/** \brief 测试指标
         * \param price\tK线价格
         * \return 成功返回0，否则参见ErrorType
         */
		int test(const T price) noexcept {
			min_max.test(price);
			if (std::isnan(min_max.get_min()) ||
				std::isnan(min_max.get_max())) return common::INDICATOR_NOT_READY_TO_WORK;

			const T diff = min_max.get_max() - min_max.get_min();
			const T value = diff == 0 ? 0 : ((2 * (price - min_max.get_min()) / diff) - 1);

			if ((1 - value) == 0) output_value = 0.5;
			else output_value = 0.5 * std::log((1 + value)/(1 - value));
            return common::OK;
        }

        /** \brief 测试指标
         * \param price\tK线价格
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        inline int test(const T price, T &out) noexcept {
            const int err = test(price);
            out = output_value;
            return err;
        }

        /** \brief 获取指标值
         * \return 指标值
         */
        inline T get() const noexcept {
            return output_value;
        }

        /** \brief 清除指标数据
         */
        void clear() noexcept {
            output_value = std::numeric_limits<T>::quiet_NaN();
			min_max.clear();
        }
    };
}; // xtechnical

#endif // XTECHNICAL_FISHER_HPP_INCLUDED
