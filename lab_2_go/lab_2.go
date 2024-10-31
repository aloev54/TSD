package main

import (
	"fmt"
	"math"
	"os"
)

func main() {
	var x_max, x_min float32
	fmt.Println("Введите xMin:")
	fmt.Fscan(os.Stdin, &x_min)
	fmt.Println("Введите xMax")
	fmt.Fscan(os.Stdin, &x_max)
	if x_min >= x_max {
		fmt.Println("x_max не может быть меньше x_min")
		os.Exit(1)
	}
	methodWithoutAccuracy(x_min, x_max)
	methodWithAccuracy(x_min, x_max)
}

func testFunc(x float32) float32 {
	return (x / 8) + float32(math.Sin(float64(x)))
}

func leftRectangleMethod(x_min float32, x_max float32, n int) float32 {
	h := (x_max - x_min) / float32(n)
	s, currentX := float32(0), x_min
	for i := 0; i < n-1; i++ {
		s += float32(math.Abs(float64(testFunc(currentX) * h)))
		currentX += h
	}
	s += float32(math.Abs(float64(testFunc(currentX) * (x_max - currentX))))
	return s
}

func rightRectangleMethod(x_min float32, x_max float32, n int) float32 {
	h := (x_max - x_min) / float32(n)
	s, currentX := float32(0), x_max
	for i := 0; i < n-1; i++ {
		s += float32(math.Abs(float64(testFunc(currentX) * h)))
		currentX -= h
	}
	s += float32(math.Abs(float64(testFunc(currentX) * (currentX - x_min))))
	return s
}
func trapezeMethod(x_min float32, x_max float32, n int) float32 {
	h := (x_max - x_min) / float32(n)
	s, current_x_left := float32(0), x_min
	current_x_right := current_x_left + h
	for i := 0; i < n-1; i++ {
		s += (((float32(math.Abs(float64(testFunc(current_x_left)))) + float32(math.Abs(float64(testFunc(current_x_right))))) / 2) * h)
		current_x_left += h
		current_x_right += h
	}
	s += ((float32(math.Abs(float64(testFunc(current_x_left)))) + float32(math.Abs(float64(testFunc(current_x_right))))) / 2) * (x_max - x_min - ((float32(n) - 1) * h))
	return s
}

func methodWithoutAccuracy(x_min float32, x_max float32) {
	var n int
	fmt.Println("Вычисление интеграла без заданной точности")
	fmt.Println("Введите количество разбиений:")
	fmt.Fscan(os.Stdin, &n)
	if n < 1 {
		fmt.Println("Количество разбиений должно быть положительным числом")
		os.Exit(1)
	}
	fmt.Println("Метод левых прямогольников: ", leftRectangleMethod(x_min, x_max, n))
	fmt.Println("Метод правых прямоугольников: ", rightRectangleMethod(x_min, x_max, n))
	fmt.Println("Метод трапеций: ", trapezeMethod(x_min, x_max, n))
}

func methodWithAccuracy(x_min float32, x_max float32) {
	var n int
	var eps float32
	fmt.Println("Вычисление интеграла с заданной точностью")
	fmt.Println("Введите епислон")
	fmt.Fscan(os.Stdin, &eps)

	if eps >= 1 || eps <= 0 {
		fmt.Println("Eps должно находится в промежутке (0,1)")
		os.Exit(1)
	}

	fmt.Println("Введите количество разбиений:")
	fmt.Fscan(os.Stdin, &n)

	if n < 1 {
		fmt.Println("Количество разбиений должно быть положительным числом")
		os.Exit(1)
	}

	n_max := 1000
	var cur_res, prev_res float32

	prev_res = leftRectangleMethod(x_min, x_max, n)
	for i := n + 1; i < n_max; i++ {
		if i == n_max-1 {
			fmt.Println("Метод левых прямоугольников не сошелся")
			break
		}
		cur_res = leftRectangleMethod(x_min, x_max, i)
		if math.Abs(float64(prev_res)-float64(cur_res)) <= float64(eps) {
			fmt.Printf("Метод левых прямоугольников: площадь с точностью %.3f = %f\n", eps, cur_res)
			break
		}
		prev_res = cur_res
	}

	prev_res = rightRectangleMethod(x_min, x_max, n)
	for i := n + 1; i < n_max; i++ {
		if i == n_max-1 {
			fmt.Println("Метод правых прямоугольников не сошелся")
			break
		}
		cur_res = rightRectangleMethod(x_min, x_max, i)
		if math.Abs(float64(prev_res)-float64(cur_res)) <= float64(eps) {
			fmt.Printf("Метод правых прямоугольников: площадь с точностью %.3f = %f\n", eps, cur_res)
			break
		}
		prev_res = cur_res
	}

	prev_res = trapezeMethod(x_min, x_max, n)
	for i := n + 1; i < n_max; i++ {
		if i == n_max-1 {
			fmt.Println("Метод трапеций не сошелся")
			break
		}
		cur_res = rightRectangleMethod(x_min, x_max, i)
		if math.Abs(float64(prev_res)-float64(cur_res)) <= float64(eps) {
			fmt.Printf("Метод трапеций: площадь с точностью %.3f = %f\n", eps, cur_res)
			break
		}
		prev_res = cur_res
	}
}
