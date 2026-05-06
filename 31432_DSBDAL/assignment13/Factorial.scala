object Factorial {
  def fact(n: Int): Int = {
    if (n == 0) 1
    else n * fact(n - 1)
  }

  def main(args: Array[String]): Unit = {
    val num = 5
    println("Factorial of " + num + " is: " + fact(num))
  }
}
