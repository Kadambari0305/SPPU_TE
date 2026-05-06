object QuickSort {
  def quickSort(arr: Array[Int]): Array[Int] = {
    if (arr.length <= 1) arr
    else {
      val pivot = arr(arr.length / 2)
      quickSort(arr.filter(_ < pivot)) ++
      arr.filter(_ == pivot) ++
      quickSort(arr.filter(_ > pivot))
    }
  }

  def main(args: Array[String]): Unit = {
    val arr = Array(5, 2, 9, 1, 6)
    val sorted = quickSort(arr)

    println("Quick Sort Output:")
    sorted.foreach(println)
  }
}
