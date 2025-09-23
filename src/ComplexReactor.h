#pragma once
#include <vector>
#include <stdexcept>

/**
 * @file ComplexReactor.h
 * @brief Модель простого "сложного" реактора (2 входа, 1 или 2 выхода).
 *
 * Класс реализует упрощённую модель реакции с предположением стехиометрии 1:1
 * (1 A + 1 B -> products). Количество прореагировавшего вещества определяется
 * как limiting = min(A, B) и reacted = limiting * conversion.
 *
 * Особенности:
 * - conversion и splitRatio в диапазоне [0, 1].
 * - При twoOutputs == false весь reacted идёт в один продукт R.
 * - При twoOutputs == true reacted делится на R и S в пропорции splitRatio / (1 - splitRatio).
 *
 * Примеры:
 * - A=2, B=1, conversion=0.5 -> limiting=1, reacted=0.5 -> если single output => R=0.5
 * - A=1, B=1, conversion=1.0, twoOutputs=true, splitRatio=0.7 -> R=0.7, S=0.3
 */

class ComplexReactor {
public:
    /**
     * @brief Конструктор по умолчанию.
     * @param conversion Доля (0..1) лимитирующего реагента, которая превращается.
     * @param twoOutputs Если true — реакция даёт 2 продукта (R и S), иначе 1 (R).
     * @param splitRatio Доля прореагировавшего вещества, идущая в R (0..1). Игнорируется если twoOutputs == false.
     *
     * @throws std::invalid_argument если conversion или splitRatio не в [0,1].
     */
    ComplexReactor(double conversion = 0.5, bool twoOutputs = false, double splitRatio = 0.5);

    /**
     * @brief Установить входные количества реагентов A и B.
     * @param a Количество A (>= 0).
     * @param b Количество B (>= 0).
     * @throws std::invalid_argument если a < 0 или b < 0.
     */
    void setInputs(double a, double b);

    /**
     * @brief Установить коэффициент превращения (conversion).
     * @param conversion Должно быть в диапазоне [0, 1].
     * @throws std::invalid_argument если значение вне диапазона.
     */
    void setConversion(double conversion);

    /**
     * @brief Включить/выключить второй выход (S).
     * @param two true - два выхода; false - один выход.
     */
    void setTwoOutputs(bool two);

    /**
     * @brief Установить отношение распределения продуктов.
     * @param ratio Доля, приходящаяся на R (0..1). При twoOutputs==false игнорируется.
     * @throws std::invalid_argument если ratio не в [0,1].
     */
    void setSplitRatio(double ratio);

    /**
     * @brief Выполнить реакцию с текущими входами/параметрами.
     *
     * Алгоритм:
     * 1. limiting = min(a_, b_)
     * 2. reacted = limiting * conversion_
     * 3. если twoOutputs_ == false: R = reacted
     *    иначе: R = reacted * splitRatio_; S = reacted * (1 - splitRatio_)
     *
     * @return vector<double> с продуктами:
     *         - single output -> { R }
     *         - two outputs   -> { R, S }
     *
     * Метод сохраняет результаты в lastOutputs_ и возвращает их копию.
     *
     * СЕМАНТИКА СОСТОЯНИЯ: метод _не изменяет_ a_ и b_ (входы остаются неизменными).
     */
    std::vector<double> runReaction();

    /**
     * @brief Сбросить входные значения и последний результат.
     *
     * После сброса вызов getLastOutput будет бросать std::out_of_range.
     */
    void reset();

    /**
     * @brief Получить последний рассчитанный продукт по индексу.
     * @param idx Номер продукта (0 — первый продукт R, 1 — S если есть).
     * @return значение продукта
     * @throws std::out_of_range если результат отсутствует или idx неверен.
     */
    double getLastOutput(size_t idx) const;

private:
    double a_{0.0};                ///< количество A (вход)
    double b_{0.0};                ///< количество B (вход)
    double conversion_{0.5};       ///< доля превращения (0..1)
    bool twoOutputs_{false};       ///< выдавать 1 или 2 продукта
    double splitRatio_{0.5};       ///< доля прореагировавшего, идущая в R (0..1)
    std::vector<double> lastOutputs_; ///< последний результат (пустой если нет)

    /**
     * @brief Валидация параметров conversion_ и splitRatio_.
     * @throws std::invalid_argument если параметры не в допустимом диапазоне.
     */
    void validateParams() const;
};
