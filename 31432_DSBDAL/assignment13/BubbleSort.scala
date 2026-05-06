object BubbleSort {
  def main(args: Array[String]): Unit = {

    var arr = Array(5, 3, 8, 1, 2)

    for (i <- arr.indices) {
      for (j <- 0 until arr.length - i - 1) {
        if (arr(j) > arr(j + 1)) {
          val temp = arr(j)
          arr(j) = arr(j + 1)
          arr(j + 1) = temp
        }
      }
    }

    println("Bubble Sort Output:")
    arr.foreach(println)
  }
}
