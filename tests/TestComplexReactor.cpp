/**
 * @file TestComplexReactor.cpp
 * @brief Тесты класса ComplexReactor с использованием GoogleTest.
 *
 * Этот файл содержит набор тестов для проверки корректной работы
 * химического реактора с 2 входами и 1 или 2 выходами.
 *
 * Проверяются следующие сценарии:
 * - одиночный выход с базовым коэффициентом превращения
 * - два выхода с разбиением по splitRatio
 * - лимитирующий реагент
 * - некорректные параметры (должны бросать исключения)
 * - многократные запуски реакции и сброс состояния
 */

#include "gtest/gtest.h"
#include "../src/ComplexReactor.h"
#include <stdexcept>

/**
 * @brief Тест 1: базовый случай с одним выходом
 *
 * Проверяет корректность расчёта продукта R при одиночном выходе.
 * Conversion = 0.5, входы A=2, B=2, ожидаемый результат R=1.0
 */
TEST(ComplexReactorTests, SingleOutputBasic) {
    ComplexReactor r; // conversion=0.5, single output
    r.setInputs(2.0, 2.0);
    auto out = r.runReaction();
    ASSERT_EQ(out.size(), 1);
    EXPECT_NEAR(out[0], 1.0, 1e-9);
}

/**
 * @brief Тест 2: два выхода и проверка splitRatio
 *
 * Проверяет правильное распределение продуктов R и S
 * в зависимости от заданного splitRatio.
 * Conversion=1.0, splitRatio=0.7 -> R=0.7, S=0.3
 */
TEST(ComplexReactorTests, TwoOutputsSplit) {
    ComplexReactor r(1.0, true, 0.7);
    r.setInputs(1.0, 1.0);
    auto out = r.runReaction();
    ASSERT_EQ(out.size(), 2);
    EXPECT_NEAR(out[0], 0.7, 1e-9); // R
    EXPECT_NEAR(out[1], 0.3, 1e-9); // S
}

/**
 * @brief Тест 3: ограничивающий реагент
 *
 * Проверяет, что расчёт основывается на минимальном входном реагенте.
 * A=0.5, B=10 -> limiting=0.5 -> reacted=0.5 -> R=0.5
 */
TEST(ComplexReactorTests, LimitingReagent) {
    ComplexReactor r(1.0, false);
    r.setInputs(0.5, 10.0);
    auto out = r.runReaction();
    ASSERT_EQ(out.size(), 1);
    EXPECT_NEAR(out[0], 0.5, 1e-9);
}

/**
 * @brief Тест 4: проверка некорректных параметров
 *
 * Проверяет, что при неверных значениях conversion или splitRatio
 * бросается исключение std::invalid_argument.
 */
TEST(ComplexReactorTests, InvalidConversionThrows) {
    EXPECT_THROW(ComplexReactor(-0.1), std::invalid_argument);
    EXPECT_THROW(ComplexReactor(1.2), std::invalid_argument);

    ComplexReactor r;
    EXPECT_THROW(r.setSplitRatio(2.0), std::invalid_argument);
}

/**
 * @brief Тест 5: многократные запуски реакции и reset
 *
 * Проверяет:
 * - корректность повторных расчётов с разными входами
 * - правильную работу метода reset
 * - выброс исключения при обращении к lastOutputs после reset
 */
TEST(ComplexReactorTests, MultipleRunsAndReset) {
    ComplexReactor r(0.5, true, 0.5);
    r.setInputs(2.0, 2.0);
    auto out1 = r.runReaction(); // reacted = 1.0 -> R=0.5 S=0.5
    EXPECT_NEAR(out1[0], 0.5, 1e-9);
    EXPECT_NEAR(out1[1], 0.5, 1e-9);

    // change inputs
    r.setInputs(1.0, 1.0);
    auto out2 = r.runReaction(); // reacted 0.5 -> R=0.25 S=0.25
    EXPECT_NEAR(out2[0], 0.25, 1e-9);
    EXPECT_NEAR(out2[1], 0.25, 1e-9);

    r.reset();
    EXPECT_THROW(r.getLastOutput(0), std::out_of_range); // no outputs after reset
}
