#ifndef XTECHNICAL_AWESOME_OSCILLATOR_HPP_INCLUDED
#define XTECHNICAL_AWESOME_OSCILLATOR_HPP_INCLUDED

#include "../xtechnical_common.hpp"
#include "xtechnical_sma.hpp"

namespace xtechnical {
    using namespace xtechnical_common;

    /** \brief Awesome Oscillator
     */
    template <class T, class MA_TYPE = SMA<T>>
    class AwesomeOscillator {
    private:
        MA_TYPE fast;
		MA_TYPE slow;
		T output_value = std::numeric_limits<T>::quiet_NaN();
    public:

        AwesomeOscillator() {}

        /** \brief Awesome Oscillator构造函数
         * \param fast_period\t快速MA周期
	 * \param slow_period\t慢速MA周期
         */
        AwesomeOscillator(const size_t fast_period, const size_t slow_period) :
            fast(fast_period), slow(slow_period) {
        }
		
		/** \brief 更新指标状态
         * \param price\tK线价格
         * \return 成功返回0，否则参见ErrorType
         */
		inline int update(const T price) noexcept {
			fast.update(price);
			slow.update(price);
			if (std::isnan(fast.get()) || 
				std::isnan(slow.get())) return INDICATOR_NOT_READY_TO_WORK;
			output_value = fast.get() - slow.get();
		}

        /** \brief 更新指标状态
         * \param high\tK线最高价
	 * \param low \tK线最低价
         * \return 成功返回0，否则参见ErrorType
         */
        int update(const T high, const T low) noexcept {
			const T price = (high + low) / 2.0d;
            return update(price);
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
		inline int test(const T price) noexcept {
			fast.test(price);
			slow.test(price);
			if (std::isnan(fast.get()) || 
				std::isnan(slow.get())) return INDICATOR_NOT_READY_TO_WORK;
			output_value = fast.get() - slow.get();
		}
		
		/** \brief 测试指标
         * \param high\tK线最高价
	 * \param low \tK线最低价
         * \return 成功返回0，否则参见ErrorType
         */
        int test(const T high, const T low) noexcept {
			const T price = (high + low) / 2.0d;
            return test(price);
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
            return OK;
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
			fast.clear();
			slow.clear();
        }
    };
}; // xtechnical

#endif // XTECHNICAL_AWESOME_OSCILLATOR_HPP_INCLUDED
