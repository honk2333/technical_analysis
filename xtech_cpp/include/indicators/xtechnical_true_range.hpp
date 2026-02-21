#ifndef XTECHNICAL_TRUE_RANGE_HPP_INCLUDED
#define XTECHNICAL_TRUE_RANGE_HPP_INCLUDED

#include "../xtechnical_common.hpp"

namespace xtechnical {

    /** \brief 真实范围 (True Range)
     */
    template <typename T>
    class TrueRange {
    private:
        T last_data = std::numeric_limits<T>::quiet_NaN();
        T output_value = std::numeric_limits<T>::quiet_NaN();

    public:
        TrueRange() {};

		inline int update(const T high, const T low, const T close) noexcept {
            output_value = std::max(std::max(high - low, high - close), close - low);
            return common::OK;
        }

		inline int update(const T high, const T low, const T close, T &out) noexcept {
            const int err = update(high, low, close);
			out = output_value;
            return err;
        }

        /** \brief 更新指标状态
         * \param in 	输入信号
         * \return 成功返回0，否则参见ErrorType
         */
        inline int update(const T in) noexcept {
            if (std::isnan(last_data)) {
				last_data = in;
				return common::NO_INIT;
			}
			output_value = std::abs(in - last_data);
			last_data = in;
			return common::OK;
        }

        /** \brief 更新指标状态
         * \param in    输入信号
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        inline int update(const T in, T &out) noexcept {
            const int err = update(in);
            out = output_value;
            return err;
        }

		inline int test(const T high, const T low, const T close) noexcept {
            output_value = std::max(std::max(high - low, high - close), close - low);
            return common::OK;
        }

		inline int test(const T high, const T low, const T close, T &out) noexcept {
            const int err = test(high, low, close);
			out = output_value;
            return err;
        }

        /** \brief 测试指标
         *
         * 此函数与update不同，
         * 不会影响指标的内部状态
         * \param in 输入信号
         * \return 成功返回0，否则参见ErrorType
         */
        inline int test(const T in) noexcept {
            if (std::isnan(last_data)) return common::NO_INIT;
			output_value = std::abs(in - last_data);
			return common::OK;
        }

        /** \brief 测试指标
         *
         * 此函数与update不同，
         * 不会影响指标的内部状态
         * \param in    输入信号
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        inline int test(const T in, T &out) noexcept {
            const int err = test(in);
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
        inline void clear() noexcept {
            output_value = std::numeric_limits<T>::quiet_NaN();
            last_data = std::numeric_limits<T>::quiet_NaN();
        }
    }; // TrueRange

}; // xtechnical

#endif // XTECHNICAL_TRUE_RANGE_HPP_INCLUDED
