package main

import (
	"fmt"
	"os"
)

func main() {
	var length int
	var searchIndex int
	fmt.Println("Input length")
	fmt.Fscan(os.Stdin, &length)
	fmt.Println("Input array")
	arr := inputArray(length)
	fmt.Println(arr)
	searchIndex = binarySearch(arr)
	fmt.Println("Searching index = ", searchIndex)
}

func binarySearch(a []int) int {
	var searchNum int
	fmt.Println("Input searching number")
	fmt.Fscan(os.Stdin, &searchNum)
	left, right := 0, len(a)-1
	for left <= right {
		mid := (left + right) / 2
		if a[mid] == searchNum {
			return mid
		} else if a[mid] < searchNum {
			left = mid + 1
		} else {
			right = mid - 1
		}
	}
	return -1

}

func inputArray(length int) []int {
	arr := make([]int, length)
	for i := 0; i < length; i++ {
		fmt.Fscan(os.Stdin, &arr[i])
	}
	return arr
}
