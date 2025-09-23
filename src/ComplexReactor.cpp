#include "ComplexReactor.h"
#include <algorithm>

/**
 * @brief Конструктор
 */
ComplexReactor::ComplexReactor(double conversion, bool twoOutputs, double splitRatio)
    : a_(0.0), b_(0.0), conversion_(conversion), twoOutputs_(twoOutputs), splitRatio_(splitRatio)
{
    validateParams();
}

/**
 * @brief Проверка параметров
 * @throws std::invalid_argument если conversion или splitRatio не в [0,1]
 */
void ComplexReactor::validateParams() const {
    if (conversion_ < 0.0 || conversion_ > 1.0) throw std::invalid_argument("conversion must be in [0,1]");
    if (splitRatio_ < 0.0 || splitRatio_ > 1.0) throw std::invalid_argument("splitRatio must be in [0,1]");
}

/**
 * @brief Установка входов
 */
void ComplexReactor::setInputs(double a, double b) {
    if (a < 0.0 || b < 0.0) throw std::invalid_argument("inputs must be non-negative");
    a_ = a;
    b_ = b;
}

/**
 * @brief Изменение коэффициента превращения
 */
void ComplexReactor::setConversion(double conversion) {
    conversion_ = conversion;
    validateParams();
}

/**
 * @brief Выбор одного или двух выходов
 */
void ComplexReactor::setTwoOutputs(bool two) {
    twoOutputs_ = two;
}

/**
 * @brief Установка доли для R при двух выходах
 */
void ComplexReactor::setSplitRatio(double ratio) {
    splitRatio_ = ratio;
    validateParams();
}

/**
 * @brief Выполнение реакции
 */
std::vector<double> ComplexReactor::runReaction() {
    double limiting = std::min(a_, b_);
    double reacted = limiting * conversion_;

    if (!twoOutputs_) {
        double R = reacted;
        lastOutputs_ = { R };
        return lastOutputs_;
    } else {
        double R = reacted * splitRatio_;
        double S = reacted * (1.0 - splitRatio_);
        lastOutputs_ = { R, S };
        return lastOutputs_;
    }
}

/**
 * @brief Сброс состояния
 */
void ComplexReactor::reset() {
    a_ = b_ = 0.0;
    lastOutputs_.clear();
}

/**
 * @brief Получение последнего результата
 */
double ComplexReactor::getLastOutput(size_t idx) const {
    if (idx >= lastOutputs_.size()) throw std::out_of_range("output index out of range");
    return lastOutputs_[idx];
}
